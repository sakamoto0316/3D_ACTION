//============================================
//
//	敵の処理 [enemy.cpp]
//	Author:sakamoto kai
//
//============================================
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "Particle.h"
#include "CubeBlock.h"
#include "sound.h"
#include "modelEffect.h"

//マクロ定義
#define COLLISION_SIZE (D3DXVECTOR3(20.0f,20.0f,20.0f))		//当たり判定
#define MOVE_SPEED (3.0f)									//移動速度
#define MOVE_COLLISION (250.0f)								//攻撃準備範囲
//====================================================================
//コンストラクタ
//====================================================================
CEnemy::CEnemy(int nPriority) : CObjectX(nPriority)
{
	SetWight(COLLISION_SIZE.x);
	SetHeight(COLLISION_SIZE.y);
	m_nIdxXModel = NULL;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_Action = ACTION_NORMAL;
	m_nStateCount = 0;
	m_ColorA = 1.0f;
	m_AttackCount = 0;
	m_AttackWave = 0;
	m_AttackCoolTime = 0;
	m_Scaling = 1.0f;
	m_bRevivalColorSwitch = false;
	m_AttackPattern = ATTACK_RUSH;
}

//====================================================================
//デストラクタ
//====================================================================
CEnemy::~CEnemy()
{

}

//====================================================================
//生成処理
//====================================================================
CEnemy* CEnemy::Create(char* pModelName)
{
	CEnemy* pBlock = NULL;

	if (pBlock == NULL)
	{
		//オブジェクト2Dの生成
		pBlock = new CEnemy();
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
HRESULT CEnemy::Init(char* pModelName)
{
	SetType(CObject::TYPE_ENEMY3D);

	CObjectX::Init(pModelName);

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
		break;

	case CScene::MODE_RESULT:
		break;
	}


	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CEnemy::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CEnemy::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CEnemy::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//位置更新
	pos += m_move;

	SetPos(pos);

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CEnemy::GameUpdate(void)
{
	//更新前の位置を過去の位置とする
	m_posOld = m_pos;

	//通常時の動き
	if (m_Action == ACTION_NORMAL)
	{
		D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

		m_rot.y = atan2f(m_pos.x - PlayerPos.x, m_pos.z - PlayerPos.z);

		m_move.x = sinf(m_rot.y + D3DX_PI) * MOVE_SPEED;
		m_move.z = cosf(m_rot.y + D3DX_PI) * MOVE_SPEED;
	}

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

	ActionManager();


	//減衰係数
	if (m_bJump == false)
	{
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

	if (m_AttackCoolTime > 0)
	{
		m_AttackCoolTime--;
	}
	else
	{
		AttackCollision();
	}

	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//画面外判定
	if (m_pos.y < 0.0f)
	{
		HitDamage(1.0f);
	}

	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//状態管理
	StateManager();

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CEnemy::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//行動管理
//====================================================================
void CEnemy::ActionManager(void)
{
	switch (m_Action)
	{
	case CEnemy::ACTION_NORMAL:
		break;

	case CEnemy::ACTION_ATTACK:
		AttackUpdate();
		break;

	case CEnemy::ACTION_EVENT:
		break;

	default:
		break;
	}
}

//====================================================================
//状態管理
//====================================================================
void CEnemy::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case ACTION_EVENT:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//攻撃更新処理
//====================================================================
void CEnemy::AttackUpdate(void)
{
	SetMatColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	switch (m_AttackPattern)
	{
	case ATTACK_RUSH:
		AttackRush();
		break;
	}
}

//====================================================================
//攻撃(突進)
//====================================================================
void CEnemy::AttackRush(void)
{
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	switch (m_AttackWave)
	{
	case 0:
		m_AttackCount++;

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
			m_Scaling = 1.0f;
			m_AttackWave++;
			m_AttackCount = 0;
		}

		break;

	case 1:

		m_rot.y = atan2f(m_pos.x - PlayerPos.x, m_pos.z - PlayerPos.z);

		m_move.x = sinf(m_rot.y + D3DX_PI) * MOVE_SPEED * 2.0f;
		m_move.y = 20.0f;
		m_move.z = cosf(m_rot.y + D3DX_PI) * MOVE_SPEED * 2.0f;
		m_bJump = true;

		m_AttackWave++;

		break;

	case 2:
		m_AttackCount++;

		if (m_AttackCount % 5 == 0)
		{
			//モデルエフェクトの生成
			CModelEffect* pMEffect = CModelEffect::Create("data\\MODEL\\enemy.x");
			pMEffect->SetPos(m_pos);
			pMEffect->SetRot(m_rot);
			pMEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (m_bJump == false)
		{
			m_AttackWave++;
		}

		break;

	default:
		m_move = INITVECTOR3;
		m_Action = ACTION_NORMAL;
		SetDefColor();
		m_AttackWave = 0;
		m_AttackCount = 0;
		m_AttackCoolTime = 60;
		break;
	}
}

//====================================================================
//ダメージ処理
//====================================================================
void CEnemy::HitDamage(float Damage)
{
	if (m_State == STATE_NORMAL)
	{
		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE_BOSS);

		//削除
		Uninit();
	}
}

//====================================================================
//キューブブロックとの当たり判定処理
//====================================================================
bool CEnemy::CollisionBlock(D3DXVECTOR3* pos, COLLISION XYZ)
{
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

				if (pObj->CollisionBlock(pos, m_posOld, &m_move, &m_Objmove, COLLISION_SIZE * 0.5f, &m_bJump, XYZ) == true)
				{
					m_bJump = false;
					m_move.y = 0.0f;

					return true;
				}
			}

			pObj = pObjNext;
		}
	}

	return false;
}

//====================================================================
//当たり判定処理
//====================================================================
void CEnemy::AttackCollision(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_PLAYER3D)
			{//種類がプレイヤーの時
				if (sqrtf((pObj->GetPos().x - m_pos.x) * (pObj->GetPos().x - m_pos.x)
					+ (pObj->GetPos().z - m_pos.z) * (pObj->GetPos().z - m_pos.z)) <= MOVE_COLLISION)
				{//円の判定が当たった
					m_Action = ACTION_ATTACK;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//当たり判定処理
//====================================================================
bool CEnemy::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool bX)
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