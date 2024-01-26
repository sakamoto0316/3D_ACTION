//============================================
//
//	ボスの処理 [boss.cpp]
//	Author:sakamoto kai
//
//============================================
#include "boss.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "Particle.h"
#include "objGauge2D.h"
#include "number.h"
#include "input.h"
#include "CubeBlock.h"
#include "CubeDamage.h"
#include "sound.h"

//マクロ定義
#define BLOCK_WIGHT (300.0f)		//横幅
#define BLOCK_HEIGHT (300.0f)		//縦幅
#define WALL_UPEND (20.0f)			//上昇位置
#define BOSS_LIFE (5000.0f)			//ボスの体力
#define BOSS_LIFESPEED_PLUS (128.0f)//体力増の移動量
#define BOSS_LIFESPEED_DEL (64.0f)	//体力減の移動量
#define BULLET_INTERVAL (20)		//弾の発射感覚
#define COLLISION_SIZE (D3DXVECTOR3(90.0f,90.0f,90.0f))		//当たり判定
#define BULLET_LIFE (600)			//弾の寿命
#define BLOCKRUN_TIME (600)			//ブロックランの時間
#define BLOCKRUN_SPEED (5.0f)		//ブロックランの速度
#define COOLTIME_BULLET (240)		//弾攻撃のクールタイム
#define BULLET_DAMAGE (50.0f)		//弾のダメージ量
#define RUSH_DAMAGE (90.0f)			//突進攻撃のダメージ
#define BLOCKRUN_DAMAGE (60.0f)		//ブロックランのダメージ
#define SPINPILLAR_DAMAGE (60.0f)	//回転柱のダメージ
#define COOLTIME_RUSH (180)			//突進攻撃のクールタイム
#define COOLTIME_BLOCKRUN (300)		//ブロックランのクールタイム
#define COOLTIME_SPINPILLAR (30)	//回転柱のクールタイム
#define COOLTIME_RAIN (240)			//雨攻撃のクールタイム

//====================================================================
//コンストラクタ
//====================================================================
CBoss::CBoss(int nPriority) : CObjectX(nPriority)
{
	SetWight(COLLISION_SIZE.x);
	SetHeight(COLLISION_SIZE.y);
	m_nIdxXModel = NULL;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_MoveCount = 0.0f;
	m_fLife = BOSS_LIFE;
	m_fMoveLife = 0.0f;
	m_bDelLife = false;
	m_fLifeMax = m_fLife;
	m_State = STATE_NORMAL;
	m_Action = ACTION_NORMAL;
	m_nStateCount = 0;
	m_ColorA = 1.0f;
	m_AttackCount = 0;
	m_AttackWave = 0;
	m_AttackCoolTime = 0;
	m_SpinCount = 0.0f;
	m_Scaling = 1.0f;
	m_nForm = 0;
	m_bRevivalColorSwitch = false;

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		m_pLifeNumber[nCnt] = nullptr;
	}
}

//====================================================================
//デストラクタ
//====================================================================
CBoss::~CBoss()
{

}

