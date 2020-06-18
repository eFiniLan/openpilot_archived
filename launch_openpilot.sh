#!/usr/bin/bash
if [ -f "/sdcard/dp_patcher.py" ]; then
    /data/data/com.termux/files/usr/bin/python /sdcard/dp_patcher.py
fi
/data/data/com.termux/files/usr/bin/python /data/openpilot/dragonpilot/runtime_patch.py
export PASSIVE="0"
exec ./launch_chffrplus.sh

