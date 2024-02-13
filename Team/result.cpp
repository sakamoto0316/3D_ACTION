//============================================
//
//	リザルト画面 [result.cpp]
//	Author:sakamoto kai
//
//============================================
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object2D.h"
#include "texture.h"
#include "sound.h"
#include "camera.h"

//マクロ定義
#define START_OK ("STARTSETSTAGE")	//スタートメッセージがあるかどうかの確認
#define END_SET_OK ("ENDSETSTAGE")	//エンドメッセージがあるかどうかの確認
#define SCORE_POSX (300.0f)
#define SCORE_MOVEX (20.1f)

//静的メンバ変数宣言
CObject2D *CResult::m_pResult = NULL;
CRanking *CResult::m_pRanking = NULL;
CObject2D *CResult::m_ClearText = NULL;
CObject2D *CResult::m_NormalText = NULL;
CObject2D *CResult::m_DeathText = NULL;
CScore *CResult::m_DeathScore = NULL;
bool CResult::m_Appear = false;
int CResult::m_AddScoreCount = 0;
int CResult::m_AddTotalScore = 0;
CEdit *CResult::m_pEdit = NULL;

//====================================================================
//コンストラクタ
//====================================================================
CResult::CResult()
{
	m_AddScoreCount = 0;
	m_AddTotalScore = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CResult::~CResult()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CResult::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();;
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_RESULT);

	m_AddTotalScore = CManager::GetInstance()->GetEndScore();

	//LoadBlock("data\\TXT\\Result02", D3DXVECTOR3(-440.0f - 700.0f, 400.0f, 200.0f));
	//LoadBlock("data\\TXT\\Result02", D3DXVECTOR3(-440.0f + 700.0f, 400.0f, 200.0f));

	//m_pRanking = CRanking::Create();

	//m_pResult = CObject2D::Create();
	//m_pResult->SetPos(D3DXVECTOR3(640.0f, 50.0f, 0.0f));
	//m_pResult->SetWight(250.0f);
	//m_pResult->SetHeight(150.0f);
	//m_pResult->SetTexture("data\\TEXTURE\\RESULT_TEXT.png");

	m_ClearText = CObject2D::Create();
	m_ClearText->SetPos(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
	m_ClearText->SetWight(1000.0f);
	m_ClearText->SetHeight(400.0f);
	if (CManager::GetInstance()->GetGameClear() == true)
	{
		m_ClearText->SetTexture("data\\TEXTURE\\STAGECLEAR.png");
	}
	else
	{
		m_ClearText->SetTexture("data\\TEXTURE\\GAMEOVER.png");
	}

	//m_DeathText = CObject2D::Create();
	//m_DeathText->SetPos(D3DXVECTOR3(640.0f, 550.0f, 0.0f));
	//m_DeathText->SetWight(700.0f);
	//m_DeathText->SetHeight(500.0f);
	//m_DeathText->SetTexture("data\\TEXTURE\\DeathCount.png");

	CManager::GetInstance()->GetCamera()->SetCameraPos(D3DXVECTOR3(3500.0f, 450.0f, 0.0f));

	//m_MaxConbo = CScore::Create();
	//m_MaxConbo->SetPos(D3DXVECTOR3(m_MaxConboPosX + 150.0f, 350.0f, 0.0f));
	//m_MaxConbo->SetScore(CManager::GetInstance()->GetEndCombo() * 1000);

	//m_MaxFall = CScore::Create();
	//m_MaxFall->SetPos(D3DXVECTOR3(m_MaxFallPosX + 150.0f, 450.0f, 0.0f));
	//m_MaxFall->SetScore((CManager::GetInstance()->GetEndFall() / 60) * 1000);

	//m_Depth = CScore::Create();
	//m_Depth->SetPos(D3DXVECTOR3(m_DepthPosX + 150.0f, 550.0f, 0.0f));
	//m_Depth->SetScore((int)(CManager::GetInstance()->GetEndDepth()));

	//m_TotalScore = CScore::Create();
	//m_TotalScore->SetPos(D3DXVECTOR3(m_TotalScorePosX + 150.0f, 670.0f, 0.0f));
	//m_TotalScore->SetScore(0);

	//m_MaxConboText = CObject2D::Create();
	//m_MaxConboText->SetPos(D3DXVECTOR3(m_MaxConboPosX, 350.0f, 0.0f));
	//m_MaxConboText->SetWight(400.0f);
	//m_MaxConboText->SetHeight(150.0f);
	//m_MaxConboText->SetTexture("data\\TEXTURE\\ComboScore.png");

	//m_MaxFallText = CObject2D::Create();
	//m_MaxFallText->SetPos(D3DXVECTOR3(m_MaxFallPosX, 450.0f, 0.0f));
	//m_MaxFallText->SetWight(400.0f);
	//m_MaxFallText->SetHeight(150.0f);
	//m_MaxFallText->SetTexture("data\\TEXTURE\\FallScore.png");

	//m_DepthText = CObject2D::Create();
	//m_DepthText->SetPos(D3DXVECTOR3(m_DepthPosX, 550.0f, 0.0f));
	//m_DepthText->SetWight(400.0f);
	//m_DepthText->SetHeight(150.0f);
	//m_DepthText->SetTexture("data\\TEXTURE\\DepthScore.png");

	//m_TotalScoreText = CObject2D::Create();
	//m_TotalScoreText->SetPos(D3DXVECTOR3(m_TotalScorePosX, 670.0f, 0.0f));
	//m_TotalScoreText->SetWight(400.0f);
	//m_TotalScoreText->SetHeight(150.0f);
	//m_TotalScoreText->SetTexture("data\\TEXTURE\\TotalScore.png");

	//m_RankText = CObject2D::Create();
	//m_RankText->SetPos(D3DXVECTOR3(RankingPosX, 670.0f, 0.0f));
	//m_RankText->SetWight(220.0f);
	//m_RankText->SetHeight(450.0f);
	//m_RankText->SetTexture("data\\TEXTURE\\RANKING_TEXT.png");

	//m_ResultText = CObject2D::Create();
	//m_ResultText->SetPos(D3DXVECTOR3(640.0f, 100.0f, 0.0f));
	//m_ResultText->SetWight(600.0f);
	//m_ResultText->SetHeight(420.0f);
	//m_ResultText->SetTexture("data\\TEXTURE\\RESULT_TEXT.png");

	//m_RankingText = CObject2D::Create();
	//m_RankingText->SetPos(D3DXVECTOR3(RankingPosX, 100.0f, 0.0f));
	//m_RankingText->SetWight(700.0f);
	//m_RankingText->SetHeight(400.0f);
	//m_RankingText->SetTexture("data\\TEXTURE\\RANKING_TEXT01.png");

	//m_pResult->SetIdx(pTexture->Regist("data\\TEXTURE\\Result.png"));

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CResult::Uninit(void)
{
	//全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//====================================================================
//更新処理
//====================================================================
void CResult::Update(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CFade::SetFade(CScene::MODE_TITLE);
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
	}
}

//====================================================================
//描画処理
//====================================================================
void CResult::Draw(void)
{

}