//====================================================================
//生成処理
//====================================================================
CBoss* CBoss::Create(char* pModelName)
{
	CBoss* pBlock = NULL;

	if (pBlock == NULL)
	{
		//オブジェクト2Dの生成
		pBlock = new CBoss();
	}

	//オブジェクトの初期化処理
	if (FAILED(pBlock->Init(pModelName)))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pBlock;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CBoss::Init(char* pModelName)
{
	SetType(CObject::TYPE_BOSS);

	CObjectX::Init(pModelName);

	m_pLifeGauge = CObjGauge2D::Create();
	m_pLifeGauge->SetPos(D3DXVECTOR3(240.0f, 20.0f, 0.0f));
	m_pLifeGauge->SetWight(800.0f);
	m_pLifeGauge->SetHeight(10.0f);
	m_pLifeGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fMoveLife);

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		m_pLifeNumber[nCnt] = CNumber::Create();
		m_pLifeNumber[nCnt]->SetPos(D3DXVECTOR3(1000.0f + nCnt * 15.0f, 50.0f, 0.0f));
		m_pLifeNumber[nCnt]->SetWight(20.0f);
		m_pLifeNumber[nCnt]->SetHeight(20.0f);
	}
	SetLifeUI();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CBoss::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CBoss::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	if (m_Action != ACTION_ATTACK)
	{
		m_MoveCount += 0.04f;

		m_move.y = sinf(m_MoveCount) * 2.0f;
	}

	if (m_Action == ACTION_WARP)
	{
		//ワープ処理
		WarpUpdate(&pos);
	}
	else if (m_Action == ACTION_REVIVAL)
	{
		//蘇生処理
		AttackRevival(&pos);
	}
	else if (m_Action == ACTION_ATTACK)
	{
		AttackUpdate(&pos);
	}

	if (m_Action == ACTION_NORMAL)
	{
		if (m_AttackCoolTime > 0)
		{
			m_AttackCoolTime--;
		}
		else
		{
			AttackSelect();
		}
	}

	if(CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_6) == true)
	{
		D3DXVECTOR3 RandPos;
		RandPos.x = (float)(rand() % 1001) - 500.0f;
		RandPos.y = (float)(rand() % 201) + 250.0f;
		RandPos.z = (float)(rand() % 1001) - 500.0f;

		m_Action = ACTION_WARP;
		m_WarpPos = RandPos;
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_7) == true)
	{
		Warp(ATTACK_BULLET);
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_8) == true)
	{
		Warp(ATTACK_RAIN);
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_9) == true)
	{
		Warp(ATTACK_SPINPILLAR);
	}

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			CCubeDamage* pCubeDamage = CCubeDamage::Create();
			pCubeDamage->SetUseSpin(true);
			pCubeDamage->SetSpinPos(pos);
			pCubeDamage->SetSpinCount(D3DXVECTOR3(m_SpinCount + nCnt * D3DX_PI * 0.5f, m_SpinCount + nCnt * D3DX_PI * 0.5f, m_SpinCount + nCnt * D3DX_PI * 0.5f));
			pCubeDamage->SetSpinSpeedY(0.05f);
			pCubeDamage->SetSpinDistance(150.0f);
			pCubeDamage->SetBreak(true);
			pCubeDamage->SetLife(600);
			pCubeDamage->SetMove(D3DXVECTOR3(0.0f, 0.0f, -10.0f));
			pCubeDamage->SetSpinDisMove(5.0f);
		}
	}

	if (m_nForm == 1)
	{
		m_SpinCount += 0.075f;

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			CCubeDamage* pCubeDamage = CCubeDamage::Create();
			pCubeDamage->SetUseSpin(true);
			pCubeDamage->SetSpinPos(pos);
			pCubeDamage->SetSpinCount(D3DXVECTOR3(m_SpinCount + nCnt * D3DX_PI * 0.2f,  m_SpinCount + nCnt * D3DX_PI * 0.25f, m_SpinCount + nCnt * D3DX_PI * 0.2f));
			pCubeDamage->SetSpinDistance(350.0f);
			pCubeDamage->SetSpinSpeedY(0.05f);
			pCubeDamage->SetBreak(false);
			pCubeDamage->SetLife(1);
			pCubeDamage->SetDamage(50.0f);
		}
	}

	//位置更新
	pos += m_move;

	SetPos(pos);
	SetRot(m_rot);

	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//ライフゲージの位置管理
	if (m_pLifeGauge != nullptr)
	{
		if (m_bDelLife == false)
		{
			m_fMoveLife += BOSS_LIFESPEED_PLUS;

			if (m_fMoveLife >= m_fLife)
			{
				m_fMoveLife = m_fLife;
				m_bDelLife = true;
			}
		}
		else
		{
			m_fMoveLife -= BOSS_LIFESPEED_DEL;

			if (m_fMoveLife <= m_fLife)
			{
				m_fMoveLife = m_fLife;
			}
		}

		m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fMoveLife);
	}

	//状態管理
	StateManager();

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CBoss::Draw(void)
{
	if (m_Action == ACTION_WARP)
	{
		CObjectX::Draw(m_ColorA);
	}
	else
	{
		CObjectX::Draw();
	}
}

//====================================================================
//状態管理
//====================================================================
void CBoss::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;

	case STATE_DAMAGE:
		if (m_nStateCount <= 0)
		{
			m_State = STATE_NORMAL;
			m_nStateCount = 0;
			SetDefColor();
		}
		break;

	case STATE_DEATH:
		if (m_nStateCount <= 0)
		{
			m_State = STATE_NORMAL;
			m_nStateCount = 0;
		}
		break;
	case STATE_INVINCIBLE:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//ワープ処理
