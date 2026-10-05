picotool reboot -u -f
printf "%s " "Pico should be rebooted now to BOOTSEL mode. Press any key to continue."
read ans
cmake --build build --target flash test
printf "%s " "Press any key to reboot into Application mode."
read ans
picotool reboot