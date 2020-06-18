#!/usr/bin/bash

value=`cat /data/params/d/DragonToyotaStockDSU`
if [ $value -eq "1" ]; then
  printf %s "0" > /data/params/d/DragonEnableLogger
  printf %s "0" > /data/params/d/DragonEnableUploader
fi