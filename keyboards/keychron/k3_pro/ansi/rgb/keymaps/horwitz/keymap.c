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
 * (4) Suspend RGB
 * (5) Enhanced color picking
 *
 * (1) SHORT NAME*: [DEBUG]
 *     DETAILS: When enabled, outputs the result of `uprintf` statements--these can be seen in the QMK Toolbox console.
 *     TO ACTIVATE: To debug, set `DEBUG` to 1 (as opposed to 0) and make sure that (horwitz/)rules.mk contains
 *                 `CONSOLE_ENABLE = yes`; then use (e.g.) `uprintf` to print debug output.
 *
 * (2) SHORT NAME: [FN-HI]
 *     DETAILS: When the `fn` key is pressed, the keys with new keycodes (i.e., not `_______`) will light up in the
 *              color complementary to the color† of layer 0‡; the remaining keys will retain the lighting behavior
 *              from before the `fn` key was pressed.
 *     TO ACTIVATE: The feature is always on.
 *
 * (3) SHORT NAME: [CPICK]
 *     DETAILS: `fn+V` presents a 12x4 rainbow grid of keys and 12 grayscale keys on the top row (with the remaining
 *              keys§ dark)--pressing any one of these keys sets the base layer (e.g., layer 0 when `fn` goes to layer
 *              1) to a solid pattern of that color. The top row (of the four color rows) are primary, secondary, and
 *              tertiary colors--in standard order: red (1), orange (3), yellow (2), chartreuse (3), green (1), spring
 *              green (3), cyan (2), azure (3), blue (1), violet (3), magenta (2), rose (3) (colors, going down then
 *              across, increase in hue by approximately 1/48 (in a range of [0,1)) (w/(HSV) saturation and value both
 *              of 1 (in [0,1])). The grayscale keys, left to right, produce an HSV value from 0 to 1 (in a range of
 *              [0,1]) (w/(HSV) hue and saturation both of 0 (in [0,1) and [0,1], respectively)).
 *     TO ACTIVATE: The feature is always on.
 *     NOTE: The feature creates a fifth layer (layer 4) for the rainbow grid (meant only to be accessed for the
 *           purposes mentioned above in DETAILS). Actual hue values are encoded as integers in [0,256) (scaled from
 *           [0,1)).
 *
 * (4) SHORT NAME: [SUS-RGB]
 *     DETAILS: Turns off lighting when the laptop sleeps (and for similar(?) behavior).
 *     TO ACTIVATE: The feature is always on.
 *
 * (5) SHORT NAME: [ECP]
 *     DETAILS: Used to set the base layer to a solid pattern of any 24-bit color. The picker's current color is shown
 *              on each key of the 3x3 grid  IOP KL; ,./  . Hitting Enter (which will be white) accepts the picker's
 *              current color, setting the base layer to a solid pattern of that color. Hitting Esc aborts, making no
 *              change in the base layer's color. A & S show the red contribution (the two highest bytes in the 6-byte
 *              RBG representation; note that A and S's colors are the same), D & F show the green contribution, and
 *              G & H show the blue contribution. Q increases the red contribution by 16 and W increases it by 1--in
 *              both cases stopping at the max of 255. Similarly Z and X respectively lower the red contribution by 16
 *              and 1, stopping at the min of 0. Similarly E,R,C,V raise/lower green by 16 or 1; T,Y,B,N raise/lower
 *              blue by 16 or 1. Q,E,T show what would happen if red, green, or blue (respectively) were maxed out with
 *              the current settings kept for the other components; Z,C,B show what would happen if red, green, or blue
 *              (respectively) were set to 0 with the current settings kept for the other components; W,R,Y show what
 *              would happen if red, green, or blue (respectively) were brought up to the nearest multiple of 16 above
 *              the current red, green, or blue (as appropriate) value (unless that's 256, in which case 255 is used);
 *              X,V,N show what would happen if red, green, or blue (respectively) were brought down to the nearest
 *              multiple of 16 below the current red, green, or blue (as appropriate) value. When any of QWERTYZXCVBN is
 *              pressed, the 16 top-row characters (Esc, F1, F2, ...) light up to show the current modifier's value, in
 *              { 0, 1, ..., 15 }: e.g., if Q is pressed and red's high byte is at 5, it will go up to 6 and the first 7
 *              keys in the top row (Esc, F1, ..., F6) will light up. 1 light represents a 0, 2 lights represent a 1,
 *              ..., all 16 lights represent a 15.
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
 * § RGB_MOD is set to RGB_RED and, when hit, leaves the layer, making no change to the base layer's color
 */

// [DEBUG]
#define DEBUG 0//1

#include QMK_KEYBOARD_H
#include <math.h> // for round() // [CPICK]
// [DEBUG]
#if DEBUG
    #include "print.h"
#endif

// [ECP]
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
// TODO(?) use (a not-broken version of) these macros instead of the macros above
//         (see https://stackoverflow.com/a/3437484)
/*
#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })
#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })
*/
// if a is in [0, 255], returns a; if a < 0, return 0; if a > 255, return 255
#define bound(a) \
    ({ __typeof__ (a) _a = (a); \
    max(min(_a, 255), 0); })

// clang-format off
enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
    CLR_PKR, // [CPICK]
    ECP // [ECP]
};

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
        COLOR44, COLOR45, COLOR46, COLOR47,
        GRAY00, GRAY01, GRAY02, GRAY03,
        GRAY04, GRAY05, GRAY06, GRAY07,
        GRAY08, GRAY09, GRAY10, GRAY11,

// [ECP] // meta ECP
        TOECP,
// [ECP]
        RHI, RLI, GHI, GLI, BHI, BLI,
        RHD, RLD, GHD, GLD, BHD, BLD,
        ECPSET
};
int MIN_COLOR_KEYCODE = COLOR00; // [CPICK]
int MAX_COLOR_KEYCODE = GRAY11; // [CPICK]
int MIN_ECP_CHANGE_KEYCODE = RHI; // [ECP]
int MAX_ECP_CHANGE_KEYCODE = ECPSET; // [ECP]

