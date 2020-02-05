#/bin/bash

eval "$(cat /sys/class/power_supply/BAT0/uevent | sed 13q )"

current=`printf %.2f "$((10000*POWER_SUPPLY_CHARGE_NOW/POWER_SUPPLY_CHARGE_FULL))e-2"`
timeleft=`printf %.4f "$((POWER_SUPPLY_CHARGE_NOW/POWER_SUPPLY_CURRENT_NOW))"`
hoursleft=$(())
minsleft=$(())
