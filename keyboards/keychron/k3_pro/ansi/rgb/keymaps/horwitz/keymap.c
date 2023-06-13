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

/**
 * Features:
 * (1) Debugging
 * (2) Highlighting used fn-layer keys
 * (3) Color picking
 *
 * (1) SHORT NAME*: [DEBUG]
 *     DETAILS: When enabled, outputs the result of `uprintf` statements--these can be seen in the QMK Toolbox console.
 *     TO ACTIVATE: To debug, set `DEBUG` to 1 (as opposed to 0) and make sure that (horwitz/)rules.mk contains
 *                 `CONSOLE_ENABLE = yes`; then use (e.g.) `uprintf` to print debug output.
 *
 * (2) SHORT NAME: [FN-HI]
 *     DETAILS: When the `fn` key is pressed, the keys with new keycodes (i.e., not `KC_TRNS`) will light up in the
 *              color complementary to the color** of layer 0***; the remaining keys will retain the lighting behavior
 *              from the previous level.
 *     TO ACTIVATE: The feature is always on.
 *
 * (3) SHORT NAME: [CPICK]
 *     DETAILS: `fn+V` presents a 10x4 rainbow grid of keys (with the remaining keys dark)--pressing any one of these
 *              keys sets the base layer (e.g., layer 0 when `fn` goes to layer 1) to a solid pattern of that color.
 *     TO ACTIVATE: The feature is always on.
 *     NOTE: The feature creates a fifth layer (layer 4) for the rainbow grid (meant only to be accessed for the
 *           purposes mentioned above in DETAILS.
 *
 * * notation just for documentation (when I comment with the SHORT NAME is found, the code below--continuing until the
 *   next blank line--is park of the feature with that SHORT NAME (additionally: some single lines have a comment with
 *   the short name at the end of the line, meaning that that one line should be taken into account, rather than
 *   continuing on to the next blank line)... also it is (or at least should be) the case that _all_ of the code for the
 *   feature is commented in that fashion
 * ** main(?)
 * *** or is it the layer from which one came (e.g., starting at a base layer of 2, will these be complementary to layer
 *     2's color**)?
 */

// [DEBUG]
#define DEBUG 0

#include QMK_KEYBOARD_H
// [DEBUG]
#if DEBUG
    #include "print.h"
#endif

// clang-format off
enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN,
  CLR_PKR // [CPICK]
};
// [FN-HI]
#define NUM_LAYERS DYNAMIC_KEYMAP_LAYER_COUNT // TODO? derive from elsewhere? inline?
#define NUM_KEYS 84 // TODO? derive from... somewhere

// [CPICK]
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

// [DEBUG]
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
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,OSL(CLR_PKR),BAT_LVL, NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS, // [CPICK] ("OSL(CLR_PKR)")
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
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,OSL(CLR_PKR),BAT_LVL, NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS, // [CPICK] ("OSL(CLR_PKR)")
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

// [CPICK]
[CLR_PKR] = LAYOUT_ansi_84(
     TG(MAC_BASE),_______,_______, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  RED0,     ORNG21,   YLLW43,   GRN85,    CYAN127,  AZRE148,  BLUE169,  VILT180,  MGTA201,  ROSE222,  _______,  _______,  _______,            _______,
     _______,  RED5,     ORNG26,   YLLW53,   GRN95,    CYAN132,  AZRE153,  BLUE172,  VILT185,  MGTA206,  ROSE230,  _______,  _______,  _______,            _______,
     _______,  RED10,    ORNG31,   YLLW63,   GRN105,   CYAN137,  AZRE158,  BLUE175,  VILT190,  MGTA211,  ROSE238,  _______,            _______,            _______,
     _______,            RED15,    ORNG36,   YLLW73,   GRN115,   CYAN142,  AZRE163,  BLUE178,  VILT195,  MGTA217,  ROSE245,            _______,  _______,  _______,
     _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______)
};

// [FN-HI]
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

// [FN-HI]
char layers_used_indices[NUM_LAYERS][NUM_KEYS];
// TODO handle this differently(?) (e.g., just initialize all 4 immediately, rather than lazily (and with (presumably)
//      less-readable code)
int layer_used_indices_size[NUM_LAYERS] = { -1, -1, -1, -1 };

