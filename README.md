# keyboard_firmware

Type this to symilink the keyboard firmware to the QMK firmware folder:
ln -s ~/keyboard_firmware/lilly58 ~/qmk_firmware/keyboards/lily58/keymaps/lilly58 
ln -s ~/keyboard_firmware/corne ~/qmk_firmware/keyboards/crkbd/keymaps/corne

Compile the firmware:
qmk compile -kb lily58 -km oaktoad -e CONVERT_TO=rp2040_ce
qmk compile -kb crkbd -km corne -e CONVERT_TO=rp2040_ce

For the Cone V4 from Aliexpress, you need to use the following website to flash the firmware:
https://vial.rocks/
