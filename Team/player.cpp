//============================================
//
//	プレイヤーの処理 [playerlevel.cpp]
//	Author:sakamoto kai
//
//============================================
#include "player.h"
#include "playerEffect.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "input.h"
#include "sound.h"
#include "Effect.h"
#include "game.h"
#include "time.h"
#include "camera.h"
#include "objGauge2D.h"
#include "objmeshCube.h"
#include "boss.h"
#include "number.h"
#include "numberFall.h"
#include "camera.h"
#include "object3D.h"
#include "CubeBlock.h"

#define PLAYER_LIFE (2000.0f)		//プレイヤーの初期ライフ
#define PLAYER_ROT_SPEED (0.2f)		//プレイヤーの回転スピード
#define PLAYER_SPEED (10.0f)		//プレイヤーの速さ
#define PLAYER_JAMPPOWER (15.0f)	//プレイヤーのジャンプ力
#define PLAYER_JAMPWALL (4.0f)		//ジャンプまでの力の振り幅
#define JAMP_ACTIONNOT (4)			//ジャンプから行動出来るまで
#define COLLISION_SIZE (D3DXVECTOR3(35.0f,80.0f,35.0f))		//横の当たり判定
#define ATTACK1_DAMAGE (200.0f)		//地上攻撃１の攻撃力
#define ATTACK2_DAMAGE (150.0f)		//地上攻撃２の攻撃力
#define ATTACK3_DAMAGE (600.0f)		//地上攻撃３の攻撃力
#define SKY_ATTACK_DAMAGE (300.0f)	//空中攻撃の攻撃力
#define DODGE_FRAME (10)			//回避フレーム
#define DODGE_COOL (10)				//回避のクールタイム
#define DODGE_EFFECTINTERVAL (3)	//回避の残像間隔
#define DODGE_SPEED (55.0f)			//回避の移動速度
#define BOSS_DAMAGE (60.0f)			//ボスに被弾した際のダメージ
#define FALL_DAMAGE (100.0f)		//落下した際のダメージ
#define WAIT_TIME (60)				//無敵時間
#define DAMAGE_TIME (10)			//ダメージのリアクション時間

//====================================================================
//コンストラクタ
//====================================================================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	SetWight(COLLISION_SIZE.x);
	SetHeight(COLLISION_SIZE.y);
	m_CameraPos = INITVECTOR3;
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_bWallJump = false;
	m_bJump = false;
	m_nActionCount = 0;
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;
	m_nAttackHit = false;
	m_nAttackDamage = 0.0f;
	m_nAttackCount = 0;
	m_nAttackCountMax = 0;
	m_nAttackChainFrame = 0;
	m_nDodgeCount = 0;
	m_nDodgeCoolTime = 0;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_ReSpownPos = D3DXVECTOR3(0.0f, 350.0f, 0.0f);
	m_GameEnd = false;
	m_pLifeGauge = nullptr;
	m_fLife = PLAYER_LIFE;
	m_fLifeMax = m_fLife;
	m_AtkPos = INITVECTOR3;
	m_bRight = true;
	CameraDiffMove = false;
	CameraDiffTime = 0;
	m_pShadow = nullptr;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pLifeNumber[nCnt] = nullptr;
	}
}

//====================================================================
//デストラクタ
//====================================================================
CPlayer::~CPlayer()
{

}

//====================================================================
//生成処理
//====================================================================
CPlayer* CPlayer::Create()
{
	CPlayer* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//プレイヤーの生成
		pPlayer = new CPlayer();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPlayer->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pPlayer;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CPlayer::Init(void)
{
	//モデルの生成
	LoadLevelData("data\\TXT\\motion_player.txt");

	//モーションの生成
	if (m_pMotion == NULL)
	{
		//モーションの生成
		m_pMotion = new CMotion;
	}
	//初期化処理
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_player.txt");

	//種類設定
	SetType(CObject::TYPE_PLAYER3D);

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		m_pMotion->Set(ACTION_TITLE, 0);
		break;

	case CScene::MODE_GAME:
		//自分が保持するオブジェクトの生成
		MyObjCreate();

		//ライフの文字設定
		SetLifeUI();
		break;

	case CScene::MODE_RESULT:
		break;
	}

	return S_OK;
}