// [CPICK]
enum COLOR_SCHEME { RGB_SCHEME, GRAY_SCHEME };

// [ECP]
enum RGB_COLOR { RED, GREEN, BLUE };
enum NIBBLE_LEVEL { HIGH, LOW };
enum DELTA_DIR { INC, DEC };

// [ECP]
typedef struct {
    enum RGB_COLOR color;
    enum NIBBLE_LEVEL nibbleLevel;
    enum DELTA_DIR deltaDir;
} ecp_key_t;

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
     _______,            _______,  _______,  TOECP,  OSL(CLR_PKR),BAT_LVL, NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,  // [ECP] ("TOECP") // [CPICK] ("OSL(CLR_PKR)")
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
     _______,            _______,  _______,  TOECP,  OSL(CLR_PKR),BAT_LVL, NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,  // [ECP] ("TOECP") // [CPICK] ("OSL(CLR_PKR)")
     _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

// [CPICK]
[CLR_PKR] = LAYOUT_ansi_84(
     // TODO go back to WIN_BASE (instead of MAC_BASE) as appropriate
     GRAY00,   GRAY01,   GRAY02,   GRAY03,   GRAY04,   GRAY05,   GRAY06,   GRAY07,   GRAY08,   GRAY09,   GRAY10,   GRAY11,   XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_NO,
     COLOR00,  COLOR04,  COLOR08,  COLOR12,  COLOR16,  COLOR20,  COLOR24,  COLOR28,  COLOR32,  COLOR36,  COLOR40,  COLOR44,  XXXXXXX,  XXXXXXX,            XXXXXXX,
     COLOR01,  COLOR05,  COLOR09,  COLOR13,  COLOR17,  COLOR21,  COLOR25,  COLOR29,  COLOR33,  COLOR37,  COLOR41,  COLOR45,  XXXXXXX,  XXXXXXX,            XXXXXXX,
     COLOR02,  COLOR06,  COLOR10,  COLOR14,  COLOR18,  COLOR22,  COLOR26,  COLOR30,  COLOR34,  COLOR38,  COLOR42,  COLOR46,            XXXXXXX,            XXXXXXX,
     COLOR03,            COLOR07,  COLOR11,  COLOR15,  COLOR19,  COLOR23,  COLOR27,  COLOR31,  COLOR35,  COLOR39,  COLOR43,            COLOR47,  XXXXXXX,  XXXXXXX,
     XXXXXXX,  XXXXXXX,  XXXXXXX,                                XXXXXXX,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX),

