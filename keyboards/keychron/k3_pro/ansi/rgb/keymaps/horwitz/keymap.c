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
 *     DETAILS: `fn+V` presents a 12x4 rainbow grid of keys (with the remaining keys dark)--pressing any one of these
 *              keys sets the base layer (e.g., layer 0 when `fn` goes to layer 1) to a solid pattern of that color. The
 *              top row (of four rows) are primary, secondary, and tertiary colors--in standard order: red (1),
 *              orange (3), yellow (2), chartreuse (3), green (1), spring green (3), cyan (2), azure (3), blue (1),
 *              violet (3), magenta (2), rose (3) (colors, going down then across, increase in hue by approximately 1/48
 *              (in a range of [0,1)) (w/saturation and value both of 1 (in [0,1])).
 *     TO ACTIVATE: The feature is always on.
 *     NOTE: The feature creates a fifth layer (layer 4) for the rainbow grid (meant only to be accessed for the
 *           purposes mentioned above in DETAILS. Actual hue values are encoded as integers in [0,256) (scaled from
 *           [0,1)).
 * (4) SHORT NAME: [SUS-RGB]
 *     DETAILS: Turns off lighting when the laptop sleeps (and similar(?) behavior).
 *     TO ACTIVATE: The feature is always on.
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
#include <math.h> // for round() // [CPICK]
// [DEBUG]
#if DEBUG
    #include "print.h"
#endif

// clang-format off
enum layers {
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
    // if COLOR00 were set to SAFE_RANGE (instead of NEW_SAFE_RANGE), collisions would occur (e.g.,
    // KC_LOPTN == COLOR00, ...)
    COLOR00 = NEW_SAFE_RANGE, COLOR01, COLOR02, COLOR03,
        COLOR04, COLOR05, COLOR06, COLOR07,
        COLOR08, COLOR09, COLOR10, COLOR11,
        COLOR12, COLOR13, COLOR14, COLOR15,
        COLOR16, COLOR17, COLOR18, COLOR19,
        COLOR20, COLOR21, COLOR22, COLOR23,
        COLOR24, COLOR25, COLOR26, COLOR27,
        COLOR28, COLOR29, COLOR30, COLOR31,
        COLOR32, COLOR33, COLOR34, COLOR35,
        COLOR36, COLOR37, COLOR38, COLOR39,
        COLOR40, COLOR41, COLOR42, COLOR43,
        COLOR44, COLOR45, COLOR46, COLOR47
};

// [DEBUG]
#if DEBUG
    void keyboard_post_init_user(void) {
        // Customise these values to desired behaviour
        debug_enable = true;
        debug_matrix = true;
        debug_keyboard = true;
    //    debug_mouse = true;
    }
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
     COLOR00,  COLOR04,  COLOR08,  COLOR12,  COLOR16,  COLOR20,  COLOR24,  COLOR28,  COLOR32,  COLOR36,  COLOR40,  COLOR44,  _______,  _______,            _______,
     COLOR01,  COLOR05,  COLOR09,  COLOR13,  COLOR17,  COLOR21,  COLOR25,  COLOR29,  COLOR33,  COLOR37,  COLOR41,  COLOR45,  _______,  _______,            _______,
     COLOR02,  COLOR06,  COLOR10,  COLOR14,  COLOR18,  COLOR22,  COLOR26,  COLOR30,  COLOR34,  COLOR38,  COLOR42,  COLOR46,            _______,            _______,
     COLOR02,            COLOR06,  COLOR10,  COLOR14,  COLOR18,  COLOR22,  COLOR26,  COLOR30,  COLOR34,  COLOR38,  COLOR42,            COLOR46,  _______,  _______,
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
int layer_used_indices_size[NUM_LAYERS];

// [CPICK]
#define PALETTE_SIZE 48 // TODO derive via sizeof color_picker_hues and/or color_picker_palette_keycodes?

// [CPICK]
int color_picker_hues[PALETTE_SIZE];

void matrix_init_user(void) {
    // [CPICK]
    /*
      0,   5,  11,  16,  21,  27,  32,  37,  43,  48,  53,  59,
     64,  69,  75,  80,  85,  91,  96, 101, 107, 112, 117, 123,
    128, 133, 139, 144, 149, 155, 160, 165, 171, 176, 181, 187,
    192, 197, 203, 208, 213, 219, 224, 229, 235, 240, 245, 251
    */
    for (int i = 0; i < PALETTE_SIZE; ++i) {
        color_picker_hues[i] = round(i * 256.0 / 48);
    }

    // [FN-HI]
    // TODO? only do for layers MAC_FN and WIN_FN (rather than all layers), since FN-HI only uses those layers (see
    //       rgb_matrix_indicators_user)
    for (int layer = 0; layer < NUM_LAYERS; ++layer) {
        layer_used_indices_size[layer] = initialize_layer_used_indices(layer, layers_used_indices[layer]);
    }
}

// [CPICK]
// TODO? check that the rvalue for the color_picker_palette_keycodes assignment actually has PALETTE_SIZE elements (not
//       fewer--too many would lead to a compilation failure, but too few would leave (presumably unwanted) 0s at the
//       end)
int color_picker_palette_keycodes[PALETTE_SIZE] = {
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

// [CPICK]
// returns 0 for COLOR00, 1 for COLOR01, ...
int get_color_picker_keycode_index(uint16_t keycode) {
    return keycode - COLOR00;
}

// [CPICK]
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool retval = true;
    switch (keycode) {
        case COLOR00:
        case COLOR01:
        case COLOR02:
        case COLOR03:
        case COLOR04:
        case COLOR05:
        case COLOR06:
        case COLOR07:
        case COLOR08:
        case COLOR09:
        case COLOR10:
        case COLOR11:
        case COLOR12:
        case COLOR13:
        case COLOR14:
        case COLOR15:
        case COLOR16:
        case COLOR17:
        case COLOR18:
        case COLOR19:
        case COLOR20:
        case COLOR21:
        case COLOR22:
        case COLOR23:
        case COLOR24:
        case COLOR25:
        case COLOR26:
        case COLOR27:
        case COLOR28:
        case COLOR29:
        case COLOR30:
        case COLOR31:
        case COLOR32:
        case COLOR33:
        case COLOR34:
        case COLOR35:
        case COLOR36:
        case COLOR37:
        case COLOR38:
        case COLOR39:
        case COLOR40:
        case COLOR41:
        case COLOR42:
        case COLOR43:
        case COLOR44:
        case COLOR45:
        case COLOR46:
        case COLOR47:
            if (record -> event.pressed) {
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
        case WIN_FN:; // ';' since o/w the call following the label is a declaration, which is not a statement
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
                rgb_matrix_set_color(
                    layers_used_indices[layer][i],
                    complement_rgb.r,
                    complement_rgb.g,
                    complement_rgb.b
                );
            }
            retval = false;
            break;

        // [CPICK]
        case CLR_PKR:
            rgb_matrix_set_color_all(0, 0, 0); // set keys not changed below to black
            rgb_matrix_set_color(0, 255, 0, 0); // ESC red // TODO? different color here
            // TODO? allow picking white (HSV = (0, 0, 255), RGB = (255, 255, 255))

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
