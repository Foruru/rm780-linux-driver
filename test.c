/* gcc -o driver test.c backend.c blob.h -lusb-1.0 -O0 -std=c99 -Wall -Wextra -g -Wno-unused-function */

#include "backend.c"

int main() {
    printf("\n========= PREINIT ========\n\n");


    printf("Struct sizes:\n");
    printf("Packet A: %ld, blob: %ld\n", sizeof(PacketA), sizeof(blob_a));
    printf("Packet B: %ld, blob: %ld\n", sizeof(PacketB), sizeof(blob_b));
    printf("Packet C: %ld, blob: %ld\n", sizeof(PacketC), sizeof(blob_c));


    printf("\n========== INIT ==========\n\n");


    printf("Initalizing dirver..");
    DriverContext ctx = {0};
    init_driver(&ctx);
    printf(" OK\n");


    printf("\n======= CONFIGURING ======\n\n");


    // printf("Configuring dpi settings..");
    // DPI       dpi_list[DPI_COUNT] = { {DPI_500,  DPI_RED,    1},
    //                                   {DPI_750,  DPI_GREEN,  1},
    //                                   {DPI_1000, DPI_BLUE,   1},
    //                                   {DPI_1250, DPI_PINK,   1},
    //                                   {DPI_1500, DPI_YELLOW, 1},
    //                                   {DPI_1750, DPI_CYAN,   1} };
    // int       initial_dpi         = 3;
    // int       dpi_switch_enabled  = 1;
    // set_dpi_settings(&ctx, dpi_list, initial_dpi, dpi_switch_enabled);
    // printf(" OK\n");

    printf("Configuring effect settings..");
    // MouseColor      colors[MOUSE_COLORS] = { {0xFF, 0x00, 0x00}, {0x00, 0xFF, 0x00} };
    /* MouseColor      colors[MOUSE_COLORS] = { {0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00}, {0xFF, 0xFF, 0xFF}, */
    /*                                          {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, */
    /*                                          {0xFF, 0xFF, 0xFF} }; */
    /* Effect          effect               = EFFECT_NONE; */
    /* EffectDirection direction            = 0; */
    /* uint8_t         speed                = 2; */
    /* uint8_t         brightness           = 6; */
    /* uint8_t         random_color         = 0; */
    // set_effect(&ctx, effect, direction, random_color, speed, brightness, colors);
    /* set_effect_breathing(&ctx, colors, speed); */
    /* set_effect_colorful_steady(&ctx, colors); */
    set_effect_none(&ctx);
    printf(" OK\n");

    printf("Configuring USB polling settings..");
    set_usb_polling_rate(&ctx, USB_POLL_500HZ);
    printf(" OK\n");

    /* printf("Set binds.."); */
    /* uint8_t[7][4] default_binds = { */
    /*     create_regular_bind(&ctx, MOUSE_BTN_RIGHT, KEY_A), */
    /* }; */
    /* uint8_t macro[6] = { 0x00, 0x01, 0x3f, 0x04, 0x81, 0x04 }; */
    /* memcpy(ctx.pkt_c.macros[0].cycle, &macro, 6*sizeof(uint8_t)); */

    /* MacroBind bind = create_macro_bind(MOUSE_BTN_MIDDLE, 0x11, 0, 0); */
    /* memcpy(ctx.pkt_c.bindings[0][2], &bind, 4*sizeof(uint8_t)); */

    /* FirekeyBind bind = create_fire_bind(MOUSE_BTN_MIDDLE, (KeyCode)0x07, 5, 10); */
    /* memcpy(ctx.pkt_c.bindings[0][2], &bind, 4*sizeof(uint8_t)); */

    /* uint8_t macro[] = {0x00, 0x01, 0x01, 0x04, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00}; */
    /* memcpy(ctx.pkt_c.macros[0].cycle, &macro, 10); */
    /* set_macro_bind(&ctx, MOUSE_BTN_MIDDLE, 1, MACRO_CYCLE_SPECIFIED_CYCLE_TIMES, 5, 1); */

    /* set_media_bind(&ctx, MOUSE_BTN_RIGHT, MEDIA_ACTION_VOL_DOWN); */
    /* uint8_t media[] = {0x00, 0x01, 0x01, 0x04, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00}; */
    /* memcpy(ctx.pkt_c.macros[0].cycle, media, 10); */
    /* printf(" OK\n"); */


    printf("\n========== DATA ==========\n\n");


    struct Data {
        const char *name;
        uint8_t *ptr;
        size_t sz;
    };

    struct Data data[3] = {
        { "Packet A", (unsigned char*)&ctx.pkt_a, sizeof ctx.pkt_a },
        { "Packet C", (unsigned char*)&ctx.pkt_b, sizeof ctx.pkt_b },
        { "Packet B", (unsigned char*)&ctx.pkt_c, sizeof ctx.pkt_c }
    };

    for (int i = 0; i < 3; i++) {
        printf("\n%s:\n", data[i].name);
        for (size_t j = 0; j < data[i].sz; j++)
            printf("%02x ", data[i].ptr[j]);
        printf("\n");
    }


    printf("\n========== SYNC ==========\n\n");


    printf("Sending packets..");
    sync(&ctx);
    printf(" OK\n");


    printf("\n======== FINALIZE ========\n\n");


    printf("Closing dirver..");
    close_driver(&ctx);
    printf(" OK\n");


    printf("\n");

    return 0;
}
