local crosshairSize = 20

local function drawCrosshair(screenWidth, screenHeight)
  local x = screenWidth / 2
  local y = screenHeight / 2

  drawLine(x - crosshairSize / 2, y, x + crosshairSize / 2, y)
  drawLine(x, y - crosshairSize / 2, x, y + crosshairSize / 2)
end

game.on("render", function(screenWidth, screenHeight)
  drawCrosshair(screenWidth, screenHeight)
end)
