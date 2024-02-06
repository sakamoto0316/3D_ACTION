//============================================
//
//	マネージャ [manager.cpp]
//	Author:sakamoto kai
//
//============================================
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "objectX.h"
#include "texture.h"
#include "XModel.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "Pause.h"
#include "sound.h"

#define SET_MODE (CScene::MODE_TITLE)

//静的メンバ変数宣言
CManager *CManager::pManager = NULL;
CScene::MODE CScene::m_mode = SET_MODE;

//====================================================================
//コンストラクタ
//====================================================================
CManager::CManager()
{
	//静的メンバ変数宣言
	m_bEdit = false;
	m_bStop = false;
	m_SetTutorial = false;
	m_Pause = false;
	m_SetScoreResult = false;
	m_SetJoyPad = false;
	m_PauseOK = false;
	m_bPad = false;
	m_EndScore = 157;

	pManager = NULL;
	m_pRenderer = NULL;
	m_pDebugProc = NULL;
	m_pInputKeyboard = NULL;
	m_pInputJoyPad = NULL;
	m_pInputMouse = NULL;
	m_pCamera = NULL;
	m_pLight = NULL;
	m_pTexture = NULL;
	m_pXModel = NULL;
	m_pScene = NULL;
	m_LevelUP = NULL;
	m_Fade = NULL;
	m_pSound = NULL;
}

//====================================================================
//デストラクタ
//====================================================================
CManager::~CManager()
{

}

//====================================================================
//初期化処理
//====================================================================
CManager *CManager::GetInstance(void)
{
	if (pManager == NULL)
	{
		pManager = new CManager;
	}
	return pManager;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer == NULL)
	{
		//レンダラーの生成
		m_pRenderer = new CRenderer;
	}

	//レンダラーの初期化処理
	if (FAILED(m_pRenderer->Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pDebugProc == NULL)
	{
		//デバッグ表示の生成
		m_pDebugProc = new CDebugProc;

		if (m_pDebugProc != NULL)
		{
			m_pDebugProc->Init();
		}
	}

	if (m_pSound == NULL)
	{
		//サウンドの生成
		m_pSound = new CSound;
	}

	//サウンドの初期化処理
	if (FAILED(m_pSound->InitSound(hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pInputKeyboard == NULL)
	{
		//キーボードの生成
		m_pInputKeyboard = new CInputKeyboard;
	}
	//キーボードの初期化処理
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pInputJoyPad == NULL)
	{
		//ジョイパッドの生成
		m_pInputJoyPad = new CInputJoypad;
	}
	//ジョイパッドの初期化処理
	if (FAILED(m_pInputJoyPad->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pInputMouse == NULL)
	{
		//マウスの生成
		m_pInputMouse = new CInputMouse;
	}
	//キーボードの初期化処理
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pCamera == NULL)
	{
		//カメラの生成
		m_pCamera = new CCamera;
	}

	//カメラの初期化処理
	if (FAILED(m_pCamera->Init()))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pLight == NULL)
	{
		//ライトの生成
		m_pLight = new CLight;
	}

	//ライトの初期化処理
	if (FAILED(m_pLight->Init()))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pTexture == NULL)
	{
		//テクスチャの生成
		m_pTexture = new CTexture;
	}

	//全てのテクスチャの読み込み
	if (FAILED(m_pTexture->Load()))
	{//読み込みが失敗した場合
		return E_FAIL;
	}

	if (m_pXModel == NULL)
	{
		//Xモデルの生成
		m_pXModel = new CXModel;
	}

	//全てのXモデルの読み込み
	if (FAILED(m_pXModel->Load()))
	{//読み込みが失敗した場合
		return E_FAIL;
	}

	if (m_pScene == NULL)
	{
		m_pScene = CScene::Create(SET_MODE);
	}

	if (m_Fade == NULL)
	{
		//フェードの生成
		m_Fade = new CFade;

		if (m_Fade != NULL)
		{
			m_Fade->Init(SET_MODE);
		}
	}

	m_PauseOK = true;

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CManager::Uninit(void)
{
	//BGMの停止
	m_pSound->StopSound();

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	if (m_Fade != NULL)
	{
		//フェードの終了処理
		m_Fade->Uninit();

		delete m_Fade;
		m_Fade = NULL;
	}

	if (m_pScene != NULL)
	{
		//シーンの終了処理
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = NULL;
	}

	if (m_pTexture != NULL)
	{
		//テクスチャの終了処理
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = NULL;
	}

	if (m_pXModel != NULL)
	{
		//Xモデルの終了処理
		m_pXModel->Unload();

		delete m_pXModel;
		m_pXModel = NULL;
	}

	if (m_pLight != NULL)
	{
		//ライトの終了処理
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pCamera != NULL)
	{
		//カメラの終了処理
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pInputMouse != NULL)
	{
		//ジョイパッドの終了処理
		m_pInputMouse->Uninit();

		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	if (m_pInputJoyPad != NULL)
	{
		//ジョイパッドの終了処理
		m_pInputJoyPad->Uninit();

		delete m_pInputJoyPad;
		m_pInputJoyPad = NULL;
	}

	if (m_pInputKeyboard != NULL)
	{
		//キーボードの終了処理
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (m_pDebugProc != NULL)
	{
		//デバッグ表示の終了処理
		m_pDebugProc->Uninit();

		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	if (m_pRenderer != NULL)
	{
		//レンダラーの終了処理
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (m_pSound != NULL)
	{
		//サウンドの終了処理
		m_pSound->UninitSound();

		delete m_pSound;
		m_pSound = NULL;
	}
}

//====================================================================
//更新処理
//====================================================================
void CManager::Update(void)
{
	//カメラの更新処理
	m_pCamera->Update();

	//ライトの更新処理
	m_pLight->Update();

	//キーボードの更新処理
	m_pInputKeyboard->Update();

	//ジョイパッドの更新処理
	m_pInputJoyPad->Update();

	//マウスの更新処理
	m_pInputMouse->Update();

	//デバッグ表示の更新処理
	m_pDebugProc->Update();

	//デバッグ表示
	m_pDebugProc->Print("FPS : %d\n", GetFps());
	m_pDebugProc->Print("移動 ([A:←] [D:→])\n");
	m_pDebugProc->Print("ジャンプ [W]\n");
	m_pDebugProc->Print("弾発射 [SPACE]\n");
	m_pDebugProc->Print("リセット処理 [R]\n");
	m_pDebugProc->Print("弾発射方向調整 ([Q:←] [E:→])\n");

#ifdef _DEBUG

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F2) == true &&
		CScene::GetMode() == CScene::MODE_GAME)
	{
		//条件？ 処理１：処理２;
		m_bEdit = m_bEdit ? false : true;
		CObject::DeleteBlock();
	}
#endif

	if ((CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_P) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_START, 0) == true) &&
		CScene::GetMode() == CScene::MODE_GAME &&
		m_PauseOK == true)
	{
		//条件？ 処理１：処理２;
		m_Pause = m_Pause ? false : true;

		if (m_Pause == true)
		{
			CGame::GetPause()->SetAppear(true);
		}
		else
		{
			CGame::GetPause()->SetAppear(false);
		}
	}

	//シーンの更新処理
	m_pScene->Update();

	//レンダラーの更新処理
	m_pRenderer->Update();

	//フェードの更新処理
	m_Fade->Update();
}