//====================================================================
//自分が保持するオブジェクトの生成
//====================================================================
void CPlayer::MyObjCreate(void)
{
	//ライフゲージの生成
	if (m_pLifeGauge == nullptr)
	{
		m_pLifeGauge = CObjGauge2D::Create();
		m_pLifeGauge->SetPos(D3DXVECTOR3(1000.0f, 500.0f, 0.0f));
		m_pLifeGauge->SetWight(250.0f);
		m_pLifeGauge->SetHeight(10.0f);
		m_pLifeGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fLife);
	}

	//影の生成
	if (m_pShadow == nullptr)
	{
		m_pShadow = CObject3D::Create();
		m_pShadow->SetPos(m_pos);
		m_pShadow->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f));
		m_pShadow->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		m_pShadow->SetTexture("data\\TEXTURE\\Effect000.jpg");
		m_pShadow->SetAddDorw(true);
	}

	//ライフ用数字UIの生成
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pLifeNumber[nCnt] = CNumber::Create();
		m_pLifeNumber[nCnt]->SetPos(D3DXVECTOR3(1200.0f + nCnt * 15.0f, 530.0f, 0.0f));
		m_pLifeNumber[nCnt]->SetWight(20.0f);
		m_pLifeNumber[nCnt]->SetHeight(20.0f);
	}
}

//====================================================================
//終了処理
//====================================================================
void CPlayer::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Uninit();
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = NULL;
	}

	//モーションの終了処理
	if (m_pMotion != NULL)
	{
		//モーションの破棄
		delete m_pMotion;
		m_pMotion = NULL;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CPlayer::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;
	case CScene::MODE_GAME:

		if (CGame::GetEvent() == false)
		{
			GameUpdate();
		}

		break;
	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CPlayer::TitleUpdate(void)
{


	//モーションの更新
	m_pMotion->Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CPlayer::GameUpdate(void)
{
	if (m_State != STATE_DEATH)
	{
		//キーボードの取得
		CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

		//過去の位置を記録する
		m_posOld = m_pos;

		//当たり判定を有効にする
		m_bHit = false;

		//重力
		m_move.y -= 0.98f;

		//落下速度が一定以上になった場合ジャンプを不能にする
		if (m_move.y < -5.0f)
		{
			if (m_bJump == false)
			{
				m_bJump = true;
			}
		}

		if (CManager::GetInstance()->GetCamera()->GetCameraMode() == CCamera::CAMERAMODE_SIDEVIEW)
		{
			//移動処理
			Move2D();

			//移動方向処理
			Rot2D();

			//回避処理
			Dodge2D();
		}
		else
		{
			//移動処理
			Move();

			//移動方向処理
			Rot();

			//回避処理
			Dodge();
		}

		//攻撃処理
		Attack();

		if (m_nAttackCount > 0)
		{
			AttackCollision();
		}

		//ジャンプ処理
		Jump();

		//行動不能時間のカウント
		m_nActionNotCount--;

		//減衰係数
		m_move.x = m_move.x * 0.5f;
		if (m_move.x <= 0.0001f && m_move.x >= -0.0001f)
		{
			m_move.x = 0.0f;
		}

		m_move.z = m_move.z * 0.5f;
		if (m_move.z <= 0.0001f && m_move.z >= -0.0001f)
		{
			m_move.z = 0.0f;
		}

		if (m_bJump == true)
		{
			m_Objmove.x = m_Objmove.x * 0.25f;
			if (m_Objmove.x <= 0.0001f && m_Objmove.x >= -0.0001f)
			{
				m_Objmove.x = 0.0f;
			}

			m_Objmove.z = m_Objmove.z * 0.25f;
			if (m_Objmove.z <= 0.0001f && m_Objmove.z >= -0.0001f)
			{
				m_Objmove.z = 0.0f;
			}
		}

		//Y軸の位置更新
		m_pos.y += m_move.y;
		m_pos.y += m_Objmove.y;

		CollisionBlock(&m_pos, COLLISION::COLLISION_Y);

		//X軸の位置更新
		m_pos.x += m_move.x;
		m_pos.x += m_Objmove.x;

		CollisionBlock(&m_pos, COLLISION::COLLISION_X);

		//Z軸の位置更新
		m_pos.z += m_move.z;
		m_pos.z += m_Objmove.z;

		CollisionBlock(&m_pos, COLLISION::COLLISION_Z);

		//当たり判定の処理
		if (m_State == STATE_NORMAL)
		{
			if (m_nDodgeCount <= 0)
			{
				CollisionDamageCube(m_pos);
				CollisionBoss();

				if (CGame::GetEventEnd() == false)
				{
					CollisionBossEvent();
				}
			}
		}

		//影の更新
		CollisionShadow();

		//カメラ位置の更新
		m_CameraPos.x = m_pos.x;
		m_CameraPos.z = m_pos.z;
		if (m_bJump == false)
		{
			if (m_CameraPos.y > m_pos.y + 50.0f ||
				m_CameraPos.y < m_pos.y - 50.0f)
			{
				CameraDiffMove = true;
				CameraDiffTime = 30;
			}
			m_CameraPos.y = m_pos.y;
		}

		//カメラの補正設定
		CameraDiff();

		//画面外判定
		if (m_pos.y < 0.0f)
		{
			FallDamage();
		}
	}

	//モーションの管理
	ActionState();

	//状態の管理
	StateManager();

	//モーションの更新
	m_pMotion->Update();
}

//====================================================================
//ライフUIの更新
//====================================================================
void CPlayer::SetLifeUI(void)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pLifeNumber[nCnt] == nullptr)
		{
			return;
		}
	}

	m_pLifeNumber[0]->SetNumber((int)m_fLife % 10000 / 1000);
	m_pLifeNumber[1]->SetNumber((int)m_fLife % 1000 / 100);
	m_pLifeNumber[2]->SetNumber((int)m_fLife % 100 / 10);
	m_pLifeNumber[3]->SetNumber((int)m_fLife % 10 / 1);
}

