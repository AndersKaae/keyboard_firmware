/*

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "eeconfig.h"

// Stores/reads a single user byte in eeconfig (portable across AVR/ARM)
enum os_type {
    OS_LINUX = 0,
    OS_MAC = 1
};

static uint8_t current_os = OS_MAC;

void eeconfig_init_user(void) {
    // Called when eeconfig is reset (e.g., via bootmagic or EEPROM clear)
    current_os = OS_MAC;
    eeconfig_update_user(current_os);
}

void keyboard_post_init_user(void) {
    // Load OS from eeconfig; sanitize in case EEPROM is uninitialized/garbage
    uint8_t v = eeconfig_read_user();
    if (v > OS_MAC) {          // guard against 0xFF or invalid values
        v = OS_MAC;
        eeconfig_update_user(v);
    }
    current_os = v;
}

void set_os(uint8_t os) {
    current_os = os;
    eeconfig_update_user(os);
}

enum custom_keycodes {
    TOGGLE_OS = QK_USER,
    AT_SIGN,
    DOLLAR,
    LESS_THAN,
    GREATER_THAN,
    L_BRACKET,
    R_BRACKET
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#if defined(CONSOLE_ENABLE) && CONSOLE_ENABLE
    if (record->event.pressed) {
        uint8_t lyr = get_highest_layer(layer_state);
        uprintf("press: kc=%u layer=%u row=%u col=%u\n",
                keycode, lyr, record->event.key.row, record->event.key.col);
    } else {
        return true;
    }
#else
    if (!record->event.pressed) return true;
#endif

    switch (keycode) {
        case TOGGLE_OS:
            set_os(current_os == OS_LINUX ? OS_MAC : OS_LINUX);
            return false;

        case AT_SIGN:
            if (current_os == OS_MAC) tap_code16(LALT(KC_NUHS));
            else                      tap_code16(RALT(KC_2));
            return false;

        case DOLLAR:
            if (current_os == OS_MAC) tap_code16(LALT(LSFT(KC_3)));
            else                      tap_code16(RALT(KC_4));
            return false;

        case LESS_THAN:
            if (current_os == OS_MAC) tap_code16(KC_GRV);
            else                      tap_code16(KC_NUBS);
            return false;

        case GREATER_THAN:
            if (current_os == OS_MAC) tap_code16(LALT(LSFT(KC_GRV)));
            else                      tap_code16(LSFT(KC_NUBS));
            return false;

        case L_BRACKET:
            if (current_os == OS_MAC) tap_code16(LALT(KC_8));
            else                      tap_code16(RALT(KC_8));
            return false;

        case R_BRACKET:
            if (current_os == OS_MAC) tap_code16(LALT(KC_9));
            else                      tap_code16(RALT(KC_9));
            return false;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_LBRC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT, 
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   MO(1),  KC_SPC,     KC_ENT,   MO(2), KC_BSPC
                                      //`--------------------------'  `--------------------------'

  ),

    [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_NUHS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,  XXXXXXX, XXXXXXX,LALT(KC_RBRC), KC_MINS, KC_UNDS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  _______,    _______,  MO(3), _______
                                      //`--------------------------'  `--------------------------'
  ),

    [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, AT_SIGN,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB, XXXXXXX, DOLLAR, L_BRACKET, R_BRACKET, LALT(KC_I),                KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, XXXXXXX,  XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, RALT(KC_7), RALT(KC_0), LESS_THAN, GREATER_THAN, XXXXXXX,                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   MO(3),  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MUTE, KC_VOLD, KC_VOLU, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, XXXXXXX, XXXXXXX,                      XXXXXXX, TOGGLE_OS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RM_NEXT, RM_HUED, RM_SATD, RM_VALD, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
};
#endif

#ifdef OLED_ENABLE

#include "print.h"
#include "wpm.h"

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
}

const char *get_layer_name(uint8_t layer) {
    switch (layer) {
        case 0: return "Base";
        case 1: return "Lower";
        case 2: return "Raise";
        case 3: return "Fn";
        default: return "???";
    }
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // Row 1: Layer
        uint8_t layer = get_highest_layer(layer_state);
        char layer_str[20];
        snprintf(layer_str, sizeof(layer_str), "Layer: %s", get_layer_name(layer));
        oled_write_ln(layer_str, false);

        // Row 2: WPM
        char wpm_str[20];
        snprintf(wpm_str, sizeof(wpm_str), "WPM:   %3d", get_current_wpm());
        oled_write_ln(wpm_str, false);

        // Row 3: OS Mode
        char os_str[20];
        snprintf(os_str, sizeof(os_str), "OS Mode: %s", current_os == OS_MAC ? "Mac" : "Linux");
        oled_write_ln(os_str, false);
    } else {
        oled_write_ln(PSTR(""), false);
    }
    return false;
}

#endif
