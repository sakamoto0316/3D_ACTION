//============================================
//
//	レンダラー[renderer.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RENSERER_H_
#define _RENSERER_H_

#include "main.h"

//レンダラークラス
class CRenderer
{
public:
	CRenderer();	//コンストラクタ
	~CRenderer();	//デストラクタ

	//レンダラーの処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); 	//返り値がHRESULTなのは頂点バッファの生成などに失敗したとき確認のため
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

private:

	LPDIRECT3D9 m_pD3D;					//Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		//Direct3Dデバイスへのポインタ
};

#endif