//====================================================================
//状態管理
//====================================================================
void CPlayer::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;

	case STATE_DEATH:
		break;

	case STATE_WAIT:
		if (m_nStateCount <= 0)
		{
			m_State = STATE_NORMAL;
			m_nStateCount = 0;

			for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
			{
				m_apModel[nCnt]->SetColorChange(false);
			}
		}
		break;

	case STATE_DAMAGE:
		if (m_nStateCount <= 0)
		{
			m_State = STATE_WAIT;
			m_nStateCount = WAIT_TIME;

			for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
			{
				m_apModel[nCnt]->SetColorChange(true);
				m_apModel[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
		}
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//移動処理
//====================================================================
void CPlayer::Move2D(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		NormarizeMove.x += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		NormarizeMove.x += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_D) == false)
	{
		//左スティックによる左右移動
		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;
	}

	m_move += NormarizeMove;

}

//====================================================================
//移動方向処理
//====================================================================
void CPlayer::Rot2D(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//移動方向に向きを合わせる処理
	float fRotMove, fRotDest, fRotDiff;

	fRotMove = m_rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->Get_Stick_Left(0).x > 0.1f)
	{
		fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
		fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y)) + D3DX_PI * -0.5f;
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->Get_Stick_Left(0).x < -0.1f)
	{
		fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
		fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y)) + D3DX_PI * 0.5f;
	}

	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > D3DX_PI * 1.0f)
	{
		fRotDiff -= D3DX_PI * 2.0f;
	}
	else if (fRotDiff < -D3DX_PI * 1.0f)
	{
		fRotDiff += D3DX_PI * 2.0f;
	}

	fRotMove += fRotDiff * PLAYER_ROT_SPEED;

	if (fRotMove > D3DX_PI * 1.0f)
	{
		fRotMove -= D3DX_PI * 2.0f;
	}
	else if (fRotMove < -D3DX_PI * 1.0f)
	{
		fRotMove += D3DX_PI * 2.0f;
	}

	if (m_nAttackChainFrame <= 0 && m_nAttackCount <= 0)
	{
		if (pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->Get_Stick_Left(0).x != 0.0f)
		{
			m_rot.y = fRotMove;
		}
	}

	if (m_move.x > 0.1f)
	{
		m_bRight = true;
	}
	else if (m_move.x < -0.1f)
	{
		m_bRight = false;
	}
}

//====================================================================
//回避処理
//====================================================================
void CPlayer::Dodge2D(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	if (
		(pInputKeyboard->GetTrigger(DIK_N) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_R, 0) == true ||
			pInputMouse->GetTrigger(CInputMouse::PUSH_RIGHT) == true) &&
		m_nDodgeCoolTime <= 0
		)
	{
		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DODGE);

		m_nDodgeCount = DODGE_FRAME;
		m_nDodgeCoolTime = DODGE_COOL;
	}

	if (m_nDodgeCount > 0)
	{
		if (m_nDodgeCount % DODGE_EFFECTINTERVAL == 0)
		{
			CPlayerEffect* pPlayerEffect = CPlayerEffect::Create();
			pPlayerEffect->SetPos(m_pos);
			pPlayerEffect->SetRot(m_rot);
			pPlayerEffect->SetAllPose(m_pMotion->GetType(), m_pMotion->GetKey(), m_pMotion->GetCounter());
		}

		if (m_bRight == true)
		{
			m_move.x = DODGE_SPEED;
		}
		else
		{
			m_move.x = -DODGE_SPEED;
		}
		m_move.y = 0.0f;
		m_Objmove.y = 0.0f;

		m_nDodgeCount--;
	}
	else
	{
		if (m_nDodgeCoolTime > 0)
		{
			m_nDodgeCoolTime--;
		}
	}
}