//====================================================================
void CBoss::WarpUpdate(D3DXVECTOR3 *pos)
{
	if (m_nWarpOK == false)
	{
		m_ColorA -= 0.1f;

		if (m_ColorA <= 0)
		{
			m_ColorA = 0;
			*pos = m_WarpPos;
			m_nWarpOK = true;
		}
	}
	else
	{
		m_ColorA += 0.1f;

		if (m_ColorA >= 1.0f)
		{
			m_ColorA = 1.0f;
			m_nWarpOK = false;
			SetDefColor();
			
			if (m_AttackPattern == ATTACK_NOT)
			{
				m_Action = ACTION_NORMAL;
			}
			else if (m_AttackPattern == ATTACK_REVIVAL)
			{
				m_Action = ACTION_REVIVAL;
			}
			else
			{
				m_Action = ACTION_ATTACK;

				//ゲームのSEを再生する
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WARNING);
			}
		}
	}
}

//====================================================================
//ワープ位置指定処理
//====================================================================
void CBoss::Warp(ATTACK Pattern)
{
	m_Action = ACTION_WARP;
	m_AttackPattern = Pattern;
	m_AttackCount = 0;
	m_AttackWave = 0;
	CPlayer* pPlayer = CGame::GetPlayer();
	int Rand;

	switch (m_AttackPattern)
	{
	case ATTACK_NOT:

		m_WarpPos.x = (float)(rand() % 1001) - 500.0f;
		m_WarpPos.y = (float)(rand() % 101) + 250.0f;
		m_WarpPos.z = (float)(rand() % 1001) - 500.0f;
		m_AttackCoolTime = 5;
		break;

	case ATTACK_BULLET:

		m_rot.y = 0.0f;
		m_WarpPos.x = sinf(m_rot.y) * 500.0f;
		m_WarpPos.y = 400.0f;
		m_WarpPos.z = cosf(m_rot.y) * 500.0f;

		break;

	case ATTACK_RUSH:

		Rand = rand() % 4;
		m_WarpPos.y = 400.0f;
		m_move = INITVECTOR3;

		switch (Rand)
		{
		case 0:
			m_rot.y = 0.0f;
			m_WarpPos.x = pPlayer->GetPos().x;
			m_WarpPos.z = 500.0f;
			break;
		case 1:
			m_rot.y = D3DX_PI;
			m_WarpPos.x = pPlayer->GetPos().x;
			m_WarpPos.z = -500.0f;
			break;
		case 2:
			m_rot.y = D3DX_PI * 0.5f;
			m_WarpPos.x = 500.0f;
			m_WarpPos.z = pPlayer->GetPos().z;
			break;
		case 3:
			m_rot.y = D3DX_PI * -0.5f;
			m_WarpPos.x = -500.0f;
			m_WarpPos.z = pPlayer->GetPos().z;
			break;
		}
		break;

	case ATTACK_BLOCKRUN:

		m_move = INITVECTOR3;

		m_rot.y = D3DX_PI;
		m_WarpPos.x = 0.0f;
		m_WarpPos.y = 550.0f;
		m_WarpPos.z = -500.0f;
		break;

	case ATTACK_SPINPILLAR:

		m_WarpPos.x = (float)(rand() % 1001) - 500.0f;
		m_WarpPos.y = 250.0f;
		m_WarpPos.z = (float)(rand() % 1001) - 500.0f;

		break;

	case ATTACK_RAIN:

		m_WarpPos.x = 0.0f;
		m_WarpPos.y = 500.0f;
		m_WarpPos.z = 0.0f;

		break;
	}
}

//====================================================================
//攻撃更新処理
//====================================================================
void CBoss::AttackUpdate(D3DXVECTOR3* pos)
{
	SetMatColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	switch (m_AttackPattern)
	{
	case ATTACK_BULLET:
		AttackBullet(pos);
		break;
	case ATTACK_RUSH:
		AttackRush(pos);
		break;
	case ATTACK_BLOCKRUN:
		AttackBlockRun(pos);
		break;
	case ATTACK_SPINPILLAR:
		AttackSpinPillar(pos);
		break;
	case ATTACK_RAIN:
		AttackRain(pos);
		break;
	}
}

