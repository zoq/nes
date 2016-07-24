/**
 * @file messages.hpp
 * @author Marcus Edel
 *
 * Miscellaneous messages.
 */
#include <string>

namespace messages {

//! Create press 'A' JSON message.
static inline std::string PressA()
{
  return "\"key\":{\"value\": \"A\"}";
}

//! Create press 'B' JSON message.
static inline std::string PressB()
{
  return "\"key\":{\"value\": \"B\"}";
}

//! Create press 'Right' JSON message.
static inline std::string PressRight()
{
  return "\"key\":{\"value\": \"Right\"}";
}

//! Create press 'Left' JSON message.
static inline std::string PressLeft()
{
  return "\"key\":{\"value\": \"Left\"}";
}

//! Create press 'Up' JSON message.
static inline std::string PressUp()
{
  return "\"key\":{\"value\": \"Up\"}";
}

//! Create press 'Down' JSON message.
static inline std::string PressDown()
{
  return "\"key\":{\"value\": \"Down\"}";
}

//! Create press 'Start' JSON message.
static inline std::string PressStart()
{
  return "\"key\":{\"value\": \"Start\"}";
}

//! Create message to get the tiles.
static inline std::string GameTiles()
{
  return "\"game\":{\"value\": \"Tiles\"}";
}

//! Create message to get the game info.
static inline std::string GameInfo()
{
  return "\"game\":{\"value\": \"Info\"}";
}

//! Create message to reset the game.
static inline std::string GameReset()
{
  return "\"game\":{\"value\": \"Reset\"}";
}

//! Create message to get the game as image (jpeg).
static inline std::string GameImage()
{
  return "\"game\":{\"value\": \"Image\"}";
}

//! Create message to set the number of frames that should be run without any
// interaction.
static inline std::string ConfigFrame(const int frame)
{
  return "\"config\":{\"frame\": " + std::to_string(frame) + "}";
}

//! Create message to set the image quality (jpeg).
static inline std::string ConfigImage(const int quality)
{
  return "\"config\":{\"image\": " + std::to_string(quality) + "}";
}

//! Create message to set the divisor.
static inline std::string ConfigDivisor(const int divisor)
{
  return "\"config\":{\"divisor\": " + std::to_string(divisor) + "}";
}

//! Create message to set the emulation speed (normal, maximum, turbo).
static inline std::string ConfigSpeed(const std::string& speed)
{
  return "\"config\":{\"speed\": " + speed + "}";
}

//! Function to append a JSON message to JSON another message.
static inline void Append(std::string& messageA, const std::string& messageB)
{
  if (messageA.empty())
  {
    messageA = messageB;
  }
  else
  {
    messageA += "," + messageB;
  }
}

//! Function to create the final JSON message.
static inline std::string JSONMessage(const std::string& messageA)
{
  return "{" + messageA + "}";
}

} // namespace messages