// [ECP]
[ECP] = LAYOUT_ansi_84(
     // TODO go back to WIN_BASE (instead of MAC_BASE) as appropriate
     TO(MAC_BASE),XXXXXXX,XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,
     XXXXXXX,  RHI,      RLI,      GHI,      GLI,      BHI,      BLI,      XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            ECPSET,             XXXXXXX,
     XXXXXXX,            RHD,      RLD,      GHD,      GLD,      BHD,      BLD,      XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,  XXXXXXX,
     XXXXXXX,  XXXXXXX,  XXXXXXX,                                XXXXXXX,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX)
};

// [FN-HI]
/*
 * sets the values in [layer_used_indices] to the indices (in increasing order; a subset of 0-83) that are used in the
 * given [layer] ("used" = not _______) and returns how many values were set. (e.g., if keys at index 0, 5, 83 are the
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
            case _______:
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
// for any given layer, layers_used_indices[layer] should not exceed RGB_MATRIX_LED_COUNT in size, since each index
// found in the array should represent a key with a distinct light (see also: doc for initialize_layer_used_indices)
// TODO? validate that RGB_MATRIX_LED_COUNT is 84
char layers_used_indices[DYNAMIC_KEYMAP_LAYER_COUNT][RGB_MATRIX_LED_COUNT];
int layer_used_indices_size[DYNAMIC_KEYMAP_LAYER_COUNT];

// [CPICK]
// TODO? (1) derive via sizeof color_picker_color_hues and/or color_picker_color_palette_keycodes OR (2) validate that
//       color_picker_color_palette_keycodes is initialized with an rvalue of COLOR_PALETTE_SIZE elements (see comment
//       above color_picker_color_palette_keycodes's initialization)
#define COLOR_PALETTE_SIZE 48
#define GRAY_PALETTE_SIZE 12

// [CPICK]
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

    // [CPICK]
    /*
      hues:
      0,   5,  11,  16,  21,  27,  32,  37,  43,  48,  53,  59,
     64,  69,  75,  80,  85,  91,  96, 101, 107, 112, 117, 123,
    128, 133, 139, 144, 149, 155, 160, 165, 171, 176, 181, 187,
    192, 197, 203, 208, 213, 219, 224, 229, 235, 240, 245, 251
    */
    for (int i = 0; i < COLOR_PALETTE_SIZE; ++i) {
        color_picker_color_hues[i] = round(i * 256.0 / COLOR_PALETTE_SIZE);
        HSV hsv = { color_picker_color_hues[i], 255, 255 };
        color_picker_color_rgbs[i] = hsv_to_rgb(hsv);
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

// [CPICK]
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

// [CPICK]
// returns 0 for COLOR00, 1 for COLOR01, ..., 47 for COLOR47
int get_color_picker_color_keycode_index(uint16_t keycode) {
    return keycode - COLOR00;
}

// [CPICK]
// returns 0 for GRAY00, 1 for COLOR01, ..., 11 for GRAY11
int get_color_picker_gray_keycode_index(uint16_t keycode) {
    return keycode - GRAY00;
}

// [CPICK]
bool is_color_picker_color_keycode(uint16_t keycode) {
    return keycode >= MIN_COLOR_KEYCODE && keycode <= MAX_COLOR_KEYCODE;
}

// [CPICK]
enum COLOR_SCHEME get_color_scheme(uint16_t keycode) {
    if (keycode >= COLOR00 && keycode <= COLOR47) {
        return RGB_SCHEME;
    } else if (keycode >= GRAY00 && keycode <= GRAY11) {
        return GRAY_SCHEME;
    } else {
        return -1; // this should never happen... (TODO handle some other way?)
    }
}

// [ECP]
// NB: does NOT include TOECP (which is not on the ECP layer)
bool is_ecp_change_keycode(uint16_t keycode) {
    return keycode >= MIN_ECP_CHANGE_KEYCODE && keycode <= MAX_ECP_CHANGE_KEYCODE;
}

// [ECP]
int index_in_byte = -1; // 0-15 value equal to the last hex value edited (one of RH, RL, GH, GL, BH, BL)

// [ECP]
// from lv_color.c (adapted to [0-255]x[0-255]x[0-255] -> [0-255]x[0-255]x[0-255])
// TODO! is this right?
HSV rgb_to_hsv(RGB rgb) {
    uint16_t r = ((uint32_t)(rgb.r) << 10) / 255;
    uint16_t g = ((uint32_t)(rgb.g) << 10) / 255;
    uint16_t b = ((uint32_t)(rgb.b) << 10) / 255;
    uint16_t rgbMin = min(r, min(g, b));
    uint16_t rgbMax = max(r, max(g, b));
    HSV hsv;
    // https://en.wikipedia.org/wiki/HSL_and_HSV#Lightness
    hsv.v = (255 * rgbMax) >> 10;
    int32_t delta = rgbMax - rgbMin;
    if (delta < 3) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }
    // https://en.wikipedia.org/wiki/HSL_and_HSV#Saturation
    hsv.s = 255 * delta / rgbMax;
    if (hsv.s < 3) {
        hsv.h = 0;
        return hsv;
    }
    // https://en.wikipedia.org/wiki/HSL_and_HSV#Hue_and_chroma
    int32_t h;
    if (rgbMax == r) {
        h = (((g - b) << 10) / delta) + (g < b ? (6 << 10) : 0); // between yellow & magenta
    } else if (rgbMax == g) {
        h = (((b - r) << 10) / delta) + (2 << 10); // between cyan & yellow
    } else if (rgbMax == b) {
        h = (((r - g) << 10) / delta) + (4 << 10); // between magenta & cyan
    } else { // should not occur!
        h = 0;
    }
    h *= 60;
    h >>= 10;
    if (h < 0) {
        h += 360;
    }
    hsv.h = h * 255.0 / 360;
    return hsv;
}

