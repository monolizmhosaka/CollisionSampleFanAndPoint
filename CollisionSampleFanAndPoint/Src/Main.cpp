#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Engine/Graphics.h"
#include "Engine/Input.h"
#include "Utility/Vec.h"

//DirectX���g�p���Ă܂��B ���C���X�g�[�����@
//https://qiita.com/nisoka/items/f501a6cc7bc3636611c4
//S1023�G���[�́A���̃T�C�g���ƁuMicrosoft Visual C++ 20xx Redistributable XXX ���������ς�����A���C���X�g�[���v
//�Ə����Ă��邪�Amicrosoft�̌����ł�2010�n�����A���C���X�g�[�������OK�Ƃ̋L�q�Ȃ̂ł�����ɏ]�����ƁB
//
//https://docs.microsoft.com/ja-jp/troubleshoot/windows/win32/s1023-error-when-you-install-directx-sdk
//
//DirectX������ƈႤ�o�[�W����������̂ŁA�K�v�Ȃ�2010�̍ŐV�o�[�W���������Ă��������B

// �_
struct Point
{
	Vec2 Position;
	float Radius;
};

// ��
struct Fan
{
	Vec2 Position;
	float FanDegree;
	float Length;
	float RotateDegree;
};

// ��Ɠ_�̔���
bool IsCollidingFanAndPoint(Fan fan, Vec2 point)
{
	// �_�Ɛ�̃x�N�g��
	Vec2 vec_fan_to_point = {
		point.X - fan.Position.X,
		point.Y - fan.Position.Y
	};

	// �x�N�g���̒����Z�o
	float vec_length = sqrtf((vec_fan_to_point.X * vec_fan_to_point.X) + (vec_fan_to_point.Y * vec_fan_to_point.Y));

	// �x�N�g���Ɛ�̋����̔�r
	if (fan.Length < vec_length)
	{
		// �������Ă��Ȃ�
		return false;
	}

	// �~�ʂ̕����x�N�g�������߂�
	float rotate_rad = D3DXToRadian(fan.RotateDegree);
	Vec2 fan_dir =
	{
		1.0f,
		0.0f
	};

	// �x�N�g������]������
	Vec2 rotate_fan_dir
	{ 
		fan_dir.X * cosf(rotate_rad) + fan_dir.Y * -sinf(rotate_rad),
		fan_dir.X * sinf(rotate_rad) + fan_dir.Y * cosf(rotate_rad)
	};

	// ��Ɠ_�̃x�N�g����P�ʃx�N�g���ɂ���
	Vec2 normal_fan_to_point = {
		vec_fan_to_point.X / vec_length,
		vec_fan_to_point.Y / vec_length
	};

	// ���όv�Z
	float dot = normal_fan_to_point.X* rotate_fan_dir.X + normal_fan_to_point.Y * rotate_fan_dir.Y;

	// ��͈̔͂�cos�ɂ���
	float fan_cos = cosf(D3DXToRadian(fan.FanDegree / 2.0f));

	// �_����͈͓̔��ɂ��邩���r����
	if (fan_cos > dot)
	{
		// �������ĂȂ�
		return false;
	}

	return true;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
	// �G���W���̏�����
	if (InitEngine(WINDOW_WIDTH, WINDOW_HEIGHT, "��Ɠ_�̓����蔻��") == false)
	{
		return 0;
	}

	int frame = 0;

	// �_
	Point point =
	{
		Vec2(80.0f, 150.0f),
		5.0f
	};

	// ��
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

			// ��Ɠ_�̓����蔻����s
			bool is_colliding = IsCollidingFanAndPoint(fan, point.Position);
			DWORD color = 0xffffffff;

			if (is_colliding == true)
			{
				DrawFont(0.0f, 0.0f, "�������Ă�", FontSize::Regular, FontColor::Red);
				color = 0xffff0000;
			}
			else
			{
				DrawFont(0.0f, 0.0f, "�������ĂȂ�", FontSize::Regular, FontColor::Red);
			}

			DrawCircle(point.Position, point.Radius, color);

			DrawEnd();
		}
	}

	// �G���W���I��
	EndEngine();

	return 0;
}

