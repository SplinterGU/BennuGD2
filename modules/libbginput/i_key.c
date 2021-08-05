/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
 *  Copyright (C) 2002-2006 Fenix Team (Fenix)
 *  Copyright (C) 1999-2002 José Luis Cebrián Pagüe (Fenix)
 *
 *  This file is part of Bennu Game Development
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "xctype.h"
#include "bgddl.h"
#include "dlvaracc.h"

#include "libbginput.h"

/* ---------------------------------------------------------------------- */

typedef struct {
    SDL_Scancode scancode;  /* SDL scancode */
    SDL_Keymod mod;         /* current key modifiers */
    HOTKEY_CALLBACK callback;
} hotkey;

static hotkey * hotkey_list = NULL;
static int hotkey_allocated = 0;
static int hotkey_count = 0;

/* ---------------------------------------------------------------------- */

static uint16_t key_serial = 1;
static uint16_t key_evt[SDL_NUM_SCANCODES];
static int key_evt_clean_idx = 0;

/* ---------------------------------------------------------------------- */

#define MAX_KEY_CODES   128

/* Publicas */
/* the size of this array must be at least the number of the largest BGD key code + 1 */
/* 128 is a good value */
key_equiv key_table[MAX_KEY_CODES]; /* Now we have a search table with equivs */
const Uint8 * keystate = NULL;          /* Pointer to key states */

/* ---------------------------------------------------------------------- */

