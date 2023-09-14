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
 * (1) SHORT NAME*: [DEBUG]
 *     DETAILS: When enabled, outputs the result of `uprintf` statements--these can be seen in the QMK Toolbox console.
 *     TO ACTIVATE: To debug, set `DEBUG` to true and make sure that (horwitz/)rules.mk contains `CONSOLE_ENABLE = yes`;
 *                  then use (e.g.) `uprintf` to print debug output.
 *
 * (2) SHORT NAME: [FN-HI]
 *     DETAILS: When the `fn` key is pressed, the keys with new keycodes (i.e., not `_______`) will light up in the
 *              color complementary to the color† of layer 0‡; the remaining keys will retain the lighting behavior
 *              from before the `fn` key was pressed.
 *     TO ACTIVATE: The feature is always on.
 *
 * (3) SHORT NAME: [GCP]
 *     DETAILS: `fn+V` presents a 12x4 rainbow grid of keys and 12 grayscale keys on the top row (with the remaining
 *              keys§ dark)--pressing any one of these keys sets the base layer (e.g., layer 0 when `fn` goes to layer
 *              1) to a solid pattern of that color. The top row (of the four color rows) are primary, secondary, and
 *              tertiary colors--in standard order: red (1), orange (3), yellow (2), chartreuse (3), green (1), spring
 *              green (3), cyan (2), azure (3), blue (1), violet (3), magenta (2), rose (3) (colors, going down then
 *              across, increase in hue by approximately 1/48 (in a range of [0,1)) (w/(HSV) saturation and value both
 *              of 1 (in [0,1])). The grayscale keys, left to right, produce an HSV value from 0 to 1 (in a range of
 *              [0,1]) (w/(HSV) hue and saturation both of 0 (in [0,1) and [0,1], respectively)).
 *     TO ACTIVATE: The feature is always on; grayscale mode if off by default--its availability is toggled by double
 *                  tapping X while holding Fn.
 *     NOTE: The feature creates a fifth layer (layer 4) for the rainbow grid (meant only to be accessed for the
 *           purposes mentioned above in DETAILS). Actual hue values are encoded as integers in [0,255] (scaled from
 *           [0,1)).
 *
 * (4) SHORT NAME: [SUS-RGB]
 *     DETAILS: Turns off lighting when the laptop sleeps (and for similar(?) behavior).
 *     TO ACTIVATE: The feature is always on.
 *
 * (5) SHORT NAME: [CCP]
 *     DETAILS: see ccp.c
 *     TO ACTIVATE: The feature is always on.
 *
 * * notation just for documentation (when a comment with the SHORT NAME is found, the code below--continuing until the
 *   next blank line--is park of the feature with that SHORT NAME (additionally: some single lines have a comment with
 *   the short name at the end of the line, meaning that that one line should be taken into account, rather than
 *   continuing on to the next blank line)... also it is (or at least should be) the case that _all_ of the code for the
 *   feature is commented in that fashion)
 * † main(?) color
 * ‡ or is it the layer from which one came (e.g., starting at a base layer of 2, will these be complementary to layer
 *     2's color†)?
 * § End is set to RGB_RED and, when hit, leaves the layer, making no change to the base layer's color
 */

// [DEBUG]
#define DEBUG false

#include QMK_KEYBOARD_H
#include <math.h> // for round() // [GCP]
#include "layers.h"
#include "ccp.h"
//#include "ctrlkeycodes.h"
// [DEBUG]
#if DEBUG
    #include "print.h"
#endif

// [GCP]
// Tap Dance declarations
enum {
    TD_GRAY
};

// [GCP]
bool cPickGrayscaleAvailable = false;

// [GCP]
void dance_grayscale(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        cPickGrayscaleAvailable = !cPickGrayscaleAvailable;
    }
}

// [GCP]
// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap twice to toggle grayscale availability
    [TD_GRAY] = ACTION_TAP_DANCE_FN(dance_grayscale)
};

int MIN_COLOR_KEYCODE = COLOR00; // [GCP]
int MAX_COLOR_KEYCODE = GRAY11; // [GCP]

// [GCP]
enum COLOR_SCHEME { RGB_SCHEME, GRAY_SCHEME };

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
     _______,            _______,  _______,  TOCCP,  OSL(GCP),BAT_LVL, NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,  // [CCP] ("TOCCP") // [GCP] ("OSL(GCP)")
     _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

