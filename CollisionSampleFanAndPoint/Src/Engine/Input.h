/**
* @file Input.h
* @brief 入力に関する関数、定数の宣言
*/
#ifndef INPUT_H_
#define INPUT_H_

#include <Windows.h>
#include "../Utility/Vec.h"

// キー情報GetKeyシリーズの関数で使用する
#define CLEAR_KEY	0x0000		// キークリア用
#define UP_KEY		0x0001		// 上キー
#define DOWN_KEY	0x0002		// 下キー
#define LEFT_KEY	0x0004		// 左キー
#define RIGHT_KEY	0x0008		// 右キー
#define RETURN_KEY	0x0010		// リターンキー
#define A_KEY		0x0020		// Aキー
#define D_KEY		0x0040		// Dキー

/**
* @brief Input機能初期化関数@n
* デバイスの入力取得に必要な初期化を行う
* ※InitEngineで実行されるので開発側が実行する必要はない
* @return 初期化結果(成功はtrue)
*/
bool InitInput();

/**
* @brief Input機能終了関数@n
* Input機能を終了させる@n
* ※EndEngineで実行されるので開発側が実行する必要はない
*/
void EndInput();

/**
* @brief 入力情報の更新@n
* デバイスの入力情報の更新を行う@n
* 毎フレームに１度必ず実行する必要がある
*/
void UpdateInput();

/**
* @brief クリックした瞬間判定関数@n
* 指定したマウスのボタンがクリックされた瞬間かどうかを判定する
* @return 判定結果(押された瞬間ならtrue)
* @param[in] button_type 判定したいボタンの種類
*/
bool OnKeyDown(DWORD key_type);

/**
* @brief クリック中判定関数@n
* 指定したマウスのボタンがクリック中かどうかを判定する
* @return 判定結果(クリック中瞬間ならtrue)
* @param[in] button_type 判定したいボタンの種類
*/
bool OnKeyPush(DWORD key_type);

/**
* @brief クリック終了判定関数@n
* 指定したマウスのボタンがクリックを終了した瞬間かどうかを判定する
* @return 判定結果(クリックをやめた瞬間ならtrue)
* @param[in] button_type 判定したいボタンの種類
*/
bool OnKeyUp(DWORD key_type);

#endif
