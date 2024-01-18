//============================================
//
//	ポーズ処理 [Pause.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Pause.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"

//静的メンバ変数宣言
CObject2D *CPause::m_pPauseUI[MAX_PAUSE] = {};
CObject2D *CPause::m_pPauseFG[3] = {};
CObject2D *CPause::m_pTutorialMOVE[3] = {};
CObject2D *CPause::m_pTutorialJUMP[3] = {};
CObject2D *CPause::m_pTutorialATTACK[3] = {};
D3DXVECTOR3 CPause::m_MovePos = INITVECTOR3;
D3DXVECTOR3 CPause::m_JumpPos = INITVECTOR3;
D3DXVECTOR3 CPause::m_AttackPos = INITVECTOR3;

//====================================================================
//コンストラクタ
//====================================================================
CPause::CPause()
{
	m_PauseSelect = 0;
	m_Appear = false;

	m_MovePos = D3DXVECTOR3(350.0f, 620.0f, 0.0f);
	m_JumpPos = D3DXVECTOR3(600.0f, 620.0f, 0.0f);;
	m_AttackPos = D3DXVECTOR3(850.0f, 620.0f, 0.0f);;
}

//====================================================================
//デストラクタ
//====================================================================
CPause::~CPause()
{

}

//====================================================================
//生成処理
//====================================================================
CPause *CPause::Create()
{
	CPause *pPause = NULL;

	if (pPause == NULL)
	{
		//敵の生成
		pPause = new CPause();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPause->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pPause;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CPause::Init(void)
{
	m_pTutorialMOVE[0] = CObject2D::Create(7);
	m_pTutorialMOVE[0]->SetType(CObject::TYPE_TUTORIALUI);
	m_pTutorialMOVE[0]->SetTexture("data\\TEXTURE\\WallMove.png");
	m_pTutorialMOVE[0]->SetPos(D3DXVECTOR3(m_MovePos.x, m_MovePos.y - 90.0f, m_MovePos.z));
	m_pTutorialMOVE[0]->SetWight(300.0f);
	m_pTutorialMOVE[0]->SetHeight(150.0f);

	m_pTutorialMOVE[1] = CObject2D::Create(7);
	m_pTutorialMOVE[1]->SetType(CObject::TYPE_TUTORIALUI);
	m_pTutorialMOVE[1]->SetTexture("data\\TEXTURE\\Tutorial_MOVE.png");
	m_pTutorialMOVE[1]->SetPos(D3DXVECTOR3(m_MovePos.x, m_MovePos.y - 40.0f, m_MovePos.z));
	m_pTutorialMOVE[1]->SetWight(200.0f);
	m_pTutorialMOVE[1]->SetHeight(200.0f);

	m_pTutorialMOVE[2] = CObject2D::Create(7);
	m_pTutorialMOVE[2]->SetType(CObject::TYPE_TUTORIALUI);
	m_pTutorialMOVE[2]->SetTexture("data\\TEXTURE\\TutorialText_MOVE.png");
	m_pTutorialMOVE[2]->SetPos(D3DXVECTOR3(m_MovePos.x, m_MovePos.y, m_MovePos.z));
	m_pTutorialMOVE[2]->SetWight(225.0f);
	m_pTutorialMOVE[2]->SetHeight(135.0f);

	m_pTutorialJUMP[0] = CObject2D::Create(7);
	m_pTutorialJUMP[0]->SetType(CObject::TYPE_TUTORIALUI);
	m_pTutorialJUMP[0]->SetTexture("data\\TEXTURE\\WallJump.png");
	m_pTutorialJUMP[0]->SetPos(D3DXVECTOR3(m_JumpPos.x, m_JumpPos.y - 60.0f, m_JumpPos.z));
	m_pTutorialJUMP[0]->SetWight(150.0f);
	m_pTutorialJUMP[0]->SetHeight(225.0f);

	m_pTutorialJUMP[1] = CObject2D::Create(7);
	m_pTutorialJUMP[1]->SetType(CObject::TYPE_TUTORIALUI);
	m_pTutorialJUMP[1]->SetTexture("data\\TEXTURE\\Tutorial_JUMP.png");
	m_pTutorialJUMP[1]->SetPos(D3DXVECTOR3(m_JumpPos.x, m_JumpPos.y - 40.0f, m_JumpPos.z));
	m_pTutorialJUMP[1]->SetWight(150.0f);
	m_pTutorialJUMP[1]->SetHeight(150.0f);

	m_pTutorialJUMP[2] = CObject2D::Create(7);
	m_pTutorialJUMP[2]->SetType(CObject::TYPE_TUTORIALUI);
	m_pTutorialJUMP[2]->SetTexture("data\\TEXTURE\\TutorialText_JUMP.png");
	m_pTutorialJUMP[2]->SetPos(D3DXVECTOR3(m_JumpPos.x, m_JumpPos.y, m_JumpPos.z));
	m_pTutorialJUMP[2]->SetWight(225.0f);
	m_pTutorialJUMP[2]->SetHeight(135.0f);

	m_pTutorialATTACK[0] = CObject2D::Create(7);
	m_pTutorialATTACK[0]->SetType(CObject::TYPE_TUTORIALUI);
	m_pTutorialATTACK[0]->SetTexture("data\\TEXTURE\\WallAttack.png");
	m_pTutorialATTACK[0]->SetPos(D3DXVECTOR3(m_AttackPos.x, m_AttackPos.y - 90.0f, m_AttackPos.z));
	m_pTutorialATTACK[0]->SetWight(225.0f);
	m_pTutorialATTACK[0]->SetHeight(102.5f);

	m_pTutorialATTACK[1] = CObject2D::Create(7);
	m_pTutorialATTACK[1]->SetType(CObject::TYPE_TUTORIALUI);
	m_pTutorialATTACK[1]->SetTexture("data\\TEXTURE\\Tutorial_ATTACK.png");
	m_pTutorialATTACK[1]->SetPos(D3DXVECTOR3(m_AttackPos.x, m_AttackPos.y - 40.0f, m_AttackPos.z));
	m_pTutorialATTACK[1]->SetWight(150.0f);
	m_pTutorialATTACK[1]->SetHeight(150.0f);

	m_pTutorialATTACK[2] = CObject2D::Create(7);
	m_pTutorialATTACK[2]->SetType(CObject::TYPE_TUTORIALUI);
	m_pTutorialATTACK[2]->SetTexture("data\\TEXTURE\\TutorialText_ATTACK.png");
	m_pTutorialATTACK[2]->SetPos(D3DXVECTOR3(m_AttackPos.x, m_AttackPos.y, m_AttackPos.z));
	m_pTutorialATTACK[2]->SetWight(225.0f);
	m_pTutorialATTACK[2]->SetHeight(135.0f);

	m_pPauseFG[0] = CObject2D::Create();
	m_pPauseFG[0]->SetType(CObject::TYPE_TUTORIALUI);
	m_pPauseFG[0]->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	m_pPauseFG[0]->SetWight(1280.0f);
	m_pPauseFG[0]->SetHeight(720.0f);
	m_pPauseFG[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));

	m_pPauseFG[1] = CObject2D::Create();
	m_pPauseFG[1]->SetType(CObject::TYPE_TUTORIALUI);
	m_pPauseFG[1]->SetPos(D3DXVECTOR3(640.0f, 300.0f, 0.0f));
	m_pPauseFG[1]->SetWight(1200.0f);
	m_pPauseFG[1]->SetHeight(600.0f);
	m_pPauseFG[1]->SetTexture("data\\TEXTURE\\Pause_FG.png");

	m_pPauseFG[2] = CObject2D::Create();
	m_pPauseFG[2]->SetType(CObject::TYPE_TUTORIALUI);
	m_pPauseFG[2]->SetPos(D3DXVECTOR3(640.0f, 620.0f, 0.0f));
	m_pPauseFG[2]->SetWight(1200.0f);
	m_pPauseFG[2]->SetHeight(600.0f);
	m_pPauseFG[2]->SetTexture("data\\TEXTURE\\PauseTutorial.png");

	for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
	{
		m_pPauseUI[nCnt] = CObject2D::Create();
		m_pPauseUI[nCnt]->SetType(CObject::TYPE_TUTORIALUI);
		switch (nCnt)
		{
		case 0:
			m_pPauseUI[nCnt]->SetWight(600.0f);
			m_pPauseUI[nCnt]->SetHeight(300.0f);
			break;
		case 1:
			m_pPauseUI[nCnt]->SetWight(600.0f);
			m_pPauseUI[nCnt]->SetHeight(300.0f);
			break;
		case 2:
			m_pPauseUI[nCnt]->SetWight(600.0f);
			m_pPauseUI[nCnt]->SetHeight(300.0f);
			break;
		}
		m_pPauseUI[nCnt]->SetPos(D3DXVECTOR3(290.0f + nCnt * 350.0f, 250.0f, 0.0f));
		m_pPauseUI[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));
	}
	m_pPauseUI[0]->SetTexture("data\\TEXTURE\\BACK_GAME.png");
	m_pPauseUI[1]->SetTexture("data\\TEXTURE\\BACK_RETRY.png");
	m_pPauseUI[2]->SetTexture("data\\TEXTURE\\BACK_TITLE.png");

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CPause::Uninit(void)
{

}

//====================================================================
//更新処理
//====================================================================
void CPause::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad *pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	if (CManager::GetInstance()->GetPause() == true)
	{
		if (pInputKeyboard->GetTrigger(DIK_A) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_LEFT, 0) == true ||
			pInputJoypad->Get_LStick_Trigger(CInputJoypad::LSTICK_LEFT, 0) == true)
		{
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT_PUSH);
			m_PauseSelect--;

			if (m_PauseSelect < 0)
			{
				m_PauseSelect = MAX_PAUSE - 1;
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_D) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_RIGHT, 0) == true ||
			pInputJoypad->Get_LStick_Trigger(CInputJoypad::LSTICK_RIGHT, 0) == true)
		{
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT_PUSH);
			m_PauseSelect++;

			if (m_PauseSelect > MAX_PAUSE - 1)
			{
				m_PauseSelect = 0;
			}
		}

		for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
		{
			if (nCnt == m_PauseSelect)
			{
				m_pPauseUI[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				m_pPauseUI[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));
			}
		}

		if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
			CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true ||
			CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 0) == true)
		{
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);

			switch (m_PauseSelect)
			{
			case 0:
				break;
			case 1:
				CFade::SetFade(CScene::MODE_GAME);
				break;
			case 2:
				CFade::SetFade(CScene::MODE_TITLE);
				break;
			default:
				CFade::SetFade(CScene::MODE_GAME);
				break;
			}
			CManager::GetInstance()->SetPause(false);
			m_Appear = false;
		}
	}

	if (m_Appear == true)
	{
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			m_pTutorialMOVE[nCnt]->SetAppear(true);
			m_pTutorialJUMP[nCnt]->SetAppear(true);
			m_pTutorialATTACK[nCnt]->SetAppear(true);
		}
		for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
		{
			m_pPauseUI[nCnt]->SetAppear(true);
		}
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			m_pPauseFG[nCnt]->SetAppear(true);
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			m_pTutorialMOVE[nCnt]->SetAppear(false);
			m_pTutorialJUMP[nCnt]->SetAppear(false);
			m_pTutorialATTACK[nCnt]->SetAppear(false);
		}
		for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
		{
			m_pPauseUI[nCnt]->SetAppear(false);
		}
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			m_pPauseFG[nCnt]->SetAppear(false);
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CPause::Draw(void)
{

}