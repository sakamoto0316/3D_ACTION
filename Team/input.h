//========================================================================================
//
//　入力デバイスの処理[input.h]
//	Author:sakamoto kai
//
//========================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"

#pragma comment(lib,"xinput.lib")	//入力処理

//マクロ定義
#define NUM_KEY_MAX (256)	//キーの最大数
#define MAX_PLAYER (4)	//プレイヤーの人数

//入力のクラス宣言
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 pDevice;
};

//キーボードのクラス宣言
class CInputKeyboard :public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRerease(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRerease[NUM_KEY_MAX];

};

//ジョイパッドのクラス宣言
class CInputJoypad :public CInput
{
public:

	//列挙型定義
	typedef enum
	{
		BUTTON_UP = 0,	//十字キーの上ボタン
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_START,
		BUTTON_BACK,
		BUTTON_L_PUSH,
		BUTTON_R_PUSH,
		BUTTON_L,
		BUTTON_R,
		BUTTON_NAZO1,
		BUTTON_NAZO2,
		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_MAX
	}JOYKEY;

	typedef enum
	{
		LSTICK_UP = 0,
		LSTICK_DOWN,
		LSTICK_RIGHT,
		LSTICK_LEFT,
		LSTICK_MAX,
	}JOYPAD_LSTICK;

	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(JOYKEY key, int nPlayer);
	bool GetTrigger(JOYKEY key, int nPlayer);
	bool GetRelease(JOYKEY key, int nPlayer);

	D3DXVECTOR3 Get_Stick_Left(int nPlayer);
	//D3DXVECTOR3 Get_Stick_Right(int nPlayer);
	//float Get_Trigger_Left(int nPlayer);
	//float Get_Trigger_Right(int nPlayer);

	bool Get_LStick_Trigger(JOYPAD_LSTICK Type, int nPlayer);
	//void Get_Vibrtion(int nPlayer, int nLeftPower, int RightPoewr);
	//void Get_Vibrtion_false(int nPlayer);

private:
	DIJOYSTATE m_aKeyState; //全入力情報の保管
	DIJOYSTATE m_aKeyStateTrigger; //トリガー情報
	DIJOYSTATE m_aKeyStateRerease; //リリース情報
	DIJOYSTATE m_aKeyStateRepeat; //リピート情報
	BYTE m_aOldState[32]; //前回の入力情報を保存

	JOYPAD_LSTICK m_LStickInput;
	bool m_OutputDOK;
	bool m_OutputUOK;
	bool m_OutputLOK;
	bool m_OutputROK;

	XINPUT_STATE m_JoyKeyState[MAX_PLAYER];
	XINPUT_STATE m_JoyKeyStateTrigger[MAX_PLAYER];
	XINPUT_STATE m_JoyKeyStateRelease[MAX_PLAYER];
	XINPUT_VIBRATION m_vibration[MAX_PLAYER];
};

#endif
