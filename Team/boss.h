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

//�I�u�W�F�N�g�v���C���[�N���X
class CBoss : public CObjectX
{
public:

	CBoss(int nPriority = 3);
	~CBoss();

	static CBoss* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool bX);

private:
	int m_nIdxXModel;				//X���f���̔ԍ�
	D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	bool m_bCollision;				//�����蔻��p�̍��W
	int m_nIdxTexture;				//�e�N�X�`���̔ԍ�
	float m_MoveCount;				//�ҋ@���̕��V�p�ϐ�

	D3DXVECTOR3 m_move;				//�ړ���	
	D3DXVECTOR3 m_rot;				//����	
};
#endif