//====================================================================
//攻撃選択処理
//====================================================================
void CBoss::AttackSelect(void)
{
	int RandAttack = rand() % 201;

	if (RandAttack <= 10)
	{
		Warp(ATTACK_BULLET);
	}
	else if (RandAttack <= 40)
	{
		Warp(ATTACK_RUSH);
	}
	else if (RandAttack <= 50)
	{
		Warp(ATTACK_BLOCKRUN);
	}
	else if (RandAttack <= 85)
	{
		Warp(ATTACK_SPINPILLAR);
	}
	else if (RandAttack <= 100)
	{
		Warp(ATTACK_RAIN);
	}
	else if (RandAttack <= 200)
	{
		Warp(ATTACK_NOT);
	}
}

//====================================================================
//攻撃(射撃)
//====================================================================
void CBoss::AttackBullet(D3DXVECTOR3* pos)
{
	CPlayer *pPlayer = CGame::GetPlayer();

	if (m_rot.y < D3DX_PI * 2.0f)
	{
		m_rot.y += 0.015f;

		pos->x = sinf(m_rot.y) * 500.0f;
		pos->z = cosf(m_rot.y) * 500.0f;

		m_MoveCount += 0.04f;
		m_move.y = sinf(m_MoveCount) * 2.0f;

		m_AttackCount++;
		if (m_AttackCount % BULLET_INTERVAL == 0)
		{
			//ゲームのSEを再生する
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_BULLET);

			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				CCubeDamage* pCubeDamage = CCubeDamage::Create();
				pCubeDamage->SetPos(*pos);
				switch (nCnt)
				{
				case 0:
					pCubeDamage->SetMove(D3DXVECTOR3(
						(pPlayer->GetPos().x - pos->x) * 0.01f,
						(pPlayer->GetPos().y - pos->y) * 0.01f,
						(pPlayer->GetPos().z - pos->z) * 0.01f));
					break;

				case 1:
					pCubeDamage->SetMove(D3DXVECTOR3(
						-(pPlayer->GetPos().x - pos->x) * 0.01f,
						(pPlayer->GetPos().y - pos->y) * 0.01f,
						(pPlayer->GetPos().z - pos->z) * 0.01f));
					break;

				case 2:
					pCubeDamage->SetMove(D3DXVECTOR3(
						(pPlayer->GetPos().x - pos->x) * 0.01f,
						-(pPlayer->GetPos().y - pos->y) * 0.01f,
						(pPlayer->GetPos().z - pos->z) * 0.01f));
					break;

				case 3:
					pCubeDamage->SetMove(D3DXVECTOR3(
						(pPlayer->GetPos().x - pos->x) * 0.01f,
						(pPlayer->GetPos().y - pos->y) * 0.01f,
						-(pPlayer->GetPos().z - pos->z) * 0.01f));
					break;
				case 4:
					pCubeDamage->SetMove(D3DXVECTOR3(
						-(pPlayer->GetPos().x - pos->x) * 0.01f,
						-(pPlayer->GetPos().y - pos->y) * 0.01f,
						(pPlayer->GetPos().z - pos->z) * 0.01f));
					break;

				case 5:
					pCubeDamage->SetMove(D3DXVECTOR3(
						(pPlayer->GetPos().x - pos->x) * 0.01f,
						-(pPlayer->GetPos().y - pos->y) * 0.01f,
						-(pPlayer->GetPos().z - pos->z) * 0.01f));
					break;

				case 6:
					pCubeDamage->SetMove(D3DXVECTOR3(
						-(pPlayer->GetPos().x - pos->x) * 0.01f,
						(pPlayer->GetPos().y - pos->y) * 0.01f,
						-(pPlayer->GetPos().z - pos->z) * 0.01f));
					break;

				case 7:
					pCubeDamage->SetMove(D3DXVECTOR3(
						-(pPlayer->GetPos().x - pos->x) * 0.01f,
						-(pPlayer->GetPos().y - pos->y) * 0.01f,
						-(pPlayer->GetPos().z - pos->z) * 0.01f));
					break;
				}
				pCubeDamage->SetBreak(true);
				pCubeDamage->SetDamage(BULLET_DAMAGE);
				pCubeDamage->SetLife(BULLET_LIFE);
			}
		}
	}
	else
	{
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_BULLET;
	}
}

