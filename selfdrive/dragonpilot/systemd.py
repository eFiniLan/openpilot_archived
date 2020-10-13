#!/usr/bin/env python3
'''
This is a service that broadcast dp config values to openpilot's messaging queues
'''
import cereal.messaging as messaging
import time
import datetime
from common.dp_conf import confs, get_struct_name, to_struct_val
from common.params import Params, put_nonblocking
import subprocess
import re
import os
from selfdrive.thermald.power_monitoring import set_battery_charging, get_battery_charging
from math import floor
params = Params()
from common.realtime import sec_since_boot
from common.i18n import locale
from common.dp_common import param_get

DASHCAM_VIDEOS_PATH = '/sdcard/dashcam/'
DASHCAM_DURATION = 180 # max is 180
DASHCAM_BIT_RATES = 4000000 # max is 4000000
DASHCAM_MAX_SIZE_PER_FILE = DASHCAM_BIT_RATES/8*DASHCAM_DURATION # 4Mbps / 8 * 180 = 90MB per 180 seconds
DASHCAM_FREESPACE_LIMIT = 0.15 # we start cleaning up footage when freespace is below 15%
DASHCAM_KEPT = DASHCAM_MAX_SIZE_PER_FILE * 240 # 12 hrs of video = 21GB

DELAY = 0.1 # 10hz

def confd_thread():
  sm = messaging.SubMaster(['thermal'])
  pm = messaging.PubMaster(['dragonConf'])

  dashcam_next_frame = 0
  dashcam_folder_exists = False
  dashcam_mkdir_retry = 0

  last_dp_msg = None
  frame = 0
  update_params = False
  last_modified = None
  idx = 0
  idx_total = len(confs)
  started = False
  free_space = 1
  battery_percent = 0
  overheat = False
  last_charging_ctrl = False
  last_started = False

  while True:
    start_sec = sec_since_boot()
    msg = messaging.new_message('dragonConf')
    if last_dp_msg is not None:
      msg.dragonConf = last_dp_msg

    '''
    ===================================================
    load thermald data every 3 seconds
    ===================================================
    '''
    started, free_space, battery_percent, overheat = pull_thermald(frame, sm, started, free_space, battery_percent, overheat)
    msg.dragonConf.dpThermalStarted = started
    msg.dragonConf.dpThermalOverheat = overheat
    '''
    ===================================================
    push once
    ===================================================
    '''
    if frame == 0:
      setattr(msg.dragonConf, get_struct_name('dp_locale'), locale)
      put_nonblocking('dp_is_updating', '0')
    '''
    ===================================================
    hotspot on boot
    we do it after 30 secs just in case
    ===================================================
    '''
    if frame == 300 and param_get("dp_hotspot_on_boot", "bool", False):
      os.system("service call wifi 37 i32 0 i32 1 &")
    '''
    ===================================================
    check dp_last_modified every 3 seconds
    ===================================================
    '''
    if not update_params and frame % 30 == 0:
      modified = param_get("dp_last_modified", "string", floor(time.time()))
      if last_modified != modified:
        update_params = True
        idx = 0
      last_modified = modified

    '''
    ===================================================
    push param vals to message
    ===================================================
    '''
    # force updating param when `started` changed
    if last_started != started:
      update_params = True
      last_started = started

    if frame == 0:
      msg = update_conf_all(confs, msg)
      idx = idx_total
    elif update_params:
      conf = confs[idx]
      msg = update_conf(frame, msg, conf)
      idx += 1

    if idx >= idx_total:
      idx = 0
      update_params = False

    '''
    ===================================================
    push ip addr every 10 secs
    ===================================================
    '''
    msg = update_ip(frame, msg)
    '''
    ===================================================
    push is_updating status every 5 secs
    ===================================================
    '''
    msg = update_updating(frame, msg)
    '''
    ===================================================
    update msg based on some custom logic
    ===================================================
    '''
    msg = update_custom_logic(msg)
    '''
    ===================================================
    battery ctrl every 30 secs
    PowerMonitor in thermald turns back on every mins
    so lets turn it off more frequent
    ===================================================
    '''
    last_charging_ctrl = process_charging_ctrl(frame, msg, last_charging_ctrl, battery_percent)
    '''
    ===================================================
    dashcam
    ===================================================
    '''
    dashcam_folder_exists, dashcam_mkdir_retry, dashcam_next_frame = process_dashcam(frame, msg, free_space, dashcam_folder_exists, dashcam_mkdir_retry, dashcam_next_frame)
    '''
    ===================================================
    finalise
    ===================================================
    '''
    last_dp_msg = msg.dragonConf
    pm.send('dragonConf', msg)
    frame += 1
    sleep = DELAY-(sec_since_boot() - start_sec)
    if sleep > 0:
      time.sleep(sleep)

def update_conf(frame, msg, conf):
  conf_type = conf.get('conf_type')
  if 'param' in conf_type and 'struct' in conf_type:
    update_this_conf = True

    update_once = conf.get('update_once')
    if update_once is not None and update_once is True and frame > 0:
      update_this_conf = False

    if frame > 0 and update_this_conf:
      update_this_conf = check_dependencies(msg, conf)

    if update_this_conf:
      msg = set_message(msg, conf)
  return msg

def update_conf_all(confs, msg):
  for conf in confs:
    msg = update_conf(0, msg, conf)
  return msg

