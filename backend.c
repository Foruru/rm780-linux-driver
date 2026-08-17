#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <libusb-1.0/libusb.h>

#include "blob.h"

/*==========================================================
 * Configuration
 *==========================================================*/

#define LIBUSB_TIMEOUT 5000
#define PROFILE 1               /* Currently only one profile is supported. */

/*==========================================================
 * Constants
 *==========================================================*/

#define VID 0x258A
#define PID 0x1007

#define PACKET_A_SIZE 59
#define PACKET_B_SIZE 9
#define PACKET_C_SIZE 1145

#define DPI_COUNT    6
#define MOUSE_COLORS 7

#define MACRO_SIZE  128
#define MAX_N_MACRO 8

#define MACRO_ACTION_SIZE     2
#define MACRO_EXT_ACTION_SIZE 4

/* (MACRO_SIZE - 2) / MACRO_EXT_ACTION_SIZE */
#define MACRO_EXT_ACTIONS_MAX 31

#define MOUSE_PROFILES 3

#define PHY_MOUSE_BUTTONS 7

static const uint16_t wIndex = 1;

/*==========================================================
 * Global Variables
 *==========================================================*/

typedef struct {
    uint8_t *ptr;
    size_t   sz;
    uint16_t wValue;
} UsbPacket;

static UsbPacket usb_packets[3];

static int replug_the_mouse = 0;

/*==========================================================
 * Macro
 *==========================================================*/

#define DO_BIND memcpy(ctx->pkt_c.bindings[PROFILE-1][button-1], &bind, 4*sizeof(uint8_t));

/*==========================================================
 * Public Types
 *==========================================================*/

typedef uint8_t MouseButton;
enum {
    MOUSE_BTN_LEFT      = 0x01,
    MOUSE_BTN_RIGHT     = 0x02,
    MOUSE_BTN_MIDDLE    = 0x03,
    MOUSE_BTN_BACK      = 0x04,
    MOUSE_BTN_FORWARD   = 0x05,
    MOUSE_BTN_DPI_PLUS  = 0x06,
    MOUSE_BTN_DPI_MINUS = 0x07
};

typedef uint8_t BindGroup;
enum {
    BIND_GROUP_REGULAR        = 1,
    BIND_GROUP_FIREKEY        = 2,
    BIND_GROUP_THREECLICK     = 3,
    BIND_GROUP_DPI            = 4,
    BIND_GROUP_DISABLE        = 5,
    BIND_GROUP_KEYCOMBINATION = 6,
    BIND_GROUP_MEDIA          = 7,
    BIND_GROUP_MACRO          = 9
};

/*----------------------------------------------------------
 * Regular Key Bind
 *----------------------------------------------------------*/

typedef uint8_t KeyCode;
enum {
    /* Letters */
    KEY_A = 0x04,
    KEY_B = 0x05,
    KEY_C = 0x06,
    KEY_D = 0x07,
    KEY_E = 0x08,
    KEY_F = 0x09,
    KEY_G = 0x0a,
    KEY_H = 0x0b,
    KEY_I = 0x0c,
    KEY_J = 0x0d,
    KEY_K = 0x0e,
    KEY_L = 0x0f,
    KEY_M = 0x10,
    KEY_N = 0x11,
    KEY_O = 0x12,
    KEY_P = 0x13,
    KEY_Q = 0x14,
    KEY_R = 0x15,
    KEY_S = 0x16,
    KEY_T = 0x17,
    KEY_U = 0x18,
    KEY_V = 0x19,
    KEY_W = 0x1a,
    KEY_X = 0x1b,
    KEY_Y = 0x1c,
    KEY_Z = 0x1d,

    /* Digits */
    KEY_1 = 0x1e,
    KEY_2 = 0x1f,
    KEY_3 = 0x20,
    KEY_4 = 0x21,
    KEY_5 = 0x22,
    KEY_6 = 0x23,
    KEY_7 = 0x24,
    KEY_8 = 0x25,
    KEY_9 = 0x26,
    KEY_0 = 0x27,

