#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <math.h>
#include "Graphics.h"
#include "Window.h"
#include "../Utility/Size.h"

// 静的ライブラリ
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

/*
	staticをグローバル変数に指定すると
	別のcppからのアクセスができなくなる

		メリット
			無駄なアクセスを減らせる

	staticとつける => さわるなを明示してる
*/

// グローバル変数
static LPDIRECT3D9 g_D3DInterface;						// DirectGraphicsインターフェース
static LPDIRECT3DDEVICE9 g_D3DDevice;					// DirectGraphicsデバイス
static LPD3DXFONT g_FontList[FontSize::FontSizeMax];	// フォントデバイスリスト

// インターフェースの作成
bool CreateGraphicsInterface();

// グラフィックデバイスの作成
bool CreateGraphicsDevice(D3DPRESENT_PARAMETERS* present_param);

// ViewPortの設定
bool SetUpViewPort(D3DPRESENT_PARAMETERS* present_param);

// フォントデバイスの生成
bool CreateFontDevice();

bool InitGraphics()
{
	D3DPRESENT_PARAMETERS present_param;
	ZeroMemory(&present_param, sizeof(D3DPRESENT_PARAMETERS));

	if (CreateGraphicsInterface() == false)
	{
		return false;
	}

	if (CreateGraphicsDevice(&present_param) == false)
	{
		return false;
	}

	if (SetUpViewPort(&present_param) == false)
	{
		return false;
	}

	if (CreateFontDevice() == false)
	{
		return false;
	}

	return true;
}

void EndGraphics()
{
	for (int i = 0; i < FontSize::FontSizeMax; i++)
	{
		g_FontList[i]->Release();
	}
	g_D3DDevice->Release();
	g_D3DInterface->Release();
}

bool DrawStart()
{
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);

	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (D3D_OK == g_D3DDevice->BeginScene())
	{
		return true;
	}

	return false;
}

void DrawEnd()
{
	g_D3DDevice->EndScene();
	g_D3DDevice->Present(NULL, NULL, NULL, NULL);
}

void DrawCircle(Vec2 center_pos, float radius, DWORD color)
{
	struct CircleVertex
	{
		CircleVertex(float x, float y, DWORD color) :
			X(x),
			Y(y),
			Z(0.0f),
			W(1.0f),
			Color(color)
		{
		}

		float X;
		float Y;
		float Z;
		float W;
		DWORD Color;
	};

	std::vector<CircleVertex> vertices;
	vertices.emplace_back(center_pos.X, center_pos.Y, color);

	float pos_x = 0.0f;
	float pos_y = 0.0f;

	for (int i = 0; i < 360; i++)
	{
		pos_x = cosf(D3DXToRadian(i * 2.0f)) * radius + center_pos.X;
		pos_y = sinf(D3DXToRadian(i * 2.0f)) * radius + center_pos.Y;

		vertices.emplace_back(pos_x, pos_y, color);
	}

	g_D3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	g_D3DDevice->SetTexture(0, nullptr);
	g_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertices.size() - 2, &vertices[0], sizeof(CircleVertex));
}

void DrawFan(Vec2 center_pos, float arc_degree, float length, float rotate_degree, DWORD color)
{
	struct CircleVertex
	{
		CircleVertex(float x, float y, DWORD color) :
			X(x),
			Y(y),
			Z(0.0f),
			W(1.0f),
			Color(color)
		{
		}

		float X;
		float Y;
		float Z;
		float W;
		DWORD Color;
	};

	std::vector<CircleVertex> vertices;
	vertices.emplace_back(center_pos.X, center_pos.Y, color);

	float pos_x = 0.0f;
	float pos_y = 0.0f;

	Vec2 rotate_pos;

	float angle = arc_degree / 180.0f;
	float start_angle = -arc_degree / 2.0f;
	float rotate_rad = D3DXToRadian(rotate_degree);
	for (int i = 0; i <= 180; i++)
	{
		pos_x = cosf(D3DXToRadian(start_angle + i * angle)) * length;
		pos_y = sinf(D3DXToRadian(start_angle + i * angle)) * length;
		
		rotate_pos.X = pos_x * cosf(rotate_rad) + pos_y * -sinf(rotate_rad) + center_pos.X;
		rotate_pos.Y = pos_x * sinf(rotate_rad) + pos_y * cosf(rotate_rad) + center_pos.Y;
		vertices.emplace_back(rotate_pos.X, rotate_pos.Y, color);
	}

	g_D3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	g_D3DDevice->SetTexture(0, nullptr);
	g_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertices.size() - 2, &vertices[0], sizeof(CircleVertex));
}

