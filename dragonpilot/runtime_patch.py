#!/usr/bin/env python3
import subprocess

try:
  with open("/data/params/d/dp_atl") as f:
    if int(f.read()) == 1:
      # toyota
      # subprocess.call(['sed -i -e "s/(it.should_log/(name != \"health\" \&\& name != \"controlsState\" \&\& name != \"carControl\" \&\& name != \"carState\" \&\& name != \"carEvents\" \&\& it.should_log/g" /data/openpilot/selfdrive/loggerd/loggerd.cc'])
      subprocess.call(['sed -i -e "s/int is_acc_msg = (addr == 0x343);/int is_acc_msg = false;/g" /data/openpilot/panda/board/safety/safety_toyota.h'], shell=True)
      subprocess.call(['sed -i -e "s/^    \/\/ 0x2E4/    controls_allowed = 1;\\n    \/\/\/ 0x2E4/g" /data/openpilot/panda/board/safety/safety_toyota.h'], shell=True)
      # honda
      subprocess.call(['sed -i -e "s/^    \/\/ if steering controls/    controls_allowed = 1;\\n    \/\/\/ if steering controls/g" /data/openpilot/panda/board/safety/safety_honda.h'], shell=True)
      # subprocess.call(['sed -i -e "s/^  bool current_controls_allowed = controls_allowed \&\& !(pedal_pressed);/  bool current_controls_allowed = controls_allowed;/g" /data/openpilot/panda/board/safety/safety_honda.h'], shell=True)
      # subprocess.call(['sed -i -e "s/^  if ((addr == 0x1FA) && (bus == bus_pt)) {/  if (false) {/g" /data/openpilot/panda/board/safety/safety_honda.h'], shell=True)
      # subprocess.call(['sed -i -e "s/^  if ((addr == 0x1DF) && (bus == bus_pt)) {/  if (false) {/g" /data/openpilot/panda/board/safety/safety_honda.h'], shell=True)
except FileNotFoundError:
  pass