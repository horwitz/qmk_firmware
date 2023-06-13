/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * to debug, set DEBUG to 1 (as opposed to 0) and make sure (horwitz/)rules.mk contains "CONSOLE_ENABLE = yes"; then use
 * uprintf to print debug output
 */
#define DEBUG 0

#include QMK_KEYBOARD_H
#if DEBUG
    #include "print.h"
#endif

// clang-format off
enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN,
  CLR_PKR
};
#define NUM_LAYERS DYNAMIC_KEYMAP_LAYER_COUNT // TODO? derive from elsewhere? inline?
#define NUM_KEYS 84 // TODO? derive from... somewhere

enum ctrl_keycodes {
    RED0 = NEW_SAFE_RANGE, // if instead set to SAFE_RANGE, collisions occur (e.g., KC_LOPTN == RED0, ...)
    RED5,
    RED10,
    RED15,
    ORNG21,
    ORNG26,
    ORNG31,
    ORNG36,
    YLLW43,
    YLLW53,
    YLLW63,
    YLLW73,
    GRN85,
    GRN95,
    GRN105,
    GRN115,
    CYAN127,
    CYAN132,
    CYAN137,
    CYAN142,
    AZRE148,
    AZRE153,
    AZRE158,
    AZRE163,
    BLUE169,
    BLUE172,
    BLUE175,
    BLUE178,
    VILT180,
    VILT185,
    VILT190,
    VILT195,
    MGTA201,
    MGTA206,
    MGTA211,
    MGTA217,
    ROSE222,
    ROSE230,
    ROSE238,
    ROSE245
};