    /* Control keys */
    KEY_ENTER     = 0x28,
    KEY_ESC       = 0x29,
    KEY_BACKSPACE = 0x2a,
    KEY_TAB       = 0x2b,
    KEY_SPACE     = 0x2c,
    KEY_CAPS_LOCK = 0x39,

    /* Punctuation */
    KEY_MINUS     = 0x2d,
    KEY_EQUALS    = 0x2e,
    KEY_LBRACKET  = 0x2f,
    KEY_RBRACKET  = 0x30,
    KEY_BACKSLASH = 0x31,
    KEY_SEMICOLON = 0x33,
    KEY_APOSTROPHE= 0x34,
    KEY_GRAVE     = 0x35,
    KEY_COMMA     = 0x36,
    KEY_DOT       = 0x37,
    KEY_SLASH     = 0x38,

    /* Function keys */
    KEY_F1  = 0x3a,
    KEY_F2  = 0x3b,
    KEY_F3  = 0x3c,
    KEY_F4  = 0x3d,
    KEY_F5  = 0x3e,
    KEY_F6  = 0x3f,
    KEY_F7  = 0x40,
    KEY_F8  = 0x41,
    KEY_F9  = 0x42,
    KEY_F10 = 0x43,
    KEY_F11 = 0x44,
    KEY_F12 = 0x45,

    /* Numpad */
    KEY_NUM_LOCK = 0x53,
    KEY_NUM_DIV  = 0x54,
    KEY_NUM_MUL  = 0x55,
    KEY_NUM_SUB  = 0x56,
    KEY_NUM_ADD  = 0x57,
    KEY_NUM_1    = 0x59,
    KEY_NUM_2    = 0x5a,
    KEY_NUM_3    = 0x5b,
    KEY_NUM_4    = 0x5c,
    KEY_NUM_5    = 0x5d,
    KEY_NUM_6    = 0x5e,
    KEY_NUM_7    = 0x5f,
    KEY_NUM_8    = 0x60,
    KEY_NUM_9    = 0x61,
    KEY_NUM_0    = 0x62,
    KEY_NUM_DOT  = 0x63,

    /* Modifiers */
    KEY_L_CTRL  = 0xe0,
    KEY_L_SHIFT = 0xe1,
    KEY_L_ALT   = 0xe2,
    KEY_L_WIN   = 0xe3,
    KEY_R_CTRL  = 0xe4,
    KEY_R_SHIFT = 0xe5,
    KEY_R_ALT   = 0xe6,
    KEY_R_WIN   = 0xe7,

    /* Mouse buttons */
    KEY_MOUSE_LEFT   = 0xf0,
    KEY_MOUSE_RIGHT  = 0xf1,
    KEY_MOUSE_MIDDLE = 0xf2,
    KEY_MOUSE_BACK   = 0xf3,
    KEY_MOUSE_FWD    = 0xf4,
};

typedef uint8_t RegularKeyAction;
enum {
    MOUSE_ACTION_LEFT   = 0xf0,
    MOUSE_ACTION_RIGHT  = 0xf1,
    MOUSE_ACTION_MIDDLE = 0xf2,
    MOUSE_ACTION_BACK   = 0xf3,
    MOUSE_ACTION_FWD    = 0xf4,
};

typedef struct {
    MouseButton mouse_btn : 4;  /* 2 */
    BindGroup group : 4;        /* 1 */
    KeyCode key;
    uint8_t reserved[2];
} RegularBind;

/*----------------------------------------------------------
 * Firekey Key Bind
 *----------------------------------------------------------*/

typedef struct {
    MouseButton mouse_btn : 4;  /* 2 */
    BindGroup group : 4;        /* 1 */
    KeyCode key;
    uint8_t interval;
    uint8_t loop;
} FirekeyBind;

/*----------------------------------------------------------
 * Threeclick Key Bind
 *----------------------------------------------------------*/

typedef struct {
    MouseButton mouse_btn : 4;
    BindGroup group : 4;
    uint8_t reserved[3];
} ThreeClickBind;

