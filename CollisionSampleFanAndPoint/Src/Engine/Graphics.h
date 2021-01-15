/**
* @file Graphics.h
* @brief 描画に関する関数、定数の宣言
*/
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../Utility/Vec.h"

/** @brief フォントカラー */
enum FontColor
{
	Black,			//!< 黒
	White,			//!< 白
	Red,			//!< 赤
};

// @brief フォントサイズ */
enum FontSize
{
	Small,			//!< 小
	Regular,		//!< 中
	Large,			//!< 大
	FontSizeMax,	//!< サイズ最大数
};

const int SmallFontSize = 16;	//!< フォントサイズ(小)
const int RegularFontSize = 24;	//!< フォントサイズ(中)
const int LargeFontSize = 32;	//!< フォントサイズ(大)

/**
* @brief Graphics機能初期化関数@n
* 描画関連を使用可能にするための初期化を行う@n
* ※Engine.cppのInitEngineで実行しているので@n
* 　使用者は自分たちで使う必要はない
* @return 初期化結果 (trueが成功)
*/
bool InitGraphics();

/**
* @brief Graphics機能終了関数@n
* 描画関連の処理の後始末を行う処理@n
* ※Engine.cppのEndEngineで実行しているので@n
* 　使用者は自分たちで使う必要はない
*/
void EndGraphics();

/**
* @brief 描画開始関数@n
* 描画開始を宣言し、バックバッファのクリアを行う
* @return 描画開始の成否 (成功はtrue)
*/
bool DrawStart();

/**
* @brief 描画終了関数@n
* 描画の終了を宣言し、バックバッファとフロントバッファを入れ替える@n
* ※この関数は必ずDrawStartの後に実行されるようにしてください。
*/
void DrawEnd();

void DrawCircle(Vec2 center_pos, float radius, DWORD color);

void DrawFan(Vec2 center_pos, float arc_degree, float length, float rotate_degree, DWORD color);

void DrawLine(Vec2 start_pos, Vec2 end_pos, DWORD color);
/**
* @brief フォント描画関数@n
* 指定された位置にフォントを描画する@n
* @param[in] x X軸描画座標
* @param[in] y Y軸描画座標
* @param[in] text 描画するテキスト
* @param[in] size フォントサイズの種類
* @param[in] color フォントの色
*/
void DrawFont(float pos_x, float pos_y, const char* text, FontSize size, FontColor color);

#endif
