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
#include "enemy.h"
#include "effect.h"

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
CObject3D* CGame::m_p3DEventBG = NULL;			//イベント時の3D背景
CObject2D *CGame::m_p2DBossName = NULL;
CObjectBillboard* CGame::m_pBillboardSample = NULL;
CObjectX* CGame::m_pXModelSample = NULL;
CObjmeshField* CGame::m_pMeshFieldSample = NULL;
CObjmeshWall* CGame::m_pMeshWallSample = NULL;
CObjmeshCylinder* CGame::m_pMeshCylinderSample = NULL;
CObjmeshDome* CGame::m_pMeshDomeUp = NULL;
CObjmeshDome* CGame::m_pMeshDomeDown = NULL;
CCubeBlock* CGame::m_pCubeBlock = NULL;
CPlayer* CGame::m_pPlayer = NULL;
CBoss*CGame::m_pBoss = NULL;
bool CGame::m_bGameEnd = false;
bool CGame::m_bEvent = false;
bool CGame::m_bEventEnd = false;
int CGame::m_nEventCount = 0;
float CGame::m_EventHeight = 0.0f;
float CGame::m_NameColorA = 0.0f;
float CGame::m_BGColorA = 1.0f;
D3DXVECTOR3 CGame::m_EventPos = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
D3DXVECTOR3 CGame::m_BGRot = INITVECTOR3;

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
	m_BGColorA = 1.0f;
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

	m_pMeshDomeUp = CObjmeshDome::Create();
	m_pMeshDomeUp->SetTexture("data\\TEXTURE\\SkyBG.jpg");

	m_pMeshDomeDown = CObjmeshDome::Create();
	m_pMeshDomeDown->SetTexture("data\\TEXTURE\\SkyBG.jpg");
	m_pMeshDomeDown->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));

	SetStageBlock();

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
	m_pBoss->SetPos(D3DXVECTOR3(0.0f, 5000.0f, 0.0f));

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
	////リセット処理
	//if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_R) == true)
	//{
	//	ReSetGame();
	//}

	//if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	//{
	//	CFade::SetFade(CScene::MODE_RESULT);
	//}

	//if (pInputKeyboard->GetTrigger(DIK_1) == true)
	//{
	//	CEnemy* pEnemy = CEnemy::Create("data\\MODEL\\enemy.x");
	//	pEnemy->SetPos(D3DXVECTOR3(0.0f, 150.0f, 0.0f));
	//}
	//if (pInputKeyboard->GetTrigger(DIK_2) == true)
	//{
	//	m_pCubeBlock = CCubeBlock::Create();
	//	m_pCubeBlock->SetPos(D3DXVECTOR3(100.0f, 500.0f, 0.0f));
	//	m_pCubeBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
	//	m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, -5.0f, 0.0f));
	//}

	//if (pInputKeyboard->GetTrigger(DIK_3) == true)
	//{
	//	CCubeDamage *pCubeDamage = CCubeDamage::Create();
	//	pCubeDamage->SetPos(D3DXVECTOR3(-300.0f, 150.0f, 0.0f));
	//	pCubeDamage->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
	//	pCubeDamage->SetMove(D3DXVECTOR3(20.0f, 0.0f, 0.0f));
	//	pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_BREAK);
	//}

	//if (pInputKeyboard->GetTrigger(DIK_4) == true)
	//{
	//	CCubeDamage* pCubeDamage = CCubeDamage::Create();
	//	pCubeDamage->SetPos(D3DXVECTOR3(-300.0f, 150.0f, 0.0f));
	//	pCubeDamage->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
	//	pCubeDamage->SetMove(D3DXVECTOR3(20.0f, 0.0f, 0.0f));
	//	pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_EXPLOSION);
	//	pCubeDamage->SetLife(60);
	//}

	//if (pInputKeyboard->GetTrigger(DIK_5) == true)
	//{
	//	m_bEvent = !m_bEvent;
	//}

