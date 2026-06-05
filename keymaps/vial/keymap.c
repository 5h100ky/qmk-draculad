#include QMK_KEYBOARD_H

enum layer_number {
    _BASE,
    _NUM,
    _SYMB,
    _ADJ
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_Q,         KC_W,    KC_E,    KC_R,    KC_T,                                                  KC_Y,             KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,         KC_S,    KC_D,    KC_F,    KC_G,                                                  KC_H,             KC_J,    KC_K,    KC_L,    KC_SCLN,
        LSFT_T(KC_Z), KC_X,    KC_C,    KC_V,    KC_B,                                                  KC_N,             KC_M,    KC_COMM, KC_DOT,  RSFT_T(KC_SLSH),
                                                  KC_MUTE,                                               TG(_ADJ),
                                        KC_LCTL, LALT_T(KC_BSPC), LT(_SYMB, KC_SPC), LT(_NUM, KC_DEL), LT(_SYMB, KC_ENT), KC_CAPS
    ),
    [_NUM] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_TAB,  XXXXXXX, KC_VOLD, KC_VOLU, XXXXXXX,                 KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_QUOT,
        KC_LSFT, XXXXXXX, XXXXXXX, KC_MUTE, QK_BOOT,                 KC_HOME, KC_END,  KC_PGUP, KC_PGDN, KC_RSFT,
                                 XXXXXXX,                             KC_BTN3,
                        XXXXXXX, KC_LALT, XXXXXXX,          _______, KC_BTN1, KC_BTN2
    ),
    [_SYMB] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,                   XXXXXXX, XXXXXXX, XXXXXXX, KC_EQL,  KC_MINS,
        XXXXXXX, KC_F5,   KC_F6,   KC_F7,   KC_F8,                   KC_LBRC, KC_RBRC, XXXXXXX, KC_GRV,  KC_BSLS,
        KC_LSFT, KC_F9,   KC_F10,  KC_F11,  KC_F12,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,
                                 XXXXXXX,                             KC_NO,
                        KC_LALT, XXXXXXX, XXXXXXX,          XXXXXXX, _______, KC_NO
    ),
    [_ADJ] = LAYOUT(
        QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        EE_CLR,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                                 XXXXXXX,                             _______,
                        XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX
    )
};

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    // Split encoder index mapping:
    // 0-1: left half encoders, 2-3: right half encoders.
    if (index == 0) {
        tap_code(clockwise ? KC_VOLU : KC_VOLD);
    } else if (index == 1) {
        tap_code(clockwise ? KC_PGUP : KC_PGDN);
    } else if (index == 2) {
        tap_code(clockwise ? KC_WH_U : KC_WH_D);
    } else if (index == 3) {
        tap_code(clockwise ? KC_UP : KC_DOWN);
    }
    return true;
}
#endif