//====================================================================
//移動処理
//====================================================================
void CPlayer::Move(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_B) == true)
	{
		CPlayerEffect *pPlayerEffect = CPlayerEffect::Create();
		pPlayerEffect->SetPos(m_pos);
		pPlayerEffect->SetRot(m_rot);
		pPlayerEffect->SetAllPose(m_pMotion->GetType(), m_pMotion->GetKey(), m_pMotion->GetCounter());
	}

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_V) == true)
	{
		CPlayerEffect* pPlayerEffect = CPlayerEffect::Create();
		pPlayerEffect->SetPos(m_pos);
		pPlayerEffect->SetRot(m_rot);
		pPlayerEffect->SetPose(m_pMotion->GetType(), m_pMotion->GetKey(), m_pMotion->GetCounter(), 2);
	}

	if (pInputKeyboard->GetPress(DIK_8) == true)
	{
		m_fLife -= 1.0f;

		if (m_pLifeGauge != nullptr)
		{
			m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fLife);
		}
	}

	if (pInputKeyboard->GetPress(DIK_9) == true)
	{
		m_fLife += 1.0f;

		if (m_pLifeGauge != nullptr)
		{
			m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fLife);
		}
	}

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		NormarizeMove.z += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		NormarizeMove.z += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		NormarizeMove.x += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		NormarizeMove.x += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_W) == false && pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_S) == false && pInputKeyboard->GetPress(DIK_D) == false)
	{
		//左スティックによる前後移動	
		m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(CameraRot.y) * PLAYER_SPEED;

		//左スティックによる左右移動
		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;
	}

	m_move += NormarizeMove;

}

//====================================================================
//移動方向処理
//====================================================================
void CPlayer::Rot(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//移動方向に向きを合わせる処理
	float fRotMove, fRotDest, fRotDiff;

	fRotMove = m_rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	if (pInputKeyboard->GetPress(DIK_E) == true)
	{
		m_rot.y += 0.1f;
	}
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{
		m_rot.y -= 0.1f;
	}

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputJoypad->Get_Stick_Left(0).y > 0.1f)
	{
		if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->Get_Stick_Left(0).x > 0.1f)
		{
			fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
			fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y)) + D3DX_PI * -0.75f;
		}
		else if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->Get_Stick_Left(0).x < -0.1f)
		{
			fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
			fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y)) + D3DX_PI * 0.75f;
		}
		else
		{
			fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
			fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y)) + D3DX_PI;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInputJoypad->Get_Stick_Left(0).y < -0.1f)
	{
		if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->Get_Stick_Left(0).x > 0.1f)
		{
			fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
			fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y)) + D3DX_PI * -0.25f;
		}
		else if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->Get_Stick_Left(0).x < -0.1f)
		{
			fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
			fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y)) + D3DX_PI * 0.25f;
		}
		else
		{
			fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
			fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y));
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->Get_Stick_Left(0).x > 0.1f)
	{
		fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
		fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y)) + D3DX_PI * -0.5f;
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->Get_Stick_Left(0).x < -0.1f)
	{
		fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
		fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y)) + D3DX_PI * 0.5f;
	}

	////左スティックによる前後移動	
	//m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(CameraRot.y) * PLAYER_SPEED;
	//m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(CameraRot.y) * PLAYER_SPEED;

	////左スティックによる左右移動
	//m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(CameraRot.y) * PLAYER_SPEED;
	//m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(CameraRot.y) * PLAYER_SPEED;

	//fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));
	//fRotDest = atan2f(sinf(CameraRot.y), cosf(CameraRot.y) + D3DX_PI * pInputJoypad->Get_Stick_Left(0).x);

	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > D3DX_PI * 1.0f)
	{
		fRotDiff -= D3DX_PI * 2.0f;
	}
	else if (fRotDiff < -D3DX_PI * 1.0f)
	{
		fRotDiff += D3DX_PI * 2.0f;
	}

	fRotMove += fRotDiff * PLAYER_ROT_SPEED;

	if (fRotMove > D3DX_PI * 1.0f)
	{
		fRotMove -= D3DX_PI * 2.0f;
	}
	else if (fRotMove < -D3DX_PI * 1.0f)
	{
		fRotMove += D3DX_PI * 2.0f;
	}

	if (m_nAttackChainFrame <= 0 && m_nAttackCount <= 0)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->Get_Stick_Left(0).y != 0.0f || pInputJoypad->Get_Stick_Left(0).x != 0.0f)
		{
			m_rot.y = fRotMove;
		}
	}
}