// [GCP]
[GCP] = LAYOUT_ansi_84(
     // TODO go back to WIN_BASE (instead of MAC_BASE) as appropriate
     GRAY00,   GRAY01,   GRAY02,   GRAY03,   GRAY04,   GRAY05,   GRAY06,   GRAY07,   GRAY08,   GRAY09,   GRAY10,   GRAY11,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
     COLOR00,  COLOR04,  COLOR08,  COLOR12,  COLOR16,  COLOR20,  COLOR24,  COLOR28,  COLOR32,  COLOR36,  COLOR40,  COLOR44,  XXXXXXX,  XXXXXXX,            XXXXXXX,
     COLOR01,  COLOR05,  COLOR09,  COLOR13,  COLOR17,  COLOR21,  COLOR25,  COLOR29,  COLOR33,  COLOR37,  COLOR41,  COLOR45,  XXXXXXX,  XXXXXXX,            XXXXXXX,
     COLOR02,  COLOR06,  COLOR10,  COLOR14,  COLOR18,  COLOR22,  COLOR26,  COLOR30,  COLOR34,  COLOR38,  COLOR42,  COLOR46,            XXXXXXX,            XXXXXXX,
     COLOR03,            COLOR07,  COLOR11,  COLOR15,  COLOR19,  COLOR23,  COLOR27,  COLOR31,  COLOR35,  COLOR39,  COLOR43,            COLOR47,  XXXXXXX,  XXXXXXX, // keycode 73 is lit RED, though pressing any XXXXXXX aborts color choosing
     XXXXXXX,  XXXXXXX,  XXXXXXX,                                XXXXXXX,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX),

// [CCP]
[CCP] = LAYOUT_ccp
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

// [GCP]
// TODO? (1) derive via sizeof color_picker_color_hues and/or color_picker_color_palette_keycodes OR (2) validate that
//       color_picker_color_palette_keycodes is initialized with an rvalue of COLOR_PALETTE_SIZE elements (see comment
//       above color_picker_color_palette_keycodes's initialization)
#define COLOR_PALETTE_SIZE 48
#define GRAY_PALETTE_SIZE 12

// [GCP]
int color_picker_color_hues[COLOR_PALETTE_SIZE];
RGB color_picker_color_rgbs[COLOR_PALETTE_SIZE];
int color_picker_gray_intensities[GRAY_PALETTE_SIZE];

void keyboard_post_init_user(void) {
    // [DEBUG]
#if DEBUG
        // Customise these values to desired behaviour
        debug_enable = true;
//        debug_matrix = true;
//        debug_keyboard = true;
//        debug_mouse = true;
#endif

    // [GCP]
    /*
      hues:
      0,   5,  11,  16,  21,  27,  32,  37,  43,  48,  53,  59,
     64,  69,  75,  80,  85,  91,  96, 101, 107, 112, 117, 123,
    128, 133, 139, 144, 149, 155, 160, 165, 171, 176, 181, 187,
    192, 197, 203, 208, 213, 219, 224, 229, 235, 240, 245, 251
    */
    for (int i = 0; i < COLOR_PALETTE_SIZE; ++i) {
        // NB: requires COLOR_PALETTE_SIZE < 512 or rvalue for largest i will be 256 (and hues must fall in [0,255])
        color_picker_color_hues[i] = round(i * 256.0 / COLOR_PALETTE_SIZE);
        HSV hsv = { .h=color_picker_color_hues[i], .s=255, .v=255 };
        color_picker_color_rgbs[i] = hsv_to_rgb_nocie(hsv);
    }
    for (int i = 0; i < GRAY_PALETTE_SIZE; ++i) {
        color_picker_gray_intensities[i] = round(i * 255.0 / (GRAY_PALETTE_SIZE - 1));
    }


    // [FN-HI]
    // TODO? only do for layer = MAC_FN and layer = WIN_FN (rather than all layers), since FN-HI only uses those layers
    //       (see rgb_matrix_indicators_user)
    for (int layer = 0; layer < DYNAMIC_KEYMAP_LAYER_COUNT; ++layer) {
        layer_used_indices_size[layer] = initialize_layer_used_indices(layer, layers_used_indices[layer]);
    }
}