/*----------------------------------------------------------
 * DPI Actions Key Bind
 *----------------------------------------------------------*/

typedef uint8_t DPIAction;
enum {
    DPI_ACTION_LOOP  = 0x00,
    DPI_ACTION_PLUS  = 0x20,
    DPI_ACTION_MINUS = 0x40,
    DPI_ACTION_500   = 0x81,
    DPI_ACTION_750   = 0x82,
    DPI_ACTION_1000  = 0x83,
    DPI_ACTION_1250  = 0x84,
    DPI_ACTION_1500  = 0x85,
    DPI_ACTION_1750  = 0x86,
    DPI_ACTION_2000  = 0x87,
    DPI_ACTION_2500  = 0x88,
    DPI_ACTION_3000  = 0x89,
    DPI_ACTION_3500  = 0x8a,
    DPI_ACTION_4000  = 0x8b
};

typedef struct {
    MouseButton mouse_btn : 4;  /* 2 */
    BindGroup group : 4;        /* 1 */
    DPIAction action;
    uint8_t reserved[2];
} DPIActionBind;

/*----------------------------------------------------------
 * Disable Key Bind
 *----------------------------------------------------------*/

typedef struct {
    MouseButton mouse_btn : 4;  /* 2 */
    BindGroup group : 4;        /* 1 */
    uint8_t reserved[3];
} DisableBind;

/*----------------------------------------------------------
 * Keycombination Key Bind
 *----------------------------------------------------------*/

typedef uint8_t ModKey;
enum {
    MOD_CTRL  = 0x01,
    MOD_SHIFT = 0x02,
    MOD_ALT   = 0x04,
    MOD_WIN   = 0x08
};

typedef struct {
    MouseButton mouse_btn : 4;  /* 2 */
    BindGroup group : 4;        /* 1 */
    uint8_t modkey;             /* ModKey bitmask */
    KeyCode key_a;
    KeyCode key_b;
} KeyCombinationBind;

/*----------------------------------------------------------
 * Media Key Bind
 *----------------------------------------------------------*/

typedef enum {
    MEDIA_ACTION_PLAYER,
    MEDIA_ACTION_PLAY,
    MEDIA_ACTION_NEXT,
    MEDIA_ACTION_PREV,
    MEDIA_ACTION_STOP,
    MEDIA_ACTION_MUTE,
    MEDIA_ACTION_VOL_UP,
    MEDIA_ACTION_VOL_DOWN,
    MEDIA_ACTION_EMAIL,
    MEDIA_ACTION_CALC,
    MEDIA_ACTION_EXPLR,
    MEDIA_ACTION_HOME
} MediaAction;


typedef struct {
    uint8_t data[3];
} MediaActionCode;

static const MediaActionCode media_actions[] = {
    [MEDIA_ACTION_PLAYER]   = {{0x00, 0x00, 0x00}},
    [MEDIA_ACTION_PLAY]     = {{0x08, 0x00, 0x00}},
    [MEDIA_ACTION_NEXT]     = {{0x01, 0x00, 0x00}},
    [MEDIA_ACTION_PREV]     = {{0x02, 0x00, 0x00}},
    [MEDIA_ACTION_STOP]     = {{0x04, 0x00, 0x00}},
    [MEDIA_ACTION_MUTE]     = {{0x10, 0x00, 0x00}},
    [MEDIA_ACTION_VOL_UP]   = {{0x40, 0x00, 0x00}},
    [MEDIA_ACTION_VOL_DOWN] = {{0x80, 0x00, 0x00}},
    [MEDIA_ACTION_EMAIL]    = {{0x00, 0x10, 0x00}},
    [MEDIA_ACTION_CALC]     = {{0x00, 0x20, 0x20}},
    [MEDIA_ACTION_EXPLR]    = {{0x00, 0x02, 0x00}},
    [MEDIA_ACTION_HOME]     = {{0x00, 0x00, 0x02}},
};

