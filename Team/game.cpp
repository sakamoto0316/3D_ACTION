//============================================
//
//	ゲーム画面 [game.cpp]
//	Author:sakamoto kai
//
//============================================
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "objmeshField.h"
#include "objmeshWall.h"
#include "objmeshCylinder.h"
#include "objmeshDome.h"
#include "CubeBlock.h"
#include "CubeDamage.h"
#include "player.h"
#include "boss.h"
#include "Pause.h"
#include "camera.h"
#include "game.h"
#include "sound.h"
#include "time.h"
#include "debugproc.h"
#include "Xmodel.h"
#include "texture.h"
#include "numberBillboard.h"
#include "numberFall.h"

//静的メンバ変数宣言
CTutorialUI *CGame::m_pTutorialUI = NULL;
CEdit *CGame::m_pEdit = NULL;
CPause *CGame::m_pPause = NULL;
CScore *CGame::m_pScore = NULL;
CTime *CGame::m_pTime = NULL;
CObject2D* CGame::m_p2DSample = NULL;
CObject2D* CGame::m_pEventBG[2] = {};
CObject2D *CGame::m_p2DUI_Attack = NULL;		//攻撃の2DUI
CObject2D *CGame::m_p2DUI_Jump = NULL;			//ジャンプの2DUI
CObject2D *CGame::m_p2DUI_Dodge = NULL;			//回避の2DUI
CObject2D* CGame::m_p2DUI_Attention = NULL;		//注目の2DUI
CObject2D *CGame::m_p2DUI_AttentionOK = NULL;	//注目の2DUI
CObject3D* CGame::m_p3DSample = NULL;
CObject3D *CGame::m_p3DEventBG = NULL;			//イベント時の3D背景
CObjectBillboard* CGame::m_pBillboardSample = NULL;
CObjectX* CGame::m_pXModelSample = NULL;
CObjmeshField* CGame::m_pMeshFieldSample = NULL;
CObjmeshWall* CGame::m_pMeshWallSample = NULL;
CObjmeshCylinder* CGame::m_pMeshCylinderSample = NULL;
CObjmeshDome* CGame::m_pMeshDome = NULL;
CCubeBlock* CGame::m_pCubeBlock = NULL;
CPlayer* CGame::m_pPlayer = NULL;
CBoss*CGame::m_pBoss = NULL;
bool CGame::m_bGameEnd = false;
bool CGame::m_bEvent = false;
bool CGame::m_bEventEnd = false;
int CGame::m_nEventCount = 0;
float CGame::m_EventHeight = 0.0f;