//====================================================================
//攻撃(突進)
//====================================================================
void CBoss::AttackRush(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeDamage* pCubeDamage = nullptr;

	switch (m_AttackWave)
	{
	case 0:
		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_RUSH);

		m_move.x = -sinf(m_rot.y) * 2.0f;
		m_move.y = -10.0f;
		m_move.z = -cosf(m_rot.y) * 2.0f;

		if (pos->y < pPlayer->GetPos().y)
		{
			m_AttackWave++;
		}

		break;

	case 1:
		m_move.x = -sinf(m_rot.y) * 15.0f;
		m_move.y = 0.0f;
		m_move.z = -cosf(m_rot.y) * 15.0f;

		pCubeDamage = CCubeDamage::Create();
		pCubeDamage->SetPos(D3DXVECTOR3(
			pos->x - sinf(m_rot.y) * 100.0f,
			pos->y + 10.0f,
			pos->z - cosf(m_rot.y) * 100.0f));
		pCubeDamage->SetMove(D3DXVECTOR3(
			sinf(m_rot.y + D3DX_PI * 0.5f) * 15.0f,
			0.0f,
			cosf(m_rot.y + D3DX_PI * 0.5f) * 15.0f));
		if (m_nForm == 0)
		{
			pCubeDamage->SetSize(D3DXVECTOR3(10.0f, 50.0f, 10.0f));
			pCubeDamage->SetLife(20);
		}
		else if (m_nForm == 1)
		{
			pCubeDamage->SetSize(D3DXVECTOR3(10.0f, 100.0f, 10.0f));
			pCubeDamage->SetLife(50);
		}
		pCubeDamage->SetBreak(false);
		pCubeDamage->SetDamage(RUSH_DAMAGE);

		pCubeDamage = CCubeDamage::Create();
		pCubeDamage->SetPos(D3DXVECTOR3(
			pos->x - sinf(m_rot.y) * 100.0f,
			pos->y + 10.0f,
			pos->z - cosf(m_rot.y) * 100.0f));
		pCubeDamage->SetMove(D3DXVECTOR3(
			sinf(m_rot.y + D3DX_PI * -0.5f) * 15.0f,
			0.0f,
			cosf(m_rot.y + D3DX_PI * -0.5f) * 15.0f));
		if (m_nForm == 0)
		{
			pCubeDamage->SetSize(D3DXVECTOR3(10.0f, 50.0f, 10.0f));
			pCubeDamage->SetLife(20);
		}
		else if (m_nForm == 1)
		{
			pCubeDamage->SetSize(D3DXVECTOR3(10.0f, 100.0f, 10.0f));
			pCubeDamage->SetLife(50);
		}
		pCubeDamage->SetBreak(false);
		pCubeDamage->SetDamage(RUSH_DAMAGE);

		if (pos->x > 500.0f ||
			pos->x < -500.0f ||
			pos->z > 500.0f ||
			pos->z < -500.0f)
		{
			m_AttackWave++;
		}

		break;

	case 2:
		m_move.x = -sinf(m_rot.y) * 2.0f;
		m_move.y = 10.0f;
		m_move.z = -cosf(m_rot.y) * 2.0f;

		if (pos->y > 400.0f)
		{
			m_AttackWave++;
		}

		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_RUSH;
		break;
	}
}