typedef struct {
    MouseButton mouse_btn : 4;  /* 2 */
    BindGroup group : 4;        /* 1 */
    uint8_t action[3];
} MediaBind;

/*----------------------------------------------------------
 * Macro Key Bind
 *----------------------------------------------------------*/

typedef uint8_t MacroCycle;
enum {
    MACRO_CYCLE_UNTIL_KEY_RELEASED    = 4,
    MACRO_CYCLE_UNTIL_KEY_PRESSED     = 2,
    MACRO_CYCLE_SPECIFIED_CYCLE_TIMES = 1
};

typedef struct {
    MouseButton mouse_btn : 4;  /* 2 */
    BindGroup group : 4;        /* 1 */
    uint8_t cycle : 4;          /* 4 */
    uint8_t macro : 4;          /* 3 */
    uint8_t interval;
    uint8_t loop;
} MacroBind;

/*----------------------------------------------------------
 * Light
 *----------------------------------------------------------*/

typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} MouseColor;

typedef uint8_t Effect;
enum {
    EFFECT_NONE              = 0x00,
    EFFECT_COLORFULSTREAMING = 0x02,
    EFFECT_STEADY            = 0x03,
    EFFECT_BREATHING         = 0x04,
    EFFECT_TAIL              = 0x05,
    EFFECT_NEON              = 0x06,
    EFFECT_COLORFULSTEADY    = 0x07,
    EFFECT_FLICKER           = 0x08,
    EFFECT_RESPONSE          = 0x09,
    EFFECT_STREAMING         = 0x0a,
    EFFECT_WAVE              = 0x0b,
    EFFECT_TRAILING          = 0x0c
};

typedef uint8_t EffectDirection;
enum {
    EFFECT_DIRECTION_FORWARD  = 8,
    EFFECT_DIRECTION_BACKWARD = 0
};

typedef uint8_t DPIColor;
enum {
    DPI_COLOR_RED    = 0x01,
    DPI_COLOR_GREEN  = 0x02,
    DPI_COLOR_BLUE   = 0x03,
    DPI_COLOR_CYAN   = 0x04,
    DPI_COLOR_YELLOW = 0x05,
    DPI_COLOR_PIN    = 0x06
};

/*----------------------------------------------------------
 * Others
 *----------------------------------------------------------*/

typedef uint8_t DPIValue;
enum {
    DPI_500  = 0x01,
    DPI_750  = 0x02,
    DPI_1000 = 0x03,
    DPI_1250 = 0x04,
    DPI_1500 = 0x05,
    DPI_1750 = 0x06,
    DPI_2000 = 0x07,
    DPI_2500 = 0x08,
    DPI_3000 = 0x09,
    DPI_3500 = 0x0a,
    DPI_4000 = 0x0b
};

typedef uint8_t UsbPollingRate;
enum {
    USB_POLL_125HZ  = 0x01,
    USB_POLL_250HZ  = 0x02,
    USB_POLL_500HZ  = 0x03,
    USB_POLL_1000HZ = 0x04
};

typedef struct {
    DPIValue value;
    DPIColor color;
    int state;
} DPI;

/*==========================================================
 * Packet Structures
 *==========================================================*/

/*
 * As `x` marked unknown fields.
 */

struct EffectConfig {
    uint8_t speed_or_brightness : 4;        /* 2 */
    uint8_t effect : 4;                     /* 1 */
    uint8_t colors_count : 4;               /* 4 */
    uint8_t direction_or_random_colors : 4; /* 3 */
};

typedef struct {
    uint8_t report_id;
    uint8_t x1;
    uint8_t dpi_count;
    uint8_t magic_0x20;      /* If the value is not 0x20, the mouse stops working. */
    uint8_t x2;
    uint8_t dpi_values[DPI_COUNT];
    uint8_t x3[10];
    struct EffectConfig effect_config;
    MouseColor colors[MOUSE_COLORS];
    uint8_t dpi_colors[DPI_COUNT];
    uint8_t x4[2];
    uint8_t magic_0x20_0x22; /* 0x22 when current effect is None. */
    uint8_t x5[6];
} PacketA;

