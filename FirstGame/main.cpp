#include "DxLib.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "Gravitas Magia";

// ウィンドウ横幅(64×20)
const int WIN_WIDTH = 1280;

// ウィンドウ縦幅(64×10+160)
const int WIN_HEIGHT = 800;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x30, 0x30, 0x30);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み
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

	// ゲームループで使う変数の宣言
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
	/*キャラグラ四角*/
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
	//アニメーション用変数
	double spin = 0, aura = 1.5;
	int animeTitle = 0, drawTitle = 0, animeIsaac = 0, drawRun = 0, animeCharge = 0, drawCharge = 0;

	enum scene { TITLE, PLAY, RESULT };
	int gameScene = TITLE;
	int gameFinished = 0;
	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	PlaySoundMem(bgm01, DX_PLAYTYPE_BACK);

	// ゲームループ
	while (1)
	{
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		for (int i = 0; i < 256; i++) {
			oldkeys[i] = keys[i];
		}
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//
		// 更新処理
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
			/*強制スクロール*/
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
			/*自由落下挙動*/
			y += gravi;

			/*自キャラ四角座標更新*/
			x1 = x - 32;
			y1 = y - r;
			x2 = x + 31;/*31＝r÷2-1*/
			y2 = y + r;

			/*速度制限付き重力加速*/
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

			/*チップ衝突判定*/
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 20; j++) {
					/*右上衝突(追突)*/
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
					/*正重力着地*/
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
					/*反重力着地*/
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

			//アイテム拾得判定
			for (int i = 0; i < 5; i++) {
				if (getApple[i] == 0 && appleX[i] >= x1 && appleX[i] <= x2 && appleY[i] >= y1 && appleY[i] <= y2) {
					PlaySoundMem(get, DX_PLAYTYPE_BACK);
					score++;
					getApple[i] = 1;
				}
			}

			//重力変動全般
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

			/*ゴール判定*/
			if (x >= WIN_WIDTH * 8) {
				gameScene = RESULT;
			}

			//落下判定
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

			//アイザックアニメーション
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

			/*キャラ反転動作*/
			if (whichGravi == 0 && r > -32 && gravi <= -8) {
				r -= 8;
			}
			if (whichGravi == 1 && r < 32 && gravi >= 8) {
				r += 8;
			}

			/*背景移動*/
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

		// 描画処理
		//背景
		for (int i = 0; i < 4; i++) {
			DrawGraph(backX[i], 0, grBack[i], true);
			DrawGraph(backX[i] + 960, 0, grBack[i], true);
			DrawGraph(backX[i] + 1920, 0, grBack[i], true);
		}
		//マップブロック
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
		//朽ちた看板
		DrawGraph(1600 - worldX, 512, grSign, true);
		DrawGraph(WIN_WIDTH * 8 - 192 - worldX, 512, grExit, true);
		//アイザップル(スコアアイテム)
		for (int i = 0; i < 5; i++) {
			if (getApple[i] == 0) {
				DrawGraph(appleX[i] - 16 - worldX, appleY[i] - 16, grIsapple, true);
			}
		}
		//アイザック
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

		/*デバッグコード */
		//DrawFormatString(400, 0, GetColor(255, 255, 255), "x:%d y:%d appleX:%d appleY:%d", x, y, appleX[0], appleY[0]);
		//DrawFormatString(400, 20, GetColor(255, 255, 255), "x1:%d y1:%d x2:%d y2:%d", x1, y1, x2, y2);

		DrawGraph(0, 0, grPinhole, true);

		if (gameScene == TITLE) {
			DrawGraph(0, 0, grTitle[drawTitle], true);
		}
		if (gameScene == RESULT) {
			DrawGraph(0, 0, grClear, true);
		}
		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}