#if DEBUG
    void keyboard_post_init_user(void) {
        // Customise these values to desired behaviour
        debug_enable=true;
        debug_matrix=true;
        debug_keyboard=true;
    //    debug_mouse=true;
    }
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_SNAP,  KC_DEL,   RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD,MO(MAC_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_ansi_84(
     KC_TRNS,  KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_TRNS,  KC_TRNS,  RGB_TOG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,OSL(CLR_PKR),BAT_LVL, NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[WIN_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_DEL,   RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT, MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN_FN] = LAYOUT_ansi_84(
     KC_TRNS,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_TRNS,  KC_TRNS,  RGB_TOG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,OSL(CLR_PKR),BAT_LVL, NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[CLR_PKR] = LAYOUT_ansi_84(
     TG(MAC_BASE),_______,_______, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  RED0,     ORNG21,   YLLW43,   GRN85,    CYAN127,  AZRE148,  BLUE169,  VILT180,  MGTA201,  ROSE222,  _______,  _______,  _______,            _______,
     _______,  RED5,     ORNG26,   YLLW53,   GRN95,    CYAN132,  AZRE153,  BLUE172,  VILT185,  MGTA206,  ROSE230,  _______,  _______,  _______,            _______,
     _______,  RED10,    ORNG31,   YLLW63,   GRN105,   CYAN137,  AZRE158,  BLUE175,  VILT190,  MGTA211,  ROSE238,  _______,            _______,            _______,
     _______,            RED15,    ORNG36,   YLLW73,   GRN115,   CYAN142,  AZRE163,  BLUE178,  VILT195,  MGTA217,  ROSE245,            _______,  _______,  _______,
     _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______)
};

/*
 * sets the values in [layer_used_indices] to the indices (in increasing order; a subset of 0-83) that are used in the
 * given [layer] ("used" = not KC_TRNS) and returns how many values were set. (e.g., if keys at index 0, 5, 83 are the
 * only ones set, layer_used_indices will start { 0, 5, 83, ... } (followed by 0s) and [initialize_layer_used_indices]
 * will return 3.)
 *
 * why is [offset] used below? rgb_matrix_set_color's first argument appears to be based on an index of keys _not_
 * including KC_NO--there are 96 (=[MATRIX_ROWS] * [MATRIX_COLS]) keycodes in each keymaps[layer], but (apparently) the
 * non-KC_NO keys are indexed 0-83 (and there are 96-84=12 instances of KC_NO (per layer))... see LAYOUT_ansi_84's
 * definition in obj_keychron_k3_pro_ansi_rgb/src/default_keyboard.h
 */
 int initialize_layer_used_indices(int layer, char* layer_used_indices) {
    int lui_i = 0;
    int offset = 0;
    for (int i = 0; i < MATRIX_ROWS * MATRIX_COLS; ++i) {
        int col = i % MATRIX_COLS;
        int row = i / MATRIX_COLS;
        switch (keymaps[layer][row][col]) {
            case KC_TRNS:
                break;
            case KC_NO:
                ++offset;
                break;
            default:
                layer_used_indices[lui_i++] = i - offset;
                break;
        }
    }

    return lui_i;
}

char layers_used_indices[NUM_LAYERS][NUM_KEYS];
// TODO handle this differently(?) (e.g., just initialize all 4 immediately, rather than lazily (and with (presumably)
//      less-readable code)
int layer_used_indices_size[NUM_LAYERS] = { -1, -1, -1, -1 };

void set_hue_if_pressed(int hue, keyrecord_t* record) {
    if (record->event.pressed) {
        rgb_matrix_mode(1);
        rgb_matrix_sethsv(hue, 255, 255);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool retval = true;
    switch (keycode) {
        case RED0:
            set_hue_if_pressed(0, record);
            retval = false;
            break;

        case RED5:
            set_hue_if_pressed(5, record);
            retval = false;
            break;

        case RED10:
            set_hue_if_pressed(10, record);
            retval = false;
            break;

        case RED15:
            set_hue_if_pressed(15, record);
            retval = false;
            break;

        case ORNG21:
            set_hue_if_pressed(21, record);
            retval = false;
            break;

        case ORNG26:
            set_hue_if_pressed(26, record);
            retval = false;
            break;

        case ORNG31:
            set_hue_if_pressed(31, record);
            retval = false;
            break;

        case ORNG36:
            set_hue_if_pressed(36, record);
            retval = false;
            break;

        case YLLW43:
            set_hue_if_pressed(43, record);
            retval = false;
            break;

        case YLLW53:
            set_hue_if_pressed(53, record);
            retval = false;
            break;

        case YLLW63:
            set_hue_if_pressed(63, record);
            retval = false;
            break;

        case YLLW73:
            set_hue_if_pressed(73, record);
            retval = false;
            break;

        case GRN85:
            set_hue_if_pressed(85, record);
            retval = false;
            break;

        case GRN95:
            set_hue_if_pressed(95, record);
            retval = false;
            break;

        case GRN105:
            set_hue_if_pressed(105, record);
            retval = false;
            break;

        case GRN115:
            set_hue_if_pressed(115, record);
            retval = false;
            break;

        case CYAN127:
            set_hue_if_pressed(127, record);
            retval = false;
            break;

        case CYAN132:
            set_hue_if_pressed(132, record);
            retval = false;
            break;

        case CYAN137:
            set_hue_if_pressed(137, record);
            retval = false;
            break;

        case CYAN142:
            set_hue_if_pressed(142, record);
            retval = false;
            break;

        case AZRE148:
            set_hue_if_pressed(148, record);
            retval = false;
            break;

        case AZRE153:
            set_hue_if_pressed(153, record);
            retval = false;
            break;

        case AZRE158:
            set_hue_if_pressed(158, record);
            retval = false;
            break;

        case AZRE163:
            set_hue_if_pressed(163, record);
            retval = false;
            break;

        case BLUE169:
            set_hue_if_pressed(169, record);
            retval = false;
            break;

        case BLUE172:
            set_hue_if_pressed(172, record);
            retval = false;
            break;

        case BLUE175:
            set_hue_if_pressed(175, record);
            retval = false;
            break;

        case BLUE178:
            set_hue_if_pressed(178, record);
            retval = false;
            break;

        case VILT180:
            set_hue_if_pressed(180, record);
            retval = false;
            break;

        case VILT185:
            set_hue_if_pressed(185, record);
            retval = false;
            break;

        case VILT190:
            set_hue_if_pressed(190, record);
            retval = false;
            break;

        case VILT195:
            set_hue_if_pressed(195, record);
            retval = false;
            break;

        case MGTA201:
            set_hue_if_pressed(201, record);
            retval = false;
            break;

        case MGTA206:
            set_hue_if_pressed(206, record);
            retval = false;
            break;

        case MGTA211:
            set_hue_if_pressed(211, record);
            retval = false;
            break;

        case MGTA217:
            set_hue_if_pressed(217, record);
            retval = false;
            break;

        case ROSE222:
            set_hue_if_pressed(222, record);
            retval = false;
            break;

        case ROSE230:
            set_hue_if_pressed(230, record);
            retval = false;
            break;

        case ROSE238:
            set_hue_if_pressed(238, record);
            retval = false;
            break;

        case ROSE245:
            set_hue_if_pressed(245, record);
            retval = false;
            break;

        default:
            retval = true; // process all other keycodes normally
    }

    return retval;
}

bool rgb_matrix_indicators_user(void) {
    uint8_t layer = biton32(layer_state);
    bool retval = true;

    switch (layer) {
        case MAC_BASE:
        case WIN_BASE:
            retval = true;
            break;

        // when fn is held down, set lights yellow on keys whose behavior changed from base layer; set others to blue
        // inspired by https://www.reddit.com/r/olkb/comments/kpro3p/comment/h3nb56h
        case MAC_FN:
        case WIN_FN:
            // just initialize layers_used_indices[layer] (at most) once per layer
            if (layer_used_indices_size[layer] == -1) {
                layer_used_indices_size[layer] = initialize_layer_used_indices(layer, layers_used_indices[layer]);
            }
//            rgb_matrix_set_color_all(255, 255, 0); // uncomment to have transparent keys appear solid blue
            RGB rgb = hsv_to_rgb(rgb_matrix_get_hsv());
            // TODO what exactly _is_ the color returned by rgb_matrix_get_hsv()? is this some overall color (as opposed
            //      to per-key ones)?
            RGB complement_rgb;
            /*
             TODO? construct complement_rgb in a simpler / more-elegant fashion (e.g., a one-liner)--but
            "RGB complement_rgb = { 255 - rgb.r, 255 - rgb.g, 255 - rgb.b };"
            seems to swap r and g--i.e., make complement_rgb.r = 255 - rgb.g, c_r.g = 255-r.r, c_r.b = 255-r.b
            (which, BTW, doesn't make sense w/RGB (a.k.a. cRGB) in quantum/color.h (if that's even the relevant def...))
            (presumably "RGB complement_rgb = { 255 - rgb.g, 255 - rgb.r, 255 - rgb.b };" could solve this, but that
            seems unnatural / potentially confusing.)
            */
            complement_rgb.r = 255 - rgb.r;
            complement_rgb.g = 255 - rgb.g;
            complement_rgb.b = 255 - rgb.b;
            for (int i = 0; i < layer_used_indices_size[layer]; ++i) {
                rgb_matrix_set_color(layers_used_indices[layer][i], complement_rgb.r, complement_rgb.g, complement_rgb.b);
            }
            retval = false;
            break;

        case CLR_PKR:
            rgb_matrix_set_color_all(0, 0, 0); // rest of keys black
            rgb_matrix_set_color(0, 255, 0, 0); // ESC red

            // RGB values from original code
//            rgb_matrix_set_color(17, 255, 0, 0); // RED0 true red, 1
//            rgb_matrix_set_color(32, 255, 68, 0); // RED5, Q
//            rgb_matrix_set_color(47, 255, 111, 0); // RED10, A
//            rgb_matrix_set_color(61, 255, 179, 0); // RED15 red orange, Z
//            rgb_matrix_set_color(18, 204, 204, 0); // ORNG21 true orange, 2
//            rgb_matrix_set_color(33, 255, 205, 0); // ORNG26, W
//            rgb_matrix_set_color(48, 255, 217, 0); // ORNG31, S
//            rgb_matrix_set_color(62, 255, 230, 0); // ORNG36, X
//            rgb_matrix_set_color(19, 255, 255, 0); // YLLW43 true yellow, 3
//            rgb_matrix_set_color(34, 196, 255, 0); // YLLW53, E
//            rgb_matrix_set_color(49, 154, 255, 0); // YLLW63, D
//            rgb_matrix_set_color(63, 111, 255, 0); // YLLW73, C
//            rgb_matrix_set_color(20, 0, 255, 0); // GRN85 true green, 4
//            rgb_matrix_set_color(35, 0, 255, 50); // GRN95, R
//            rgb_matrix_set_color(50, 0, 255, 75); // GRN105, F
//            rgb_matrix_set_color(64, 0, 255, 120); // GRN115, V
//            rgb_matrix_set_color(21, 0, 255, 255); // CYAN127 true cyan, 5
//            rgb_matrix_set_color(36, 0, 213, 255); // CYAN132, T
//            rgb_matrix_set_color(51, 0, 196, 255); // CYAN137, G
//            rgb_matrix_set_color(65, 0, 154, 255); // CYAN142, B
//            rgb_matrix_set_color(22, 0, 128, 255); // AZRE148 true azure, 6
//            rgb_matrix_set_color(37, 0, 102, 255); // AZRE153, Y
//            rgb_matrix_set_color(52, 0, 77, 255); // AZRE158, H
//            rgb_matrix_set_color(66, 0, 60, 255); // AZRE163, N
//            rgb_matrix_set_color(23, 0, 0, 255); // BLUE169 true blue, 7
//            rgb_matrix_set_color(38, 43, 0, 255); // BLUE172, U
//            rgb_matrix_set_color(53, 60, 0, 255); // BLUE175, J
//            rgb_matrix_set_color(67, 77, 0, 255); // BLUE178, M
//            rgb_matrix_set_color(24, 127, 0, 255); // VILT180 true violet, 8
//            rgb_matrix_set_color(39, 171, 0, 255); // VILT185, I
//            rgb_matrix_set_color(54, 200, 0, 255); // VILT190, K
//            rgb_matrix_set_color(68, 215, 0, 255); // VILT195, <
//            rgb_matrix_set_color(25, 255, 0, 255); // MGTA201 true magenta, 9
//            rgb_matrix_set_color(40, 255, 0, 240); // MGTA206, O
//            rgb_matrix_set_color(55, 255, 0, 220); // MGTA211, L
//            rgb_matrix_set_color(69, 255, 0, 200); // MGTA217, >
//            rgb_matrix_set_color(26, 255, 0, 127); // ROSE222 true rose, 0
//            rgb_matrix_set_color(41, 255, 0, 111); // ROSE230, P
//            rgb_matrix_set_color(56, 255, 0, 70); // ROSE238, :
//            rgb_matrix_set_color(70, 255, 0, 26); // ROSE245, ?

            /*
             RGB values derived (in a separate program) via https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB:
                 fun rgbOf(hue: Int) = (6.0 * hue / 256).let { hp: Double ->
                     val x = 256 * (1 - Math.abs((hp % 2) - 1))
                     when {
                         hp >= 0.0 && hp < 1.0 -> listOf(255.0, x, 0.0)
                         hp >= 1.0 && hp < 2.0 -> listOf(x, 255.0, 0.0)
                         hp >= 2.0 && hp < 3.0 -> listOf(0.0, 255.0, x)
                         hp >= 3.0 && hp < 4.0 -> listOf(0.0, x, 255.0)
                         hp >= 4.0 && hp < 5.0 -> listOf(x, 0.0, 255.0)
                         hp >= 5.0 && hp <= 6.0 -> listOf(255.0, 0.0, x)
                         else -> error("error: hp = $hp")
                     }
                 }
             */
            rgb_matrix_set_color(17, 255, 0, 0); // RED0 true red, 1
            rgb_matrix_set_color(32, 255, 30, 0); // RED5, Q
            rgb_matrix_set_color(47, 255, 60, 0); // RED10, A
            rgb_matrix_set_color(61, 255, 90, 0); // RED15 red orange, Z
            rgb_matrix_set_color(18, 255, 126, 0); // ORNG21 true orange, 2
            rgb_matrix_set_color(33, 255, 156, 0); // ORNG26, W
            rgb_matrix_set_color(48, 255, 186, 0); // ORNG31, S
            rgb_matrix_set_color(62, 255, 216, 0); // ORNG36, X
            rgb_matrix_set_color(19, 254, 255, 0); // YLLW43 true yellow, 3
            rgb_matrix_set_color(34, 194, 255, 0); // YLLW53, E
            rgb_matrix_set_color(49, 134, 255, 0); // YLLW63, D
            rgb_matrix_set_color(63, 74, 255, 0); // YLLW73, C
            rgb_matrix_set_color(20, 2, 255, 0); // GRN85 true green, 4
            rgb_matrix_set_color(35, 0, 255, 58); // GRN95, R
            rgb_matrix_set_color(50, 0, 255, 118); // GRN105, F
            rgb_matrix_set_color(64, 0, 255, 178); // GRN115, V
            rgb_matrix_set_color(21, 0, 255, 250); // CYAN127 true cyan, 5
            rgb_matrix_set_color(36, 0, 232, 255); // CYAN132, T
            rgb_matrix_set_color(51, 0, 202, 255); // CYAN137, G
            rgb_matrix_set_color(65, 0, 172, 255); // CYAN142, B
            rgb_matrix_set_color(22, 0, 136, 255); // AZRE148 true azure, 6
            rgb_matrix_set_color(37, 0, 106, 255); // AZRE153, Y
            rgb_matrix_set_color(52, 0, 76, 255); // AZRE158, H
            rgb_matrix_set_color(66, 0, 46, 255); // AZRE163, N
            rgb_matrix_set_color(23, 0, 10, 255); // BLUE169 true blue, 7
            rgb_matrix_set_color(38, 8, 0, 255); // BLUE172, U
            rgb_matrix_set_color(53, 26, 0, 255); // BLUE175, J
            rgb_matrix_set_color(67, 44, 0, 255); // BLUE178, M
            rgb_matrix_set_color(24, 56, 0, 255); // VILT180 true violet, 8
            rgb_matrix_set_color(39, 86, 0, 255); // VILT185, I
            rgb_matrix_set_color(54, 116, 0, 255); // VILT190, K
            rgb_matrix_set_color(68, 146, 0, 255); // VILT195, <
            rgb_matrix_set_color(25, 182, 0, 255); // MGTA201 true magenta, 9
            rgb_matrix_set_color(40, 212, 0, 255); // MGTA206, O
            rgb_matrix_set_color(55, 242, 0, 255); // MGTA211, L
            rgb_matrix_set_color(69, 255, 0, 234); // MGTA217, >
            rgb_matrix_set_color(26, 255, 0, 204); // ROSE222 true rose, 0
            rgb_matrix_set_color(41, 255, 0, 156); // ROSE230, P
            rgb_matrix_set_color(56, 255, 0, 108); // ROSE238, :
            rgb_matrix_set_color(70, 255, 0, 66); // ROSE245, ?

            retval = false;
            break;
    }

    return retval;
}
