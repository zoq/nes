 --[[
 @file super_mario_bros.lua
 @author Marcus Edel

 Definition of game state.
 --]]

-- Manually set the package path
-- package.path = package.path .. ';/path/to/nes/SuperMarioBros/?.lua'

local readMemory = require("read_memory");
local writeJoypad = require("write_joypad");
local server = require("server");
local json = require("cjson");

-- Check for the gd module.
local hasgd, gd = pcall(require,"gd")

-- Locally stored save state.
saveState = 0

-- Locally stored frame counter.
frameCounter = 0

-- Locally stored start indication parameter.
start = false

-- Locally stored current key value.
currentKey = ""

-- Locally stored frame divisor.
frameDivisor = 30

-- Locally stored image quality parameter.
imageQuality = 80

-- Locally stored port.
port = 4561


-- Skip the start screen and create a savestate.
function StartGame()
  emu.speedmode("maximum")

  while(start ~= true) do
    frameCounter = frameCounter + 1;

    if (frameCounter == 150) then
      writeJoypad.PressStart()
    end

    if (frameCounter == 350) then
      saveState = savestate.object(1)
      savestate.save(saveState)

      start = true
    end

    emu.frameadvance()
  end
end

-- Function handler for following events:
-- Press A -> "key" : {"value" : "A"}
-- Press B -> "key" : {"value" : "B"}
-- Press right -> "key" : {"value" : "Right"}
-- Press left -> "key" : {"value" : "Left"}
-- Press up -> "key" : {"value" : "Up"}
-- Press down -> "key" : {"value" : "Down"}
-- Press Start -> "key" : {"value" : "Start"}
-- Reset game -> "game" : {"value" : "Reset"}
-- Send game tiles -> "game" : {"value" : "Tiles"}
-- Send all game Infos -> "game" : {"value" : "Info"}
-- Set the frame divisor -> "config" : frameDivisor
function FunctionHandler(data)
  if data ~= nil and string.len(data) > 2 then

    -- Act as balancer in case the client sends a get enpoint message.
    if string.match(data, "get") then
      endpoint = {host = "*", port = port}
      server.Send(json.encode({endpoint = endpoint}))
      return
    end

    local success, values = pcall(json.decode, data);
    if (success) then

      if (values ~= nil) then

        -- Check the key values.
        if (values["key"] ~= nil) then
          if (values["key"]["value"] == "A") then
            currentKey = "A"
            writeJoypad.PressA()
          elseif (values["key"]["value"] == "B") then
            currentKey = "B"
            writeJoypad.PressB()
          elseif (values["key"]["value"] == "Right") then
            currentKey = "Right"
            writeJoypad.PressRight()
          elseif (values["key"]["value"] == "Left") then
            currentKey = "Left"
            writeJoypad.PressLeft()
          elseif (values["key"]["value"] == "Up") then
            currentKey = "Up"
            writeJoypad.PressUp()
          elseif (values["key"]["value"] == "Down") then
            currentKey = "Down"
            writeJoypad.PressDown()
          elseif (values["key"]["value"] == "Start") then
            currentKey = "Start"
            writeJoypad.PressStart()
          else
            print("Unknown key value: " + values["key"]["value"])
          end
        end

        -- Check the game values.
        if (values["game"] ~= nil) then
          if (values["game"]["value"] == "Reset") then
            savestate.load(saveState)
          end

          if (values["game"]["value"] == "Image" and hasgd) then
            local gdStr = gui.gdscreenshot();
            local gdImg = gd.createFromGdStr(gdStr);
            local image = gdImg:jpegStr(imageQuality)

            server.Send(image)
          end

          if (values["game"]["value"] == "Tiles") then

            local mario = readMemory.MarioPostion();
            local tiles = readMemory.ReadTiles(mario['x'], mario['y']);

            server.Send(json.encode({tiles = tiles}))
          end

          if (values["game"]["value"] == "Info") then

            local mario = readMemory.MarioPostion();
            local tiles = readMemory.ReadTiles(mario['x'], mario['y']);
            local lives = readMemory.MarioLives();
            local coins = readMemory.MarioCoins();
            local state = readMemory.PlayersState();

            server.Send(json.encode({mario = mario,
                                     tiles = tiles,
                                     lives = lives,
                                     coins = coins,
                                     state = state}))
          end
        end

         -- Check the config values.
        if (values["config"] ~= nil) then
          if (values["config"]["frame"] ~= nil) then

            -- Set frame counter.
            frameCounter = values["config"]["frame"]
          elseif (values["config"]["image"] ~= nil) then

            -- Set image quality.
            imageQuality = values["config"]["image"]
          elseif (values["config"]["divisor"] ~= nil) then

            -- Set frame divisor.
            frameDivisor = values["config"]["divisor"]
          elseif (values["config"]["speed"] ~= nil) then

            -- Set emulation speed (maximum, normal, turbo).
            local speed = values["config"]["speed"]
            if (speed == "maximum") then
              emu.speedmode("maximum")
            elseif (speed == "normal") then
              emu.speedmode("normal")
            elseif (speed == "turbo") then
              emu.speedmode("turbo")
            else
              print("Unknown speed value: " + speed)
            end
          else
            print("Unknown config value: " + values["config"])
          end
        end
      end
    end
  end
end

-- Start the game and wait for connections.
StartGame()
server.Server("*", port, 1)
server.Accept()
savestate.load(saveState)

while (true) do
  -- Handle the input data.
  if (frameCounter % frameDivisor) == 0 then
    local data = server.Receive()
    if (data ~= nil) then
      FunctionHandler(data)
    else
      print("Lost connection listen.")
      server.Accept()
      savestate.load(saveState)
    end
  end

  -- Continue with the last key if the divisor isn't 1.
  if (frameDivisor ~= 1) then
    if (currentKey == "A") then
      writeJoypad.PressA()
    elseif (currentKey == "B") then
      writeJoypad.PressB()
    elseif (currentKey == "Right") then
      writeJoypad.PressRight()
    elseif (currentKey == "Left") then
      writeJoypad.PressLeft()
    elseif (currentKey == "Down") then
      writeJoypad.PressDown()
    elseif (currentKey == "Up") then
      writeJoypad.PressUp()
    end
  end

  frameCounter = frameCounter + 1
  emu.frameadvance()
end