// [ECP]
RGB ecpRgb;

// [ECP]
ecp_key_t get_ecp_key(uint16_t keycode) {
    ecp_key_t ecp_key;

    switch (keycode) {
        case RHI:
        case RHD:
        case RLI:
        case RLD:
            ecp_key.color = RED;
            break;
        case GHI:
        case GHD:
        case GLI:
        case GLD:
            ecp_key.color = GREEN;
            break;
        case BHI:
        case BHD:
        case BLI:
        case BLD:
            ecp_key.color = BLUE;
            break;
    }

    switch (keycode) {
        case RHI:
        case RHD:
        case GHI:
        case GHD:
        case BHI:
        case BHD:
            ecp_key.nibbleLevel = HIGH;
            break;
        case RLI:
        case RLD:
        case GLI:
        case GLD:
        case BLI:
        case BLD:
            ecp_key.nibbleLevel = LOW;
            break;
    }

    switch (keycode) {
        case RHI:
        case RLI:
        case GHI:
        case GLI:
        case BHI:
        case BLI:
            ecp_key.deltaDir = INC;
            break;
        case RHD:
        case RLD:
        case GHD:
        case GLD:
        case BHD:
        case BLD:
            ecp_key.deltaDir = DEC;
            break;
    }

    return ecp_key;
}

// [ECP]
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case ECP:
            index_in_byte = -1; // clear the white lights from the top row
            break;
        default:
            break;
        }

    return state;
}

