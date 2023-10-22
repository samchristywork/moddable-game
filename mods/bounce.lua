function Vector(x, y, z)
  local vec = {};
  vec.x = x;
  vec.y = y;
  vec.z = z;
  return vec;
end

local position = Vector(0, 20, 0);
local velocity = Vector(0, 0, 0);
local acceleration = Vector(0, -0.1, 0);

game.on("render3d", function(screenWidth, screenHeight)
  -- Update position
  velocity.x = velocity.x + acceleration.x;
  velocity.y = velocity.y + acceleration.y;
  velocity.z = velocity.z + acceleration.z;
  position.x = position.x + velocity.x;
  position.y = position.y + velocity.y;
  position.z = position.z + velocity.z;

  -- Bounce off the ground
  if position.y < 0 then
    position.y = 0;
    velocity.y = -velocity.y * 0.8;
  end

  updatePosition(position.x, position.y, position.z);
end)
