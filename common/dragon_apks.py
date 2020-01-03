import os
import subprocess
import glob
import hashlib
from common.basedir import BASEDIR
from selfdrive.swaglog import cloudlog
from common.apk import appops_set, pm_grant, system, install_apk, get_installed_apks
from common.params import Params
params = Params()


android_packages = ("com.waze", "tw.com.ainvest.outpack", "com.autonavi.amapauto", "com.mixplorer", "com.tomtom.speedcams.android.map")

def init_git():
  try:
    r = subprocess.check_output(NICE_LOW_PRIORITY + ["git", "submodule", "init"], stderr=subprocess.STDOUT).decode('utf8')
    cloudlog.info("git submodule init success: %s", r)
  except subprocess.CalledProcessError as e:
    cloudlog.event("git submodule init failed",
                   cmd=e.cmd,
                   output=e.output,
                   returncode=e.returncode)

def update_git():
  try:
    r = subprocess.check_output(NICE_LOW_PRIORITY + ["git", "submodule", "update"], stderr=subprocess.STDOUT).decode('utf8')
    cloudlog.info("git submodule update success: %s", r)
  except subprocess.CalledProcessError as e:
    cloudlog.event("git submodule update failed",
                   cmd=e.cmd,
                   output=e.output,
                   returncode=e.returncode)

def set_dragon_package_permissions():
  pm_grant("com.waze", "android.permission.ACCESS_FINE_LOCATION")
  appops_set("com.waze", "READ_EXTERNAL_STORAGE", "allow")
  appops_set("com.waze", "WRITE_EXTERNAL_STORAGE", "allow")

  pm_grant("tw.com.ainvest.outpack", "android.permission.ACCESS_FINE_LOCATION")
  appops_set("tw.com.ainvest.outpack", "READ_EXTERNAL_STORAGE", "allow")
  appops_set("tw.com.ainvest.outpack", "WRITE_EXTERNAL_STORAGE", "allow")

  pm_grant("com.autonavi.amapauto", "android.permission.ACCESS_FINE_LOCATION")
  appops_set("com.autonavi.amapauto", "READ_EXTERNAL_STORAGE", "allow")
  appops_set("com.autonavi.amapauto", "WRITE_EXTERNAL_STORAGE", "allow")

  pm_grant("com.tomtom.speedcams.android.map", "android.permission.ACCESS_FINE_LOCATION")
  appops_set("com.tomtom.speedcams.android.map", "READ_EXTERNAL_STORAGE", "allow")
  appops_set("com.tomtom.speedcams.android.map", "WRITE_EXTERNAL_STORAGE", "allow")

# *** external functions ***

def update_dragon_apks():
  dragon_apps = True if params.get("DragonApps", encoding='utf8') == "1" else False
  if dragon_apps:
    # install apks
    installed = get_installed_apks()
    install_apks = glob.glob(os.path.join(BASEDIR, "dragon_apk/*.apk"))
    # init submodule
    if not install_apks:
      init_git()
    # update submodule
    update_git()

    for apk in install_apks:
      app = os.path.basename(apk)[:-4]
      if app not in installed:
        installed[app] = None

    # cloudlog.info("installed apks %s" % (str(installed), ))

    for app in installed.keys():
      apk_path = os.path.join(BASEDIR, "dragon_apk/"+app+".apk")
      if not os.path.exists(apk_path):
        continue

      h1 = hashlib.sha1(open(apk_path, 'rb').read()).hexdigest()
      h2 = None
      if installed[app] is not None:
        h2 = hashlib.sha1(open(installed[app], 'rb').read()).hexdigest()
        cloudlog.info("comparing version of %s  %s vs %s" % (app, h1, h2))

      if h2 is None or h1 != h2:
        cloudlog.info("installing %s" % app)

        success = install_apk(apk_path)
        if not success:
          cloudlog.info("needing to uninstall %s" % app)
          system("pm uninstall %s" % app)
          success = install_apk(apk_path)

        assert success
    pm_apply_dragon_packages('disable')
  else:
    pm_apply_dragon_packages('uninstall')

def pm_apply_dragon_packages(cmd):
  for p in android_packages:
    system("pm %s %s" % (cmd, p))

if __name__ == "__main__":
  update_dragon_apks()