//====================================================================
//攻撃(ブロックラン)
//====================================================================
void CBoss::AttackBlockRun(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeBlock* pCubeBlock = nullptr;
	CCubeDamage* pCubeDamage = nullptr;
	int nRand = rand() % 4;

	m_MoveCount += 0.04f;
	m_move.y = sinf(m_MoveCount) * 2.0f;

	switch (m_AttackWave)
	{
	case 0:

		pCubeBlock = CCubeBlock::Create();
		pCubeBlock->SetPos(INITVECTOR3);
		pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 10.0f, 500.0f));
		pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
		pCubeBlock->SetUninitPos(D3DXVECTOR3(pCubeBlock->GetPos().x, 500.0f, pCubeBlock->GetPos().z));
		pCubeBlock->SetBoolLife(true);
		pCubeBlock->SetPlusMove(true);

		m_AttackWave++;
		m_AttackCount++;

		break;

	case 1:

		m_AttackCount++;

		if (m_AttackCount % 60 == 0)
		{
			pCubeBlock = CCubeBlock::Create();
			pCubeBlock->SetPos(D3DXVECTOR3(0.0f, 200.0f, -0.0f));
			pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 50.0f, 500.0f));
			pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, BLOCKRUN_SPEED));
			pCubeBlock->SetUninitPos(D3DXVECTOR3(pCubeBlock->GetPos().x, pCubeBlock->GetPos().y, 1000.0f));
			pCubeBlock->SetBoolLife(true);
			pCubeBlock->SetPlusMove(true);

			pCubeBlock = CCubeBlock::Create();
			pCubeBlock->SetPos(D3DXVECTOR3(0.0f, 300.0f, 500.0f));
			pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 50.0f, 10.0f));
			pCubeBlock->SetLife(BLOCKRUN_TIME);

			pCubeDamage = CCubeDamage::Create();
			pCubeDamage->SetPos(D3DXVECTOR3(0.0f, 400.0f, 490.0f));
			pCubeDamage->SetSize(D3DXVECTOR3(500.0f, 300.0f, 10.0f));
			pCubeDamage->SetLife(BLOCKRUN_TIME);
			pCubeDamage->SetBreak(false);
			pCubeDamage->SetDamage(false);

			pCubeDamage = CCubeDamage::Create();
			pCubeDamage->SetPos(D3DXVECTOR3(0.0f, 120.0f, 0.0f));
			pCubeDamage->SetSize(D3DXVECTOR3(500.0f, 10.0f, 500.0f));
			pCubeDamage->SetLife(BLOCKRUN_TIME);
			pCubeDamage->SetBreak(false);
			pCubeDamage->SetDamage(BLOCKRUN_DAMAGE);

			m_AttackWave++;
		}

		break;

	case 2:

		m_AttackCount++;

		if (m_AttackCount % 30 == 0)
		{
			pCubeBlock = CCubeBlock::Create();

			switch (nRand)
			{
			case 0:
				pCubeBlock->SetPos(D3DXVECTOR3(-125.0f, 200.0f, -500.0f));
				break;
			case 1:
				pCubeBlock->SetPos(D3DXVECTOR3(-375.0f, 200.0f, -500.0f));
				break;
			case 2:
				pCubeBlock->SetPos(D3DXVECTOR3(125.0f, 200.0f, -500.0f));
				break;
			case 3:
				pCubeBlock->SetPos(D3DXVECTOR3(375.0f, 200.0f, -500.0f));
				break;
			}
			if (m_nForm == 0)
			{
				pCubeBlock->SetSize(D3DXVECTOR3(125.0f, 50.0f, 125.0f));
			}
			else if (m_nForm == 1)
			{
				pCubeBlock->SetSize(D3DXVECTOR3(75.0f, 50.0f, 75.0f));
			}
			pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, BLOCKRUN_SPEED));
			pCubeBlock->SetUninitPos(D3DXVECTOR3(pCubeBlock->GetPos().x, pCubeBlock->GetPos().y, 625.0f));
			pCubeBlock->SetBoolLife(true);
			pCubeBlock->SetPlusMove(true);
		}

		if (m_AttackCount > BLOCKRUN_TIME)
		{
			m_AttackWave++;
		}

		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_BLOCKRUN;
		break;
	}
}

//====================================================================
//攻撃(柱の回転)
//====================================================================
void CBoss::AttackSpinPillar(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeBlock* pCubeBlock = nullptr;
	CCubeDamage* pCubeDamage = nullptr;
	m_move = INITVECTOR3;

	switch (m_AttackWave)
	{
	case 0:

		if (m_AttackCount % 5 == 0)
		{
			m_Scaling += 0.02f;
		}
		else
		{
			m_Scaling -= 0.02f;
		}

		if (m_Scaling <= 0.5f)
		{
			m_AttackWave++;
			m_AttackCount = 0;
		}

		m_AttackCount++;
		break;

	case 1:
		m_Scaling += 0.1f;

		if (m_Scaling >= 1.0f)
		{
			m_Scaling = 1.0f;
			m_AttackWave++;

			//ゲームのSEを再生する
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_BULLET);

			if (m_nForm == 0)
			{
				for (int nCnt = 0; nCnt < 4; nCnt++)
				{
					CCubeDamage* pCubeDamage = CCubeDamage::Create();
					pCubeDamage->SetUseSpin(true);
					pCubeDamage->SetSpinPos(*pos);
					pCubeDamage->SetSpinCount(D3DXVECTOR3(m_SpinCount + nCnt * D3DX_PI * 0.5f, m_SpinCount + nCnt * D3DX_PI * 0.5f, m_SpinCount + nCnt * D3DX_PI * 0.5f));
					pCubeDamage->SetSpinSpeedY(0.015f);
					pCubeDamage->SetSpinDistance(100.0f);
					pCubeDamage->SetBreak(true);
					pCubeDamage->SetLife(900);
					pCubeDamage->SetSpinDisMove(3.0f);
					pCubeDamage->SetSize(D3DXVECTOR3(20.0f, 5000.0f, 20.0f));
					pCubeDamage->SetDamage(SPINPILLAR_DAMAGE);
				}
			}
			else if (m_nForm == 1)
			{
				for (int nCnt = 0; nCnt < 8; nCnt++)
				{
					CCubeDamage* pCubeDamage = CCubeDamage::Create();
					pCubeDamage->SetUseSpin(true);
					pCubeDamage->SetSpinPos(*pos);
					pCubeDamage->SetSpinCount(D3DXVECTOR3(m_SpinCount + nCnt * D3DX_PI * 0.25f, m_SpinCount + nCnt * D3DX_PI * 0.25f, m_SpinCount + nCnt * D3DX_PI * 0.25f));
					pCubeDamage->SetSpinSpeedY(0.015f);
					pCubeDamage->SetSpinDistance(100.0f);
					pCubeDamage->SetBreak(true);
					pCubeDamage->SetLife(900);
					pCubeDamage->SetSpinDisMove(3.0f);
					pCubeDamage->SetSize(D3DXVECTOR3(20.0f, 5000.0f, 20.0f));
					pCubeDamage->SetDamage(SPINPILLAR_DAMAGE);
				}
			}
		}
		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_SPINPILLAR;
		break;
	}
}

