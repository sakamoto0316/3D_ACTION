//============================================
//
//	キューブダメージ [CubeDamage.cpp]
//	Author:sakamoto kai
//
//============================================
#include "CubeDamage.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"

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

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCubeDamage::Uninit(void)
{
	CObjmeshCube::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CCubeDamage::Update(void)
{
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