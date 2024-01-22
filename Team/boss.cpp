//============================================
//
//	�{�X�̏��� [boss.cpp]
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

//�}�N����`
#define BLOCK_WIGHT (300.0f)		//����
#define BLOCK_HEIGHT (300.0f)		//�c��
#define WALL_UPEND (20.0f)			//�㏸�ʒu
#define BOSS_LIFE (30000.0f)		//�{�X�̗̑�

//====================================================================
//�R���X�g���N�^
//====================================================================
CBoss::CBoss(int nPriority) : CObjectX(nPriority)
{
	m_nIdxXModel = NULL;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_MoveCount = 0.0f;
	m_fLife = BOSS_LIFE;
	m_fLifeMax = m_fLife;
	m_State = STATE_NORMAL;
	m_Action = ACTION_NORMAL;
	m_nStateCount = 0;
	m_ColorA = 1.0f;
	m_BulletCount = 0;

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		m_pLifeNumber[nCnt] = nullptr;
	}
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CBoss::~CBoss()
{

}

//====================================================================
//��������
//====================================================================
CBoss* CBoss::Create(char* pModelName)
{
	CBoss* pBlock = NULL;

	if (pBlock == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pBlock = new CBoss();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pBlock->Init(pModelName)))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pBlock;
}

//====================================================================
//����������
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
	m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fLife);

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
//�I������
//====================================================================
void CBoss::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//�X�V����
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
		//���[�v����
		WarpUpdate(&pos);
	}
	else if (m_Action == ACTION_ATTACK)
	{
		AttackBullet(&pos);
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

	//�ʒu�X�V
	pos += m_move;

	SetPos(pos);
	SetRot(m_rot);

	//��ԊǗ�
	StateManager();

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�`�揈��
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
//��ԊǗ�
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
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//���[�v����
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
			else
			{
				m_Action = ACTION_ATTACK;
			}
		}
	}
}

//====================================================================
//���[�v�ʒu�w�菈��
//====================================================================
void CBoss::Warp(ATTACK Pattern)
{
	m_Action = ACTION_WARP;
	m_AttackPattern = Pattern;

	switch (m_AttackPattern)
	{
	case ATTACK_NOT:

		m_WarpPos.x = (float)(rand() % 1001) - 500.0f;
		m_WarpPos.y = (float)(rand() % 201) + 250.0f;
		m_WarpPos.z = (float)(rand() % 1001) - 500.0f;
		break;

	case ATTACK_BULLET:

		m_BulletCount = 0;
		m_rot.y = 0.0f;
		m_WarpPos.x = sinf(m_rot.y) * 500.0f;
		m_WarpPos.y = 400.0f;
		m_WarpPos.z = cosf(m_rot.y) * 500.0f;
		break;
	}
}

//====================================================================
//�U��(�ˌ�)
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

		m_BulletCount++;
		if (m_BulletCount % 30 == 0)
		{
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
			}
		}
	}
	else
	{
		Warp(ATTACK_NOT);
		SetDefColor();
	}
}

//====================================================================
//�_���[�W����
//====================================================================
void CBoss::HitDamage(float Damage)
{
	m_fLife -= Damage;
	m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fLife);
	m_State = STATE_DAMAGE;
	m_nStateCount = 5;
	SetMatColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	SetLifeUI();
}

//====================================================================
//���C�tUI�̍X�V
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
//�����蔻�菈��
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
		{//�ォ��߂荞�񂾎�
			bOn = true;
			pPos->y = mtPos.y + myHeight;		//�u���b�N�̂����ɂ�������
			pMove->y = 0.0f;					//�ړ��ʂ��O�ɂ���
			*bHit = true;
		}

		if (
			(pPos->y + fHeight > mtPos.y - myHeight &&
				pPosOld.y <= mtPos.y - myHeight) &&
			pPos->x + fWidth > mtPos.x - myWight &&
			pPos->x - fWidth < mtPos.x + myWight
			)
		{//������߂荞�񂾎�
			*bJumpMove = false;
			pPos->y = mtPos.y - myHeight - fHeight;		//�u���b�N�̉��Ŏ~�߂�
			pMove->y = 0.0f;					//�ړ��ʂ��O�ɂ���
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
		{//������߂荞�񂾎�
			pPos->x = mtPos.x - myWight - fWidth;		//�u���b�N�̍��Ŏ~�߂�
			pMove->x = 0.0f;					//�ړ��ʂ��O�ɂ���
			*bHit = true;
		}

		if (
			(pPos->x - fWidth < mtPos.x + myWight &&
				pPosOld.x - fWidth >= mtPos.x + myWight) &&
			pPos->y + fHeight > mtPos.y - myHeight &&
			pPos->y < mtPos.y + myHeight
			)
		{//�E����߂荞�񂾎�
			pPos->x = mtPos.x + myWight + fWidth;		//�u���b�N�̉E�Ŏ~�߂�
			pMove->x = 0.0f;					//�ړ��ʂ��O�ɂ���
			*bHit = true;
		}
	}

	return bOn;
}