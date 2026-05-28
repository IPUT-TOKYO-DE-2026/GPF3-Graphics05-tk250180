#include "FrameBufferEmulator.h"


//  buff フレームバッファの先頭アドレス
//  width, height フレームバッファの高さと横幅
//  radius 円の半径
//  centerX, centerY 中心座標(X, Y)
//  color 描画色（B,G,Rの配列）

void drawFilledCircle(unsigned char* buff, int width, int height, int radius, int centerX, int centerY, unsigned char color[3])
{
	const int squaredRadius = radius * radius; // 半径の二乗
	for (int y = 0; y < height; y++) { // 縦方向のループ
		int squaredY = y - centerY;	// Y軸の中心からの距離
		squaredY *= squaredY;  // 二乗しておく
		for (int x = 0; x < width; x++) { // 横方向のループ
			int squaredX = x - centerX;	// X軸の中心からの距離
			squaredX *= squaredX;  // 二乗しておく
			if (squaredY + squaredX <= squaredRadius) { // 距離が半径以下ならば（二乗どうしで比較）
				// 現在のX,Yで示す位置は円の内側として色を置く
				*buff++ = color[0];  // B
				*buff++ = color[1];  // G
				*buff++ = color[2];  // R
			}
			else {
				buff += 3;	// 現在のX,Yで示す位置は円の外側（色は置かず次のピクセルに移る）
			}
		}
	}
}

void drawFilledSquare(unsigned char* buff, int width, int height, int halfpitch, int centerX, int centerY, unsigned char color[3])
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (centerX - halfpitch < x && x < centerX + halfpitch && centerY - halfpitch < y && y < centerY + halfpitch)
			{
				*buff++ = color[0];  // B
				*buff++ = color[1];  // G
				*buff++ = color[2];  // R
			}
			else
			{
				buff += 3;	// 現在のX,Yで示す位置は円の外側（色は置かず次のピクセルに移る）
			}
		}
	}
}

int centerX; // 円の中心座標X
int centerY; // 円の中心座標Y
int radius;  // 円の半径

// 初期化処理（最初に1回だけ呼び出される）
void FrameBufferEmulator::initUser()
{
	// フレームバッファの中心座標を求める
	centerX = width / 2;
	centerY = height / 2;
	radius = 100; // 初期の半径
}


int Block[100] = { 1, 3, 1, 4, 2, 4 };

void sortblock1()
{
	int add = 0;
	// ブロックを一つずつ引いて最後に足す
	for (int i = 0; i < 100; i++)
	{
		if (Block[i] > 0)
		{
			Block[i]--;
			add++;
		}
		else
		{
			Block[i] = add;
			break;
		}
	}

}

void sortblock2()
{
	int nextBlock[100] = {};
	// 次のブロックの配置を計算
	int j = 0;
	for (int i = 0; i < 100; i++)
	{
		if (Block[i] > 0)
		{
			nextBlock[j] = Block[i];
			j++;
		}
	}

	// もともとの変数に代入する
	for (int i = 0; i < 100; i++)
	{
		Block[i] = nextBlock[i];
	}
}

bool sortcheck()
{
	bool isSortfinish = true;

	// 並びが正しいか判断する
	for (int i = 0; i < 100; i++)
	{
		if (Block[i] == 0)
		{
			break;
		}
		else if (Block[i] != i + 1)
		{
			isSortfinish = false;
			break;
		}
	}

	return isSortfinish;
}

bool isSortfinish = false;
int Mystate = 1;

// 描画処理（毎フレーム呼び出される）
void FrameBufferEmulator::drawUser(unsigned char* buff, int mode, int keyLevel, int keyTrigger)
{
	unsigned char color[3] = { 10, 200, 0 }; // B, G, R

	if (!isSortfinish && keyTrigger == SDLK_RIGHT)
	{
		if (Mystate == 1) {
			sortblock1();
			Mystate = 2;
		}
		else if (Mystate == 2) {
			sortblock2();
			Mystate = 1;
			isSortfinish = sortcheck();
		}
	}

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < Block[i]; j++)
		{
			drawFilledSquare(buff, width, height, 18, i * 40 + 20, j * 40 + 100, color); // 円を描画する
		}
	}
}
