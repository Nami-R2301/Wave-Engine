//
// Created by nami on 11/12/22.
//

#pragma once

// KEYBOARD KEY CODES FROM glfw3.h.
#define WAVE_KEY_UNKNOWN  -1
#define WAVE_KEY_SPACE  32
#define WAVE_KEY_APOSTROPHE  39
#define WAVE_KEY_COMMA  44
#define WAVE_KEY_MINUS  45
#define WAVE_KEY_PERIOD  46
#define WAVE_KEY_SLASH  47
#define WAVE_KEY_0  48
#define WAVE_KEY_1  49
#define WAVE_KEY_2  50
#define WAVE_KEY_3  51
#define WAVE_KEY_4  52
#define WAVE_KEY_5  53
#define WAVE_KEY_6  54
#define WAVE_KEY_7  55
#define WAVE_KEY_8  56
#define WAVE_KEY_9  57
#define WAVE_KEY_SEMICOLON  59 /*  */
#define WAVE_KEY_EQUAL  61 /*  */
#define WAVE_KEY_A  65
#define WAVE_KEY_B  66
#define WAVE_KEY_C  67
#define WAVE_KEY_D  68
#define WAVE_KEY_E  69
#define WAVE_KEY_F  70
#define WAVE_KEY_G  71
#define WAVE_KEY_H  72
#define WAVE_KEY_I  73
#define WAVE_KEY_J  74
#define WAVE_KEY_K  75
#define WAVE_KEY_L  76
#define WAVE_KEY_M  77
#define WAVE_KEY_N  78
#define WAVE_KEY_O  79
#define WAVE_KEY_P  80
#define WAVE_KEY_Q  81
#define WAVE_KEY_R  82
#define WAVE_KEY_S  83
#define WAVE_KEY_T  84
#define WAVE_KEY_U  85
#define WAVE_KEY_V  86
#define WAVE_KEY_W  87
#define WAVE_KEY_X  88
#define WAVE_KEY_Y  89
#define WAVE_KEY_Z  90
#define WAVE_KEY_LEFT_BRACKET  91 /* [ */
#define WAVE_KEY_BACKSLASH  92 /* \ */
#define WAVE_KEY_RIGHT_BRACKET  93 /* ] */
#define WAVE_KEY_GRAVE_ACCENT  96 /* ` */
#define WAVE_KEY_WORLD_1  161 /* non-US #1 */
#define WAVE_KEY_WORLD_2  162 /* non-US #2 */
#define WAVE_KEY_ESCAPE  256
#define WAVE_KEY_ENTER  257
#define WAVE_KEY_TAB  258
#define WAVE_KEY_BACKSPACE  259
#define WAVE_KEY_INSERT  260
#define WAVE_KEY_DELETE  261
#define WAVE_KEY_RIGHT  262
#define WAVE_KEY_LEFT  263
#define WAVE_KEY_DOWN  264
#define WAVE_KEY_UP  265
#define WAVE_KEY_PAGE_UP  266
#define WAVE_KEY_PAGE_DOWN  267
#define WAVE_KEY_HOME  268
#define WAVE_KEY_END  269
#define WAVE_KEY_CAPS_LOCK  280
#define WAVE_KEY_SCROLL_LOCK  281
#define WAVE_KEY_NUM_LOCK  282
#define WAVE_KEY_PRINT_SCREEN  283
#define WAVE_KEY_PAUSE  284
#define WAVE_KEY_F1  290
#define WAVE_KEY_F2  291
#define WAVE_KEY_F3  292
#define WAVE_KEY_F4  293
#define WAVE_KEY_F5  294
#define WAVE_KEY_F6  295
#define WAVE_KEY_F7  296
#define WAVE_KEY_F8  297
#define WAVE_KEY_F9  298
#define WAVE_KEY_F10  299
#define WAVE_KEY_F11  300
#define WAVE_KEY_F12  301
#define WAVE_KEY_F13  302
#define WAVE_KEY_F14  303
#define WAVE_KEY_F15  304
#define WAVE_KEY_F16  305
#define WAVE_KEY_F17  306
#define WAVE_KEY_F18  307
#define WAVE_KEY_F19  308
#define WAVE_KEY_F20  309
#define WAVE_KEY_F21  310
#define WAVE_KEY_F22  311
#define WAVE_KEY_F23  312
#define WAVE_KEY_F24  313
#define WAVE_KEY_F25  314
#define WAVE_KEY_KP_0  320
#define WAVE_KEY_KP_1  321
#define WAVE_KEY_KP_2  322
#define WAVE_KEY_KP_3  323
#define WAVE_KEY_KP_4  324
#define WAVE_KEY_KP_5  325
#define WAVE_KEY_KP_6  326
#define WAVE_KEY_KP_7  327
#define WAVE_KEY_KP_8  328
#define WAVE_KEY_KP_9  329
#define WAVE_KEY_KP_DECIMAL  330
#define WAVE_KEY_KP_DIVIDE  331
#define WAVE_KEY_KP_MULTIPLY  332
#define WAVE_KEY_KP_SUBTRACT  333
#define WAVE_KEY_KP_ADD  334
#define WAVE_KEY_KP_ENTER  335
#define WAVE_KEY_KP_EQUAL  336
#define WAVE_KEY_LEFT_SHIFT  340
#define WAVE_KEY_LEFT_CONTROL  341
#define WAVE_KEY_LEFT_ALT  342
#define WAVE_KEY_LEFT_SUPER  343
#define WAVE_KEY_RIGHT_SHIFT  344
#define WAVE_KEY_RIGHT_CONTROL  345
#define WAVE_KEY_RIGHT_ALT  346
#define WAVE_KEY_RIGHT_SUPER  347
#define WAVE_KEY_MENU  348
#define WAVE_KEY_LAST  WAVE_KEY_MENU

// KEY ACTIONS.
#define WAVE_RELEASE  0
#define WAVE_PRESS  1
#define WAVE_HOLD  2
