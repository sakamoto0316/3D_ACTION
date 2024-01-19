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

//�}�N����`
#define BLOCK_WIGHT (300.0f)		//����
#define BLOCK_HEIGHT (300.0f)		//�c��
#define WALL_UPEND (20.0f)			//�㏸�ʒu
#define BOSS_LIFE (300.0f)			//�㏸�ʒu

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
	m_pLifeGauge->SetPos(D3DXVECTOR3(20.0f, 60.0f, 0.0f));
	m_pLifeGauge->SetWight(800.0f);
	m_pLifeGauge->SetHeight(50.0f);
	m_pLifeGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fLife);

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

	m_MoveCount += 0.04f;

	m_move.y = sinf(m_MoveCount) * 2.0f;

	//�ʒu�X�V
	pos += m_move;

	SetPos(pos);

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CBoss::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//�_���[�W����
//====================================================================
void CBoss::HitDamage(float Damage)
{
	m_fLife -= Damage;
	m_pLifeGauge->SetGaugeWight(m_fLifeMax, m_fLife);
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