// [CPICK]
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
                    rgb_matrix_sethsv(0, 0, color_picker_gray_intensities[get_color_picker_gray_keycode_index(keycode)]);
                    break;
            }
        }
        retval = false;
    // [ECP]
    } else if (is_ecp_change_keycode(keycode)) {
        if (record -> event.pressed) {
            if (keycode == ECPSET) {
                rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
                HSV hsv = rgb_to_hsv(ecpRgb);
//                uprintf("ECPSET: ecpRgb=(%d,%d,%d) -> hsv=(%d,%d,%d)\n", ecpRgb.r, ecpRgb.g, ecpRgb.b, hsv.h, hsv.s, hsv.v);
                rgb_matrix_sethsv(hsv.h, hsv.s, hsv.v);
                layer_off(ECP);
            } else {
//                uprintf("keycode-RHI: %2u\n", keycode - RHI);
//                uprintf(">> ecpRgb: (%2u,%2u,%2u)\n", ecpRgb.r, ecpRgb.g, ecpRgb.b);
                // if RHI, then ecpRgb.r += 16, index_in_byte = ecpRgb.r/16
                // if RHD, then ecpRgb.r -= 16, index_in_byte = ecpRgb.r/16
                // if RLI, then ++(ecpRgb.r), index_in_byte = ecpRgb.r % 16
                // ...
                // if BLD, then --(ecpRgb.b), index_in_byte = ecpRgb.b % 16
                // TODO! is int16_t the right type?
                // TODO? don't initialize?
                int16_t component = -1; // whichever of ecpRgb.r, .g, or .b is going to change

                ecp_key_t ecp_key = get_ecp_key(keycode);

                switch (ecp_key.color) {
                    case RED:
                        component = ecpRgb.r;
                        break;
                    case GREEN:
                        component = ecpRgb.g;
                        break;
                    case BLUE:
                        component = ecpRgb.b;
                        break;
                }

                int absDelta = -1;
                 switch (ecp_key.nibbleLevel) {
                     case HIGH:
                         absDelta = 16;
                         break;
                     case LOW:
                         absDelta = 1;
                         break;
                }
                int delta = -1;
                switch (ecp_key.deltaDir) {
                    case INC:
                        delta = absDelta;
                        break;
                    case DEC:
                        delta = -absDelta;
                        break;
                }
                component = bound(component + delta);

                switch (ecp_key.nibbleLevel) {
                    case HIGH:
                        index_in_byte = component / 16;
                        break;
                    case LOW:
                        index_in_byte = component % 16;
                        break;
                }

//uprintf("before: (%2u,%2u,%2u)\n", ecpRgb.r, ecpRgb.g, ecpRgb.b);
                switch (ecp_key.color) {
                    case RED:
                        ecpRgb.r = component;
                        break;
                    case GREEN:
                        ecpRgb.g = component;
                        break;
                    case BLUE:
                        ecpRgb.b = component;
                        break;
                }
//uprintf("after: (%2u,%2u,%2u)\n", ecpRgb.r, ecpRgb.g, ecpRgb.b);

            }
//            uprintf("<< ecpRgb: (%2u,%2u,%2u)\n", ecpRgb.r, ecpRgb.g, ecpRgb.b);
//            uprintf("iib: %2u\n", index_in_byte);
        }
        retval = false;
    // [ECP]
    } else if (keycode == TOECP) {
        ecpRgb = hsv_to_rgb(rgb_matrix_get_hsv());
//        uprintf("setting ecpRgb: (%d,%d,%d)\n", ecpRgb.r, ecpRgb.g, ecpRgb.b);
        layer_on(ECP);
        retval = false;
    } else {
        retval = true; // process all other keycodes normally
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
            // TODO? if all r,g,b are close to 255/2, the complement will be close to (and hard to discern from) the
            //       original color (should this be changed?)
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
            break;
        }

        // [CPICK]
        case CLR_PKR: {
            rgb_matrix_set_color_all(RGB_BLACK); // set keys not changed below to black
            rgb_matrix_set_color(15, RGB_RED); // RGB_MOD red // TODO? different color here

            // [CPICK]
            for (int i = 0; i < COLOR_PALETTE_SIZE; ++i) {
                RGB rgb = color_picker_color_rgbs[i];
                rgb_matrix_set_color(color_picker_color_palette_keycodes[i], rgb.r, rgb.g, rgb.b);
            }
            for (int i = 0; i < GRAY_PALETTE_SIZE; ++i) {
                int intensity = color_picker_gray_intensities[i];
                rgb_matrix_set_color(color_picker_gray_palette_keycodes[i], intensity, intensity, intensity);
            }

            break;
        }

        // [ECP]
        case ECP: {
            // set A,S to R level; D,F to G level; G,H to B level; set ENTER white; set some of top row to white (see
            // below); set QWERTY ZXCVBN as described above; set all else black
            rgb_matrix_set_color_all(RGB_BLACK); // set keys not changed below to black
            // ESC currently used for top-row 0-15 readout instead of signifying that it's the abort key (by coloring it
            // red, e.g.)
//            rgb_matrix_set_color(15, RGB_RED); // RGB_MOD red // TODO? different color here
//            uprintf("R (AS): %2u / G (DF): %2u / B (GH): %2u\n", ecpRgb.r, ecpRgb.g, ecpRgb.b);

            rgb_matrix_set_color(32, 255, ecpRgb.g, ecpRgb.b); // Q
            rgb_matrix_set_color(33, (ecpRgb.r / 16) * 16 + 16 - 1, ecpRgb.g, ecpRgb.b); // W
            rgb_matrix_set_color(34, ecpRgb.r, 255, ecpRgb.b); // E
            rgb_matrix_set_color(35, ecpRgb.r, (ecpRgb.g / 16) * 16 + 16 - 1, ecpRgb.b); // R
            rgb_matrix_set_color(36, ecpRgb.r, ecpRgb.g, 255); // T
            rgb_matrix_set_color(37, ecpRgb.r, ecpRgb.g, (ecpRgb.b / 16) * 16 + 16 - 1); // Y

            rgb_matrix_set_color(39, ecpRgb.r, ecpRgb.g, ecpRgb.b); // I
            rgb_matrix_set_color(40, ecpRgb.r, ecpRgb.g, ecpRgb.b); // O
            rgb_matrix_set_color(41, ecpRgb.r, ecpRgb.g, ecpRgb.b); // P
            rgb_matrix_set_color(54, ecpRgb.r, ecpRgb.g, ecpRgb.b); // K
            rgb_matrix_set_color(55, ecpRgb.r, ecpRgb.g, ecpRgb.b); // L
            rgb_matrix_set_color(56, ecpRgb.r, ecpRgb.g, ecpRgb.b); // ;
            rgb_matrix_set_color(68, ecpRgb.r, ecpRgb.g, ecpRgb.b); // ,
            rgb_matrix_set_color(69, ecpRgb.r, ecpRgb.g, ecpRgb.b); // .
            rgb_matrix_set_color(70, ecpRgb.r, ecpRgb.g, ecpRgb.b); // /

            rgb_matrix_set_color(47, ecpRgb.r, 0, 0); // A
            rgb_matrix_set_color(48, ecpRgb.r, 0, 0); // S
            rgb_matrix_set_color(49, 0, ecpRgb.g, 0); // D
            rgb_matrix_set_color(50, 0, ecpRgb.g, 0); // F
            rgb_matrix_set_color(51, 0, 0, ecpRgb.b); // G
            rgb_matrix_set_color(52, 0, 0, ecpRgb.b); // H

            rgb_matrix_set_color(61, 0, ecpRgb.g, ecpRgb.b); // Z
            rgb_matrix_set_color(62, (ecpRgb.r / 16) * 16, ecpRgb.g, ecpRgb.b); // X
            rgb_matrix_set_color(63, ecpRgb.r, 0, ecpRgb.b); // C
            rgb_matrix_set_color(64, ecpRgb.r, (ecpRgb.g / 16) * 16, ecpRgb.b); // V
            rgb_matrix_set_color(65, ecpRgb.r, ecpRgb.g, 0); // B
            rgb_matrix_set_color(66, ecpRgb.r, ecpRgb.g, (ecpRgb.b / 16) * 16); // N

            rgb_matrix_set_color(58, RGB_WHITE); // ENTER

//            if (index_in_byte >= 0) {
//                uprintf("setting 0-%2u to white\n", index_in_byte);
//            }
            // if index_in_byte >= 0, color ESC (white)
            // and if index_in_byte >= 1, color F1
            // ...
            // and if index_in_byte >= 15, color RGB_TOG
            for (int top_row_keycode = 0; top_row_keycode <= index_in_byte; ++top_row_keycode) {
                rgb_matrix_set_color(top_row_keycode, RGB_WHITE); // TODO? some other color than white (perhaps state dependent?)
            }
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