//====================================================================
//回避処理
//====================================================================
void CPlayer::Dodge(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	if (
		(pInputKeyboard->GetTrigger(DIK_N) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_R, 0) == true || 
			pInputMouse->GetTrigger(CInputMouse::PUSH_RIGHT) == true) &&
		m_nDodgeCoolTime <= 0
		)
	{
		m_nDodgeCount = DODGE_FRAME;
		m_nDodgeCoolTime = DODGE_COOL;

		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DODGE);
	}

	if (m_nDodgeCount > 0)
	{
		if (m_nDodgeCount % DODGE_EFFECTINTERVAL == 0)
		{
			CPlayerEffect* pPlayerEffect = CPlayerEffect::Create();
			pPlayerEffect->SetPos(m_pos);
			pPlayerEffect->SetRot(m_rot);
			pPlayerEffect->SetAllPose(m_pMotion->GetType(), m_pMotion->GetKey(), m_pMotion->GetCounter());
		}

		m_move.x = sinf(m_rot.y + D3DX_PI) * DODGE_SPEED;
		m_move.z = cosf(m_rot.y + D3DX_PI) * DODGE_SPEED;
		m_move.y = 0.0f;
		m_Objmove.y = 0.0f;

		m_nDodgeCount--;
	}
	else
	{
		if (m_nDodgeCoolTime > 0)
		{
			m_nDodgeCoolTime--;
		}
	}
}

//====================================================================
//ジャンプ処理
//====================================================================
void CPlayer::Jump(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	if (
		(pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_B, 0) == true) &&
		m_bJump == false)
	{//ジャンプ使用状態じゃない時にジャンプボタンを押したとき

		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_JUMP);

		m_move.y = 20.0f;
		m_bJump = true;

		//ジャンプ直後は攻撃などのアクションを行えない
		m_nActionNotCount = JAMP_ACTIONNOT;
	}
}

