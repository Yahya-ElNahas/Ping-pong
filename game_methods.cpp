#define isDown(b) input->buttons[b].isPressed
#define isPressed(b) input->buttons[b].isPressed && input->buttons[b].isChanged
#define isReleased(b) !(input->buttons[b].isPressed) && input->buttons[b].isChanged

float player_1_Pos, player_1_DPos, player_2_Pos, player_2_DPos;
float arenaHalfSizeX = 85, arenaHalfSizeY = 45;
float playerHalfSizeX = 2.5, playerHalfSizeY = 12, ballHalfSize = 1;
float ballPosX, ballPosY, ballDpX = 100, ballDpY;

internal void updateScreen(Input* input, float deltaT) {
	clear_screen(0x005500);

	draw_rect(0, 0, arenaHalfSizeX, arenaHalfSizeY, 0x305030);

	float player_1_DDPos = 0.f;
	float player_2_DDPos = 0.f;

	if (isDown(BTN_W))
		player_1_DDPos += 2000;
	if (isDown(BTN_S))
		player_1_DDPos -= 2000;
	if (isDown(BTN_UP))
		player_2_DDPos += 2000;
	if (isDown(BTN_DOWN))
		player_2_DDPos -= 2000;

	player_1_DDPos -= player_1_DPos * 10.f;
	player_1_Pos = player_1_Pos + player_1_DPos * deltaT + player_1_DDPos * deltaT * deltaT * .5f;
	player_1_DPos = player_1_DPos + player_1_DDPos * deltaT;

	if (player_1_Pos + playerHalfSizeY > arenaHalfSizeY) {
		player_1_Pos = arenaHalfSizeY - playerHalfSizeY;
		player_1_DPos *= -1;
	}
	if (player_1_Pos - playerHalfSizeY < -arenaHalfSizeY) {
		player_1_Pos = -arenaHalfSizeY + playerHalfSizeY;
		player_1_DPos *= -1;
	}

	player_2_DDPos -= player_2_DPos * 10.f;
	player_2_Pos = player_2_Pos + player_2_DPos * deltaT + player_2_DDPos * deltaT * deltaT * .5f;
	player_2_DPos = player_2_DPos + player_2_DDPos * deltaT;

	if (player_2_Pos + playerHalfSizeY > arenaHalfSizeY) {
		player_2_Pos = arenaHalfSizeY - playerHalfSizeY;
		player_2_DPos *= -1;
	}
	if (player_2_Pos - playerHalfSizeY < -arenaHalfSizeY) {
		player_2_Pos = -arenaHalfSizeY + playerHalfSizeY;
		player_2_DPos *= -1;
	}

	ballPosX += ballDpX * deltaT;
	ballPosY += ballDpY * deltaT;

	draw_rect(ballPosX, ballPosY, ballHalfSize, ballHalfSize, 0xffffff);

	if (ballPosX + ballHalfSize > 80 - playerHalfSizeX &&
		ballPosX - ballHalfSize < 80 + playerHalfSizeX &&
		ballPosY + ballHalfSize > player_2_Pos - playerHalfSizeY &&
		ballPosY + ballHalfSize < player_2_Pos + playerHalfSizeY) 
	{
		ballPosX = 80 - playerHalfSizeX - ballHalfSize;
		ballDpX *= -1;
		ballDpY = (ballPosY - player_2_Pos) * 2 + player_2_DPos * .75f;
	} else if(ballPosX + ballHalfSize > -80 - playerHalfSizeX &&
		ballPosX - ballHalfSize < -80 + playerHalfSizeX &&
		ballPosY + ballHalfSize > player_1_Pos - playerHalfSizeY &&
		ballPosY + ballHalfSize < player_1_Pos + playerHalfSizeY)
	{
		ballPosX = -80 + playerHalfSizeX + ballHalfSize;
		ballDpX *= -1;
		ballDpY = (ballPosY - player_1_Pos) * 2 + player_1_DPos * .75f;
	}

	if (ballPosY + ballHalfSize > arenaHalfSizeY) {
		ballPosY = arenaHalfSizeY - ballHalfSize;
		ballDpY *= -1;
	} else if (ballPosY - ballHalfSize < -arenaHalfSizeY) {
		ballPosY = -arenaHalfSizeY + ballHalfSize;
		ballDpY *= -1;
	}

	draw_rect(80, player_2_Pos, playerHalfSizeX, playerHalfSizeY, 0xab2020);
	draw_rect(-80, player_1_Pos, playerHalfSizeX, playerHalfSizeY, 0x2020ab);
}