static int64_t equivs[] = {
    SDL_SCANCODE_ESCAPE,        1,
    SDL_SCANCODE_1,             2,
    SDL_SCANCODE_KP_1,          2,
    SDL_SCANCODE_2,             3,
    SDL_SCANCODE_KP_2,          3,
    SDL_SCANCODE_3,             4,
    SDL_SCANCODE_KP_3,          4,
    SDL_SCANCODE_4,             5,
    SDL_SCANCODE_KP_4,          5,
    SDL_SCANCODE_5,             6,
    SDL_SCANCODE_KP_5,          6,
    SDL_SCANCODE_6,             7,
    SDL_SCANCODE_KP_6,          7,
    SDL_SCANCODE_7,             8,
    SDL_SCANCODE_KP_7,          8,
    SDL_SCANCODE_8,             9,
    SDL_SCANCODE_KP_8,          9,
    SDL_SCANCODE_9,             10,
    SDL_SCANCODE_KP_9,          10,
    SDL_SCANCODE_0,             11,
    SDL_SCANCODE_KP_0,          11,
    SDL_SCANCODE_MINUS,         12,
    SDL_SCANCODE_EQUALS,        13,
    SDL_SCANCODE_BACKSPACE,     14,
    SDL_SCANCODE_TAB,           15,
    SDL_SCANCODE_Q,             16,
    SDL_SCANCODE_W,             17,
    SDL_SCANCODE_E,             18,
    SDL_SCANCODE_R,             19,
    SDL_SCANCODE_T,             20,
    SDL_SCANCODE_Y,             21,
    SDL_SCANCODE_U,             22,
    SDL_SCANCODE_I,             23,
    SDL_SCANCODE_O,             24,
    SDL_SCANCODE_P,             25,
    SDL_SCANCODE_LEFTBRACKET,   26,
    SDL_SCANCODE_RIGHTBRACKET,  27,
    SDL_SCANCODE_RETURN,        28,
    SDL_SCANCODE_KP_ENTER,      28,
    SDL_SCANCODE_LCTRL,         29,
    SDL_SCANCODE_RCTRL,         29,
    SDL_SCANCODE_LCTRL,         96,
    SDL_SCANCODE_RCTRL,         94,
    SDL_SCANCODE_A,             30,
    SDL_SCANCODE_S,             31,
    SDL_SCANCODE_D,             32,
    SDL_SCANCODE_F,             33,
    SDL_SCANCODE_G,             34,
    SDL_SCANCODE_H,             35,
    SDL_SCANCODE_J,             36,
    SDL_SCANCODE_K,             37,
    SDL_SCANCODE_L,             38,
    SDL_SCANCODE_SEMICOLON,     39,
    SDL_SCANCODE_APOSTROPHE,    40,
    SDL_SCANCODE_GRAVE,         41,
    SDL_SCANCODE_LSHIFT,        42,
    SDL_SCANCODE_BACKSLASH,     43,
    SDL_SCANCODE_Z,             44,
    SDL_SCANCODE_X,             45,
    SDL_SCANCODE_C,             46,
    SDL_SCANCODE_V,             47,
    SDL_SCANCODE_B,             48,
    SDL_SCANCODE_N,             49,
    SDL_SCANCODE_M,             50,
    SDL_SCANCODE_COMMA,         51,
    SDL_SCANCODE_PERIOD,        52,
    SDL_SCANCODE_KP_PERIOD,     52,
    SDL_SCANCODE_SLASH,         53,
    SDL_SCANCODE_KP_DIVIDE,     53,
    SDL_SCANCODE_RSHIFT,        54,
    SDL_SCANCODE_PRINTSCREEN,   55,
    SDL_SCANCODE_KP_MULTIPLY,   55,
    SDL_SCANCODE_LALT,          56,
    SDL_SCANCODE_RALT,          56,
    SDL_SCANCODE_LALT,          95,
    SDL_SCANCODE_RALT,          93,
    SDL_SCANCODE_MODE,          93,
    SDL_SCANCODE_MODE,          56,
    SDL_SCANCODE_RGUI,          56,
    SDL_SCANCODE_LGUI,          56,
    SDL_SCANCODE_SPACE,         57,
    SDL_SCANCODE_CAPSLOCK,      58,
    SDL_SCANCODE_F1,            59,
    SDL_SCANCODE_F2,            60,
    SDL_SCANCODE_F3,            61,
    SDL_SCANCODE_F4,            62,
    SDL_SCANCODE_F5,            63,
    SDL_SCANCODE_F6,            64,
    SDL_SCANCODE_F7,            65,
    SDL_SCANCODE_F8,            66,
    SDL_SCANCODE_F9,            67,
    SDL_SCANCODE_F10,           68,
    SDL_SCANCODE_NUMLOCKCLEAR,  69,
    SDL_SCANCODE_SCROLLLOCK,    70,
    SDL_SCANCODE_HOME,          71,
    SDL_SCANCODE_UP,            72,
    SDL_SCANCODE_PAGEUP,        73,
    SDL_SCANCODE_KP_MINUS,      74,
    SDL_SCANCODE_LEFT,          75,
    SDL_SCANCODE_RIGHT,         77,
    SDL_SCANCODE_KP_PLUS,       78,
    SDL_SCANCODE_END,           79,
    SDL_SCANCODE_DOWN,          80,
    SDL_SCANCODE_PAGEDOWN,      81,
    SDL_SCANCODE_INSERT,        82,
    SDL_SCANCODE_DELETE,        83,
    SDL_SCANCODE_F11,           87,
    SDL_SCANCODE_F12,           88,
    SDL_SCANCODE_KP_LESS,       89,
    SDL_SCANCODE_KP_PLUS,       90,
    SDL_SCANCODE_KP_GREATER,    91,
//    SDL_SCANCODE_QUESTION,      92,
    SDL_SCANCODE_GRAVE,         93,
    SDL_SCANCODE_SYSREQ,        55,
    SDL_SCANCODE_PAUSE,         95,
    SDL_SCANCODE_MENU,          97,
    SDL_SCANCODE_LGUI,          98,
    SDL_SCANCODE_RGUI,          99,
//    SDL_SCANCODE_COMPOSE,       99,
//    SDL_SCANCODE_VOLUMEUP,     100,
//    SDL_SCANCODE_VOLUMEDOWN,   101,
//    SDL_SCANCODE_AC_BACK,      102,
//    SDL_SCANCODE_AC_SEARCH,    103,
    -1, -1
};

/* ---------------------------------------------------------------------- */

int key_event_happend( SDL_Scancode scancode, uint16_t mask_event )
{
    if ( scancode < 0 || scancode >= SDL_NUM_SCANCODES ) return 0;
    return key_evt[ scancode ] == ( key_serial | mask_event );
}

/* ---------------------------------------------------------------------- */

