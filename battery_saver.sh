#!/usr/bin/bash
#
# SCRIPT: battery_saver.sh
# AUTHOR: Rick Lan <ricklan@gmail.com>
# DATE:   2018-09-10
#
# PURPOSE: Reduce battery usage and heat depending on the status of temp + usb status (connect or disconnect)
#          * when battery capacity is greater than bat_limit, and battery temp is greater than temp_limit, then stop charging
#          * when phone is not connected to USB, then change CPU max scale freq to minimum.

# default values
temp_limit=400 # temp limit - 40 degree
bat_limit=50 # battery limit (percentage)

# function to loop through available CPUs
# @param $1 set to max when it's 1, set to min when it's 0
set_cpu_freq(){
  cd /sys/devices/system/cpu/
  for c in ./cpu[0-4]* ; do
    check_n_set_freq $1 ${c}
  done
}

# function to set CPU to max/min available frequency (max scaling only)
# @param $1 set to max when it's 1, set to min when it's 0
# @param $2 cpu name, e.g. cpu0, cpu1, cpu2. cpu3
check_n_set_freq() {
  cd /sys/devices/system/cpu/

  if [ $1 -eq "1" ]; then
    # get max cpu freq
    freq=`awk '{print $NF}' ./$2/cpufreq/scaling_available_frequencies`
  else
    # get min cpu freq
    freq=`awk '{print $1}' ./$2/cpufreq/scaling_available_frequencies`
  fi

  # set max/min freq to scaling_max_freq
  echo $freq > ./$2/cpufreq/scaling_max_freq

  # we dont modify min scaling freq for now.
  #echo $freq > ./$2/cpufreq/scaling_min_freq
}


##### logic start here #####

# when first execute, we set CPU freq to min/max
PREVIOUS=$(cat /sys/class/power_supply/usb/present)
set_cpu_freq $PREVIOUS

# when first execute, we set to start charging
echo 1 > /sys/class/power_supply/battery/charging_enabled

# loop every second
while [ 1 ]; do
  # retrieve values
  temp=$(cat /sys/class/power_supply/battery/temp)
  bat_now=$(cat /sys/class/power_supply/battery/capacity)
  charging_status=$(cat /sys/class/power_supply/battery/charging_enabled)

  # if temp is high AND we still have enough battery capacity, then we stop charging the battery
  if ([ $temp -gt $temp_limit ] && [ $bat_now -gt $bat_limit ]); then
    allow_charge=0
  else
    allow_charge=1
  fi

  # set battery charging status
  if [ $charging_status -ne $allow_charge ]; then
    echo $allow_charge > /sys/class/power_supply/battery/charging_enabled
  fi

  # if USB status changed, we update CPU frequency accordingly.
  CURRENT=$(cat /sys/class/power_supply/usb/present)
  if [ $CURRENT -ne $PREVIOUS ]; then
    set_cpu_freq $CURRENT
    PREVIOUS=$(echo $CURRENT)
  fi

  sleep 1
done