typedef struct {
    uint8_t report_id;
    uint8_t dpi_switch_enabled;
    UsbPollingRate usb_polling_rate;
    uint8_t x1[2];
    uint8_t initial_dpi;
    uint8_t x2[3];
} PacketB;

struct PktMacro {
    uint8_t cycle[2];
    uint8_t sequence[126];
};

typedef struct {
    uint8_t report_id;
    struct PktMacro macros[MAX_N_MACRO];
    uint8_t bindings[MOUSE_PROFILES][10][4]; /* 10 is a number of mouse button but mouse has only 7. */
} PacketC;

/*==========================================================
 * Driver Context
 *==========================================================*/

typedef struct {
    libusb_context *ctx;
    libusb_device_handle *dev_handle;
    PacketA pkt_a;
    PacketB pkt_b;
    PacketC pkt_c;
} DriverContext;

/*==========================================================
 * Public API
 *==========================================================*/

/*----------------------------------------------------------
 * General
 *----------------------------------------------------------*/

static void init_driver(DriverContext *ctx);
static void close_driver(DriverContext *ctx);

static void sync(DriverContext *ctx);

/*----------------------------------------------------------
 * Mouse configuration
 *----------------------------------------------------------*/

/* dpi */
static void set_dpi_settings(DriverContext *ctx, DPI dpi_list[6], size_t dpi_list_sz, uint8_t initial_dpi, uint8_t dpi_switch_enabled);

/* effects */
static void set_effect_none(DriverContext *ctx);
static void set_effect_colorful_streaming(DriverContext *ctx, uint8_t speed, EffectDirection direction);
static void set_effect_steady(DriverContext *ctx, MouseColor color, uint8_t brightness);
static void set_effect_breathing(DriverContext *ctx, MouseColor colors[7], uint8_t speed);
static void set_effect_tail(DriverContext *ctx, uint8_t speed);
static void set_effect_neon(DriverContext *ctx, uint8_t speed);
static void set_effect_colorful_steady(DriverContext *ctx, MouseColor colors[7]);
static void set_effect_flicker(DriverContext *ctx, MouseColor colors[2]);
static void set_effect_response(DriverContext *ctx, MouseColor colors[7], int random_colors, uint8_t speed);
static void set_effect_streaming(DriverContext *ctx, uint8_t speed, EffectDirection direction);
static void set_effect_wave(DriverContext *ctx, uint8_t speed);
static void set_effect_trailing(DriverContext *ctx, uint8_t speed);

/* usb polling rate */
static void set_usb_polling_rate(DriverContext *ctx, UsbPollingRate usb_polling_rate);

/* bindings */
static void set_regular_bind(DriverContext *ctx, MouseButton button, RegularKeyAction action);
static void set_fire_bind(DriverContext *ctx, MouseButton button, KeyCode key, uint8_t interval, uint8_t loop);
static void set_threeclick_bind(DriverContext *ctx, MouseButton button);
static void set_dpi_bind(DriverContext *ctx, MouseButton button, DPIAction dpi_key);
static void set_disable_bind(DriverContext *ctx, MouseButton button);
static void set_keycombination_bind(DriverContext *ctx, MouseButton button, uint8_t modkey, KeyCode key_a, KeyCode key_b);
static void set_media_bind(DriverContext *ctx, MouseButton button, MediaAction media_key);
static void set_macro_bind(DriverContext *ctx, MouseButton button, uint8_t macro, MacroCycle cycle, uint8_t interval, uint8_t loop);

/*==========================================================
 * Internal Helpers
 *==========================================================*/

static struct EffectConfig* init_effect_cfg(DriverContext *ctx, Effect effect);
static void set_effect_colors(DriverContext *ctx, struct EffectConfig *cfg, MouseColor *colors, uint8_t amount);

/*==========================================================
 * Implementations
 *==========================================================*/

