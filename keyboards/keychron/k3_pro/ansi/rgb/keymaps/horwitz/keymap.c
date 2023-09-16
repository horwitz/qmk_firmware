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
 * (3) Granular color picking
 * (4) Suspend RGB
 * (5) Complete color picking
 *
 * (1) SHORT NAME†: [DEBUG]
 *     DETAILS: When enabled, outputs the result of `uprintf` statements--these can be seen in the QMK Toolbox console.
 *     TO ACTIVATE: To debug, set `DEBUG` to true and make sure that (horwitz/)rules.mk contains `CONSOLE_ENABLE = yes`;
 *                  then use (e.g.) `uprintf` to print debug output.
 *
 * (2) SHORT NAME: [FN-HI]
 *     DETAILS: When the `fn` key is pressed, the keys with new keycodes (i.e., not `_______`) will light up in the
 *              color complementary to the color‡ of layer 0§; the remaining keys will retain the lighting behavior
 *              from before the `fn` key was pressed.
 *     TO ACTIVATE: The feature is always on.
 *
 * (3) SHORT NAME: [GCP]
 *     DETAILS: see gcp.c
 *     TO ACTIVATE: The feature is always on; grayscale mode if off by default--its availability is toggled by double
 *                  tapping X while holding Fn.
 *
 * (4) SHORT NAME: [SUS-RGB]
 *     DETAILS: Turns off lighting when the laptop sleeps (and for similar(?) behavior).
 *     TO ACTIVATE: The feature is always on.
 *
 * (5) SHORT NAME: [CCP]
 *     DETAILS: see ccp.c
 *     TO ACTIVATE: The feature is always on.
 *
 * † notation just for documentation (when a comment with the SHORT NAME is found, the code below--continuing until the
 *   next blank line--is park of the feature with that SHORT NAME (additionally: some single lines have a comment with
 *   the short name at the end of the line, meaning that that one line should be taken into account, rather than
 *   continuing on to the next blank line)... also it is (or at least should be) the case that _all_ of the code for the
 *   feature is commented in that fashion)
 * ‡ main(?) color
 * § or is it the layer from which one came (e.g., starting at a base layer of 2, will these be complementary to layer
 *     2's color‡)?
 */

// [DEBUG]
#define DEBUG false

#include QMK_KEYBOARD_H
#include "layers.h"
#include "gcp.h"
#include "ccp.h"
//#include "ctrlkeycodes.h"
// [DEBUG]
#if DEBUG
    #include "print.h"
#endif

// [SUS-RGB]
void suspend_power_down_user(void) {
    rgb_matrix_set_suspend_state(true);
    // suspend_power_down_keymap(); // should this be called?
}
void suspend_wakeup_init_user(void) {
    rgb_matrix_set_suspend_state(false);
    // suspend_wakeup_init_keymap(); // should this be called?
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_SNAP,  KC_DEL,   RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD,MO(MAC_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_ansi_84(
     _______,  KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  RGB_TOG,
     _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
     _______,            _______, TD(TD_GRAY),TOCCP, OSL(GCP),BAT_LVL, NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,  // [CCP] ("TOCCP") // [GCP] ("OSL(GCP)")
     _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

[WIN_BASE] = LAYOUT_ansi_84(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_DEL,   RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT, MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN_FN] = LAYOUT_ansi_84(
     _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  RGB_TOG,
     _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
     _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
     _______,            _______, TD(TD_GRAY),TOCCP, OSL(GCP),BAT_LVL, NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,  // [CCP] ("TOCCP") // [GCP] ("OSL(GCP)")
     _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

[GCP] = LAYOUT_gcp, // [GCP]

[CCP] = LAYOUT_ccp // [CCP]
};

// [FN-HI]
/*
 * sets the values in [layer_used_indices] to the indices (in increasing order; a subset of 0-83) that are used in the
 * given [layer] ("used" = not _______) and returns how many values were set. (e.g., if keys at index 0, 5, 83 are the
 * only ones set, layer_used_indices will start { 0, 5, 83, ... } (followed by 0s) and [initialize_layer_used_indices]
 * will return 3.)
 *
 * [offset] is used below because [rgb_matrix_set_color]'s first argument appears to be based on an index of keys _not_
 * including KC_NO (== XXXXXXX)--there are 96 (=[MATRIX_ROWS] * [MATRIX_COLS]) keycodes in each keymaps[layer], but
 * (apparently) the non-KC_NO keys are indexed 0-83 (and there are 96-84=12 instances of KC_NO (per layer))... see
 * LAYOUT_ansi_84's definition in obj_keychron_k3_pro_ansi_rgb/src/default_keyboard.h
 */
int initialize_layer_used_indices(int layer, char* layer_used_indices) {
    int lui_i = 0;
    int offset = 0;
    for (int i = 0; i < MATRIX_ROWS * MATRIX_COLS; ++i) {
        int col = i % MATRIX_COLS;
        int row = i / MATRIX_COLS;
        switch (keymaps[layer][row][col]) {
            case _______:
                break;
            case KC_NO: // == XXXXXXX
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
// for any given layer, layers_used_indices[layer] should not exceed RGB_MATRIX_LED_COUNT in size, since each index
// found in the array should represent a key with a distinct light (see also: doc for initialize_layer_used_indices)
// TODO? validate that RGB_MATRIX_LED_COUNT is 84
char layers_used_indices[DYNAMIC_KEYMAP_LAYER_COUNT][RGB_MATRIX_LED_COUNT];
int layer_used_indices_size[DYNAMIC_KEYMAP_LAYER_COUNT];

void keyboard_post_init_user(void) {
    // [DEBUG]
#if DEBUG
        // Customise these values to desired behaviour
        debug_enable = true;
//        debug_matrix = true;
//        debug_keyboard = true;
//        debug_mouse = true;
#endif

    keyboard_post_init_user_gcp(); // [GCP]

    // [FN-HI]
    // TODO? only do for layer = MAC_FN and layer = WIN_FN (rather than all layers), since FN-HI only uses those layers
    //       (see rgb_matrix_indicators_user)
    for (int layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; ++layer) {
        layer_used_indices_size[layer] = initialize_layer_used_indices(layer, layers_used_indices[layer]);
    }
}

// [CCP]
layer_state_t layer_state_set_user(layer_state_t state) {
    return layer_state_set_user_ccp(state);
}

// note potential short circuit
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return process_record_user_gcp(keycode, record) && // [GCP]
        process_record_user_ccp(keycode, record); // [CCP]
}

//uint8_t last_layer = -1;

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = biton32(layer_state);

//    if (layer != last_layer) {
//        uprintf("layer: %d\n", layer);
//        last_layer = layer;
//    }

    switch (layer) {
        case MAC_BASE:
        case WIN_BASE:
            break;

        // [FN-HI]
        // when fn is held down, set lights yellow on keys whose behavior changed from base layer; set others to blue
        // inspired by https://www.reddit.com/r/olkb/comments/kpro3p/comment/h3nb56h
        case MAC_FN:
        case WIN_FN: {
//            rgb_matrix_set_color_all(RGB_BLUE); // uncomment to have transparent keys appear solid blue
            RGB rgb = hsv_to_rgb_nocie(rgb_matrix_get_hsv());
            // TODO what exactly _is_ the color returned by rgb_matrix_get_hsv()? is this some overall color (as opposed
            //      to per-key ones)?
            // TODO? if all r,g,b are close to 255/2, the complement will be close to (and hard to discern from) the
            //       original color (should this be changed?)
            RGB complement_rgb = { .r = 255 - rgb.r, .g = 255 - rgb.g, .b = 255 - rgb.b };
            for (int i = 0; i < layer_used_indices_size[layer]; ++i) {
                rgb_matrix_set_color(
                    layers_used_indices[layer][i],
                    complement_rgb.r,
                    complement_rgb.g,
                    complement_rgb.b
                );
            }
            break;
        }

        // [GCP]
        case GCP: {
            rgb_matrix_indicators_advanced_user_gcp();
            break;
        }

        // [CCP]
        case CCP: {
            rgb_matrix_indicators_advanced_user_ccp();
            break;
        }
    }
    // TODO should this always return true?
    // assuming effects set to RGB_MATRIX_SOLID_COLOR
    // return true: if CAPS off, CAPS matches solid color; if CAPS on, CAPS is red
    // return false: if CAPS off, CAPS matches solid color; if CAPS on, CAPS is some color that is a function of the
    //               solid color (e.g., if solid color is blue, CAPS is magenta(ish) when on; if solid color is green,
    //                CAPS is yellow(ish) when on)
    return true;
}