void DrawLine(Vec2 start_pos, Vec2 end_pos, DWORD color)
{
	struct LineVertex
	{
		LineVertex(float x, float y, DWORD color) :
			X(x),
			Y(y),
			Z(0.0f),
			W(1.0f),
			Color(color)
		{
		}

		float X;
		float Y;
		float Z;
		float W;
		DWORD Color;
	};

	LineVertex vertices[2] = 
	{
		LineVertex(start_pos.X, start_pos.Y, color),
		LineVertex(end_pos.X, end_pos.Y, color),
	};

	g_D3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	g_D3DDevice->SetTexture(0, nullptr);
	g_D3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, vertices, sizeof(LineVertex));
}

void DrawFont(float pos_x, float pos_y, const char* text, FontSize font_type, FontColor color)
{
	RECT rect =
	{
		(long)pos_x,
		(long)pos_y,
		(long)pos_x + 400,
		(long)pos_y + 200,
	};

	int r, g, b;
	r = g = b = 255;

	if (color == FontColor::Black)
	{
		r = g = b = 0;
	}
	else if (color == FontColor::Red)
	{
		r = 255;
		g = b = 0;
	}

	g_FontList[font_type]->DrawTextA(
		NULL,
		text,
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_XRGB(r, g, b)
	);
}

bool CreateGraphicsInterface()
{
	// インターフェース作成
	g_D3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_D3DInterface == NULL)
	{
		// 作成失敗
		return false;
	}

	// 作成成功
	return true;
}

bool CreateGraphicsDevice(D3DPRESENT_PARAMETERS* present_param)
{
	// バックバッファの数 => 1
	present_param->BackBufferCount = 1;
	// バックバッファのフォーマット => D3DFMT_UNKNOWN(フォーマットを知りません)
	present_param->BackBufferFormat = D3DFMT_UNKNOWN;
	// ウィンドウモード設定 => 定数で切り替え
	present_param->Windowed = true;

	// スワップエフェクト => バックバッファとフロントバッファへの切り替え方法
	present_param->SwapEffect = D3DSWAPEFFECT_DISCARD;

	// DirectDeviceの作成
	if (FAILED(g_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		FindWindowA(WINDOW_CLASS_NAME, nullptr),
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		present_param,
		&g_D3DDevice)))
	{
		return false;
	}

	return true;
}

bool SetUpViewPort(D3DPRESENT_PARAMETERS* present_param)
{
	// ビューポートパラメータ
	D3DVIEWPORT9 view_port;

	// ビューポートの左上座標
	view_port.X = 0;
	view_port.Y = 0;
	// ビューポートの幅
	view_port.Width = present_param->BackBufferWidth;
	// ビューポートの高さ
	view_port.Height = present_param->BackBufferHeight;
	// ビューポート深度設定
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	// ビューポート設定
	if (FAILED(g_D3DDevice->SetViewport(&view_port)))
	{
		return false;
	}

	return true;
}

bool CreateFontDevice()
{
	int size_list[] =
	{
		SmallFontSize,
		RegularFontSize,
		LargeFontSize
	};

	for (int i = 0; i < FontSize::FontSizeMax; i++)
	{
		if (FAILED(D3DXCreateFont(g_D3DDevice,
			size_list[i],
			size_list[i] / 2,
			FW_REGULAR,
			NULL,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			PROOF_QUALITY,
			FIXED_PITCH | FF_SCRIPT,
			TEXT("ＭＳ　Ｐゴシック"),
			&g_FontList[i])))
		{
			return false;
		}
	}

	return true;
}