void
init_driver(DriverContext *ctx)
{
    int err;

    err = libusb_init(&ctx->ctx);
    if (err < 0) {
        perror("libusb_init_ctx");
        exit(-1);
    }
    libusb_set_option(ctx->ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO);

    ctx->dev_handle = libusb_open_device_with_vid_pid(ctx->ctx, VID, PID);
    if (!ctx->dev_handle) {
        printf("Device not found or permission denied, potential fixes:\n"
               "1) Run program as superuser\n"
               "2) Put text below into the file /etc/udev/rules.d/RM-780_Gaming_RGB.rules\n"
               "   `SUBSYSTEMS==\"usb\", ATTRS{VID}==\"258a\", ATTRS{PID}==\"1007\", MODE=\"0666\"`\n"
               "   then execute those commands:\n"
               "   $ udevadm control --reload-rules\n"
               "   $ udevadm trigger\n");
        libusb_close(ctx->dev_handle);
        libusb_exit(ctx->ctx);
        exit(-1);
    }

    if (libusb_kernel_driver_active(ctx->dev_handle, wIndex))
        if (libusb_detach_kernel_driver(ctx->dev_handle, wIndex) < 0) {
            perror("libusb_detach_kernel_driver");
            libusb_exit(ctx->ctx);
            exit(-1);
        }

    err = libusb_claim_interface(ctx->dev_handle, wIndex);
    if (err < 0) {
        fprintf(stderr, "Cannot Claim Interface. Error: %s\n", libusb_error_name(err));
        libusb_close(ctx->dev_handle);
        libusb_exit(ctx->ctx);
        exit(-1);
    }

    memcpy(&ctx->pkt_a, blob_a, sizeof(blob_a));
    memcpy(&ctx->pkt_b, blob_b, sizeof(blob_b));
    memcpy(&ctx->pkt_c, blob_c, sizeof(blob_c));

    usb_packets[0] = (UsbPacket){ (uint8_t*)&ctx->pkt_a, PACKET_A_SIZE, 0x0304 };
    usb_packets[1] = (UsbPacket){ (uint8_t*)&ctx->pkt_b, PACKET_B_SIZE, 0x0308 };
    usb_packets[2] = (UsbPacket){ (uint8_t*)&ctx->pkt_c, PACKET_C_SIZE, 0x0306 };
}

void
close_driver(DriverContext *ctx)
{
    libusb_release_interface(ctx->dev_handle, wIndex);
    libusb_close(ctx->dev_handle);
    libusb_exit(ctx->ctx);
}

void
sync(DriverContext *ctx)
{
    int err;

    for (int i = 0; i < 3; i++) {
        err = libusb_control_transfer(
            ctx->dev_handle,
            0x21,                  /* bmRequestType */
            0x09,                  /* bRequest */
            usb_packets[i].wValue,
            wIndex,
            usb_packets[i].ptr,
            usb_packets[i].sz,
            LIBUSB_TIMEOUT
        );
        if (err < 0) perror("libusb_control_transfer");
    }

    /* Problem: After sending bindings to the mouse, the bound mouse button(s)
     *          or function(s) stop working until the mouse is unplugged and
     *          plugged in again.
     *
     * Solution: Force a USB device unbind/rebind from userspace.
     * 1) echo "3-1" | sudo tee /sys/bus/usb/drivers/usb/unbind
     * 2) echo "3-1" | sudo tee /sys/bus/usb/drivers/usb/bind
     */
    if (replug_the_mouse) {
        if (getuid())
            printf("Unable to rebind the mouse automatically.\n"
                   "Please unplug and reconnect the mouse, or run:\n"
                   "1) echo \"3-1\" | sudo tee /sys/bus/usb/drivers/usb/unbind\n"
                   "2) echo \"3-1\" | sudo tee /sys/bus/usb/drivers/usb/bind\n");
        else
            system("echo \"3-1\" >> /sys/bus/usb/drivers/usb/unbind"
                   " && "
                   "echo \"3-1\" >> /sys/bus/usb/drivers/usb/bind");
    }
}

/*
 * @param dpi_list The array must contain exactly 6 elements.
 */