def process_dashcam(frame, msg, free_space, dashcam_folder_exists, dashcam_mkdir_retry, dashcam_next_frame):
  if msg.dragonConf.dpDashcam and frame % 10 == 0:
    if not dashcam_folder_exists and dashcam_mkdir_retry <= 5:
      # create dashcam folder if not exist
      try:
        if not os.path.exists(DASHCAM_VIDEOS_PATH):
          os.makedirs(DASHCAM_VIDEOS_PATH)
        else:
          dashcam_folder_exists = True
      except OSError:
        dashcam_folder_exists = False
        dashcam_mkdir_retry += 1
    if dashcam_folder_exists:
      # start recording
      if msg.dragonConf.dpThermalStarted:
        if frame >= dashcam_next_frame:
          now = datetime.datetime.now()
          file_name = now.strftime("%Y-%m-%d_%H-%M-%S")
          os.system("screenrecord --bit-rate %s --time-limit %s %s%s.mp4 &" % (DASHCAM_BIT_RATES, DASHCAM_DURATION, DASHCAM_VIDEOS_PATH, file_name))
          dashcam_next_frame = frame + DASHCAM_DURATION * 10 - 2 # frame increases every 0.1 sec
      else:
        dashcam_next_frame = 0
      # clean up
      if frame % 600 == 0 and ((free_space < DASHCAM_FREESPACE_LIMIT) or (get_used_spaces() > DASHCAM_KEPT)):
        try:
          files = [f for f in sorted(os.listdir(DASHCAM_VIDEOS_PATH)) if os.path.isfile(DASHCAM_VIDEOS_PATH + f)]
          os.system("rm -fr %s &" % (DASHCAM_VIDEOS_PATH + files[0]))
        except (IndexError, FileNotFoundError, OSError):
          pass
  return dashcam_folder_exists, dashcam_mkdir_retry, dashcam_next_frame

def process_charging_ctrl(frame, msg, last_charging_ctrl, battery_percent):
  charging_ctrl = msg.dragonConf.dpChargingCtrl
  if last_charging_ctrl != charging_ctrl:
    set_battery_charging(True)
  if charging_ctrl and frame % 300 == 0:
    if battery_percent >= msg.dragonConf.dpDischargingAt and get_battery_charging():
      set_battery_charging(False)
    elif battery_percent <= msg.dragonConf.dpChargingAt and not get_battery_charging():
      set_battery_charging(True)
  return charging_ctrl


def pull_thermald(frame, sm, started, free_space, battery_percent, overheat):
  if frame % 30 == 0:
    sm.update(1000)
    if sm.updated['thermal']:
      started = sm['thermal'].started
      free_space = sm['thermal'].freeSpace
      battery_percent = sm['thermal'].batteryPercent
      overheat = sm['thermal'].thermalStatus >= 2
  return started, free_space, battery_percent, overheat

def update_custom_logic(msg):
  if msg.dragonConf.dpAssistedLcMinMph > msg.dragonConf.dpAutoLcMinMph:
    put_nonblocking('dp_auto_lc_min_mph', str(msg.dragonConf.dpAssistedLcMinMph))
    msg.dragonConf.dpAutoLcMinMph = msg.dragonConf.dpAssistedLcMinMph
  if msg.dragonConf.dpAtl:
    msg.dragonConf.dpAllowGas = True
    msg.dragonConf.dpDynamicFollow = 0
    msg.dragonConf.dpAccelProfile = 0
    msg.dragonConf.dpGearCheck = False
  if msg.dragonConf.dpAppWaze or msg.dragonConf.dpAppHr:
    msg.dragonConf.dpDrivingUi = False
  if not msg.dragonConf.dpDriverMonitor:
    msg.dragonConf.dpUiFace = False
  return msg

def update_updating(frame, msg):
  if frame % 50 == 0:
    setattr(msg.dragonConf, get_struct_name('dp_is_updating'), to_struct_val('dp_is_updating', param_get("dp_is_updating", "bool", False)))
  return msg

def update_ip(frame, msg):
  if frame % 100 == 0:
    val = 'N/A'
    try:
      result = subprocess.check_output(["ifconfig", "wlan0"], encoding='utf8')
      val = re.findall(r"inet addr:((\d+\.){3}\d+)", result)[0][0]
    except:
      pass
    msg.dragonConf.dpIpAddr = val
  return msg


def set_message(msg, conf):
  val = params.get(conf['name'], encoding='utf8')
  if val is not None:
    val = val.rstrip('\x00')
  else:
    val = conf.get('default')
    params.put(conf['name'], str(val))
  struct_val = to_struct_val(conf['name'], val)
  orig_val = struct_val
  if conf.get('min') is not None:
    struct_val = max(struct_val, conf.get('min'))
  if conf.get('max') is not None:
    struct_val = min(struct_val, conf.get('max'))
  if orig_val != struct_val:
    params.put(conf['name'], str(struct_val))
  setattr(msg.dragonConf, get_struct_name(conf['name']), struct_val)
  return msg

def check_dependencies(msg, conf):
  passed = True
  # if has dependency and the depend param val is not in depend_vals, we dont update that conf val
  # this should reduce chance of reading unnecessary params
  dependencies = conf.get('depends')
  if dependencies is not None:
    for dependency in dependencies:
      if getattr(msg.dragonConf, get_struct_name(dependency['name'])) not in dependency['vals']:
        passed = False
        break
  return passed

def get_used_spaces():
  return sum(os.path.getsize(DASHCAM_VIDEOS_PATH + f) for f in os.listdir(DASHCAM_VIDEOS_PATH) if os.path.isfile(DASHCAM_VIDEOS_PATH + f))

def main():
  confd_thread()

if __name__ == "__main__":
  main()
