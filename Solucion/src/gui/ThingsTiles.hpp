#ifndef __ThingsTiles_HPP__
#define __ThingsTiles_HPP__

#include "modelo/TiposJuego.h"
#include "LTexture.h"
#include <SDL2/SDL.h>

class ThingsTiles {

public:
  ThingsTiles(SDL_Renderer *renderer, int _scale)
      : tiles_texture(renderer), chars_texture(renderer), things_texture(renderer),
        scale(_scale) {
    tiles_texture.loadFromFile("resources/basictiles.png");
    chars_texture.loadFromFile("resources/characters.png");
    things_texture.loadFromFile("resources/things.png");
    this->renderer = renderer;
  }

  void render_ground(int x, int y) {
    tiles_texture.render(x, y, &ground_clip, scale);
  }

  void render_wall(int x, int y) {
    tiles_texture.render(x, y, &wall_clip, scale);
  }

  void render_char(int x, int y, Dir d) {
    chars_texture.render(x, y, &player_clips[d], scale);
  }

  void render_ghost(int x, int y, Dir d) {
    chars_texture.render(x, y, &ghost_clips[d], scale);
  }

  void render_fire(int x, int y) {
    things_texture.render(x, y, &fire_clip, scale);
  }
  
  void render_spikes(int x, int y) {
    things_texture.render(x, y, &spikes_clip, scale);
  }

  int tile_width() const { return base_tile_width * scale; }

  int tile_height() const { return base_tile_height * scale; }

  ~ThingsTiles() { tiles_texture.free(); }

private:
  const static int basic_tiles_rows = 8;
  const static int chars_tiles_rows = 12;
  const static int things_tiles_rows = 12;
  const static int base_tile_width = 16;  // px
  const static int base_tile_height = 16; // px

  LTexture tiles_texture;
  LTexture chars_texture;
  LTexture things_texture;
  SDL_Renderer *renderer;
  int scale;

  static SDL_Rect tile_rect_by_idx(int idx, int rows) {
    int row = idx / rows;
    int col = idx % rows;
    return SDL_Rect({idx * col * base_tile_width, idx * row * base_tile_height,
                     base_tile_width, base_tile_height});
  }

  static SDL_Rect tile_rect_by_pos(int row, int col, int rows) {
    return SDL_Rect({col * base_tile_width, row * base_tile_height,
                     base_tile_width, base_tile_height});
  }

  SDL_Rect ground_clip = tile_rect_by_idx(1, basic_tiles_rows);
  SDL_Rect wall_clip = tile_rect_by_idx(0, basic_tiles_rows);
  SDL_Rect player_clips[4] = {
      tile_rect_by_pos(3, 8, chars_tiles_rows), // UP
      tile_rect_by_pos(0, 8, chars_tiles_rows), // DOWN
      tile_rect_by_pos(1, 8, chars_tiles_rows), // LEFT
      tile_rect_by_pos(2, 8, chars_tiles_rows)  // RIGHT
  };
  SDL_Rect ghost_clips[4] = {
      tile_rect_by_pos(4 + 3, 8, chars_tiles_rows), // UP
      tile_rect_by_pos(4 + 0, 8, chars_tiles_rows), // DOWN
      tile_rect_by_pos(4 + 1, 8, chars_tiles_rows), // LEFT
      tile_rect_by_pos(4 + 2, 8, chars_tiles_rows)  // RIGHT
  };
  SDL_Rect fire_clip = tile_rect_by_pos(7, 8, things_tiles_rows);
  SDL_Rect spikes_clip = tile_rect_by_pos(6, 8, things_tiles_rows);
};

#endif
