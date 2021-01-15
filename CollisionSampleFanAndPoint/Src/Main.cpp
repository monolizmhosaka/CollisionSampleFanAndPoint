#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Engine/Graphics.h"
#include "Engine/Input.h"
#include "Utility/Vec.h"

//DirectXを使用してます。 ↓インストール方法
//https://qiita.com/nisoka/items/f501a6cc7bc3636611c4
//S1023エラーは、このサイトだと「Microsoft Visual C++ 20xx Redistributable XXX をかたっぱしからアンインストール」
//と書いてあるが、microsoftの公式では2010系だけアンインストールすればOKとの記述なのでそちらに従うこと。
//
//https://docs.microsoft.com/ja-jp/troubleshoot/windows/win32/s1023-error-when-you-install-directx-sdk
//
//DirectXを入れると違うバージョンが入るので、必要なら2010の最新バージョンを入れてください。

// 点
struct Point
{
	Vec2 Position;
	float Radius;
};

// 扇
struct Fan
{
	Vec2 Position;
	float FanDegree;
	float Length;
	float RotateDegree;
};

// 扇と点の判定
bool IsCollidingFanAndPoint(Fan fan, Vec2 point)
{
	// 点と扇のベクトル
	Vec2 vec_fan_to_point = {
		point.X - fan.Position.X,
		point.Y - fan.Position.Y
	};

	// ベクトルの長さ算出
	float vec_length = sqrtf((vec_fan_to_point.X * vec_fan_to_point.X) + (vec_fan_to_point.Y * vec_fan_to_point.Y));

	// ベクトルと扇の距離の比較
	if (fan.Length < vec_length)
	{
		// 当たっていない
		return false;
	}

	// 円弧の方向ベクトルを求める
	float rotate_rad = D3DXToRadian(fan.RotateDegree);
	Vec2 fan_dir =
	{
		1.0f,
		0.0f
	};

	// ベクトルを回転させる
	Vec2 rotate_fan_dir
	{ 
		fan_dir.X * cosf(rotate_rad) + fan_dir.Y * -sinf(rotate_rad),
		fan_dir.X * sinf(rotate_rad) + fan_dir.Y * cosf(rotate_rad)
	};

	// 扇と点のベクトルを単位ベクトルにする
	Vec2 normal_fan_to_point = {
		vec_fan_to_point.X / vec_length,
		vec_fan_to_point.Y / vec_length
	};

	// 内積計算
	float dot = normal_fan_to_point.X* rotate_fan_dir.X + normal_fan_to_point.Y * rotate_fan_dir.Y;

	// 扇の範囲をcosにする
	float fan_cos = cosf(D3DXToRadian(fan.FanDegree / 2.0f));

	// 点が扇の範囲内にあるかを比較する
	if (fan_cos > dot)
	{
		// 当たってない
		return false;
	}

	return true;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
	// エンジンの初期化
	if (InitEngine(WINDOW_WIDTH, WINDOW_HEIGHT, "扇と点の当たり判定") == false)
	{
		return 0;
	}

	int frame = 0;

	// 点
	Point point =
	{
		Vec2(80.0f, 150.0f),
		5.0f
	};

	// 扇
	Fan fan =
	{
		Vec2(200.0f, 200.0f),
		270.0f,
		100.0f
	};

	float rad = 0.0f;

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			float speed = 1.0f;
			UpdateInput();

			if (OnKeyPush(LEFT_KEY))
			{
				point.Position.X -= 2.0f;
			}
			else if (OnKeyPush(RIGHT_KEY))
			{
				point.Position.X += 2.0f;
			}

			if (OnKeyPush(UP_KEY))
			{
				point.Position.Y -= 2.0f;
			}
			else if (OnKeyPush(DOWN_KEY))
			{
				point.Position.Y += 2.0f;
			}

			if (OnKeyPush(A_KEY))
			{
				fan.FanDegree -= 2.0f;
			}
			else if (OnKeyPush(D_KEY))
			{
				fan.FanDegree += 2.0f;
			}

			if (fan.FanDegree < 0.0f)
			{
				fan.FanDegree = 0.0f;
			}
			else if (fan.FanDegree > 360.0f)
			{
				fan.FanDegree = 360.0f;
			}

			fan.RotateDegree += 1.0f;

			DrawStart();

			DrawFan(fan.Position, fan.FanDegree, fan.Length, fan.RotateDegree, 0xffffffff);

			// 扇と点の当たり判定実行
			bool is_colliding = IsCollidingFanAndPoint(fan, point.Position);
			DWORD color = 0xffffffff;

			if (is_colliding == true)
			{
				DrawFont(0.0f, 0.0f, "当たってる", FontSize::Regular, FontColor::Red);
				color = 0xffff0000;
			}
			else
			{
				DrawFont(0.0f, 0.0f, "当たってない", FontSize::Regular, FontColor::Red);
			}

			DrawCircle(point.Position, point.Radius, color);

			DrawEnd();
		}
	}

	// エンジン終了
	EndEngine();

	return 0;
}