//====================================================================
//攻撃処理
//====================================================================
void CPlayer::Attack(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	if (
		(pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_A, 0) == true ||
			pInputMouse->GetTrigger(CInputMouse::PUSH_LEFT) == true) &&
		m_nAttackCount <= 0
		)
	{//攻撃中じゃない時にジャンプボタンを押したとき

		if (m_bJump == true)
		{//空中にいる時
			if (m_bAirAttack == false)
			{//空中攻撃を行っていないとき

				//ゲームのSEを再生する
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ATTACK_SKY);

				//攻撃モーション
				if (m_Action != ACTION_SKYATTACK)
				{
					m_Action = ACTION_SKYATTACK;
					m_pMotion->Set(ACTION_SKYATTACK, 3);
					m_bAirAttack = true;

					m_nAttackCount = 20;
					m_nAttackDamage = SKY_ATTACK_DAMAGE + (rand() % (int)SKY_ATTACK_DAMAGE) * 0.1f;
					m_nAttackCountMax = m_nAttackCount;
					m_nAttackChainFrame = 1;

					if (m_move.y <= 10.0f)
					{
						m_move.y = 10.0f;
					}
				}
			}
		}
		else
		{//地上にいる時

			//ゲームのSEを再生する
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ATTACK_NORMAL);

			switch (m_AtkAction)
			{
			default:
				//攻撃モーション
				if (m_Action != ACTION_ATTACK1)
				{
					m_Action = ACTION_ATTACK1;
					m_pMotion->Set(ACTION_ATTACK1, 3);
					m_AtkAction = ACTION_ATTACK1;

					m_nAttackDamage = ATTACK1_DAMAGE + (rand() % (int)ATTACK1_DAMAGE) * 0.1f;
					m_nAttackCount = 20;
					m_nAttackCountMax = m_nAttackCount;
					m_nAttackChainFrame = 30;
				}

				break;

			case ACTION_ATTACK1:

				//攻撃モーション
				if (m_Action != ACTION_ATTACK2)
				{
					m_Action = ACTION_ATTACK2;
					m_pMotion->Set(ACTION_ATTACK2, 3);
					m_AtkAction = ACTION_ATTACK2;

					m_nAttackDamage = ATTACK2_DAMAGE + (rand() % (int)ATTACK2_DAMAGE) * 0.1f;
					m_nAttackCount = 20;
					m_nAttackCountMax = m_nAttackCount;
					m_nAttackChainFrame = 30;
				}

				break;

			case ACTION_ATTACK2:

				//攻撃モーション
				if (m_Action != ACTION_ATTACK3)
				{
					m_Action = ACTION_ATTACK3;
					m_pMotion->Set(ACTION_ATTACK3, 3);
					m_AtkAction = ACTION_WAIT;

					m_nAttackDamage = ATTACK3_DAMAGE + (rand() % (int)ATTACK3_DAMAGE) * 0.1f;
					m_nAttackCount = 40;
					m_nAttackCountMax = m_nAttackCount;
					m_nAttackChainFrame = 0;
				}

				break;
			}
		}

		////SE
		//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_PLAYER_JUMP);

		//ジャンプ直後は攻撃などのアクションを行えない
		m_nActionNotCount = JAMP_ACTIONNOT;

		m_nAttackHit = false;
	}

	if (m_bJump == false)
	{
		m_bAirAttack = false;
	}

	if (m_nAttackCount > 0)
	{
		m_nAttackCount--;
	}
	else
	{
		if (m_nAttackChainFrame > 0)
		{
			m_nAttackChainFrame--;
		}
		else
		{
			m_AtkAction = ACTION_WAIT;
		}
	}

	//攻撃位置の設定
	m_AtkPos = m_apModel[3]->GetWorldPos();

	if (m_nAttackCount > 0)
	{
		CPlayerEffect* pPlayerEffect = CPlayerEffect::Create();
		pPlayerEffect->SetPos(m_pos);
		pPlayerEffect->SetRot(m_rot);
		pPlayerEffect->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		pPlayerEffect->SetDel(0.05f);
		pPlayerEffect->SetPose(m_pMotion->GetType(), m_pMotion->GetKey(), m_pMotion->GetCounter(), 2);
	}
	else if (m_nAttackChainFrame > 0)
	{
		CPlayerEffect* pPlayerEffect = CPlayerEffect::Create();
		pPlayerEffect->SetPos(m_pos);
		pPlayerEffect->SetRot(m_rot);
		pPlayerEffect->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		pPlayerEffect->SetDel(0.4f);
		pPlayerEffect->SetPose(m_pMotion->GetType(), m_pMotion->GetKey(), m_pMotion->GetCounter(), 2);
	}
}

//====================================================================
//攻撃判定処理
//====================================================================
void CPlayer::AttackCollision(void)
{
	if (m_nAttackHit == false)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();			//種類を取得

				if (type == TYPE_BOSS)
				{//種類がボスの時

					if (CollisionCircle(m_AtkPos, pObj->GetPos(), 100.0f) == true)
					{
						pObj->HitDamage(m_nAttackDamage);
						m_nAttackHit = true;

						for (int nCnt = 0; nCnt < 3; nCnt++)
						{
							float x = atan2f(pObj->GetPos().x, CManager::GetInstance()->GetCamera()->GetPosV().x);
							float z = atan2f(pObj->GetPos().z, CManager::GetInstance()->GetCamera()->GetPosV().z);

							CNumberFall* pNumber = CNumberFall::Create();
							pNumber->SetDigit(nCnt);
							pNumber->SetPos(D3DXVECTOR3(
								pObj->GetPos().x,
								pObj->GetPos().y,
								pObj->GetPos().z));
							switch (nCnt)
							{
							case 0:
								pNumber->SetNumber((int)m_nAttackDamage % 1000 / 100);
								break;
							case 1:
								pNumber->SetNumber((int)m_nAttackDamage % 100 / 10);
								break;
							case 2:
								pNumber->SetNumber((int)m_nAttackDamage % 10 / 1);
								break;
							}
						}
					}
				}

				pObj = pObjNext;
			}
		}
	}
}

