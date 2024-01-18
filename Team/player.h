//============================================
//
//	�v���C���[�̏��� [playerlevel.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYERLEVEL_H_
#define _PLAYERLEVEL_H_

#include "object.h"

//�O���錾
class CModel;
class CMotion;

//�I�u�W�F�N�g�v���C���[�N���X
class CPlayer : public CObject
{
public:
	CPlayer(int nPriority = 3);
	~CPlayer();

	//�v���C���[�̃��[�V����
	typedef enum
	{
		ACTION_WAIT = 0,
		ACTION_MOVE,
		ACTION_ATTACK1,
		ACTION_ATTACK2,
		ACTION_ATTACK3,
		ACTION_SKYATTACK,
		ACTION_JAMP,
		ACTION_EVASION,
		ACTION_MAX,

	}ACTION_TYPE;

	//�v���C���[�̏��
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DEATH,
		STATE_WAIT,
		STATE_MAX,
		
	}STATE;

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CPlayer *Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadLevelData(const char *pFilename);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWight(float Wight) { m_fWight = Wight; }
	float GetWight(void) { return m_fWight; }
	void SetHeight(float Height) { m_fHeight = Height; }
	float GetHeight(void) { return m_fHeight; }
	D3DXVECTOR3 GetReSpownPos(void) { return m_ReSpownPos; }
	STATE GetState(void) { return m_State; }
	bool GetJump(void) { return m_bJump; }
	void HitDamage(void);								//�U�����󂯂����̏���

private:
	void StateManager(void);							//��ԊǗ�
	void Move(void);									//�ړ�����
	void Rot(void);										//�ړ���������
	void Jump(void);									//�W�����v����
	void Attack(void);									//�U������
	void Dodge(void);									//�������
	void ActionState(void);								//���[�V�����Ə�Ԃ̊Ǘ�
	bool CollisionBlock(D3DXVECTOR3 *pos, COLLISION XYZ);	//�I�u�W�F�N�g�Ƃ̓����蔻��
	void CollisionBoss(void);							//�{�X�Ƃ̓����蔻��
	void DeleteMap(void);								//�}�b�v�̍폜
	
	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu
	D3DXVECTOR3 m_ReSpownPos;	//�����n�_
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_Objmove;		//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	bool m_bWallJump;			//�ǐ����W�����v
	bool m_bRight;				//�E������
	bool m_bAirAttack;			//�󒆂ōU�����������ǂ���
	bool m_bJump;				//�W�����v���������ǂ���
	int m_nActionCount;			//��Ԃ̃J�E���g
	int m_nAttackCount;			//�U���̃J�E���g
	int m_nDodgeCount;			//����̃J�E���g
	int m_nDodgeCoolTime;		//����̃N�[���^�C��
	int m_nAttackChainFrame;	//�A���U���̗P�\�t���[��
	int m_nWallMoveCount;		//�ړ��̃J�E���g
	int m_nActionNotCount;		//�s���s�\�̃J�E���g
	float m_fWight;				//��
	float m_fHeight;			//����
	STATE m_State;				//���
	int m_nStateCount;			//��ԊǗ��p�J�E���g
	bool m_bHit;				//�U��������������ǂ���
	bool m_GameEnd;				//�Q�[�����I��������ǂ���

	CModel *m_apModel[64];
	CMotion *m_pMotion;
	char *m_aModelName[64];
	int m_nNumModel;
};

#endif