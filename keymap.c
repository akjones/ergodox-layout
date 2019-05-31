#include QMK_KEYBOARD_H
#include "version.h"

#define KC_MAC_UNDO LGUI(KC_Z)
#define KC_MAC_CUT LGUI(KC_X)
#define KC_MAC_COPY LGUI(KC_C)
#define KC_MAC_PASTE LGUI(KC_V)
#define KC_PC_UNDO LCTL(KC_Z)
#define KC_PC_CUT LCTL(KC_X)
#define KC_PC_COPY LCTL(KC_C)
#define KC_PC_PASTE LCTL(KC_V)

typedef enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    DOUBLE_HOLD = 4,
    DOUBLE_SINGLE_TAP = 5, //send two single taps
    TRIPLE_TAP = 6,
    TRIPLE_HOLD = 7
} td_state_t;

static td_state_t td_state;

enum custom_keycodes {
    RGB_SLD = SAFE_RANGE, // can always be here
    TOGGLE_LAYER_COLOR,
    EPRM,
};

enum {
    COLEMAK = 0,
    QWERTY,
    SYM_AND_MOV,
    MEDIA,
};

enum {
    CT_LBP,
    CT_RBP,
};

int cur_dance (qk_tap_dance_state_t *state);
static void _td_brackets_finished (qk_tap_dance_state_t *state, void *user_data);
static void _td_brackets_reset (qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [COLEMAK] = LAYOUT_ergodox(
        KC_ESCAPE,KC_1,KC_2,KC_3,KC_4,KC_5,KC_ASTG,
        KC_TAB,KC_Q,KC_W,KC_F,KC_G,KC_J,TD(CT_LBP),
        KC_LCTRL,KC_A,KC_R,KC_S,KC_T,KC_D,
        KC_LSHIFT,KC_Z,KC_X,KC_C,KC_V,KC_B,KC_HYPR,
        OSL(SYM_AND_MOV),KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,

        KC_LGUI,KC_DELETE,KC_HOME,KC_BSPACE,KC_LALT,KC_END,


        KC_TRANSPARENT,KC_6,KC_7,KC_8,KC_9,KC_0,KC_TRANSPARENT,
        TD(CT_RBP),KC_J,KC_L,KC_U,KC_Y,KC_SCOLON,KC_BSLASH,
        KC_H,KC_N,KC_E,KC_I,KC_O,KC_QUOTE,
        KC_MEH,KC_K,KC_M,KC_COMMA,KC_DOT,KC_SLASH,KC_TRANSPARENT,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,TO(QWERTY),

        OSL(MEDIA),KC_ESCAPE,KC_PGUP,KC_PGDOWN,KC_ENTER,KC_SPACE
        ),

    [QWERTY] = LAYOUT_ergodox(
        KC_ESCAPE,KC_1,KC_2,KC_3,KC_4,KC_5,KC_ASTG,
        KC_TAB,KC_Q,KC_W,KC_E,KC_R,KC_T,TD(CT_LBP),
        KC_LCTRL,KC_A,KC_S,KC_D,KC_F,KC_G,
        KC_LSHIFT,KC_Z,KC_X,KC_C,KC_V,KC_B,KC_HYPR,
        OSL(SYM_AND_MOV),KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,

        KC_LGUI,KC_DELETE,KC_HOME,KC_BSPACE,KC_LALT,KC_END,


        KC_TRANSPARENT,KC_6,KC_7,KC_8,KC_9,KC_0,KC_TRANSPARENT,
        TD(CT_RBP),KC_Y,KC_U,KC_I,KC_O,KC_P,KC_BSLASH,
        KC_H,KC_J,KC_K,KC_L,KC_SCOLON,KC_QUOTE,
        KC_MEH,KC_N,KC_M,KC_COMMA,KC_DOT,KC_SLASH,KC_QUES,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,TO(COLEMAK),

        OSL(MEDIA),KC_ESCAPE,KC_PGUP,KC_PGDOWN,KC_ENTER,KC_SPACE
        ),

    [SYM_AND_MOV] = LAYOUT_ergodox(
        KC_ESCAPE,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_EXLM,KC_AT,KC_LCBR,KC_RCBR,KC_PIPE,KC_TRANSPARENT,
        KC_LCTRL,KC_HASH,KC_DLR,KC_LPRN,KC_RPRN,KC_GRAVE,
        KC_TRANSPARENT,KC_PERC,KC_CIRC,KC_LBRACKET,KC_RBRACKET,KC_TILD,KC_TRANSPARENT,
        OSL(SYM_AND_MOV),KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,

        KC_LGUI,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_LALT,KC_TRANSPARENT,


        KC_TRANSPARENT,KC_F6,KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MINUS,KC_TRANSPARENT,KC_ASTR,KC_F12,
        KC_LEFT,KC_DOWN,KC_UP,KC_RIGHT,KC_PLUS,KC_TRANSPARENT,
        KC_TRANSPARENT,KC_AMPR,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_BSLASH,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_DOT,KC_0,KC_EQUAL,KC_TRANSPARENT,

        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT
        ),

    [MEDIA] = LAYOUT_ergodox(
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MS_UP,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_MS_LEFT,KC_MS_DOWN,KC_MS_RIGHT,KC_TRANSPARENT,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MS_BTN1,KC_MS_BTN2,

        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,


        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MEDIA_PLAY_PAUSE,
        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MEDIA_PREV_TRACK,KC_MEDIA_NEXT_TRACK,KC_TRANSPARENT,KC_TRANSPARENT,
        KC_AUDIO_VOL_UP,KC_AUDIO_VOL_DOWN,KC_AUDIO_MUTE,KC_TRANSPARENT,KC_TRANSPARENT,

        KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_WWW_BACK
        ),
};