//====================================================================
//コンストラクタ
//====================================================================
CGame::CGame()
{
	m_bGameEnd = false;
	m_bEvent = false;
	m_bEventEnd = false;
	m_EventHeight = 0.0f;
	m_nEventCount = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CGame::~CGame()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CGame::Init(void)
{
	//ゲームのBGMを再生する
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_BOSS);

	//m_p2DSample = CObject2D::Create();
	//m_p2DSample->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	//m_p2DSample->SetWight(100.0f);
	//m_p2DSample->SetHeight(100.0f);
	//m_p2DSample->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	//m_p3DSample = CObject3D::Create();
	//m_p3DSample->SetPos(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	//m_p3DSample->SetRot(D3DXVECTOR3(1.57f, 0.0f, 0.0f));
	//m_p3DSample->SetWight(100.0f);
	//m_p3DSample->SetHeight(100.0f);

	//m_pBillboardSample = CObjectBillboard::Create();
	//m_pBillboardSample->SetPos(INITVECTOR3);

	//m_pXModelSample = CObjectX::Create("data\\MODEL\\player00.x");
	//m_pXModelSample->SetPos(D3DXVECTOR3(-200.0f, 0.0f, 0.0f));

	//m_pMeshFieldSample = CObjmeshField::Create();
	//m_pMeshFieldSample->SetPos(D3DXVECTOR3(0.0f, 0.0f, 200.0f));

	//m_pMeshWallSample = CObjmeshWall::Create();
	//m_pMeshWallSample->SetPos(D3DXVECTOR3(0.0f, 0.0f, 300.0f));

	//m_pMeshCylinderSample = CObjmeshCylinder::Create();

	m_pMeshDome = CObjmeshDome::Create();
	m_pMeshDome->SetTexture("data\\TEXTURE\\SkyBG.jpg");

	m_pMeshDome = CObjmeshDome::Create();
	m_pMeshDome->SetTexture("data\\TEXTURE\\SkyBG.jpg");
	m_pMeshDome->SetRot(D3DXVECTOR3(D3DX_PI, D3DX_PI * 0.9f, 0.0f));

	m_pCubeBlock = CCubeBlock::Create();
	m_pCubeBlock->SetPos(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	m_pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 10.0f, 500.0f));
	m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//m_pCubeBlock = CCubeBlock::Create();
	//m_pCubeBlock->SetPos(D3DXVECTOR3(150.0f, 250.0f, -150.0f));
	//m_pCubeBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
	//m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//m_pCubeBlock = CCubeBlock::Create();
	//m_pCubeBlock->SetPos(D3DXVECTOR3(-150.0f, 250.0f, 150.0f));
	//m_pCubeBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
	//m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 150.0f, -400.0f));

	m_pBoss = CBoss::Create("data\\MODEL\\boss.x");
	m_pBoss->SetPos(D3DXVECTOR3(0.0f, 300.0f, 0.0f));

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_pEventBG[nCnt] = CObject2D::Create();
		m_pEventBG[nCnt]->SetPos(D3DXVECTOR3(640.0f, nCnt * 720.0f, 0.0f));
		m_pEventBG[nCnt]->SetWight(1280.0f);
		m_pEventBG[nCnt]->SetHeight(0.0f);
		m_pEventBG[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_pEventBG[nCnt]->SetTexture("data\\TEXTURE\\Test.jpg");
	}

	m_p2DUI_Attention = CObject2D::Create();
	m_p2DUI_Attention->SetPos(D3DXVECTOR3(130.0f, 600.0f, 0.0f));
	m_p2DUI_Attention->SetWight(150.0f);
	m_p2DUI_Attention->SetHeight(40.0f);
	m_p2DUI_Attention->SetTexture("data\\TEXTURE\\UI_Attention.png");

	m_p2DUI_AttentionOK = CObject2D::Create();
	m_p2DUI_AttentionOK->SetPos(D3DXVECTOR3(130.0f, 650.0f, 0.0f));
	m_p2DUI_AttentionOK->SetWight(100.0f);
	m_p2DUI_AttentionOK->SetHeight(100.0f);
	m_p2DUI_AttentionOK->SetTexture("data\\TEXTURE\\UI_AttentionOFF.png");

	m_p2DUI_Dodge = CObject2D::Create();
	m_p2DUI_Dodge->SetPos(D3DXVECTOR3(1120.0f, 550.0f, 0.0f));
	m_p2DUI_Dodge->SetWight(150.0f);
	m_p2DUI_Dodge->SetHeight(40.0f);
	m_p2DUI_Dodge->SetTexture("data\\TEXTURE\\UI_Dodge.png");

	m_p2DUI_Jump = CObject2D::Create();
	m_p2DUI_Jump->SetPos(D3DXVECTOR3(1170.0f, 600.0f, 0.0f));
	m_p2DUI_Jump->SetWight(150.0f);
	m_p2DUI_Jump->SetHeight(40.0f);
	m_p2DUI_Jump->SetTexture("data\\TEXTURE\\UI_Jump.png");

	m_p2DUI_Attack = CObject2D::Create();
	m_p2DUI_Attack->SetPos(D3DXVECTOR3(1120.0f, 650.0f, 0.0f));
	m_p2DUI_Attack->SetWight(150.0f);
	m_p2DUI_Attack->SetHeight(40.0f);
	m_p2DUI_Attack->SetTexture("data\\TEXTURE\\UI_Attack.png");

	if (m_pPause == NULL)
	{
		m_pPause = CPause::Create();
	}

	m_bGameEnd = false;
	CManager::GetInstance()->GetInstance()->SetStop(false);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CGame::Uninit(void)
{
	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	if (m_pPause != NULL)
	{
		delete m_pPause;
		m_pPause = NULL;
	}
}

//====================================================================
//終了処理
//====================================================================
void CGame::DeleteTutorial(void)
{

}

//====================================================================
//更新処理
//====================================================================
void CGame::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