static void add_key_equiv( int64_t equiv, int64_t keyconst ) {
    key_equiv * curr = &key_table[keyconst];

//    if ( curr->next != NULL ) 
    while ( curr->next != NULL ) curr = curr->next;

    if ( curr->sdlk_equiv != 0 ) {
        curr->next = malloc( sizeof( key_equiv ) );
        curr = curr->next;
        curr->next = NULL;
    }

    curr->sdlk_equiv = equiv;
}

/* ---------------------------------------------------------------------- */

void hotkey_add( int64_t mod, int64_t scancode, HOTKEY_CALLBACK callback ) {
    if ( hotkey_count >= hotkey_allocated ) {
        hotkey_allocated = hotkey_count + 5;
        hotkey_list = realloc( hotkey_list, hotkey_allocated * sizeof( hotkey_list[0] ) );
    }

    if ( !hotkey_list ) {
        fprintf( stderr, "No memory for alloc hotkey\n" );
        exit( -1 );
    }

    hotkey_list [hotkey_count].mod = mod;
    hotkey_list [hotkey_count].scancode = scancode;
    hotkey_list [hotkey_count].callback = callback;
    hotkey_count++;
}

/* ---------------------------------------------------------------------- */

static struct {
    int64_t ascii;
    int64_t scancode;
} keyring [64];
static int keyring_start = 0, keyring_tail = 0;
static int64_t scancode = 0;

/* ---------------------------------------------------------------------- */

static void process_key( SDL_Event *e, int64_t ascii, int *keypress ) {
    int n;
    SDL_Keymod m;
    int ignore_key = 0;

    /* KeyDown HotKey */
    for ( n = 0; n < hotkey_count; n++ ) {
        if ((( hotkey_list[n].mod & e->key.keysym.mod ) == hotkey_list[n].mod ) &&
             ( !hotkey_list[n].scancode || ( hotkey_list[n].scancode == e->key.keysym.sym ) ) ) {
            if ( ascii ) e->key.keysym.sym = ascii;
            ignore_key = hotkey_list[n].callback( e->key.keysym );
        }
    }
    /* KeyDown HotKey */

    if ( ignore_key ) return;

    /* Save key */

    int k = e->key.keysym.scancode;

    if ( !*keypress ) {
        GLOQWORD( libbginput, SCANCODE ) = scancode = k;
        GLOQWORD( libbginput, ASCII ) = ascii;
        *keypress = 1;
    } else {
        keyring[keyring_tail].scancode = k;
        keyring[keyring_tail].ascii = ascii;
        if ( ++keyring_tail == 64 ) keyring_tail = 0;
    }
}

/* ---------------------------------------------------------------------- */

