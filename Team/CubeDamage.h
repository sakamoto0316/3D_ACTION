//============================================
//
//	�L���[�u�_���[�W [CubeDamage.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CUBEDAMAGE_H_
#define _CUBEDAMAGE_H_

#include "main.h"
#include "objmeshCube.h"

//�I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CCubeDamage : public CObjmeshCube
{
public:
	CCubeDamage(int nPriority = 5);
	~CCubeDamage();

	static CCubeDamage* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool CollisionDamageBlock(D3DXVECTOR3 pPos, D3DXVECTOR3 Size, float *Damage);
	void SetBreak(bool Set);
	void SetDamage(float Damage) { m_fDamage = Damage; }

protected:

private:
	float m_fDamage;	//�_���[�W��
	bool m_bBreak;	//��x�ŏ����邩�ǂ���

};
#endif