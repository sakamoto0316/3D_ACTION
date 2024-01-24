//============================================
//
//	キューブダメージ [CubeDamage.cpp]
//	Author:sakamoto kai
//
//============================================
#include "CubeDamage.h"
#include "CubeBlock.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "object3D.h"

#define WAIGHT_SIZE (11)		//横の頂点数
#define HEIGHT_SIZE (11)		//縦の頂点数
#define WAIGHT_CENTER (0.5f)	//横の原点(0.0f～1.0f)
#define HEIGHT_CENTER (0.5f)	//縦の原点(0.0f～1.0f)
#define FIELD_SIZE (200.0f)		//床一枚の大きさ

//====================================================================
//コンストラクタ
//====================================================================
CCubeDamage::CCubeDamage(int nPriority) :CObjmeshCube(nPriority)
{
	m_fDamage = 0.0f;
	m_bBreak = false;
	m_pShadow = nullptr;
	m_fSpinCount = INITVECTOR3;
	m_fSpinSpeed = D3DXVECTOR3(-100.0f, -100.0f, -100.0f);
	m_fBOOLSpin = INITVECTOR3;
	m_bSpin = false;
	SpinPos = INITVECTOR3;
	m_fSpinDistance = 0.0f;
	m_fSpinDisMove = 0.0f;
	m_Move = INITVECTOR3;
}

//====================================================================
//デストラクタ
//====================================================================
CCubeDamage::~CCubeDamage()
{
	
}

//====================================================================
//生成処理
//====================================================================
CCubeDamage* CCubeDamage::Create(void)
{
	CCubeDamage* pObject3D = NULL;

	if (pObject3D == NULL)
	{
		//オブジェクト3Dの生成
		pObject3D = new CCubeDamage();
	}

	//オブジェクトの初期化処理
	if (FAILED(pObject3D->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pObject3D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CCubeDamage::Init(void)
{
	CObjmeshCube::Init();

	SetTexture("data\\TEXTURE\\Test.jpg");

	SetType(TYPE_CUBEDAMEGE);

	m_pShadow = CObject3D::Create();
	m_pShadow->SetPos(INITVECTOR3);
	m_pShadow->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	m_pShadow->SetWight(0.0f);
	m_pShadow->SetHeight(0.0f);
	m_pShadow->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f));

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCubeDamage::Uninit(void)
{
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetDeathFlag(true);
	}

	CObjmeshCube::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CCubeDamage::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	if (m_bSpin == true)
	{
		m_fSpinCount += m_fSpinSpeed;
		m_fSpinDistance += m_fSpinDisMove;

		if (m_fSpinSpeed.x > -100.0f)
		{
			m_fBOOLSpin.x = 1.0f;
		}
		if (m_fSpinSpeed.y > -100.0f)
		{
			m_fBOOLSpin.y = 1.0f;
		}
		if (m_fSpinSpeed.z > -100.0f)
		{
			m_fBOOLSpin.z = 1.0f;
		}

		pos.x = SpinPos.x + (sinf(m_fSpinCount.y) * m_fSpinDistance * m_fBOOLSpin.y) + (sinf(m_fSpinCount.x) * m_fSpinDistance * m_fBOOLSpin.x);
		pos.y = SpinPos.y + (cosf(m_fSpinCount.x) * m_fSpinDistance * m_fBOOLSpin.x) + (sinf(m_fSpinCount.z) * m_fSpinDistance * m_fBOOLSpin.z);
		pos.z = SpinPos.z + (cosf(m_fSpinCount.y) * m_fSpinDistance * m_fBOOLSpin.y) + (cosf(m_fSpinCount.z) * m_fSpinDistance * m_fBOOLSpin.z);

		SpinPos += m_Move;
	}

	pos += m_Move;

	SetPos(pos);

	CollisionShadow();

	CObjmeshCube::Update();
}

//====================================================================
//描画処理
//====================================================================
void CCubeDamage::Draw(void)
{
	CObjmeshCube::Draw();
}

//====================================================================
//描画処理
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
//ブロックの当たり判定
//====================================================================
bool CCubeDamage::CollisionDamageBlock(D3DXVECTOR3 pPos, D3DXVECTOR3 Size, float *Damage)
{
	D3DXVECTOR3 MyPos = GetPos();
	D3DXVECTOR3 MySize = GetSize();

	//矩形の範囲内かどうかの判定
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

//====================================================================
//オブジェクトとの当たり判定処理
//====================================================================
bool CCubeDamage::CollisionShadow(void)
{
	float BlockPosY = -10000.0f;
	bool bShadow = false;;

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
				CCubeBlock* pBlock = (CCubeBlock*)pObj;

				D3DXVECTOR3 MyPos = GetPos();
				D3DXVECTOR3 MySize = GetSize();
				D3DXVECTOR3 BlockPos = pBlock->GetPos();
				D3DXVECTOR3 BlockSize = pBlock->GetSize();
				D3DXVECTOR3 BlockMove = pBlock->GetMove();

				if (BlockPos.y + BlockSize.y > BlockPosY)
				{
					if (BlockPos.x + BlockSize.x > MyPos.x &&
						BlockPos.x - BlockSize.x < MyPos.x &&
						BlockPos.z + BlockSize.z > MyPos.z &&
						BlockPos.z - BlockSize.z < MyPos.z &&
						BlockPos.y - BlockSize.y < MyPos.y)
					{
						float ColorA = (MyPos.y - BlockPos.y + BlockSize.y) * 0.002f;
						m_pShadow->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f - ColorA));

						m_pShadow->SetPos(D3DXVECTOR3(
							MyPos.x,
							BlockPos.y + BlockSize.y + BlockMove.y,
							MyPos.z));
						m_pShadow->SetWight(MySize.x * 2.0f);
						m_pShadow->SetHeight(MySize.z * 2.0f);

						BlockPosY = BlockPos.y + BlockSize.y;
						bShadow = true;
					}
				}
			}

			pObj = pObjNext;
		}
	}

	if (bShadow == false)
	{
		m_pShadow->SetWight(0.0f);
		m_pShadow->SetHeight(0.0f);
	}

	return false;
}