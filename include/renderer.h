/* Author: Shing-Yan Loo (yan99033@gmail.com)
 *
 * Get the game objects and the players' scores and render them on
 * the screen
 * 
 * Credit: The code is based on Udacity Capstone snake game starter code
 * https://github.com/udacity/CppND-Capstone-Snake-Game
 */ 

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"

#include "paddle.h"
#include "ball.h"
#include "score.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height);
  ~Renderer();

  void Render(Paddle* left_paddle, Paddle* right_paddle,
              Ball* ball, const int* scores);
  void Render(const Winner& player);
  void UpdateWindowTitle(int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  void DrawPaddle(Paddle* paddle);

  // Draw circle
  // Algoritm Reference: 
  // https://github.com/libsdl-org/SDL/blob/c59d4dcd38c382a1e9b69b053756f1139a861574/test/testgesture.c
  void DrawCircle(Ball* ball);
  SDL_Point SetPix(int x, int y);

  // Documentation:
  // https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html
  // To draw the right player's score, set the second argument to true
  void DrawScore(const int left_score, bool is_right=false);
  void DrawWinner(const Winner& player);

  // Font
  void LoadFont(TTF_Font*& font, std::string ttf_fname, std::size_t font_size);
  const float sm_font_factor_{12.0f};
  const float lg_font_factor_{2.0f};
  const std::string sm_font_fname_{"./../FreeSans.ttf"};
  const std::string lg_font_fname_{"./../UbuntuMono-B.ttf"};
  TTF_Font* font_;
  TTF_Font* win_font_;

  const std::size_t screen_width;
  const std::size_t screen_height;

  const SDL_Colour bg_rgb_{0, 0, 0};
  const SDL_Colour paddle_rgb_{255, 204, 0};
  const SDL_Colour ball_rgb_{255, 0, 0};
  const SDL_Colour score_rgb_{255, 255, 255};
  const SDL_Colour winner_rgb_{0, 255, 0};
  const SDL_Colour win_rgb_{255, 155, 255};
};

#endif