// [GCP]
// TODO? check that the rvalue for the color_picker_color_palette_keycodes assignment actually has COLOR_PALETTE_SIZE
//       elements (not fewer--too many would lead to a compilation failure, but too few would leave (presumably
//       unwanted) 0s at the end)
int color_picker_color_palette_keycodes[COLOR_PALETTE_SIZE] = {
    16, // `    (COLOR00: (1) red)
    31, // TAB  (COLOR01: (5) scarlet)
    46, // CAPS (COLOR02: (4) vermilion)
    60, // LSFT (COLOR03: (5) persimmon)
    17, // 1    (COLOR04: (3) orange)
    32, // Q    (COLOR05: (5) orange peel)
    47, // A    (COLOR06: (4) amber)
    61, // Z    (COLOR07: (5) golden yellow)
    18, // 2    (COLOR08: (2) yellow)
    33, // W    (COLOR09: (5) lemon)
    48, // S    (COLOR10: (4) lime)
    62, // X    (COLOR11: (5) spring bud)
    19, // 3    (COLOR12: (3) chartreuse)
    34, // E    (COLOR13: (5) bright green)
    49, // D    (COLOR14: (4) harlequin)
    63, // C    (COLOR15: (5) neon green)
    20, // 4    (COLOR16: (1) green)
    35, // R    (COLOR17: (5) jade)
    50, // F    (COLOR18: (4) erin)
    64, // V    (COLOR19: (5) emerald)
    21, // 5    (COLOR20: (3) spring green)
    36, // T    (COLOR21: (5) mint)
    51, // G    (COLOR22: (4) aquamarine)
    65, // B    (COLOR23: (5) turquoise)
    22, // 6    (COLOR24: (2) cyan)
    37, // Y    (COLOR25: (5) sky blue)
    52, // H    (COLOR26: (4) capri)
    66, // N    (COLOR27: (5) cornflower)
    23, // 7    (COLOR28: (3) azure)
    38, // U    (COLOR29: (5) cobalt)
    53, // J    (COLOR30: (4) cerulean)
    67, // M    (COLOR31: (5) sapphire)
    24, // 8    (COLOR32: (1) blue)
    39, // I    (COLOR33: (5) iris)
    54, // K    (COLOR34: (4) indigo)
    68, // ,    (COLOR35: (5) veronica)
    25, // 9    (COLOR36: (3) violet)
    40, // O    (COLOR37: (5) amethyst)
    55, // L    (COLOR38: (4) purple)
    69, // .    (COLOR39: (5) phlox)
    26, // 0    (COLOR40: (2) magenta)
    41, // P    (COLOR41: (5) fuchsia)
    56, // ;    (COLOR42: (4) cerise)
    70, // /    (COLOR43: (5) deep pink)
    27, // -    (COLOR44: (3) rose)
    42, // []   (COLOR45: (5) raspberry)
    57, // '    (COLOR46: (4) crimson)
    71  // RSFT (COLOR47: (5) amaranth)
};
int color_picker_gray_palette_keycodes[GRAY_PALETTE_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

// [GCP]
// returns 0 for COLOR00, 1 for COLOR01, ..., 47 for COLOR47
int get_color_picker_color_keycode_index(uint16_t keycode) {
    return keycode - COLOR00;
}

// [GCP]
// returns 0 for GRAY00, 1 for COLOR01, ..., 11 for GRAY11
int get_color_picker_gray_keycode_index(uint16_t keycode) {
    return keycode - GRAY00;
}

// [GCP]
bool is_color_picker_color_keycode(uint16_t keycode) {
    return keycode >= MIN_COLOR_KEYCODE && keycode <= MAX_COLOR_KEYCODE;
}

// [GCP]
enum COLOR_SCHEME get_color_scheme(uint16_t keycode) {
    if (keycode >= COLOR00 && keycode <= COLOR47) {
        return RGB_SCHEME;
    } else if (keycode >= GRAY00 && keycode <= GRAY11) {
        return GRAY_SCHEME;
    } else {
        return -1; // this should never happen... (TODO handle some other way?)
    }
}

// [CCP]
layer_state_t layer_state_set_user(layer_state_t state) {
    return layer_state_set_user_ccp(state);
}

// [GCP]
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool retval = true;
    if (is_color_picker_color_keycode(keycode)) {
        if (record -> event.pressed) {
            rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
            switch (get_color_scheme(keycode)) {
                case RGB_SCHEME:
                    rgb_matrix_sethsv(color_picker_color_hues[get_color_picker_color_keycode_index(keycode)], 255, 255);
                    break;
                case GRAY_SCHEME:
                    if (cPickGrayscaleAvailable) {
                        rgb_matrix_sethsv(0, 0, color_picker_gray_intensities[get_color_picker_gray_keycode_index(keycode)]);
                    }
                    break;
            }
        }
        retval = false;
    // [CCP]
    } else {
        retval = process_record_user_ccp(keycode, record);
    }

    return retval;
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
            rgb_matrix_set_color_all(RGB_OFF); // RGB_OFF == RGB_BLACK // keys not changed below to black
            rgb_matrix_set_color(73, RGB_RED); // set End to red // TODO? different color here

            // [GCP]
            for (int i = 0; i < COLOR_PALETTE_SIZE; ++i) {
                RGB rgb = color_picker_color_rgbs[i];
                rgb_matrix_set_color(color_picker_color_palette_keycodes[i], rgb.r, rgb.g, rgb.b);
            }
            if (cPickGrayscaleAvailable) {
                for (int i = 0; i < GRAY_PALETTE_SIZE; ++i) {
                    int intensity = color_picker_gray_intensities[i];
                    rgb_matrix_set_color(color_picker_gray_palette_keycodes[i], intensity, intensity, intensity);
                }
            }
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
