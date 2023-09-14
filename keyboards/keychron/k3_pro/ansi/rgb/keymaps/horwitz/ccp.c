#include QMK_KEYBOARD_H
#include <math.h> // for round() // [GCP] // [CCP]
#include "layers.h"
#include "ccp.h"
//#include "ctrlkeycodes.h"

/*
 * Used to set the base layer to a solid pattern of any 24-bit color. The picker's current color is shown on each key of
 * the 3x3 grid  IOP KL; ,./  . Hitting Enter (which will be white) accepts the picker's current color, setting the base
 * layer to a solid pattern of that color. Hitting End aborts, making no change in the base layer's color. A & S show
 * the red contribution (the two highest bytes in the 6-byte RGB representation; note that A and S's colors are the
 * same), D & F show the green contribution, and G & H show the blue contribution. Q increases the red contribution by
 * 16 and W increases it by 1--in both cases stopping at the max of 255. Similarly Z and X respectively lower the red
 * contribution by 16 and 1, stopping at the min of 0. Similarly E,R,C,V raise/lower green by 16 or 1; T,Y,B,N
 * raise/lower blue by 16 or 1. Q,E,T show what would happen if red, green, or blue (respectively) were maxed out with
 * the current settings kept for the other components; Z,C,B show what would happen if red, green, or blue
 * (respectively) were set to 0 with the current settings kept for the other components; W,R,Y show what would happen if
 * red, green, or blue (respectively) were brought up to the nearest multiple of 16 above the current red, green, or
 * blue (as appropriate) value (unless that's 256, in which case 255 is used); X,V,N show what would happen if red,
 * green, or blue (respectively) were brought down to the nearest multiple of 16 below the current red, green, or blue
 * (as appropriate) value. When any of QWERTYZXCVBN is pressed, the 16 top-row characters (Esc, F1, F2, ...) light up to
 * show the current modifier's value, in { 0, 1, ..., 15 }: e.g., if Q is pressed and red's high byte is at 5, it will
 * go up to 6 and the first 7 keys in the top row (Esc, F1, ..., F6) will light up. The representation is 0-based--i.e.,
 * 1 light represents a 0, 2 lights represent a 1, ..., all 16 lights represent a 15.
 */

// TODO(?) move (MIN|MAX)_CCP_CHANGE_KEYCODE definitions to ccp.h
int MIN_CCP_CHANGE_KEYCODE = RHI;
int MAX_CCP_CHANGE_KEYCODE = CCPSET;

// NB: does NOT include TOCCP (which is not on the CCP layer)
bool is_ccp_change_keycode(uint16_t keycode) {
    return keycode >= MIN_CCP_CHANGE_KEYCODE && keycode <= MAX_CCP_CHANGE_KEYCODE;
}

