local frame = 0

local function drawFrame(screenWidth, screenHeight)
  local x = 10
  local y = 10
  local text = "Frame: " .. frame
  drawText(text, x, y, 5)
  frame = frame + 1
  if (frame > 100) then
    frame = 0
  end
end

game.on("render2d", function(screenWidth, screenHeight)
  drawFrame(screenWidth, screenHeight)
end)