// [CPICK]
#define PALETTE_SIZE 40 // TODO derive via sizeof?

// [CPICK]
int color_picker_hues[] = { 0, 5, 10, 15, 21, 26, 31, 36, 43, 53, 63, 73, 85, 95, 105, 115, 127, 132, 137, 142,
    148, 153, 158, 163, 169, 172, 175, 178, 180, 185, 190, 195, 201, 206, 211, 217, 222, 230, 238, 245 };

// [CPICK]
int color_picker_palette_keycodes[] = {
    17, // 1 (RED0 true red)
    32, // Q (RED5)
    47, // A (RED10)
    61, // Z (RED15 red orange)
    18, // 2 (ORNG21 true orange)
    33, // W (ORNG26)
    48, // S (ORNG31)
    62, // X (ORNG36)
    19, // 3 (YLLW43 true yellow)
    34, // E (YLLW53)
    49, // D (YLLW63)
    63, // C (YLLW73)
    20, // 4 (GRN85 true green)
    35, // R (GRN95)
    50, // F (GRN105)
    64, // V (GRN115)
    21, // 5 (CYAN127 true cyan)
    36, // T (CYAN132)
    51, // G (CYAN137)
    65, // B (CYAN142)
    22, // 6 (AZRE148 true azure)
    37, // Y (AZRE153)
    52, // H (AZRE158)
    66, // N (AZRE163)
    23, // 7 (BLUE169 true blue)
    38, // U (BLUE172)
    53, // J (BLUE175)
    67, // M (BLUE178)
    24, // 8 (VILT180 true violet)
    39, // I (VILT185)
    54, // K (VILT190)
    68, // , (VILT195)
    25, // 9 (MGTA201 true magenta)
    40, // O (MGTA206)
    55, // L (MGTA211)
    69, // . (MGTA217)
    26, // 0 (ROSE222 true rose)
    41, // P (ROSE230)
    56, // ; (ROSE238)
    70  // / (ROSE245)
};

// [CPICK]
// returns 0 for RED0, 1 for RED5, 2 for RED10, 3 for RED15, 4 for ORNG21, ...
int get_color_picker_keycode_index(uint16_t keycode) {
    return keycode - RED0;
}

// [CPICK]
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool retval = true;
    switch (keycode) {
        case RED0:
        case RED5:
        case RED10:
        case RED15:
        case ORNG21:
        case ORNG26:
        case ORNG31:
        case ORNG36:
        case YLLW43:
        case YLLW53:
        case YLLW63:
        case YLLW73:
        case GRN85:
        case GRN95:
        case GRN105:
        case GRN115:
        case CYAN127:
        case CYAN132:
        case CYAN137:
        case CYAN142:
        case AZRE148:
        case AZRE153:
        case AZRE158:
        case AZRE163:
        case BLUE169:
        case BLUE172:
        case BLUE175:
        case BLUE178:
        case VILT180:
        case VILT185:
        case VILT190:
        case VILT195:
        case MGTA201:
        case MGTA206:
        case MGTA211:
        case MGTA217:
        case ROSE222:
        case ROSE230:
        case ROSE238:
        case ROSE245:
            if (record->event.pressed) {
                rgb_matrix_mode(1);
                rgb_matrix_sethsv(color_picker_hues[get_color_picker_keycode_index(keycode)], 255, 255);
            }
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

        // [FN-HI]
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

        // [CPICK]
        case CLR_PKR:
            rgb_matrix_set_color_all(0, 0, 0); // rest of keys black
            rgb_matrix_set_color(0, 255, 0, 0); // ESC red

            for (int i = 0; i < PALETTE_SIZE; ++i) {
                HSV hsv = { color_picker_hues[i], 255, 255 };
                RGB rgb = hsv_to_rgb(hsv);
                rgb_matrix_set_color(color_picker_palette_keycodes[i], rgb.r, rgb.g, rgb.b);
            }

            retval = false;
            break;
    }

    return retval;
}