void
set_dpi_settings(DriverContext *ctx, DPI dpi_list[6], size_t dpi_list_sz, uint8_t initial_dpi, uint8_t dpi_switch_enabled)
{
    assert(dpi_list_sz == 6);

    ctx->pkt_b.initial_dpi = initial_dpi;
    ctx->pkt_b.dpi_switch_enabled = dpi_switch_enabled;

    ctx->pkt_a.dpi_count = 0;
    for (int i = 0; i < DPI_COUNT; i++) {
        if (!dpi_list[i].value) {
            printf("Invalid DPI value at dpi[%d]", i);
            exit(1);
        }

        ctx->pkt_a.dpi_values[i] = dpi_list[i].value + dpi_list[i].state ? 0x00 : 0x80;
        ctx->pkt_a.dpi_colors[i] = dpi_list[i].color;
        ctx->pkt_a.dpi_count += dpi_list[i].state;
    }
}

struct EffectConfig*
init_effect_cfg(DriverContext *ctx, Effect effect)
{
    struct EffectConfig *cfg = &ctx->pkt_a.effect_config;
    memset(cfg, 0, sizeof(struct EffectConfig));
    cfg->effect = effect;

    return cfg;
}

void
set_effect_colors(DriverContext *ctx, struct EffectConfig *cfg, MouseColor *colors, uint8_t amount)
{
    cfg->colors_count = 0;
    for (int i = 0; i < amount; i++) {
        if (colors[i].R == 0 && colors[i].G == 0 && colors[i].B == 0)
            continue;

        memcpy(&ctx->pkt_a.colors[cfg->colors_count],
               (uint8_t*)&colors[i], sizeof(MouseColor));
        cfg->colors_count++;
    }
}

void
set_effect_none(DriverContext *ctx)
{
    init_effect_cfg(ctx, EFFECT_NONE);
    ctx->pkt_a.magic_0x20_0x22 = 0x22;
}

/*
 * @param speed Speed level in the range [1, 3].
 */
void
set_effect_colorful_streaming(DriverContext *ctx, uint8_t speed, EffectDirection direction)
{
    assert(speed >= 1 && speed <= 3);

    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_COLORFULSTREAMING);
    cfg->speed_or_brightness = speed;
    cfg->direction_or_random_colors = direction;
}

void
set_effect_steady(DriverContext *ctx, MouseColor color, uint8_t brightness)
{
    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_STEADY);
    cfg->speed_or_brightness = brightness;
    set_effect_colors(ctx, cfg, &color, 1);
}

/*
 * @param speed Speed level in the range [1, 3].
 */
void
set_effect_breathing(DriverContext *ctx, MouseColor colors[7], uint8_t speed)
{
    assert(speed >= 1 && speed <= 3);

    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_BREATHING);
    cfg->speed_or_brightness = speed;
    set_effect_colors(ctx, cfg, colors, 7);
}

/*
 * @param speed Speed level in the range [1, 3].
 */
void
set_effect_tail(DriverContext *ctx, uint8_t speed)
{
    assert(speed >= 1 && speed <= 3);

    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_TAIL);
    cfg->speed_or_brightness = speed;
}

/*
 * @param speed Speed level in the range [1, 3].
 */
void
set_effect_neon(DriverContext *ctx, uint8_t speed)
{
    assert(speed >= 1 && speed <= 3);

    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_NEON);
    cfg->speed_or_brightness = speed;
}

void
set_effect_colorful_steady(DriverContext *ctx, MouseColor colors[7])
{
    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_COLORFULSTEADY);
    set_effect_colors(ctx, cfg, colors, 7);
}

void
set_effect_flicker(DriverContext *ctx, MouseColor colors[2])
{
    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_FLICKER);
    set_effect_colors(ctx, cfg, colors, 2);
}

/*
 * @param speed Speed level in the range [1, 3].
 */
void
set_effect_response(DriverContext *ctx, MouseColor colors[7], int random_colors, uint8_t speed)
{
    assert(speed >= 1 && speed <= 3);

    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_RESPONSE);
    cfg->speed_or_brightness = speed;
    cfg->direction_or_random_colors = random_colors;
    set_effect_colors(ctx, cfg, colors, 7);
}

