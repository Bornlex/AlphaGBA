#include "video/screen.h"
#include <stdio.h>

int main(void)
{
  // Write the tiles for our sprites into the fourth tile block in VRAM.
  // Four tiles for an 8x32 paddle sprite, and one tile for an 8x8 ball
  // sprite. Using 4bpp, 0x1111 is four pixels of colour index 1, and
  // 0x2222 is four pixels of colour index 2.
  
  volatile uint16 *paddle_tile_mem = (uint16 *) tile_mem[4][1];
  volatile uint16 *ball_tile_mem   = (uint16 *) tile_mem[4][5];
  volatile uint16 *digit_tile_mem  = (uint16 *) tile_mem[4][6];

  for (int i = 0; i < 4 * (sizeof(tile_4bpp) / 2); ++i)
    paddle_tile_mem[i] = 0x1111;
  for (int i = 0; i < (sizeof(tile_4bpp) / 2); ++i)
    ball_tile_mem[i] = 0x2222;
  for (int i = 0; i < (sizeof(tile_4bpp) / 2); ++i)
    digit_tile_mem[i] = 0x1111;

  //write the colour palette for our sprites into the first palette of 16 colours in memory
  object_palette_mem[1] = RGB15(0x1f, 0x1f, 0x1f);
  object_palette_mem[2] = RGB15(0x1f, 0x00, 0x1f);
  
  //create our sprites by writing their object attributes into OAM memory
  volatile obj_attrs *paddle_attrs = &oam_mem[0];
  paddle_attrs->attr0 = 0x8000; //4bpp tiles, TALL shape
  paddle_attrs->attr1 = 0x4000; //8x32 shape
  paddle_attrs->attr2 = 1;

  volatile obj_attrs *ball_attrs = &oam_mem[1];
  ball_attrs->attr0 = 0; //4bpp tiles, SQUARE shape
  ball_attrs->attr1 = 0; //8x8 size
  ball_attrs->attr2 = 5;

  volatile obj_attrs *digit_attrs = &oam_mem[2];
  digit_attrs->attr0 = 0;
  digit_attrs->attr1 = 0;
  digit_attrs->attr2 = 1;

  const int player_width = 8;
  const int player_height = 32;
  const int ball_width = 8;
  const int ball_height = 8;
  const int digit_height = 8;
  const int digit_width = 2;

  int player_velocity = 2;
  int ball_velocity_x = 2;
  int ball_velocity_y = 1;
  int player_x = 5;
  int player_y = 96;
  int ball_x = 22;
  int ball_y = 96;
  int digit_x = 200;
  int digit_y = 140;
  set_object_position(paddle_attrs, player_x, player_y);
  set_object_position(ball_attrs, ball_x, ball_y);
  set_object_position(digit_attrs, digit_x, digit_y);
  int player_max_clamp_y = SCREEN_HEIGHT - player_height;

  //set the display parameter to enable objects and use a 1d object->tile mapping
  REG_DISPLAY = 0x1000 | 0x0040;

  uint32 key_states = 0;
  
  while (1)
    {
      while (REG_DISPLAY_VCOUNT >= 160);
      while (REG_DISPLAY_VCOUNT <  160);

      key_states = ~REG_KEY_INPUT & KEY_ANY;

      if (key_states & KEY_UP)
	player_y = clamp(player_y - player_velocity, 0, player_max_clamp_y);
      if (key_states & KEY_DOWN)
	player_y = clamp(player_y + player_velocity, 0, player_max_clamp_y);
      if (key_states & KEY_UP || key_states & KEY_DOWN)
	set_object_position(paddle_attrs, player_x, player_y);

      int ball_max_clamp_x = SCREEN_WIDTH - ball_width;
      int ball_max_clamp_y = SCREEN_HEIGHT - ball_height;

      if ((ball_x >= player_x && ball_x <= player_x + player_width) &&
	  (ball_y >= player_y && ball_y <= player_y + player_height))
	{
	  ball_x = player_x + player_width;
	  ball_velocity_x = -ball_velocity_x;
	}
      else
	{
	  if (ball_x == 0 || ball_x == ball_max_clamp_x)
	    ball_velocity_x = -ball_velocity_x;
	  if (ball_y == 0 || ball_y == ball_max_clamp_y)
	    ball_velocity_y = -ball_velocity_y;
	}

      ball_x = clamp(ball_x + ball_velocity_x, 0, ball_max_clamp_x);
      ball_y = clamp(ball_y + ball_velocity_y, 0, ball_max_clamp_y);
      set_object_position(ball_attrs, ball_x, ball_y);
    }

  printf("ball_x: %d\n", ball_x);
  
  return 0;
}
