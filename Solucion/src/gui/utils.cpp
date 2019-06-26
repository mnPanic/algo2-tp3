#include "utils.h"

void update_game_status(ExtremeExorcism &g, SDL_Event &e) {
/*  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
    case SDLK_i:
      g.perform_action(0, STEP_UP);
      break;
    case SDLK_k:
      g.perform_action(0, STEP_DOWN);
      break;
    case SDLK_j:
      g.perform_action(0, STEP_LEFT);
      break;
    case SDLK_l:
      g.perform_action(0, STEP_RIGHT);
      break;
    }
  }
  */
}

void render_game_status(const ExtremeExorcism &g, const G_Map &gm, ThingsTiles &tt,
                        SDL_Environment &env) {
/*	gm.render(env.renderer);

	for (int player_n : g.players_alive()) {
      Pos p = gm.translate(g.player_pos(player_n));
      tt.render_char(p.first, p.second, g.player_dir(player_n));
	}

    SDL_RenderPresent(env.renderer);
    */
}