int cur_dance (qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)  return SINGLE_TAP;
        //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
        else return SINGLE_HOLD;
    }
    else if (state->count == 2) {
        /*
         * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
         * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
         * keystrokes of the key, and not the 'double tap' action/macro.
         */
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    //Assumes no one is trying to type the same letter three times (at least not quickly).
    //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
        else return TRIPLE_HOLD;
    }
    else return 8; //magic number. At some point this method will expand to work for more presses
}


static void _td_brackets_finished (qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);

    switch(td_state) {
    case SINGLE_TAP:
        if (state->keycode == TD(CT_LBP))
            register_code16(KC_LBRC);
        else
            register_code16(KC_RBRC);
        break;
    case SINGLE_HOLD:
        if (state->keycode == TD(CT_LBP))
            register_code16(KC_LCBR);
        else
            register_code16(KC_RCBR);
        break;
    case DOUBLE_TAP:
        if (state->keycode == TD(CT_LBP))
            register_code16(KC_LPRN);
        else
            register_code16(KC_RPRN);
        break;
    default: break;
    }
}

static void _td_brackets_reset (qk_tap_dance_state_t *state, void *user_data) {
    switch(td_state) {
    case SINGLE_TAP:
        if (state->keycode == TD(CT_LBP))
            unregister_code16(KC_LBRC);
        else
            unregister_code16(KC_RBRC);
        break;
    case SINGLE_HOLD:
        if (state->keycode == TD(CT_LBP))
            unregister_code16(KC_LCBR);
        else
            unregister_code16(KC_RCBR);
        break;
    case DOUBLE_TAP:
        if (state->keycode == TD(CT_LBP))
            unregister_code16(KC_LPRN);
        else
            unregister_code16(KC_RPRN);
        break;
    default: break;
    }

    td_state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {[CT_LBP] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, _td_brackets_finished, _td_brackets_reset),
    [CT_RBP] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, _td_brackets_finished, _td_brackets_reset)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case EPRM:
        if (record->event.pressed) {
            eeconfig_init();
        }
        return false;
    }
    return true;
}

uint32_t layer_state_set_user(uint32_t state) {
    uint8_t layer = biton32(state);
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
    case 1:
        ergodox_right_led_1_on();
        break;
    case 2:
        ergodox_right_led_2_on();
        break;
    case 3:
        ergodox_right_led_3_on();
        break;
    case 4:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        break;
    case 5:
        ergodox_right_led_1_on();
        ergodox_right_led_3_on();
        break;
    case 6:
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        break;
    case 7:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        break;
    default:
        break;
    }
    return state;
};