//====================================================================
//攻撃(雨)
//====================================================================
void CBoss::AttackRain(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeDamage* pCubeDamage = nullptr;

	switch (m_AttackWave)
	{
	case 0:

		m_move = D3DXVECTOR3(0.0f, -20.0f, 0.0f);

		if (pos->y < 200.0f)
		{
			m_AttackWave++;
		}
		break;

	case 1:

		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_RAIN);

		m_move = D3DXVECTOR3(0.0f, 20.0f, 0.0f);

		if (pos->y >= 300.0f)
		{
			m_AttackWave++;
		}
		break;

	case 2:

		m_move = INITVECTOR3;

		if (m_AttackCount % 3 == 0)
		{
			CCubeDamage* pCubeDamage = CCubeDamage::Create();
			pCubeDamage->SetPos(D3DXVECTOR3(
			(float)(rand() % 1001) - 500.0f,
			1000.0f,
			(float)(rand() % 1001) - 500.0f
			));
			pCubeDamage->SetMove(D3DXVECTOR3(0.0f, -5.0f - (float)(rand() % 4), 0.0f));
			pCubeDamage->SetBreak(true);
			pCubeDamage->SetDamage(BULLET_DAMAGE);
			pCubeDamage->SetLife(BULLET_LIFE);
		}

		if (m_AttackCount > 600)
		{
			m_AttackWave++;
		}

		m_AttackCount++;
		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_RAIN;
		break;
	}
}

//====================================================================
//攻撃(蘇生)
//====================================================================
void CBoss::AttackRevival(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeDamage* pCubeDamage = nullptr;

	switch (m_AttackWave)
	{
	case 0:
		m_MoveCount += 0.1f;
		m_move.y = sinf(m_MoveCount) * 4.0f;

		m_AttackCount++;

		if (m_AttackCount % 10 == 0)
		{
			m_bRevivalColorSwitch = m_bRevivalColorSwitch ? false : true;

			if (m_bRevivalColorSwitch == true)
			{
				SetMatColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				SetDefColor();
			}
		}

		if (m_AttackCount > 100)
		{
			m_AttackCount = 0;
			m_AttackWave++;
		}
		break;

	case 1:
		m_MoveCount += 0.1f;
		m_move.y = sinf(m_MoveCount) * 4.0f;

		m_AttackCount++;

		if (m_AttackCount % 5 == 0)
		{
			m_bRevivalColorSwitch = m_bRevivalColorSwitch ? false : true;

			if (m_bRevivalColorSwitch == true)
			{
				SetMatColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				SetDefColor();
			}
		}

		if (m_AttackCount % 5 == 0)
		{
			pos->x = (float)(rand() % 1001) - 500.0f;
			pos->y = (float)(rand() % 101) + 250.0f;
			pos->z = (float)(rand() % 1001) - 500.0f;
		}

		if (m_AttackCount > 120)
		{
			m_AttackCount = 0;
			m_AttackWave++;
		}
		break;

	case 2:
		m_move = INITVECTOR3;

		SetDefColor();

		if (m_AttackCount % 30 == 0)
		{
			pos->x = 0.0f;
			pos->y = 300.0f;
			pos->z = 0.0f;
		}

		m_AttackWave++;
		break;

	case 3:
		m_move = INITVECTOR3;

		m_Scaling -= 0.01f;

		if (m_Scaling <= 0.0f)
		{
			m_Scaling = 1.0f;
			m_AttackWave++;
			m_nForm++;
			m_fLife = BOSS_LIFE;
			m_bDelLife = false;
			SetLifeUI();
		}

		break;

	case 4:
		m_MoveCount += 0.03f;
		m_move.y = sinf(m_MoveCount) * 2.0f;

		m_AttackCount++;

		if (m_AttackCount > 100)
		{
			m_AttackWave++;
		}

		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_RAIN;
		m_State = STATE_NORMAL;
		break;
	}
}