HSV rgb_to_hsv(RGB rgb) {
    HSV hsv;

    uint8_t rgbMin = min(rgb.r, min(rgb.g, rgb.b));
    uint8_t rgbMax = max(rgb.r, max(rgb.g, rgb.b));

    hsv.v = rgbMax;
    uint8_t chroma = rgbMax - rgbMin;
    if (chroma == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    // NB: hsv.v != 0 since chroma != 0
    hsv.s = round(255.0 * chroma / hsv.v);

    double unscaledSextupledH;
    if (rgbMax == rgb.r) {
        unscaledSextupledH = 1.0 * (rgb.g - rgb.b) / chroma;
    } else if (rgbMax == rgb.g) {
        unscaledSextupledH = (1.0 * (rgb.b - rgb.r) / chroma) + 2;
    } else if (rgbMax == rgb.b) {
        unscaledSextupledH = (1.0 * (rgb.r - rgb.g) / chroma) + 4;
    } else { // impossible case
        unscaledSextupledH = 0;
    }
    hsv.h = (int) round(unscaledSextupledH * 255 / 6);

    if (hsv.h < 0) hsv.h += 255;

    return hsv;
}

int index_in_byte = -1; // 0-15 value equal to the last hex value edited (one of RH, RL, GH, GL, BH, BL)

RGB ccpRgb;

ccp_key_t get_ccp_key(uint16_t keycode) {
    ccp_key_t ccp_key;

    switch (keycode) {
        case RHI:
        case RHD:
        case RLI:
        case RLD:
            ccp_key.color = RED;
            break;
        case GHI:
        case GHD:
        case GLI:
        case GLD:
            ccp_key.color = GREEN;
            break;
        case BHI:
        case BHD:
        case BLI:
        case BLD:
            ccp_key.color = BLUE;
            break;
    }

    switch (keycode) {
        case RHI:
        case RHD:
        case GHI:
        case GHD:
        case BHI:
        case BHD:
            ccp_key.nibbleLevel = HIGH;
            break;
        case RLI:
        case RLD:
        case GLI:
        case GLD:
        case BLI:
        case BLD:
            ccp_key.nibbleLevel = LOW;
            break;
    }

    switch (keycode) {
        case RHI:
        case RLI:
        case GHI:
        case GLI:
        case BHI:
        case BLI:
            ccp_key.deltaDir = INC;
            break;
        case RHD:
        case RLD:
        case GHD:
        case GLD:
        case BHD:
        case BLD:
            ccp_key.deltaDir = DEC;
            break;
    }

    return ccp_key;
}

layer_state_t layer_state_set_user_ccp(layer_state_t state) {
    // TODO replace switch with if
    switch (get_highest_layer(state)) {
        case CCP:
            index_in_byte = -1; // clear the white lights from the top row
            break;
        default:
            break;
        }

    return state;
}

bool process_record_user_ccp(uint16_t keycode, keyrecord_t *record) {
    bool retval;

    if (is_ccp_change_keycode(keycode)) {
        if (record -> event.pressed) {
            if (keycode == CCPSET) {
                rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
                HSV hsv = rgb_to_hsv(ccpRgb);
//                uprintf("CCPSET: ccpRgb=(%d,%d,%d) -> hsv=(%d,%d,%d)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b, hsv.h, hsv.s, hsv.v);
                rgb_matrix_sethsv(hsv.h, hsv.s, hsv.v);
                layer_off(CCP);
            } else {
//                uprintf("keycode-RHI: %2u\n", keycode - RHI);
//                uprintf(">> ccpRgb: (%2u,%2u,%2u)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
                // if RHI, then ccpRgb.r += 16, index_in_byte = ccpRgb.r/16
                // if RHD, then ccpRgb.r -= 16, index_in_byte = ccpRgb.r/16
                // if RLI, then ++(ccpRgb.r), index_in_byte = ccpRgb.r % 16
                // ...
                // if BLD, then --(ccpRgb.b), index_in_byte = ccpRgb.b % 16
                // TODO! is int16_t the right type?
                // TODO? don't initialize?
                int16_t component = -1; // whichever of ccpRgb.r, .g, or .b is going to change

                ccp_key_t ccp_key = get_ccp_key(keycode);

                switch (ccp_key.color) {
                    case RED:
                        component = ccpRgb.r;
                        break;
                    case GREEN:
                        component = ccpRgb.g;
                        break;
                    case BLUE:
                        component = ccpRgb.b;
                        break;
                }

                int absDelta = -1;
                 switch (ccp_key.nibbleLevel) {
                     case HIGH:
                         absDelta = 16;
                         break;
                     case LOW:
                         absDelta = 1;
                         break;
                }
                int delta = -1;
                switch (ccp_key.deltaDir) {
                    case INC:
                        delta = absDelta;
                        break;
                    case DEC:
                        delta = -absDelta;
                        break;
                }
                component = bound(component + delta);

                switch (ccp_key.nibbleLevel) {
                    case HIGH:
                        index_in_byte = component / 16;
                        break;
                    case LOW:
                        index_in_byte = component % 16;
                        break;
                }

//uprintf("before: (%2u,%2u,%2u)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
                switch (ccp_key.color) {
                    case RED:
                        ccpRgb.r = component;
                        break;
                    case GREEN:
                        ccpRgb.g = component;
                        break;
                    case BLUE:
                        ccpRgb.b = component;
                        break;
                }
//uprintf("after: (%2u,%2u,%2u)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);

            }
//            uprintf("<< ccpRgb: (%2u,%2u,%2u)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
//            uprintf("iib: %2u\n", index_in_byte);
        }
        retval = false;
    // [CCP]
    } else if (keycode == TOCCP) {
        ccpRgb = hsv_to_rgb_nocie(rgb_matrix_get_hsv());
//        uprintf("setting ccpRgb: (%d,%d,%d)\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);
        layer_on(CCP);
        retval = false;
    } else {
        retval = true; // process all other keycodes normally
    }

    return retval;
}

// TODO bool?
void rgb_matrix_indicators_advanced_user_ccp(void) {
        // set A,S to R level; D,F to G level; G,H to B level; set ENTER white; set some of top row to white (see
        // below); set QWERTY ZXCVBN as described above; set all else black
        rgb_matrix_set_color_all(RGB_OFF); // RGB_OFF == RGB_BLACK // set keys not changed below to black
        // ESC currently used for top-row 0-15 readout, so we couldn't use it as the abort key (at least we couldn't
        // color it to _signify_ that it's the abort key), so we use the End key as the abort key
        rgb_matrix_set_color(73, RGB_RED); // set End to red // TODO? different color here
//            uprintf("R (AS): %2u / G (DF): %2u / B (GH): %2u\n", ccpRgb.r, ccpRgb.g, ccpRgb.b);

        rgb_matrix_set_color(32, 255, ccpRgb.g, ccpRgb.b); // Q
        rgb_matrix_set_color(33, (ccpRgb.r / 16) * 16 + 16 - 1, ccpRgb.g, ccpRgb.b); // W
        rgb_matrix_set_color(34, ccpRgb.r, 255, ccpRgb.b); // E
        rgb_matrix_set_color(35, ccpRgb.r, (ccpRgb.g / 16) * 16 + 16 - 1, ccpRgb.b); // R
        rgb_matrix_set_color(36, ccpRgb.r, ccpRgb.g, 255); // T
        rgb_matrix_set_color(37, ccpRgb.r, ccpRgb.g, (ccpRgb.b / 16) * 16 + 16 - 1); // Y

        rgb_matrix_set_color(39, ccpRgb.r, ccpRgb.g, ccpRgb.b); // I
        rgb_matrix_set_color(40, ccpRgb.r, ccpRgb.g, ccpRgb.b); // O
        rgb_matrix_set_color(41, ccpRgb.r, ccpRgb.g, ccpRgb.b); // P
        rgb_matrix_set_color(54, ccpRgb.r, ccpRgb.g, ccpRgb.b); // K
        rgb_matrix_set_color(55, ccpRgb.r, ccpRgb.g, ccpRgb.b); // L
        rgb_matrix_set_color(56, ccpRgb.r, ccpRgb.g, ccpRgb.b); // ;
        rgb_matrix_set_color(68, ccpRgb.r, ccpRgb.g, ccpRgb.b); // ,
        rgb_matrix_set_color(69, ccpRgb.r, ccpRgb.g, ccpRgb.b); // .
        rgb_matrix_set_color(70, ccpRgb.r, ccpRgb.g, ccpRgb.b); // /

        rgb_matrix_set_color(47, ccpRgb.r, 0, 0); // A
        rgb_matrix_set_color(48, ccpRgb.r, 0, 0); // S
        rgb_matrix_set_color(49, 0, ccpRgb.g, 0); // D
        rgb_matrix_set_color(50, 0, ccpRgb.g, 0); // F
        rgb_matrix_set_color(51, 0, 0, ccpRgb.b); // G
        rgb_matrix_set_color(52, 0, 0, ccpRgb.b); // H

        rgb_matrix_set_color(61, 0, ccpRgb.g, ccpRgb.b); // Z
        rgb_matrix_set_color(62, (ccpRgb.r / 16) * 16, ccpRgb.g, ccpRgb.b); // X
        rgb_matrix_set_color(63, ccpRgb.r, 0, ccpRgb.b); // C
        rgb_matrix_set_color(64, ccpRgb.r, (ccpRgb.g / 16) * 16, ccpRgb.b); // V
        rgb_matrix_set_color(65, ccpRgb.r, ccpRgb.g, 0); // B
        rgb_matrix_set_color(66, ccpRgb.r, ccpRgb.g, (ccpRgb.b / 16) * 16); // N

        rgb_matrix_set_color(58, RGB_WHITE); // ENTER

//            if (index_in_byte >= 0) {
//                uprintf("setting 0-%2u to white\n", index_in_byte);
//            }
        // if index_in_byte >= 0, color ESC (white)
        // and if index_in_byte >= 1, color F1
        // ...
        // and if index_in_byte >= 15, color RGB_MOD
        for (int top_row_keycode = 0; top_row_keycode <= index_in_byte; ++top_row_keycode) {
            rgb_matrix_set_color(top_row_keycode, RGB_WHITE); // TODO? some other color than white (perhaps state dependent?)
        }
}
