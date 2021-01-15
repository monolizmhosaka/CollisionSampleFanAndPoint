#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include "../Utility/Vec.h"
#include "Window.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// キー情報構造体
struct IputState
{
	DWORD current;
	DWORD trg;
	DWORD ntrg;
};

static LPDIRECTINPUT8 g_InputInterface;			//!< DIRECTINPUT8のポインタ
static LPDIRECTINPUTDEVICE8 g_KeyboardDevice;	//!< DIRECTINPUTDEVICE8のポインタ
static IputState g_InputState;

// 入力インターフェースの作成
bool CreateInputInterface();

// キーボードデバイスの作成
bool CreateKeyBoardDevice();


#define MOUSE_ON_VALUE (0x80)		// オン状態の値

bool InitInput()
{
	if (CreateInputInterface() == false)
	{
		return false;
	}

	if (CreateKeyBoardDevice() == false)
	{
		EndInput();
		return false;
	}

	return true;
}

void EndInput()
{
	if (g_KeyboardDevice != nullptr)
	{
		g_KeyboardDevice->Release();
		g_KeyboardDevice = nullptr;
	}

	if (g_InputInterface != nullptr)
	{
		g_InputInterface->Release();
		g_InputInterface = nullptr;
	}
}

bool OnKeyDown(DWORD key_type)
{
	return (g_InputState.trg & key_type) != 0 ? true : false;
}

bool OnKeyPush(DWORD key_type)
{
	return (g_InputState.current & key_type) != 0 ? true : false;
}

bool OnKeyUp(DWORD key_type)
{
	return (g_InputState.ntrg & key_type) != 0 ? true : false;
}

void UpdateInput()
{
	// キー情報取格納用
	BYTE key_state[256];
	HRESULT hr;

	// キーボードデバイスのゲッター
	hr = g_KeyboardDevice->GetDeviceState(256, key_state);
	if (SUCCEEDED(hr))
	{
		// 1フレーム前のキー情報の確保
		DWORD old = g_InputState.current;

		// キー情報クリア
		g_InputState.current = CLEAR_KEY;

		// 上キー
		if (key_state[DIK_UP] & 0x80)
		{
			g_InputState.current |= UP_KEY;
		}

		// 下キー
		if (key_state[DIK_DOWN] & 0x80)
		{
			g_InputState.current |= DOWN_KEY;
		}

		// 左キー
		if (key_state[DIK_LEFT] & 0x80)
		{
			g_InputState.current |= LEFT_KEY;
		}

		// 右キー
		if (key_state[DIK_RIGHT] & 0x80)
		{
			g_InputState.current |= RIGHT_KEY;
		}

		// リターンキー
		if (key_state[DIK_RETURN] & 0x80)
		{
			g_InputState.current |= RETURN_KEY;
		}

		// リターンキー
		if (key_state[DIK_A] & 0x80)
		{
			g_InputState.current |= A_KEY;
		}

		// リターンキー
		if (key_state[DIK_D] & 0x80)
		{
			g_InputState.current |= D_KEY;
		}

		g_InputState.trg = (g_InputState.current & (~old));	// トリガー情報取得
		g_InputState.ntrg = (~g_InputState.current) & old;	// 逆トリガー情報取得

	}
	else if (hr == DIERR_INPUTLOST) 
	{
		g_KeyboardDevice->Acquire();
	}
}


bool CreateInputInterface()
{
	HRESULT ret = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)& g_InputInterface, nullptr);
	if (FAILED(ret))
	{
		return false;	// 作成に失敗
	}

	return true;
}

bool CreateKeyBoardDevice()
{
	// IDirectInputDevice8インターフェイスの取得
	HRESULT hr = g_InputInterface->CreateDevice(GUID_SysKeyboard, &g_KeyboardDevice, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	// デバイスのフォーマットの設定
	hr = g_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}

	// 協調モードの設定
	hr = g_KeyboardDevice->SetCooperativeLevel(FindWindow(WINDOW_CLASS_NAME, nullptr), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}

	// デバイスの取得開始
	g_KeyboardDevice->Acquire();

	return true;
}
