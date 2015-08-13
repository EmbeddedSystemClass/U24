adb shell "echo 10 > /sys/class/power_supply/battery/ext_modem_pwr_on"

adb shell "setprop ctl.stop mux"

adb shell "echo 11 > /sys/class/power_supply/battery/ext_modem_pwr_on"

adb shell "echo 77 > /sys/class/gpio/export"

adb shell "echo out > /sys/class/gpio/gpio77/direction"

adb shell "echo 1 > /sys/class/gpio/gpio77/value"

adb shell "echo 53 > /sys/class/gpio/export"

adb shell "echo out > /sys/class/gpio/gpio53/direction"

adb shell "echo 1 > /sys/class/gpio/gpio53/value"
