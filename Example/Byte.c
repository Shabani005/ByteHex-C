#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <renderer.h>
#include <microui.h>

/* simple background colour */
static float bg[3] = { 20, 20, 25 };

/* text metrics so microui can measure text */
static int text_width(mu_Font font, const char* text, int len) {
    if (len == -1) { len = (int)strlen(text); }
    return r_get_text_width(text, len);
}

static int text_height(mu_Font font) {
    return r_get_text_height();
}

static void process_frame(mu_Context* ctx) {
    mu_begin(ctx);

    if (mu_begin_window(ctx, "Hex Editor", mu_rect(40, 40, 600, 500))) {

        /* This draws all the hex rows */
        // hex_draw(ctx);

        mu_end_window(ctx);
    }

    mu_end(ctx);
}

/* input mapping (kept from demo) */
static const char button_map[256] = {
    [SDL_BUTTON_LEFT & 0xff] = MU_MOUSE_LEFT,
    [SDL_BUTTON_RIGHT & 0xff] = MU_MOUSE_RIGHT,
    [SDL_BUTTON_MIDDLE & 0xff] = MU_MOUSE_MIDDLE,
};

static const char key_map[256] = {
    [SDLK_LSHIFT & 0xff] = MU_KEY_SHIFT,
    [SDLK_RSHIFT & 0xff] = MU_KEY_SHIFT,
    [SDLK_LCTRL & 0xff] = MU_KEY_CTRL,
    [SDLK_RCTRL & 0xff] = MU_KEY_CTRL,
    [SDLK_LALT & 0xff] = MU_KEY_ALT,
    [SDLK_RALT & 0xff] = MU_KEY_ALT,
    [SDLK_RETURN & 0xff] = MU_KEY_RETURN,
    [SDLK_BACKSPACE & 0xff] = MU_KEY_BACKSPACE,
};

int main(int argc, char** argv) {
    /* init SDL */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* init microui OpenGL backend */
    r_init();

    /* init microui context */
    mu_Context* ctx = malloc(sizeof * ctx);
    if (!ctx) {
        fprintf(stderr, "malloc failed\n");
        return EXIT_FAILURE;
    }

    mu_init(ctx);
    ctx->text_width = text_width;
    ctx->text_height = text_height;

    // hex_load_file("C:\\Program Files (x86)\\EA Games\\LOTR The Return of the King tm\\ROTK.exe");   /* or whatever file you want */


    /* main loop */
    for (;;) {
        /* SDL events → microui input */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                SDL_Quit();
                return EXIT_SUCCESS;

            case SDL_MOUSEMOTION:
                mu_input_mousemove(ctx, e.motion.x, e.motion.y);
                break;

            case SDL_MOUSEWHEEL:
                mu_input_scroll(ctx, 0, e.wheel.y * -30);
                break;

            case SDL_TEXTINPUT:
                mu_input_text(ctx, e.text.text);
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                int b = button_map[e.button.button & 0xff];
                if (b && e.type == SDL_MOUSEBUTTONDOWN)
                    mu_input_mousedown(ctx, e.button.x, e.button.y, b);
                if (b && e.type == SDL_MOUSEBUTTONUP)
                    mu_input_mouseup(ctx, e.button.x, e.button.y, b);
                break;
            }

            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                int c = key_map[e.key.keysym.sym & 0xff];
                if (c && e.type == SDL_KEYDOWN) mu_input_keydown(ctx, c);
                if (c && e.type == SDL_KEYUP)   mu_input_keyup(ctx, c);
                break;
            }
            }
        }

        /* build OUR UI for this frame */
        process_frame(ctx);

        /* clear using their backend */
        r_clear(mu_color(bg[0], bg[1], bg[2], 255));


        /* feed microui commands into their renderer backend */
        mu_Command* cmd = NULL;
        while (mu_next_command(ctx, &cmd)) {
            switch (cmd->type) {
            case MU_COMMAND_TEXT:
                r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color);
                break;
            case MU_COMMAND_RECT:
                r_draw_rect(cmd->rect.rect, cmd->rect.color);
                break;
            case MU_COMMAND_ICON:
                r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
                break;
            case MU_COMMAND_CLIP:
                r_set_clip_rect(cmd->clip.rect);
                break;
            }
        }

        r_present();  /* swap buffers */
    }

    return 0;
}
