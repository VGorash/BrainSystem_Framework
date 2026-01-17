#ifndef COLORS_H
#define COLORS_H

namespace vgs
{

struct Color
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

Color colorFromPlayerNumber(int playerNumber)
{
  if(playerNumber < 0)
  {
	playerNumber = 0;
  }
  
  constexpr int numColors = 16;
  playerNumber %= numColors;
  
  constexpr Color colors[] = 
  {
    {255, 255, 0},   // Yellow
    {0, 0, 255},     // Blue
    {0, 255, 0},     // Green
    {255, 0, 0},     // Red
    {255, 0, 255},   // Magenta
    {0, 255, 255},   // Cyan
    {255, 128, 0},   // Orange
    {128, 0, 255},   // Violet
    {0, 255, 128},   // Spring Green
    {255, 0, 128},   // Rose
    {128, 255, 0},   // Lime
    {0, 128, 255},   // Azure
    {128, 128, 0},   // Olive
    {128, 0, 128},   // Purple
    {0, 128, 128},   // Teal
    {64, 128, 255}   // Light Blue
  };
  
  return colors[playerNumber];
}

} // namespace vgs

#endif