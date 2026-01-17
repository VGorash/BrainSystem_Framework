#ifndef SOUNDS_H
#define SOUNDS_H

#include "Core/Hal.h"

namespace vgs
{

int toneFromSound(HalSound sound) // returns tone in Hz
{
  constexpr int tones[] = {
    2000, // Start
    1000, // Press
    500,  // Falstart
    1500, // Tick
    1500, // Signal
    250,  // End
    0,    // None
  };
  return tones[static_cast<int>(sound)];
}

int durationFromSound(HalSound sound) // return duration in milliseconds
{
  constexpr int durations[] = {
    1000, // Start
    1000, // Press
    500,  // Falstart
    250,  // Tick
    1000, // Signal
    1000, // End
    0,    // None
  };
  return durations[static_cast<int>(sound)];
}

} // namespace vgs

#endif