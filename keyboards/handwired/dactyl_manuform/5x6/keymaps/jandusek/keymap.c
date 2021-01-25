/* An updated layout for the Dactyl Manuform 5x6 Keyboard with Rotary Encoders */ 
#include QMK_KEYBOARD_H

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 16

enum custom_keycodes {
    M_MIS_CON = SAFE_RANGE,
};

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define ADJUST MO(_ADJUST)

bool is_gui_tab_active = false;
uint16_t gui_tab_timer = 0;

// Backlight timeout feature
#define BACKLIGHT_TIMEOUT 5   // in minutes
static uint16_t idle_timer = 0;
static uint8_t halfmin_counter = 0;

// defining Adjust layer
uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT_5x6(
     KC_GRAVE , KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                      KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_MINS,
     KC_TAB ,   KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                      KC_Y  , KC_U  , KC_I  , KC_O  , KC_P , KC_BSLASH,
     KC_ESC,    KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                      KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN, KC_QUOT,
     KC_LSFT,   KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                      KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH, KC_BSPC,
                      KC_LALT, KC_LCTL,                                                     KC_EQUAL, _______,
                                      RAISE, KC_LGUI,                        KC_SPC, LOWER,
                                      KC_ENT, KC_HOME,                       KC_END,  KC_DEL,
                                      KC_BSPC, _______,                      M_MIS_CON, KC_LALT
  ),

  [_LOWER] = LAYOUT_5x6(

     KC_TILD,KC_EXLM, KC_AT ,KC_HASH,KC_DLR ,KC_PERC,                        KC_CIRC,KC_AMPR,KC_ASTR,KC_LPRN,KC_RPRN,KC_UNDS,
     _______,_______,_______,_______,_______,KC_LBRC,                        KC_RBRC, KC_P7 , KC_P8 , KC_P9 ,_______,KC_PLUS,
     _______,KC_HOME,KC_PGUP,KC_PGDN,KC_END ,KC_LPRN,                        KC_RPRN, KC_P4 , KC_P5 , KC_P6 ,KC_MINS,KC_PIPE,
     _______,_______,_______,_______,_______,KC_LCBR,                        KC_RCBR, KC_P1 , KC_P2 , KC_P3 ,KC_EQL ,KC_DEL,
                     _______,KC_PSCR,                                                         KC_P0, _______, 
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______

  ),

  [_RAISE] = LAYOUT_5x6(
       KC_F12 , KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 ,                      KC_F6  , KC_F7 , KC_F8 , KC_F9 ,KC_F10 ,KC_F11 ,
       _______,_______,_______,_______,_______,KC_LBRC,                      KC_RBRC,_______,KC_NLCK,KC_INS ,KC_SLCK,KC_MUTE,
       _______,KC_LEFT,KC_UP  ,KC_DOWN,KC_RGHT,KC_LPRN,                      KC_RPRN,KC_MPRV,KC_MPLY,KC_MNXT,_______,KC_VOLU,
       _______,_______,_______,_______,_______,KC_LCBR,                      KC_RCBR,_______,_______,_______,_______,KC_VOLD,
                       _______,_______,                                                      KC_EQL ,_______,
                                       _______,_______,                      _______,_______,
                                       _______,_______,                      _______,_______,
                                       _______,_______,                      _______,_______
  ),

  [_ADJUST] = LAYOUT_5x6(
       _______,_______,_______,_______,_______,_______,                      _______,_______,_______,_______,_______,_______,
       RGB_TOG,RGB_HUI,RGB_SAI,RGB_VAI,_______,_______,                      _______,_______,_______,_______,_______,_______,
       RGB_MOD,RGB_HUD,RGB_SAD,RGB_VAD,_______,_______,                      _______,_______,_______,_______,_______,_______,
       RGB_M_P,RGB_M_B,RGB_M_R,_______,_______,_______,                      _______,_______,_______,_______,_______,_______,
                       _______,_______,                                                      _______,_______,
                                       _______,_______,                      _______,_______,
                                       _______,_______,                      _______,_______,
                                       _______,_______,                      _______,_______
  ),   
};

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (biton32(layer_state)) {
            case _QWERTY:
                // Move between virtual desktop spaces
                if (!is_gui_tab_active) {
                    is_gui_tab_active = true;
                    register_code(KC_LGUI);
                }
                gui_tab_timer = timer_read();
                if (clockwise) {
                    tap_code16(S(KC_RIGHT));
                } else {
                    tap_code16(S(KC_LEFT));
                }
                break;
            default:
                // Volume control.
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
        }
    } else if (index == 1) {
        switch (biton32(layer_state)) {
            case _QWERTY:
                if (!is_gui_tab_active) {
                    is_gui_tab_active = true;
                    register_code(KC_LGUI);
                }
                gui_tab_timer = timer_read();
                if (clockwise) {
                    tap_code16(S(KC_RBRC));
                } else {
                    tap_code16(S(KC_LBRC));
                }
                break;                
            default:
                // Scrolling with PageUp and PgDn.
                if (clockwise) {
                    tap_code(KC_PGDN);
                } else {
                    tap_code(KC_PGUP);
                }
                break;
        }
    }
}
#endif

#ifdef RGBLIGHT_ENABLE
void keyboard_post_init_user(void) {
  rgblight_sethsv(HSV_ORANGE);
  rgblight_enable_noeeprom(); // Enables RGB, without saving settings
  rgblight_sethsv_noeeprom(HSV_ORANGE);
  wait_us(175);  // Add a slight delay between color and mode to ensure it's processed correctly
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}

void rgblight_set_hsv_and_mode(uint8_t hue, uint8_t sat, uint8_t val, uint8_t mode) {
    rgblight_sethsv_noeeprom(hue, sat, val);
    wait_us(175);  // Add a slight delay between color and mode to ensure it's processed correctly
    rgblight_mode_noeeprom(mode);
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef RGBLIGHT_ENABLE
    if (record->event.pressed) {
        if (rgblight_is_enabled() == false) {
            rgblight_enable_noeeprom();
        }
        idle_timer = timer_read();
        halfmin_counter = 0;
    }
#endif
    switch (keycode) {
        case M_MIS_CON:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_UP))));
            } else {
                // when keycode QMKBEST is released
            }
            break;
    }
    return true;
}

void matrix_scan_user(void) {
    if (is_gui_tab_active) {
      if (timer_elapsed(gui_tab_timer) > 500) {
          unregister_code(KC_LGUI);
         is_gui_tab_active = false;
      }
    }

    // idle_timer needs to be set one time
    if (idle_timer == 0) idle_timer = timer_read();

    #ifdef RGBLIGHT_ENABLE
        if ( rgblight_is_enabled() == true && timer_elapsed(idle_timer) > 30000) {
            halfmin_counter++;
            idle_timer = timer_read();
        }

        if ( rgblight_is_enabled() == true && halfmin_counter >= BACKLIGHT_TIMEOUT * 2) {
            rgblight_disable_noeeprom();
            halfmin_counter = 0;
        }
    #endif
}
