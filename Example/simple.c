#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define NB_IMPLEMENTATION
#include "../nb.h"
#include "microui.h"
#include "murl.h"

int main(int argc, char** argv) {

  if (argc < 2){
    fprintf(stderr, "Usage %s <file>\n", argv[0]);
    return 1;
  }

  if (!nb_does_file_exist(argv[1])){
    printf("Error: File '%s' does not exist\n", argv[1]);
    return 1;
  }

  // PROGRAM STATES
  bool showFiles = false;
  bool showHex = false;
  // --------------------------------------
  InitWindow(1280, 720, "ByteHex");
  SetTargetFPS(60);

  
  
  mu_Context *ctx = malloc(sizeof(mu_Context));
  mu_init(ctx);
  murl_setup_font(ctx);

  while (!WindowShouldClose()) {
  {
    murl_handle_input(ctx);

    mu_begin(ctx);

    if (mu_begin_window(ctx, "Hello", mu_rect(50, 50, 500, 400))) {
      mu_label(ctx, "Hello, raylib");

      if (mu_button(ctx, "The button")) {
        mu_open_popup(ctx, "popup");
      }

      if (IsFileDropped()){
        FilePathList filelist = LoadDroppedFiles();
        mu_label(ctx, filelist.paths[filelist.count]);
      }

      if (mu_button(ctx, "Load Hex")){
        showHex = !showHex;
      }
      
      if (mu_button(ctx, "Choose File")){
        showFiles = !showFiles;
      }

      if (showHex){
      }

      if (showFiles){  
        FilePathList flist = LoadDirectoryFiles(GetWorkingDirectory());
        char buf[1024];
        sprintf(buf, "%zu", flist.count);
        mu_label(ctx, buf);
        for (size_t i=0; i<flist.count; ++i){
          mu_label(ctx, flist.paths[i]);
        }
      }

      if (mu_begin_popup(ctx, "popup")) {
        mu_label(ctx, "This is a popup");
        mu_end_popup(ctx);
      }

      
    }
    mu_end_window(ctx);
  }

    mu_end(ctx);

    BeginDrawing();
    ClearBackground(BLACK);
    murl_render(ctx);
    EndDrawing();
  }

  free(ctx);
  CloseWindow();
  return 0;
}
