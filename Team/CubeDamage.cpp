//============================================
//
//	�L���[�u�_���[�W [CubeDamage.cpp]
//	Author:sakamoto kai
//
//============================================
#include "CubeDamage.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"

#define WAIGHT_SIZE (11)		//���̒��_��
#define HEIGHT_SIZE (11)		//�c�̒��_��
#define WAIGHT_CENTER (0.5f)	//���̌��_(0.0f�`1.0f)
#define HEIGHT_CENTER (0.5f)	//�c�̌��_(0.0f�`1.0f)
#define FIELD_SIZE (200.0f)		//���ꖇ�̑傫��

//====================================================================
//�R���X�g���N�^
//====================================================================
CCubeDamage::CCubeDamage(int nPriority) :CObjmeshCube(nPriority)
{
	m_fDamage = 0.0f;
	m_bBreak = false;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CCubeDamage::~CCubeDamage()
{

}

//====================================================================
//��������
//====================================================================
CCubeDamage* CCubeDamage::Create(void)
{
	CCubeDamage* pObject3D = NULL;

	if (pObject3D == NULL)
	{
		//�I�u�W�F�N�g3D�̐���
		pObject3D = new CCubeDamage();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pObject3D->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pObject3D;
}

//====================================================================
//����������
//====================================================================
HRESULT CCubeDamage::Init(void)
{
	CObjmeshCube::Init();

	SetTexture("data\\TEXTURE\\Test.jpg");

	SetType(TYPE_CUBEDAMEGE);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CCubeDamage::Uninit(void)
{
	CObjmeshCube::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CCubeDamage::Update(void)
{
	CObjmeshCube::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CCubeDamage::Draw(void)
{
	CObjmeshCube::Draw();
}

//====================================================================
//�`�揈��
//====================================================================
void CCubeDamage::SetBreak(bool Set)
{
	m_bBreak = Set;

	if (m_bBreak == true)
	{
		SetColor(D3DXCOLOR(0.8f, 0.3f, 0.0f, 0.5f));
	}
	else
	{
		SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
	}
}

//====================================================================
//�u���b�N�̓����蔻��
//====================================================================
bool CCubeDamage::CollisionDamageBlock(D3DXVECTOR3 pPos, D3DXVECTOR3 Size, float *Damage)
{
	D3DXVECTOR3 MyPos = GetPos();
	D3DXVECTOR3 MySize = GetSize();

	//��`�͈͓̔����ǂ����̔���
	if (MyPos.x + MySize.x >= pPos.x - Size.x &&
		MyPos.x - MySize.x <= pPos.x + Size.x &&
		MyPos.z + MySize.z >= pPos.z - Size.z &&
		MyPos.z - MySize.z <= pPos.z + Size.z &&
		MyPos.y + MySize.y >= pPos.y &&
		MyPos.y - MySize.y <= pPos.y + Size.y)
	{
		*Damage = m_fDamage;

		if (m_bBreak == true)
		{
			Uninit();
		}

		return true;
	}

	return false;
}