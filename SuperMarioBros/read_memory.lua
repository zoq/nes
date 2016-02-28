 --[[
 @file read_memory.lua
 @author Marcus Edel

 Definition of memory routines.
 --]]

local S = {};

-- Check if there are any enemies drawn. Note there are max 5 enemies at once:
-- 0x000F-0x0013
-- 0 - No
-- 1 - Yes
--@return The enemy postions.
local function EnemySprites()
  -- Enemy horizontal position in level RAM: 0x006E-0x0072.
  -- Enemy x position on screen RAM: 0x0087/B
  -- Length of one level: 256 (0x100)
  -- Enemy y pos on screen RAM: 0x00CF-0x00D3

  local sprites = {};

  for slot = 0,4 do
    if memory.readbyte(0xF+slot) ~= 0 then
      local enemyX = memory.readbyte(0x6E + slot) * 0x100 +
          memory.readbyte(0x87+slot);

      local enemyY = memory.readbyte(0xCF + slot) + 24;
      
      sprites[#sprites+1] = {["x"] = enemyX, ["y"] = enemyY};
    end
  end

  return sprites
end

-- Get the player postion (mario).
-- Player horizontal position in level: 0x006D)
-- Player x position on screen: 0x0086
-- Player y pos within current screen: 0x03B8
--@return The postion of mario.
local function MarioPostion()
  local mario = {};

  mario['x'] = memory.readbyte(0x6D) * 0x100 + memory.readbyte(0x86);
  mario['y'] = memory.readbyte(0x03B8) + 16;

  return mario
end

--Read and return the tile types as matrix.
--@param marioX The x coordinate of the starting postion.
--@param marioY The y coordinate of the starting postion.
--@param radius The radius of the view field (Default 6).
--@return The tiles types of the viewfiled as matrix.
local function ReadTiles(marioX, marioY, radius)
  local radius = radius or 6
  local address = 0;
  local tilesRow = 0;
  local tiles = {}; 

  --Iterate through all tiles and find their types.
  for dy = -radius * 16, radius * 16, 16 do
    --Create a new row for each iteration.
    tilesRow = dy / 16
    tiles[tilesRow] = {}

    local subSpriteY = math.floor((marioY + dy - 48) / 16)

    for dx = -radius * 16, radius * 16, 16 do
      --Create a new tile for each iteration.
      tiles[tilesRow][#tiles[tilesRow] + 1] = 0;

      --Check if the sprite is outside of our range of drawing.
      if subSpriteY >= 0 then
        local spriteX = marioX + dx + 8;

        -- Depending of the page we have to add an offset to the address.
        if math.floor(spriteX/256)%2 > 0 then
          address = 0x5D0 + subSpriteY * 16 + math.floor((spriteX % 256) / 16)
        else
          address = 0x500 + subSpriteY * 16 + math.floor((spriteX % 256) / 16)
        end

        local tileVal = memory.readbyte(address)

        if tileVal ~= 0 and (marioY + dy) < 0x1B0 then
          tiles[tilesRow][#tiles[tilesRow]] = 1;
        end
      end
    end        
  end

  --Check if there's an enemy in the block.
  sprites = EnemySprites();

  for i = 1, #sprites do
    local enemyTileX = math.floor(-(marioX - sprites[i]["x"]) / 16);
    local enemyTileY = math.floor(-(marioY - sprites[i]["y"]) / 16);

    if math.abs(enemyTileX) <= 6 and enemyTileY <= 8 then

      if (enemyTileY > 6) then
        enemyTileY = 6 - enemyTileY
      end;
      
      -- Ensure we're drawing in the bounds of our coordinates.
      if -6 < enemyTileY and enemyTileY < 6 and 0 < (enemyTileX + 8) and (enemyTileX + 8) < 14 then
        tiles[enemyTileY][enemyTileX + 8] = 2
      end
    end
  end

  tiles[1][radius + 1] = 3;

  return tiles;
end

-- Get the player lives.
--@return The current lives.
local function MarioLives()
  return memory.readbyte(0x075A)
end

-- Get the player coins.
--@return The current coins.
local function MarioCoins()
  return memory.readbyte(0x075E)
end

S.EnemySprites = EnemySprites;
S.MarioPostion = MarioPostion;
S.ReadTiles = ReadTiles;
S.MarioLives = MarioLives;
S.MarioCoins = MarioCoins;

return S