//====================================================================
//描画処理
//====================================================================
void CManager::Draw(void)
{
	//レンダラーの描画処理
	m_pRenderer->Draw();
}

//====================================================================
//生成処理
//====================================================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = NULL;

	if (pScene == NULL)
	{
		//シーンの生成
		switch (mode)
		{
		case MODE_TITLE:
			pScene = new CTitle();
			break;
		case MODE_GAME:
			pScene = new CGame();
			break;
		case MODE_RESULT:
			pScene = new CResult();
			break;
		}
	}

	CManager::GetInstance()->GetInstance()->GetCamera()->ResetCamera();

	CManager::GetInstance()->GetInstance()->SetEdit(false);

	m_mode = mode;

	if (m_mode == MODE_GAME)
	{

	}

	//オブジェクトの初期化処理
	if (FAILED(pScene->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pScene;
}

//====================================================================
//コンストラクタ
//====================================================================
CScene::CScene()
{

}

//====================================================================
//デストラクタ
//====================================================================
CScene::~CScene()
{
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CScene::Init(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Init();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CScene::Uninit(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CScene::Update(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Update();
}

//====================================================================
//描画処理
//====================================================================
void CScene::Draw(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Draw();
}

//====================================================================
//モード設定処理
//====================================================================
void CScene::SetMode(MODE mode)
{
	CScene *pScene = CManager::GetInstance()->GetScene();
	CManager::GetInstance()->GetSound()->StopSound();

	if (pScene != NULL)
	{
		//シーンの終了処理
		pScene->Uninit();

		delete pScene;
		pScene = NULL;
	}

	//モードの生成
	pScene = Create(mode);

	CManager::GetInstance()->GetInstance()->SetScene(pScene);
}