/*
 * @param speed Speed level in the range [1, 3].
 */
void
set_effect_streaming(DriverContext *ctx, uint8_t speed, EffectDirection direction)
{
    assert(speed >= 1 && speed <= 3);

    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_STREAMING);
    cfg->speed_or_brightness = speed;
    cfg->direction_or_random_colors = direction;
}

/*
 * @param speed Speed level in the range [1, 3].
 */
void
set_effect_wave(DriverContext *ctx, uint8_t speed)
{
    assert(speed >= 1 && speed <= 3);

    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_WAVE);
    cfg->speed_or_brightness = speed;
}

/*
 * @param speed Speed level in the range [1, 3].
 */
void
set_effect_trailing(DriverContext *ctx, uint8_t speed)
{
    assert(speed >= 1 && speed <= 3);

    struct EffectConfig *cfg = init_effect_cfg(ctx, EFFECT_TRAILING);
    cfg->speed_or_brightness = speed;
}

void
set_usb_polling_rate(DriverContext *ctx, UsbPollingRate usb_polling_rate)
{
    ctx->pkt_b.usb_polling_rate = usb_polling_rate;
}

void
set_regular_bind(DriverContext *ctx, MouseButton button, RegularKeyAction action)
{
    RegularBind bind = {button, BIND_GROUP_REGULAR, action, {0}};
    DO_BIND
}

/*
 * @param interval Interval in the range [5, 255].
 * @param loop Loop in the range [1, 255].
 */
void
set_fire_bind(DriverContext *ctx, MouseButton button, KeyCode key, uint8_t interval, uint8_t loop)
{
    assert(interval >= 5);
    assert(loop >= 1);

    replug_the_mouse = 1;

    FirekeyBind bind = {button, BIND_GROUP_FIREKEY, key, interval, loop};
    DO_BIND
}

void
set_threeclick_bind(DriverContext *ctx, MouseButton button)
{
    ThreeClickBind bind = {button, BIND_GROUP_THREECLICK, {0}};
    DO_BIND
}

void
set_dpi_bind(DriverContext *ctx, MouseButton button, DPIAction action)
{
    replug_the_mouse = 1;

    DPIActionBind bind = {button, BIND_GROUP_DPI, action, {0}};
    DO_BIND
}

void
set_disable_bind(DriverContext *ctx, MouseButton button)
{
    /* DisableBind bind = {button, BIND_GROUP_DISABLE, {0x01, 0, 0}}; */
    /* DO_BIND */

    memset(ctx->pkt_c.bindings[PROFILE-1][button-1], 0, 4);
}

void
set_keycombination_bind(DriverContext *ctx, MouseButton button, uint8_t modkey, KeyCode key_a, KeyCode key_b)
{
    replug_the_mouse = 1;

    KeyCombinationBind bind = {button, BIND_GROUP_KEYCOMBINATION, modkey, key_a, key_b};
    DO_BIND
}

void
set_media_bind(DriverContext *ctx, MouseButton button, MediaAction action)
{
    /* FIXME: Media keys doesn't work on Linux. */
    replug_the_mouse = 1;

    MediaBind bind = {button, BIND_GROUP_MEDIA, {0}};
    memcpy(bind.action, &media_actions[action], 3);

    DO_BIND
}

/*
 * @param macro Macro in the range [1, 8].
 * @param interval Interval in the range [5, 255].
 * @param loop Loop in the range [1, 255].
 */
void
set_macro_bind(DriverContext *ctx, MouseButton button, uint8_t macro, MacroCycle cycle, uint8_t interval, uint8_t loop)
{
    assert(macro >= 1 && macro <= 8);
    assert(interval >= 5);
    assert(loop >= 1);
        
    replug_the_mouse = 1;

    MacroBind bind = {button, BIND_GROUP_MACRO, macro, cycle, interval, loop};
    DO_BIND
}
