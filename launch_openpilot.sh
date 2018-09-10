#!/usr/bin/bash

/system/bin/sh ./battery_saver.sh &
export PASSIVE="0"
exec ./launch_chffrplus.sh