//====================================================================
//円の当たり判定
//====================================================================
bool CPlayer::CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut)
{
	bool nHit = false;

	if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y)) <= nRadiusOut)
	{//円の判定が当たった
		if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
			+ (pos1.z - pos2.z) * (pos1.z - pos2.z)) <= nRadiusOut)
		{//円の判定が当たった
			nHit = true;
		}
	}

	return nHit;
}

//====================================================================
//攻撃を受けた時の処理
//====================================================================
void CPlayer::FallDamage(void)
{
	HitDamage(FALL_DAMAGE);
	CEffect* pEffect = CEffect::Create();
	pEffect->SetPos(m_pos);
	pEffect->SetRadius(500.0f);
	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pos = D3DXVECTOR3(m_ReSpownPos.x, m_ReSpownPos.y + -100.0f, m_ReSpownPos.z);
}

//====================================================================
//攻撃を受けた時の処理
//====================================================================
void CPlayer::HitDamage(float Damage)
{
	if (m_State == STATE_NORMAL)
	{
		m_State = STATE_DAMAGE;
		m_nStateCount = DAMAGE_TIME;
		m_Action = ACTION_WAIT;
		m_nAttackCount = 0;
		m_pMotion->Set(ACTION_WAIT, 5);
		m_fLife -= Damage;
		SetLifeUI();

		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			m_apModel[nCnt]->SetColorChange(true);
			m_apModel[nCnt]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (m_pLifeGauge != nullptr)
		{
			m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fLife);
		}
	}
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CPlayer::ActionState(void)
{
	if (m_nAttackCount <= 0)
	{
		//回避モーション
		if (m_nDodgeCount > 0)
		{
			if (m_Action != ACTION_EVASION)
			{
				m_Action = ACTION_EVASION;
				m_pMotion->Set(ACTION_EVASION, 1);
			}
		}
		//ジャンプモーション
		else if (m_bJump == true)
		{
			if (m_Action != ACTION_JAMP)
			{
				m_Action = ACTION_JAMP;
				m_pMotion->Set(ACTION_JAMP, 5);
			}
		}
		//移動モーション
		else if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
		{
			if (m_Action != ACTION_MOVE)
			{
				m_Action = ACTION_MOVE;
				m_pMotion->Set(ACTION_MOVE, 5);
			}
		}
		//ニュートラルモーション
		else
		{
			if (m_Action != ACTION_WAIT)
			{
				m_Action = ACTION_WAIT;
				m_pMotion->Set(ACTION_WAIT, 5);
			}
		}
	}
}

//====================================================================
//オブジェクトとの当たり判定処理
//====================================================================
void CPlayer::CameraDiff(void)
{
	if (CameraDiffMove == true)
	{
		if (CameraDiffTime > 0)
		{
			CameraDiffTime--;
		}
		else
		{
			CameraDiffMove = false;
		}
	}
}

//====================================================================
//オブジェクトとの当たり判定処理
//====================================================================
bool CPlayer::CollisionBlock(D3DXVECTOR3* pos, COLLISION XYZ)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (m_State != STATE_DEATH)
			{
				if (type == TYPE_CUBEBLOCK)
				{//種類がブロックの時

					if (pObj->CollisionBlock(pos, m_posOld, &m_move, &m_Objmove, COLLISION_SIZE * 0.5f, &m_bJump, XYZ) == true)
					{
						m_bJump = false;
						m_bAirAttack = false;
						m_move.y = 0.0f;

						return true;
					}

					if (m_bHit == true)
					{
						return true;
					}
				}
			}

			pObj = pObjNext;
		}
	}

	return false;
}

//====================================================================
//オブジェクトとの当たり判定処理
//====================================================================
bool CPlayer::CollisionDamageCube(D3DXVECTOR3 pos)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_CUBEDAMEGE)
			{//種類がブロックの時

				float fDamage = 0.0f;

				if (pObj->CollisionDamageBlock(pos, COLLISION_SIZE, &fDamage) == true)
				{
					HitDamage(fDamage);

					return true;
				}
			}

			pObj = pObjNext;
		}
	}

	return false;
}

