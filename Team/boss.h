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
class CCubeSpin;

//�I�u�W�F�N�g�v���C���[�N���X
class CBoss : public CObjectX
{
public:

	CBoss(int nPriority = 3);
	~CBoss();

	//�{�X�̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_DAMAGE,		//��e
		STATE_DEATH,		//���S
		STATE_INVINCIBLE,	//���G
		STATE_MAX,
	};

	//�{�X�̍s��
	enum ACTION
	{
		ACTION_NORMAL = 0,	//�ʏ�
		ACTION_ATTACK,		//�U��
		ACTION_WARP,		//�u�Ԉړ�
		ACTION_REVIVAL,		//����
		ACTION_EVENT,		//�C�x���g��
		ACTION_MAX,
	};

	//�{�X�̍U��
	enum ATTACK
	{
		ATTACK_NOT = 0,		//�U�����Ȃ�
		ATTACK_BULLET,		//�e
		ATTACK_RUSH,		//�ːi
		ATTACK_BLOCKRUN,	//�u���b�N����
		ATTACK_SPINPILLAR,	//�u���b�N�̒�
		ATTACK_RAIN,		//�u���b�N�̉J
		ATTACK_REVIVAL,		//����
		ATTACK_MAX,

	};

	static CBoss* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool bX);
	void HitDamage(float Damage);
	void SetLifeUI(void);
	void SetAction(ACTION Action) { m_Action = Action; }
	ACTION GetAction(void) { return m_Action; }

private:
	void StateManager(void);				//��ԊǗ�
	void WarpUpdate(D3DXVECTOR3* pos);		//���[�v�X�V����
	void Warp(ATTACK Pattern);				//���[�v�ʒu�w�菈��
	void AttackUpdate(D3DXVECTOR3* pos);	//�U���X�V����
	void AttackSelect(void);				//�U���I������
	void AttackBullet(D3DXVECTOR3* pos);	//�U���p�^�[���ˌ�
	void AttackRush(D3DXVECTOR3* pos);		//�U���p�^�[���ːi
	void AttackBlockRun(D3DXVECTOR3* pos);	//�U���p�^�[���u���b�N����
	void AttackSpinPillar(D3DXVECTOR3* pos);//�U���p�^�[����]���钌
	void AttackRain(D3DXVECTOR3* pos);		//�U���p�^�[���J
	void AttackRevival(D3DXVECTOR3* pos);	//�U���p�^�[������

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
	int m_AttackCoolTime;			//�U���Ɏg���J�E���g
	float m_SpinCount;				//��]�̃J�E���g
	float m_Scaling;				//�傫��
	int m_nForm;					//�`��
	bool m_bRevivalColorSwitch;		//�h�����̐F�̕ω�

	D3DXVECTOR3 m_move;				//�ړ���	
	D3DXVECTOR3 m_rot;				//����	
	float m_fLife;					//�{�X�̃��C�t
	float m_fMoveLife;				//���o�p���C�t
	bool m_bDelLife;				//���C�t�������ԂɂȂ�����
	float m_fLifeMax;				//�{�X�̃��C�t�̍ő�l
	CObjGauge2D* m_pLifeGauge;		//���C�t�Q�[�W�̃|�C���^
	CNumber* m_pLifeNumber[5];		//���C�t�pUI
	CCubeSpin* m_CubeSpin;			//��]�p�̃L���[�u
	CCubeSpin* m_CubeSpinTitle;		//��]�p�̃L���[�u(�^�C�g�����o�p)
};
#endif