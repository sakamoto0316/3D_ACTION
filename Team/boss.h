//============================================
//
//	�{�X�̏��� [boss.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "objectX.h"

//�O���錾
class CObjGauge2D;
class CNumber;

//�I�u�W�F�N�g�v���C���[�N���X
class CBoss : public CObjectX
{
public:

	CBoss(int nPriority = 3);
	~CBoss();

	//�{�X�̏��
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX,
	}STATE;

	//�{�X�̍s��
	typedef enum
	{
		ACTION_NORMAL = 0,
		ACTION_ATTACK,
		ACTION_WARP,
		ACTION_MAX,
	}ACTION;

	//�{�X�̍U��
	typedef enum
	{
		ATTACK_NOT = 0,
		ATTACK_BULLET,
		ATTACK_RUSH,
		ATTACK_BLOCKRUN,
		ATTACK_MAX,

	}ATTACK;

	static CBoss* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool bX);
	void HitDamage(float Damage);
	void SetLifeUI(void);
	ACTION GetAction(void) { return m_Action; }

private:
	void StateManager(void);				//��ԊǗ�
	void WarpUpdate(D3DXVECTOR3* pos);		//���[�v�X�V����
	void Warp(ATTACK Pattern);				//���[�v�ʒu�w�菈��
	void AttackUpdate(D3DXVECTOR3* pos);	//�U���X�V����
	void AttackBullet(D3DXVECTOR3* pos);	//�U���p�^�[���ˌ�
	void AttackRush(D3DXVECTOR3* pos);		//�U���p�^�[���ːi
	void AttackBlockRun(D3DXVECTOR3* pos);	//�U���p�^�[���u���b�N����

	int m_nIdxXModel;				//X���f���̔ԍ�
	D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	D3DXVECTOR3 m_WarpPos;			//���[�v��̈ʒu
	bool m_bCollision;				//�����蔻��p�̍��W
	int m_nIdxTexture;				//�e�N�X�`���̔ԍ�
	float m_MoveCount;				//�ҋ@���̕��V�p�ϐ�
	STATE m_State;					//���
	ACTION m_Action;				//�s��
	int m_nStateCount;				//��ԊǗ��p�ϐ�
	bool m_nWarpOK;					//���[�v�O�����[�v�ォ
	float m_ColorA;					//�s�����x
	ATTACK m_AttackPattern;			//�U���p�^�[��
	int m_AttackCount;				//�U���Ɏg���J�E���g
	int m_AttackWave;				//�U���i�K

	D3DXVECTOR3 m_move;				//�ړ���	
	D3DXVECTOR3 m_rot;				//����	
	float m_fLife;					//�{�X�̃��C�t
	float m_fLifeMax;				//�{�X�̃��C�t�̍ő�l
	CObjGauge2D* m_pLifeGauge;		//���C�t�Q�[�W�̃|�C���^
	CNumber* m_pLifeNumber[5];		//���C�t�pUI
};
#endif