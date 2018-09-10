#!/usr/bin/bash
#
# SCRIPT: battery_saver.sh
# AUTHOR: Rick Lan <ricklan@gmail.com>
# DATE:   2018-09-10
#
# PURPOSE: Set CPU to the minimum frequency if unplugged from USB, otherwise set to maximum frequency

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

# when first execute, we set CPU freq to min/max
PREVIOUS=$(cat /sys/class/power_supply/usb/online)
set_cpu_freq $PREVIOUS

# if USB status changed, we update CPU frequency accordingly.
while [ 1 ]; do
  CURRENT=$(cat /sys/class/power_supply/usb/online)

  if [ $CURRENT -ne $PREVIOUS ]; then
    set_cpu_freq $CURRENT
    PREVIOUS=$(echo $CURRENT)
  fi

  sleep 1
done
