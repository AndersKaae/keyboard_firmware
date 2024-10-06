# keyboard_firmware

Type this to symilink the keyboard firmware to the QMK firmware folder:
ln -s ~/keyboard_firmware/oaktoad ~/qmk_firmware/keyboards/lily58/keymaps/oaktoad

Compile the firmware:
qmk compile -kb lily58 -km oaktoad -e CONVERT_TO=rp2040_ce