#if _DEBUG
	//リセット処理
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_R) == true)
	{
		ReSetGame();
	}

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		CFade::SetFade(CScene::MODE_RESULT);
	}

	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		m_pCubeBlock = CCubeBlock::Create();
		m_pCubeBlock->SetPos(D3DXVECTOR3(-100.0f, -60.0f, 0.0f));
		m_pCubeBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
		m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	}
	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		m_pCubeBlock = CCubeBlock::Create();
		m_pCubeBlock->SetPos(D3DXVECTOR3(100.0f, 500.0f, 0.0f));
		m_pCubeBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
		m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, -5.0f, 0.0f));
	}

	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{
		CCubeDamage *pCubeDamage = CCubeDamage::Create();
		pCubeDamage->SetPos(D3DXVECTOR3(-300.0f, 150.0f, 0.0f));
		pCubeDamage->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
		pCubeDamage->SetMove(D3DXVECTOR3(20.0f, 0.0f, 0.0f));
		pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_BREAK);
	}

	if (pInputKeyboard->GetTrigger(DIK_4) == true)
	{
		CCubeDamage* pCubeDamage = CCubeDamage::Create();
		pCubeDamage->SetPos(D3DXVECTOR3(-300.0f, 150.0f, 0.0f));
		pCubeDamage->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
		pCubeDamage->SetMove(D3DXVECTOR3(20.0f, 0.0f, 0.0f));
		pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_EXPLOSION);
		pCubeDamage->SetLife(60);
	}

	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{
		m_bEvent = !m_bEvent;
	}

#endif


	//注目の切り替え
	if (pInputKeyboard->GetTrigger(DIK_LSHIFT) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::BUTTON_L, 0) == true)
	{
		if (CManager::GetInstance()->GetCamera()->GetAttention() == false)
		{
			m_p2DUI_AttentionOK->SetTexture("data\\TEXTURE\\UI_AttentionON.png");
		}
		else
		{
			m_p2DUI_AttentionOK->SetTexture("data\\TEXTURE\\UI_AttentionOFF.png");
		}
	}

	//ポーズの更新処理
	m_pPause->Update();

	//イベント発生時のポリゴン移動
	if (m_bEvent == false)
	{
		if (m_EventHeight > 0.0f)
		{
			m_EventHeight -= 5.0f;
		}
	}
	else
	{
		//イベントの更新
		EventUpdate();
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pEventBG[nCnt] != nullptr)
		{
			m_pEventBG[nCnt]->SetHeight(m_EventHeight);
		}
	}
}

//====================================================================
//イベントの更新
//====================================================================
void CGame::EventUpdate(void)
{
	if (m_EventHeight < 150.0f)
	{
		m_EventHeight += 5.0f;
	}

	if (m_nEventCount == 0)
	{
		m_p3DEventBG = CObject3D::Create();
		m_p3DEventBG->SetWight(450.0f);
		m_p3DEventBG->SetHeight(200.0f);
		m_p3DEventBG->SetTexture("data\\TEXTURE\\BG_BOSS00.png");
	}
	if (m_p3DEventBG != nullptr)
	{
		if (m_nEventCount < 30)
		{
			m_p3DEventBG->SetPos(D3DXVECTOR3(m_pBoss->GetPos().x - 50.0f - ((float)30 - (float)m_nEventCount) * 50.0f, m_pBoss->GetPos().y, m_pBoss->GetPos().z));
		}
		else
		{
			m_p3DEventBG->SetPos(D3DXVECTOR3(m_pBoss->GetPos().x - 50.0f, m_pBoss->GetPos().y, m_pBoss->GetPos().z));
		}
	}

	if (m_nEventCount < 300)
	{
		m_nEventCount++;
	}
	else
	{
		CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FOLLOW);
		m_bEvent = false;
		m_bEventEnd = true;

		if (m_p3DEventBG != nullptr)
		{
			m_p3DEventBG->SetDeathFlag(true);
		}

		if (m_pBoss != nullptr)
		{
			m_pBoss->SetAction(CBoss::ACTION_NORMAL);
		}
	}
}

//====================================================================
//リセット処理
//====================================================================
void CGame::EventStart(void)
{
	m_bEvent = true;
	CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
}

//====================================================================
//リセット処理
//====================================================================
void CGame::ReSetGame(void)
{

}

//====================================================================
//描画処理
//====================================================================
void CGame::Draw(void)
{

}
