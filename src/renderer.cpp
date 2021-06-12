#include "renderer.h"

#include <iostream>
#include <assert.h>
#include <string>
#include <math.h>

#define int(x) static_cast<int>(x)

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height)
    : screen_width(screen_width),
      screen_height(screen_height)
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Initialize the truetype font AP
  assert(TTF_Init() == 0);
  int sm_font_size = int(std::min(screen_width, screen_height) / sm_font_factor_);
  int lg_font_size = int(std::min(screen_width, screen_height) / lg_font_factor_);
  LoadFont(font_, sm_font_fname_, sm_font_size);
  LoadFont(win_font_, lg_font_fname_, lg_font_size);

  // Create Window
  sdl_window = SDL_CreateWindow("Pong Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() 
{
  SDL_DestroyWindow(sdl_window);

  // Free memory
  TTF_CloseFont(font_);
  TTF_CloseFont(win_font_);

  // cleanup the truetype font API
  TTF_Quit(); 

  SDL_Quit();
}

void Renderer::Render(Paddle* left_paddle, Paddle* right_paddle,
                      Ball* ball, const int* scores) 
{
  // Make sure the pointers are valid
  assert(left_paddle && right_paddle && ball);

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, bg_rgb_.r, bg_rgb_.g, bg_rgb_.b, 255);
  SDL_RenderClear(sdl_renderer);

  // Render paddles
  DrawPaddle(left_paddle);
  DrawPaddle(right_paddle);

  // Render ball
  DrawCircle(ball);

  // Render score
  DrawScore(scores[0]);
  DrawScore(scores[1], true);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::Render(const Winner& player)
{
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, bg_rgb_.r, bg_rgb_.g, bg_rgb_.b, 255);
  SDL_RenderClear(sdl_renderer);

  // Show who won
  DrawWinner(player);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int fps) 
{
  std::string title{"Pong game (FPS: " + std::to_string(fps) + ")"};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::DrawPaddle(Paddle* paddle)
{
  SDL_Rect block;
  int width, height;
  int x, y;

  // Get attributes
  paddle->GetPaddleSize(width, height);
  paddle->GetPosition(x, y);

  // Set attributes
  block.x = x;
  block.y = y;
  block.w = width;
  block.h = height;

  SDL_SetRenderDrawColor(sdl_renderer, paddle_rgb_.r, paddle_rgb_.g, paddle_rgb_.b, 255);
  SDL_RenderFillRect(sdl_renderer, &block);
}


// Algorithm reference: 
// https://github.com/libsdl-org/SDL/blob/c59d4dcd38c382a1e9b69b053756f1139a861574/test/testgesture.c#L105
void Renderer::DrawCircle(Ball* ball)
{
  std::vector<SDL_Point> points;
  // 
  int radius;
  int x, y;
  int xr;
  ball->GetBallRadius(radius);
  ball->GetPosition(x, y);

  SDL_SetRenderDrawColor(sdl_renderer, ball_rgb_.r, ball_rgb_.g, ball_rgb_.b, 255);
  for (int ty=-radius; ty<radius; ty++)
  {
    xr = static_cast<int>(sqrt(radius*radius - ty*ty));
    for (int tx=-xr; tx < xr; tx++)
      points.emplace_back(SetPix(x+tx, y+ty));
  }
  SDL_RenderDrawPoints(sdl_renderer, points.data(), points.size());

}

SDL_Point Renderer::SetPix(int x, int y)
{
  SDL_Point point;
  point.x = x;
  point.y = y;

  return point; // does not create a copy due to return value optimization (RVO)
}


// Adapted from:
// https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
void Renderer::DrawScore(const int score, bool is_right)
{
  SDL_Surface* surface_msg =
    TTF_RenderText_Solid(font_, std::to_string(score).c_str(), score_rgb_); 

  // now you can convert it into a texture
  SDL_Texture* tex_msg = SDL_CreateTextureFromSurface(sdl_renderer, surface_msg);

  SDL_Rect msg_rect;
  msg_rect.x = is_right ? int(screen_width) - surface_msg->w : 0;
  msg_rect.y = 0;
  msg_rect.w = surface_msg->w;
  msg_rect.h = surface_msg->h;

  SDL_RenderCopy(sdl_renderer, tex_msg, NULL, &msg_rect);

  // Don't forget to free your surface and texture
  SDL_FreeSurface(surface_msg);
  SDL_DestroyTexture(tex_msg);
}

void Renderer::DrawWinner(const Winner& player)
{
  // Generate player name
  std::string player_name;
  if (player == Winner::Player1)
    player_name = "Player 1";
  else
    player_name = "Player 2";

  // Create surfaces from text
  SDL_Surface* top_surface_msg =
    TTF_RenderText_Solid(font_, player_name.c_str(), winner_rgb_); 
  SDL_Surface* bottom_surface_msg =
    TTF_RenderText_Solid(font_, "Wins", win_rgb_); 

  // now you can convert them into a texture
  SDL_Texture* top_tex_msg = SDL_CreateTextureFromSurface(sdl_renderer, top_surface_msg);
  SDL_Texture* bottom_tex_msg = SDL_CreateTextureFromSurface(sdl_renderer, bottom_surface_msg);

  // Set text loctions
  int y_offset = int(screen_height) / sm_font_factor_;
  SDL_Rect top_msg_rect;
  top_msg_rect.x = int((int(screen_width) - top_surface_msg->w) / 2);
  top_msg_rect.y = int(int(screen_height) / 2) - top_surface_msg->h - y_offset;
  top_msg_rect.w = top_surface_msg->w;
  top_msg_rect.h = top_surface_msg->h;

  SDL_Rect bottom_msg_rect;
  bottom_msg_rect.x = int((int(screen_width) - bottom_surface_msg->w) / 2);
  bottom_msg_rect.y = int(int(screen_height) / 2) + y_offset;
  bottom_msg_rect.w = bottom_surface_msg->w;
  bottom_msg_rect.h = bottom_surface_msg->h;

  SDL_RenderCopy(sdl_renderer, top_tex_msg, NULL, &top_msg_rect);
  SDL_RenderCopy(sdl_renderer, bottom_tex_msg, NULL, &bottom_msg_rect);

  // free surface and texture
  SDL_FreeSurface(top_surface_msg);
  SDL_FreeSurface(bottom_surface_msg);
  SDL_DestroyTexture(top_tex_msg);
  SDL_DestroyTexture(bottom_tex_msg);
}

void Renderer::LoadFont(TTF_Font*& font, std::string ttf_fname, std::size_t font_size)
{
  font = TTF_OpenFont(ttf_fname.c_str(), font_size);
  if(!font)
    std::cerr << "TTF_OpenFont: " << TTF_GetError() << "\n"; 
  assert(font);
}