/*
 *  FUNCTION : process_key_events
 *
 *  Process all pending SDL events, updating all global variables
 *  and handling debug key presses and cannot be changed
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

void process_key_events() {
    SDL_Event e, keydown_e;
    SDL_Keymod m;
    int k, ascii;
    int pressed;
    key_equiv * curr;
    int keypress;
    int ignore_key, n;

    /* Update events */

    // Avoid false events state results
    key_evt[ key_evt_clean_idx++ ] = 0;
    if ( key_evt_clean_idx == SDL_NUM_SCANCODES ) key_evt_clean_idx = 0;

    // key serial = 1 to 0x3fff
    key_serial = ( ( key_serial + 1 ) | 1 ) & 0x3fff;

    keypress = 0;
    m = SDL_GetModState();

    /* Process all pending events */
    /* Reset ascii and scancode if last key was released... */
    /* must check all the linked equivs */

    pressed = 0;
    if ( scancode ) {
        curr = &key_table[scancode];
        while ( curr != NULL && pressed == 0 ) {
            if ( keystate[curr->sdlk_equiv] ) pressed = 1;
            curr = curr->next;
        }
    }

    if ( !pressed ) {
        GLOQWORD( libbginput, ASCII ) = 0;
        GLOQWORD( libbginput, SCANCODE ) = scancode = 0;
    }

    int ready = 0;

    /* Process most event types */
    while ( SDL_PeepEvents( &e, 1, SDL_GETEVENT, SDL_KEYDOWN, SDL_TEXTINPUT ) > 0 ) {
        switch ( e.type ) {
            case SDL_KEYDOWN:
                // keydown
                if ( !e.key.repeat ) key_evt[ e.key.keysym.scancode ] = key_serial | IKEY_MASK_KEY_DOWN;

                if ( ready ) process_key( &keydown_e, ascii, &keypress );

                keydown_e = e;
                ascii = 0;
                ready = 1;

                // process, non ascii
                if ( e.key.keysym.sym < ' ' || e.key.keysym.sym > '~' ) ascii = e.key.keysym.sym;
                break;

            case SDL_KEYUP:
                // keyup
                key_evt[ e.key.keysym.scancode ] = key_serial | IKEY_MASK_KEY_UP;
                break;

            case SDL_TEXTINPUT: // this handles text input events
                for ( int i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; i++ ) {
                    char c = e.text.text[i];
                    // cancel if a non-ascii char is encountered
                    if ( c < ' ' || c > '~' ) break;
                    if ( ready ) process_key( &keydown_e, win_to_dos[c], &keypress );
                    ready = 0;
                    ascii = 0;
                }
                break;
        }
    }

    if ( ready ) process_key( &keydown_e, ascii, &keypress );

    if ( !keypress && keyring_start != keyring_tail ) {
        GLOQWORD( libbginput, ASCII ) = keyring[keyring_start].ascii;
        GLOQWORD( libbginput, SCANCODE ) = scancode = keyring[keyring_start].scancode;
        if ( ++keyring_start == 64 ) keyring_start = 0;
    }

    /* Now actualized every frame... */
    GLOQWORD( libbginput,  SHIFTSTATUS ) =
        ( ( m & KMOD_RSHIFT ) ? STAT_RSHIFT : 0 ) |
        ( ( m & KMOD_LSHIFT ) ? STAT_LSHIFT : 0 ) |

        ( ( m & KMOD_CTRL   ) ? STAT_CTRL   : 0 ) |
        ( ( m & KMOD_ALT    ) ? STAT_ALT    : 0 ) |

        ( ( m & KMOD_RCTRL  ) ? STAT_RCTRL  : 0 ) |
        ( ( m & KMOD_LCTRL  ) ? STAT_LCTRL  : 0 ) |

        ( ( m & KMOD_RALT   ) ? STAT_RALT   : 0 ) |
        ( ( m & KMOD_LALT   ) ? STAT_LALT   : 0 ) |

        ( ( m & KMOD_NUM    ) ? STAT_NUM    : 0 ) |
        ( ( m & KMOD_CAPS   ) ? STAT_CAPS   : 0 ) |
        ( ( m & KMOD_SHIFT  ) ? STAT_SHIFT  : 0 );
}

/* ---------------------------------------------------------------------- */
/* Funciones de inicializacion del modulo/plugin                     */

void key_init() {
    int64_t * ptr = equivs;

    if ( !SDL_WasInit( SDL_INIT_VIDEO ) ) SDL_InitSubSystem( SDL_INIT_VIDEO );

    SDL_EventState(SDL_TEXTEDITING, SDL_DISABLE);

    memset( key_table, 0, sizeof( key_table ) );

    while ( *ptr != -1 ) {
        add_key_equiv( ptr[0], ptr[1] );
        ptr += 2;
    }

    if ( !keystate ) keystate = SDL_GetKeyboardState(NULL);

    memset( key_evt, 0, sizeof( key_evt ) );

}

/* ---------------------------------------------------------------------- */

void key_exit() {
    /* FREE used key_equivs... note base MAX_KEY_CODES equivs are static not allocated... */
    int i;
    key_equiv * aux;
    key_equiv * curr;

    for ( i = 0; i < MAX_KEY_CODES; i++ ) {
        curr = &key_table[i];
        if ( curr->next ) {
            curr = curr->next;
            while ( curr ) {
                aux = curr;
                curr = curr->next;
                free( aux );
            }
        }
    }

    if ( SDL_WasInit( SDL_INIT_VIDEO ) ) SDL_QuitSubSystem( SDL_INIT_VIDEO );
}

/* ---------------------------------------------------------------------- */