#endif

	if (m_pBoss != nullptr)
	{
		if (m_pBoss->GetBossForm() == 0)
		{
			m_BGRot.x += 0.00f;
			m_BGRot.y += 0.0005f;
			m_BGRot.z = 0.0f;

			if (m_pMeshDomeUp != nullptr)
			{
				m_pMeshDomeUp->SetRot(D3DXVECTOR3(m_BGRot.x, m_BGRot.y, m_BGRot.z));
			}
			if (m_pMeshDomeDown != nullptr)
			{
				m_pMeshDomeDown->SetRot(D3DXVECTOR3(D3DX_PI + m_BGRot.x, m_BGRot.y, -m_BGRot.z));
			}
		}
		else if(m_pBoss->GetBossForm() == 1)
		{
			m_BGRot.x += 0.00f;
			m_BGRot.y += 0.01f;
			m_BGRot.z = D3DX_PI * 0.5f;

			if (m_pMeshDomeUp != nullptr)
			{
				m_pMeshDomeUp->SetRot(D3DXVECTOR3(m_BGRot.x, m_BGRot.y, m_BGRot.z));
				m_pMeshDomeUp->SetScroll(D3DXVECTOR2(m_BGRot.y, m_BGRot.y));
				m_pMeshDomeUp->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_BGColorA));
			}
			if (m_pMeshDomeDown != nullptr)
			{
				m_pMeshDomeDown->SetRot(D3DXVECTOR3(D3DX_PI + m_BGRot.x, m_BGRot.y, -m_BGRot.z));
				m_pMeshDomeDown->SetScroll(D3DXVECTOR2(m_BGRot.y, m_BGRot.y));
				m_pMeshDomeDown->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_BGColorA));
			}
		}
	}

	//注目の切り替え
	if (CManager::GetInstance()->GetCamera()->GetCameraMode() == CCamera::CAMERAMODE_FOLLOW)
	{
		if (CManager::GetInstance()->GetCamera()->GetAttention() == true)
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

	//各種オブジェクトの生成と初期化
	if (m_nEventCount == 0)
	{
		CCubeBlock* pBlock;

		for (int nCnt = 0; nCnt < 50; nCnt++)
		{
			float randX = (float)(rand() % 751);
			float randY = (float)(rand() % 201);

			pBlock = CCubeBlock::Create();
			pBlock->SetPos(D3DXVECTOR3(m_EventPos.x - 750.0f + randX - 250.0f, m_EventPos.y + randY - 100.0f, m_EventPos.z - 100.0f));
			pBlock->SetMove(D3DXVECTOR3(15.0f, 0.0f, 0.0f));
			pBlock->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			pBlock->SetSize(D3DXVECTOR3(50.0f, 50.0f, 25.0f));
			pBlock->SetLife(120);
		}
	}
	else if (m_nEventCount == 30)
	{
		if (m_p3DEventBG == nullptr)
		{
			m_p3DEventBG = CObject3D::Create();
			m_p3DEventBG->SetWight(500.0f);
			m_p3DEventBG->SetHeight(250.0f);
			m_p3DEventBG->SetTexture("data\\TEXTURE\\BG_BOSS00.png");
		}

		if (m_p2DBossName == nullptr)
		{
			m_p2DBossName = CObject2D::Create();
			m_p2DBossName->SetPos(D3DXVECTOR3(640.0f, 600.0f, 0.0f));
			m_p2DBossName->SetWight(500.0f);
			m_p2DBossName->SetHeight(500.0f);
			m_p2DBossName->SetTexture("data\\TEXTURE\\BossName.png");
			m_NameColorA = 0.0f;
			m_p2DBossName->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_NameColorA));
		}

		if (m_pBoss != nullptr)
		{
			m_pBoss->SetPos(D3DXVECTOR3(0.0f, 300.0f, 0.0f));
		}
	}

	//ボスの動き管理
	if (m_pBoss != nullptr)
	{

	}

	//カメラ振動の管理
	if (m_nEventCount == 120)
	{
		//カメラのバイブレーションをオンにする
		CManager::GetInstance()->GetCamera()->SetBib(true);
	}
	else if (m_nEventCount == 240)
	{
		//カメラのバイブレーションをオフにする
		CManager::GetInstance()->GetCamera()->SetBib(false);
	}

	if (m_nEventCount >= 120 && m_nEventCount < 240)
	{
		if (m_nEventCount % 5 == 0)
		{
			CEffect* pEffect = CEffect::Create();
			pEffect->SetTexName("data\\TEXTURE\\RingEffect.png");
			pEffect->SetPos(D3DXVECTOR3(m_pBoss->GetPos().x, m_pBoss->GetPos().y, m_pBoss->GetPos().x - 50.0f));
			pEffect->SetDel(-40.0f);
			pEffect->SetLife(60);
			pEffect->SetRadius(0.0f);
			pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	//イベント背景の更新
	if (m_p3DEventBG != nullptr)
	{
		if (m_nEventCount < 60)
		{
			m_p3DEventBG->SetPos(D3DXVECTOR3(m_EventPos.x - 50.0f - ((float)60 - (float)m_nEventCount) * 50.0f, m_EventPos.y, m_EventPos.z));
		}
		else
		{
			m_p3DEventBG->SetPos(D3DXVECTOR3(m_EventPos.x - 50.0f, m_EventPos.y, m_EventPos.z));
		}
	}

	//イベントボスネームの更新
	if (m_p2DBossName != nullptr)
	{
		if (m_nEventCount >= 240)
		{
			m_NameColorA -= 0.02f;
			m_p2DBossName->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_NameColorA));
		}
		else if (m_nEventCount > 60)
		{
			m_NameColorA += 0.02f;

			if (m_NameColorA > 1.0f)
			{
				m_NameColorA = 1.0f;
			}

			m_p2DBossName->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_NameColorA));
		}
	}

	//イベント終了までのカウント
	if (m_nEventCount < 300)
	{
		m_nEventCount++;
	}
	else
	{//イベント終了時
		CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FOLLOW);
		m_bEvent = false;
		m_bEventEnd = true;

		if (m_p3DEventBG != nullptr)
		{
			m_p3DEventBG->SetDeathFlag(true);
			m_p3DEventBG = nullptr;
		}

		if (m_p2DBossName != nullptr)
		{
			m_p2DBossName->SetDeathFlag(true);
			m_p2DBossName = nullptr;
		}

		if (m_pBoss != nullptr)
		{
			m_pBoss->SetAction(CBoss::ACTION_NORMAL);
		}

		//ゲームのBGMを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_BOSS);

		m_pBoss->LifeGaugeCreate();
	}
}

//====================================================================
//ステージのブロック配置
//====================================================================
void CGame::SetStageBlock(void)
{
	m_pCubeBlock = CCubeBlock::Create();
	m_pCubeBlock->SetPos(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	m_pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 10.0f, 500.0f));
	m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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