//====================================================================
//ダメージ処理
//====================================================================
void CBoss::HitDamage(float Damage)
{
	if (m_State == STATE_NORMAL)
	{
		m_fLife -= Damage;
		if (m_fLife < 0.0f)
		{
			m_fLife = 0.0f;

			if (m_nForm == 0)
			{
				m_State = STATE_INVINCIBLE;
				Warp(ATTACK_REVIVAL);
			}
		}
		else
		{
			//ゲームのSEを再生する
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE_BOSS);

			m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fLife);
			m_State = STATE_DAMAGE;
			m_nStateCount = 5;
			SetMatColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		SetLifeUI();
	}
}

//====================================================================
//ライフUIの更新
//====================================================================
void CBoss::SetLifeUI(void)
{
	m_pLifeNumber[0]->SetNumber((int)m_fLife % 100000 / 10000);
	m_pLifeNumber[1]->SetNumber((int)m_fLife % 10000 / 1000);
	m_pLifeNumber[2]->SetNumber((int)m_fLife % 1000 / 100);
	m_pLifeNumber[3]->SetNumber((int)m_fLife % 100 / 10);
	m_pLifeNumber[4]->SetNumber((int)m_fLife % 10 / 1);
}

//====================================================================
//当たり判定処理
//====================================================================
bool CBoss::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool bX)
{
	bool bOn = false;

	float myWight = GetWight();
	float myHeight = GetHeight();
	D3DXVECTOR3 mtPos = GetPos();


	if (bX == false)
	{
		if (
			(pPos->y < mtPos.y + myHeight &&
				pPosOld.y >= mtPos.y + myHeight) &&
			pPos->x + fWidth > mtPos.x - myWight &&
			pPos->x - fWidth < mtPos.x + myWight
			)
		{//上からめり込んだ時
			bOn = true;
			pPos->y = mtPos.y + myHeight;		//ブロックのうえにたたせる
			pMove->y = 0.0f;					//移動量を０にする
			*bHit = true;
		}

		if (
			(pPos->y + fHeight > mtPos.y - myHeight &&
				pPosOld.y <= mtPos.y - myHeight) &&
			pPos->x + fWidth > mtPos.x - myWight &&
			pPos->x - fWidth < mtPos.x + myWight
			)
		{//下からめり込んだ時
			*bJumpMove = false;
			pPos->y = mtPos.y - myHeight - fHeight;		//ブロックの下で止める
			pMove->y = 0.0f;					//移動量を０にする
			*bHit = true;
		}
	}
	else
	{
		if (
			(pPos->x + fWidth > mtPos.x - myWight &&
				pPosOld.x + fWidth <= mtPos.x - myWight) &&
			pPos->y + fHeight > mtPos.y - myHeight &&
			pPos->y < mtPos.y + myHeight
			)
		{//左からめり込んだ時
			pPos->x = mtPos.x - myWight - fWidth;		//ブロックの左で止める
			pMove->x = 0.0f;					//移動量を０にする
			*bHit = true;
		}

		if (
			(pPos->x - fWidth < mtPos.x + myWight &&
				pPosOld.x - fWidth >= mtPos.x + myWight) &&
			pPos->y + fHeight > mtPos.y - myHeight &&
			pPos->y < mtPos.y + myHeight
			)
		{//右からめり込んだ時
			pPos->x = mtPos.x + myWight + fWidth;		//ブロックの右で止める
			pMove->x = 0.0f;					//移動量を０にする
			*bHit = true;
		}
	}

	return bOn;
}