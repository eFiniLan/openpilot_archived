#!/usr/bin/bash
#
# SCRIPT: battery_saver.sh
# AUTHOR: Rick Lan <ricklan@gmail.com>
# DATE:   2018-09-10
#
# PURPOSE: Reduce battery usage and heat depending on the status of temp + usb status (connect or disconnect)

# function to loop through available CPUs
# @param $1 set to max when it's 1, set to min when it's 0
set_cpu_freq(){
  cd /sys/devices/system/cpu/
  for c in ./cpu[0-4]* ; do
    check_n_set_freq $1 ${c}
  done
}

# function to set CPU to max/min available frequency
# @param $1 set to max when it's 1, set to min when it's 0
# @param $2 cpu name, e.g. cpu0, cpu1, cpu2. cpu3
check_n_set_freq() {
  cd /sys/devices/system/cpu/
  cpu=$2
  if [ $1 -eq "1" ]; then
    echo set max to $cpu
    freq=`awk '{print $NF}' ./$cpu/cpufreq/scaling_available_frequencies`
  else
    echo set min to $cpu
    freq=`awk '{print $1}' ./$cpu/cpufreq/scaling_available_frequencies`
  fi
  echo $freq > ./$cpu/cpufreq/scaling_max_freq
  echo $freq > ./$cpu/cpufreq/scaling_min_freq
}


# some default values
temp_limit=400 # temp limit - 40 degree
vol_full=4350000 # voltage max
vol_limit=vol_full*0.5 # voltage limit

# when first execute, we set CPU freq to min/max
PREVIOUS=$(cat /sys/class/power_supply/usb/present)
set_cpu_freq $PREVIOUS

# when first execute, we set to start charging
echo 1 > /sys/class/power_supply/battery/charging_enabled

# if USB status changed, we update CPU frequency accordingly.
while [ 1 ]; do
  temp=$(cat /sys/class/power_supply/battery/temp)
  vol_now=$(cat /sys/class/power_supply/battery/voltage_now)

  charging_status=$(cat /sys/class/power_supply/battery/charging_enabled)

  # temp is too high and we still have enough voltage, then we stop charging battery
  if ([ $temp -gt $temp_limit ] && [ $vol_now -gt $vol_limit ]); then
    allow_charge=0
  else
    allow_charge=1
  fi

  if [ $charging_status -ne $allow_charge ]; then
    echo $allow_charge > /sys/class/power_supply/battery/charging_enabled
  fi

  CURRENT=$(cat /sys/class/power_supply/usb/present)
  if [ $CURRENT -ne $PREVIOUS ]; then
    set_cpu_freq $CURRENT
    PREVIOUS=$(echo $CURRENT)
  fi

  sleep 1
done