//====================================================================
//マップの削除処理
//====================================================================
void CPlayer::DeleteMap(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_MAPCHECKPOINT)
			{//種類がブロックの時

				D3DXVECTOR3 ObjPos = pObj->GetPos();

				if (ObjPos.x <= 18800.0f)
				{
					pObj->Uninit();
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//ボスとの当たり判定処理
//====================================================================
void CPlayer::CollisionBoss(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_BOSS)
			{//種類がブロックの時

				CBoss* pBoss = (CBoss*)pObj;

				D3DXVECTOR3 ObjPos = pBoss->GetPos();
				float ObjWight = pBoss->GetWight();

				if (pBoss->GetAction() == CBoss::ACTION_ATTACK)
				{
					if (CollisionCircle(m_pos, ObjPos, COLLISION_SIZE.x + ObjWight) == true)
					{
						HitDamage(BOSS_DAMAGE);
					}
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//ボスとの当たり判定処理
//====================================================================
void CPlayer::CollisionBossEvent(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_BOSS)
			{//種類がブロックの時

				CBoss* pBoss = (CBoss*)pObj;

				D3DXVECTOR3 ObjPos = pBoss->GetPos();
				float ObjWight = pBoss->GetWight();

				if (pBoss->GetAction() == CBoss::ACTION_EVENT)
				{
					if (CollisionCircle(m_pos, ObjPos, 300.0f) == true)
					{
						CGame::EventStart();
					}
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//影の当たり判定処理
//====================================================================
bool CPlayer::CollisionShadow(void)
{
	float BlockPosY = -10000.0f;
	bool bShadow = false;;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_CUBEBLOCK)
			{//種類がブロックの時
				CCubeBlock* pBlock = (CCubeBlock*)pObj;

				D3DXVECTOR3 MyPos = GetPos();
				D3DXVECTOR3 MySize = COLLISION_SIZE * 2.0f;
				D3DXVECTOR3 BlockPos = pBlock->GetPos();
				D3DXVECTOR3 BlockSize = pBlock->GetSize();
				D3DXVECTOR3 BlockMove = pBlock->GetMove();

				if (BlockPos.y + BlockSize.y > BlockPosY)
				{
					if (BlockPos.x + BlockSize.x > MyPos.x &&
						BlockPos.x - BlockSize.x < MyPos.x &&
						BlockPos.z + BlockSize.z > MyPos.z &&
						BlockPos.z - BlockSize.z < MyPos.z &&
						BlockPos.y - BlockSize.y < MyPos.y)
					{
						m_pShadow->SetPos(D3DXVECTOR3(
							MyPos.x,
							BlockPos.y + BlockSize.y + BlockMove.y,
							MyPos.z));
						m_pShadow->SetWight(MySize.x);
						m_pShadow->SetHeight(MySize.z);

						BlockPosY = BlockPos.y + BlockSize.y;
						bShadow = true;
					}
				}
			}

			pObj = pObjNext;
		}
	}

	if (bShadow == false)
	{
		m_pShadow->SetWight(0.0f);
		m_pShadow->SetHeight(0.0f);
	}

	return false;
}

//====================================================================
//描画処理
//====================================================================
void CPlayer::Draw(void)
{
	if (m_State != STATE_DEATH)
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
		D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//モデルの描画(全パーツ)
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			m_apModel[nCntModel]->Draw();
		}
	}
}

//====================================================================
//ロード処理
//====================================================================
void CPlayer::LoadLevelData(const char* pFilename)
{
	FILE* pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		int ModelParent = 0;
		D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char ModelName[128] = {};
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//ゴミ箱
		char aMessage[128] = {};			//スタートとエンドのメッセージ
		char aBool[128] = {};				//bool変換用メッセージ

		// 読み込み開始-----------------------------------------------------
		while (1)
		{//「SCRIPT」を探す
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// モデル数読み込み-----------------------------------------------------
				while (1)
				{//「NUM_MODEL」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &m_nNumModel);		//モデル数の設定
						break;
					}
				}

				//モデルファイルの読み込み
				while (1)
				{//「MODEL_FILENAME」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelName[0]);		//読み込むモデルのパスを取得

						m_apModel[nCntModel] = CModel::Create(&ModelName[0]);
						nCntModel++;
					}
					if (nCntModel >= m_nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// キャラクター情報読み込み-----------------------------------------------------
				while (1)
				{//「PARTSSET」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nCntModel);	//インデックスを設定
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//親モデルのインデックスを設定

								if (ModelParent == -1)
								{
									m_apModel[nCntModel]->SetParent(NULL);
								}
								else
								{
									m_apModel[nCntModel]->SetParent(m_apModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.y);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.z);				//位置(オフセット)の初期設定

								m_apModel[nCntModel]->SetPos(ModelPos);
								m_apModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.y);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.z);				////向きの初期設定

								m_apModel[nCntModel]->SetRot(ModelRot);
								m_apModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_nNumModel)
						{
							break;
						}
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}