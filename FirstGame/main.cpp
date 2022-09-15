#include "DxLib.h"

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "Gravitas Magia";

// �E�B���h�E����(64�~20)
const int WIN_WIDTH = 1280;

// �E�B���h�E�c��(64�~10+160)
const int WIN_HEIGHT = 800;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x30, 0x30, 0x30);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	int grMagia[8], grChargeIsaac[3], grBlock[4], grNumber[10], grBack[4], grPinhole, grUi, grSign,
		grIsapple, grCharge, grGauge, grCircle, grTitle[34], grExit, grClear, grMiniMap;
	LoadDivGraph("Magia.png", 8, 8, 1, 64, 64, grMagia);
	LoadDivGraph("IsaacCharge.png", 3, 3, 1, 64, 64, grChargeIsaac);
	grBlock[0] = LoadGraph("Ground.png");
	grBlock[1] = LoadGraph("Branch.png");
	grBlock[2] = LoadGraph("Stone.png");
	grBlock[3] = LoadGraph("Stem.png");
	LoadDivGraph("Number.png", 10, 5, 2, 32, 40, grNumber);
	grBack[0] = LoadGraph("Back01.png");
	grBack[1] = LoadGraph("Back02.png");
	grBack[2] = LoadGraph("Back03.png");
	grBack[3] = LoadGraph("Back04.png");
	grPinhole = LoadGraph("vignette.png");
	grUi = LoadGraph("UI.png");
	grSign = LoadGraph("SignBoard.png");
	grIsapple = LoadGraph("Isapple.png");
	grCharge = LoadGraph("Charge.png");
	grGauge = LoadGraph("ChargeBox.png");
	grCircle = LoadGraph("MagiCircle.png");
	LoadDivGraph("Title.png", 2, 2, 1, 1280, 800, grTitle);
	grExit = LoadGraph("GoalBoard.png");
	grClear = LoadGraph("Clear.png");
	grMiniMap = LoadGraph("IsaacUI.png");

	int bgm01, chargeSE, magi, get, falled;
	bgm01 = LoadSoundMem("Field01.mp3");
	chargeSE = LoadSoundMem("Charge.mp3");
	magi = LoadSoundMem("Gravitas.mp3");
	get = LoadSoundMem("Get.mp3");
	falled = LoadSoundMem("Falled.mp3");
	ChangeVolumeSoundMem(200, bgm01);

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	int x = 20, y = 200, r = 32, oldX, whichGravi = 1, gravi = 16, charge = 0, score = 0,
		charger = 0, appleX[5], appleY[5], getApple[5] = {};
	appleX[0] = 0;
	appleX[1] = 0;
	appleX[2] = 0;
	appleX[3] = WIN_WIDTH * 5;
	appleX[4] = 0;
	appleY[0] = 0;
	appleY[1] = 0;
	appleY[2] = 0;
	appleY[3] = 300;
	appleY[4] = 0;

	int worldX = 0;
	/*�L�����O���l�p*/
	int x1, y1, x2, y2;

	int blockSize = 64,
		map1[10][20] = {
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},
		map2[10][20] = {
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
		{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},
		map3[10][20] = {
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 2},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}
	},
		map4[10][20] = {
		{2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2,2,2,2,2},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1}
	},
		map5[10][20] = {
		{2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,2,2,2,2,2,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	},
		map6[10][20] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	},
	map7[10][20] = {
		{2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,2,2,2},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,4,2,2,2,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,3},
		{0,0,0,0,0,0,0,0,4,2,2,4,0,0,0,0,0,0,0,3},
		{0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,0,0,0,0,3},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
		{0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,3},
		{1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1}
	},
		map8[10][20] = {
			{2,2,2,2,2,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2},
			{0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1}
	};
	int mapX = 0, backX[4] = {};
	//�A�j���[�V�����p�ϐ�
	double spin = 0, aura = 1.5;
	int animeTitle = 0, drawTitle = 0, animeIsaac = 0, drawRun = 0, animeCharge = 0, drawCharge = 0;

	enum scene { TITLE, PLAY, RESULT };
	int gameScene = TITLE;
	int gameFinished = 0;
	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	PlaySoundMem(bgm01, DX_PLAYTYPE_BACK);

	// �Q�[�����[�v
	while (1)
	{
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; i++) {
			oldkeys[i] = keys[i];
		}
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//
		// �X�V����
		switch (gameScene) {
		case TITLE:
			animeTitle++;
			if (animeTitle >= 20) {
				animeTitle = 0;
				drawTitle++;
				if (drawTitle >= 2) {
					drawTitle = 0;
				}
			}
			if (keys[KEY_INPUT_SPACE] == 1) {
				gameScene = PLAY;
			}
			break;

		case PLAY:
			oldX = x;
			/*�����X�N���[��*/
			if (keys[KEY_INPUT_SPACE] == 0 || whichGravi == 0) {
				x += 4;
			} else if (whichGravi == 1 && keys[KEY_INPUT_SPACE] == 1) {
				x++;
			}
			if (x >= 100 && x <= WIN_WIDTH * 7 + 100) {
				worldX = x - 100;
			}
			if (x - worldX < 100) {
				x++;
			}
			/*���R��������*/
			y += gravi;

			/*���L�����l�p���W�X�V*/
			x1 = x - 32;
			y1 = y - r;
			x2 = x + 31;/*31��r��2-1*/
			y2 = y + r;

			/*���x�����t���d�͉���*/
			if (whichGravi == 1) {
				gravi += 1;
			} else {
				gravi -= 1;
			}
			if (whichGravi == 1 && gravi > 63) {
				gravi = 63;
			}
			if (whichGravi == 0 && gravi < -63) {
				gravi = -63;
			}

			/*�`�b�v�Փ˔���*/
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 20; j++) {
					/*�E��Փ�(�Ǔ�)*/
					if (map1[i][j] != 0 && (x2 + 1) / 64 == j && y1 / 64 == i) {
						x = oldX - 4;
					}
					if (map2[i][j] != 0 && (x2 - WIN_WIDTH * 1 + 1) / 64 == j && y1 / 64 == i) {
						x = oldX - 4;
					}
					if (map3[i][j] != 0 && (x2 - WIN_WIDTH * 2 + 1) / 64 == j && y1 / 64 == i) {
						x = oldX - 4;
					}
					if (map4[i][j] != 0 && (x2 - WIN_WIDTH * 3 + 1) / 64 == j && y1 / 64 == i) {
						x = oldX - 4;
					}
					if (map5[i][j] != 0 && (x2 - WIN_WIDTH * 4 + 1) / 64 == j && y1 / 64 == i) {
						x = oldX - 4;
					}
					if (map6[i][j] != 0 && (x2 - WIN_WIDTH * 5 + 1) / 64 == j && y1 / 64 == i) {
						x = oldX - 4;
					}
					if (map7[i][j] != 0 && (x2 - WIN_WIDTH * 6 + 1) / 64 == j && y1 / 64 == i) {
						x = oldX - 4;
					}
					if (map8[i][j] != 0 && (x2 - WIN_WIDTH * 7 + 1) / 64 == j && y1 / 64 == i) {
						x = oldX - 4;
					}
					/*���d�͒��n*/
					if (whichGravi == 1) {
						if (map1[i][j] != 0 && x1 / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map1[i][j] != 0 && x2 / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map2[i][j] != 0 && (x1 - WIN_WIDTH * 1) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map2[i][j] != 0 && (x2 - WIN_WIDTH * 1) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map3[i][j] != 0 && (x1 - WIN_WIDTH * 2) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map3[i][j] != 0 && (x2 - WIN_WIDTH * 2) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map4[i][j] != 0 && (x1 - WIN_WIDTH * 3) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map4[i][j] != 0 && (x2 - WIN_WIDTH * 3) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map5[i][j] != 0 && (x1 - WIN_WIDTH * 4) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map5[i][j] != 0 && (x2 - WIN_WIDTH * 4) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map6[i][j] != 0 && (x1 - WIN_WIDTH * 5) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map6[i][j] != 0 && (x2 - WIN_WIDTH * 5) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map7[i][j] != 0 && (x1 - WIN_WIDTH * 6) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map7[i][j] != 0 && (x2 - WIN_WIDTH * 6) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map8[i][j] != 0 && (x1 - WIN_WIDTH * 7) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map8[i][j] != 0 && (x2 - WIN_WIDTH * 7) / 64 == j && (y2) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}
					}
					/*���d�͒��n*/
					else {
						if (map1[i][j] != 0 && x1 / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map1[i][j] != 0 && x2 / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map2[i][j] != 0 && (x1 - WIN_WIDTH * 1) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map2[i][j] != 0 && (x2 - WIN_WIDTH * 1) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map3[i][j] != 0 && (x1 - WIN_WIDTH * 2) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map3[i][j] != 0 && (x2 - WIN_WIDTH * 2) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map4[i][j] != 0 && (x1 - WIN_WIDTH * 3) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map4[i][j] != 0 && (x2 - WIN_WIDTH * 3) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map5[i][j] != 0 && (x1 - WIN_WIDTH * 4) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map5[i][j] != 0 && (x2 - WIN_WIDTH * 4) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map6[i][j] != 0 && (x1 - WIN_WIDTH * 5) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map6[i][j] != 0 && (x2 - WIN_WIDTH * 5) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map7[i][j] != 0 && (x1 - WIN_WIDTH * 6) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map7[i][j] != 0 && (x2 - WIN_WIDTH * 6) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}

						if (map8[i][j] != 0 && (x1 - WIN_WIDTH * 7) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						} else if (map8[i][j] != 0 && (x2 - WIN_WIDTH * 7) / 64 == j && (y2 - 1) / 64 == i) {
							y = 64 * (i + 1 - whichGravi) - r;
						}
					}
				}
			}

			//�A�C�e���E������
			for (int i = 0; i < 5; i++) {
				if (getApple[i] == 0 && appleX[i] >= x1 && appleX[i] <= x2 && appleY[i] >= y1 && appleY[i] <= y2) {
					PlaySoundMem(get, DX_PLAYTYPE_BACK);
					score++;
					getApple[i] = 1;
				}
			}

			//�d�͕ϓ��S��
			if (charge == 0 && keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0) {
				PlaySoundMem(chargeSE, DX_PLAYTYPE_BACK);
				charger = 1;
			}
			if (charger == 1 && keys[KEY_INPUT_SPACE] == 1) {
				aura -= 0.01;
				charge++;
			}
			if (aura < 0) {
				aura = 0;
			}
			if (charge > 0) {
				spin += 0.1;
			}
			if (charge >= 100) {
				charge = 100;
			}
			if (charger == 1 && keys[KEY_INPUT_SPACE] == 0 && oldkeys[KEY_INPUT_SPACE] == 1) {
				StopSoundMem(chargeSE);
				charger = 0;
				aura = 1.5;
				PlaySoundMem(magi, DX_PLAYTYPE_BACK);
				gravi = -5;
				whichGravi = 0;
			}
			if (charge > 0 && charger == 0) {
				charge--;
			}

			if (whichGravi == 0 && charge <= 0) {
				gravi = 5;
				whichGravi = 1;
			}

			/*�S�[������*/
			if (x >= WIN_WIDTH * 8) {
				gameScene = RESULT;
			}

			//��������
			if (y < 0 + r * 2 || y>640 + r * 2) {
				PlaySoundMem(falled, DX_PLAYTYPE_NORMAL);
				if (x <= WIN_WIDTH * 2) {
					x = WIN_WIDTH;
					y = 200;
					gravi = 5;
					whichGravi = 1;
				} else if (x <= WIN_WIDTH * 3) {
					x = WIN_WIDTH * 2 - 120;
					y = 200;
					gravi = 5;
					whichGravi = 1;
				} else if (x <= WIN_WIDTH * 4) {
					x = WIN_WIDTH * 3 - 60;
					y = 100;
					gravi = 5;
					whichGravi = 1;
				} else if (x <= WIN_WIDTH * 5) {
					x = WIN_WIDTH * 5 - 576;
					y = 448;
					gravi = 5;
					whichGravi = 1;
				} else if (x <= WIN_WIDTH * 6) {
					y = 60;
					gravi = 5;
					whichGravi = 1;
				} else if (x <= WIN_WIDTH * 7) {
					x = WIN_WIDTH * 6 - 80;
					y = 200;
					gravi = 5;
					whichGravi = 1;
				} else if (x <= WIN_WIDTH * 8) {
					x = WIN_WIDTH * 7;
					y = 100;
					gravi = 5;
					whichGravi = 1;
				}
			}

			//�A�C�U�b�N�A�j���[�V����
			animeIsaac++;
			if (animeIsaac >= 2) {
				animeIsaac = 0;
				drawRun++;
				if (drawRun >= 8) {
					drawRun = 0;
				}
			}
			if (charger == 1) {
				animeCharge++;
				if (animeCharge >= 8) {
					animeCharge = 0;
					drawCharge++;
					if (drawCharge >= 3) {
						drawCharge = 0;
					}
				}
			}

			/*�L�������]����*/
			if (whichGravi == 0 && r > -32 && gravi <= -8) {
				r -= 8;
			}
			if (whichGravi == 1 && r < 32 && gravi >= 8) {
				r += 8;
			}

			/*�w�i�ړ�*/
			for (int i = 0; i < 4; i++) {
				backX[i] -= (i + 1) * 2;
				if (backX[i] <= -960) {
					backX[i] = 0;
				}
			}
			break;
		case RESULT:
			if (keys[KEY_INPUT_SPACE] == 1) {
				gameFinished = 1;
			}
			break;
		}

		if (gameFinished == 1) {
			break;
		}

		// �`�揈��
		//�w�i
		for (int i = 0; i < 4; i++) {
			DrawGraph(backX[i], 0, grBack[i], true);
			DrawGraph(backX[i] + 960, 0, grBack[i], true);
			DrawGraph(backX[i] + 1920, 0, grBack[i], true);
		}
		//�}�b�v�u���b�N
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 20; j++) {
				for (int b = 1; b <= 4; b++) {
					if (map1[i][j] == b) {
						DrawGraph(j * blockSize - worldX, i * blockSize, grBlock[b - 1], true);
					}
					if (map2[i][j] == b) {
						DrawGraph(j * blockSize + (WIN_WIDTH * 1) - worldX, i * blockSize, grBlock[b - 1], true);
					}
					if (map3[i][j] == b) {
						DrawGraph(j * blockSize + (WIN_WIDTH * 2) - worldX, i * blockSize, grBlock[b - 1], true);
					}
					if (map4[i][j] == b) {
						DrawGraph(j * blockSize + (WIN_WIDTH * 3) - worldX, i * blockSize, grBlock[b - 1], true);
					}
					if (map5[i][j] == b) {
						DrawGraph(j * blockSize + (WIN_WIDTH * 4) - worldX, i * blockSize, grBlock[b - 1], true);
					}
					if (map6[i][j] == b) {
						DrawGraph(j * blockSize + (WIN_WIDTH * 5) - worldX, i * blockSize, grBlock[b - 1], true);
					}
					if (map7[i][j] == b) {
						DrawGraph(j * blockSize + (WIN_WIDTH * 6) - worldX, i * blockSize, grBlock[b - 1], true);
					}
					if (map8[i][j] == b) {
						DrawGraph(j * blockSize + (WIN_WIDTH * 7) - worldX, i * blockSize, grBlock[b - 1], true);
					}
				}
			}
		}
		//�������Ŕ�
		DrawGraph(1600 - worldX, 512, grSign, true);
		DrawGraph(WIN_WIDTH * 8 - 192 - worldX, 512, grExit, true);
		//�A�C�U�b�v��(�X�R�A�A�C�e��)
		for (int i = 0; i < 5; i++) {
			if (getApple[i] == 0) {
				DrawGraph(appleX[i] - 16 - worldX, appleY[i] - 16, grIsapple, true);
			}
		}
		//�A�C�U�b�N
		if (charge > 0) {
			DrawRotaGraph(x - worldX, y, 1, spin, grCircle, true);
		}
		if (charger == 1) {
			DrawRotaGraph(x - worldX, y, aura, 0, grCharge, true);
			DrawExtendGraph(x - 32 - worldX, y - r, x + 32 - worldX, y + r, grChargeIsaac[animeCharge], true);
		} else if (charger == 0) {
			DrawExtendGraph(x - 32 - worldX, y - r, x + 32 - worldX, y + r, grMagia[drawRun], true);
		}
		DrawGraph(x - 36 - worldX, y - 48, grGauge, true);
		DrawBox(x - 36 - worldX, y - 48, x - 36 - worldX + charge, y - 32, GetColor(255, 255, 255), true);
		//UI
		DrawGraph(0, 640, grUi, false);
		DrawGraph(400, 640, grMiniMap, true);
		DrawGraph(1100, 730, grNumber[2], true);

		/*�f�o�b�O�R�[�h */
		//DrawFormatString(400, 0, GetColor(255, 255, 255), "x:%d y:%d appleX:%d appleY:%d", x, y, appleX[0], appleY[0]);
		//DrawFormatString(400, 20, GetColor(255, 255, 255), "x1:%d y1:%d x2:%d y2:%d", x1, y1, x2, y2);

		DrawGraph(0, 0, grPinhole, true);

		if (gameScene == TITLE) {
			DrawGraph(0, 0, grTitle[drawTitle], true);
		}
		if (gameScene == RESULT) {
			DrawGraph(0, 0, grClear, true);
		}
		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}