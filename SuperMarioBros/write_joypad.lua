 --[[
 @file write_joypad.lua
 @author Marcus Edel

 Definition of joypad routines.
 --]]

local S = {};

-- Table keys used to set the button.
-- True means that the button is set.
local joypadA = { A = true };
local joypadB = { B = true };
local joypadRight = { right = true };
local joypadLeft = { left = true };
local joypadUp = { up = true };
local joypadDown = { down = true };
local joypadStart = { start = true };

-- Press the A button.
local function PressA()
  joypad.set(1, joypadA);
end

-- Press the B button.
local function PressB()
  joypad.set(1, joypadB);
end

-- Press the right button.
local function PressRight()
  joypad.set(1, joypadRight);
end

-- Press the left button.
local function PressLeft()
  joypad.set(1, joypadLeft);
end

-- Press the up button.
local function PressUp()
  joypad.set(1, joypadUp);
end

-- Press the down button.
local function PressDown()
  joypad.set(1, joypadDown);
end

-- Press the start button.
local function PressStart()
  joypad.set(1, joypadStart);
end

S.PressA = PressA;
S.PressB = PressB;
S.PressRight = PressRight;
S.PressLeft = PressLeft;
S.PressUp = PressUp;
S.PressDown = PressDown;
S.PressStart = PressStart;

return S
