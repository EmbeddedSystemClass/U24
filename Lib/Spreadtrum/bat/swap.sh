echo 77 > /sys/class/gpio/export
cd /sys/class/gpio/gpio77
echo "out" > direction
echo "0" > value
echo "1" > /sys/class/power_supply/battery/ext_modem_pwr_on
echo 53 > /sys/class/gpio/export
cd /sys/class/gpio/gpio53
echo "out" > direction
echo "1" > value
