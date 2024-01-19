//============================================
//
//	キューブブロック [CubeBlock.cpp]
//	Author:sakamoto kai
//
//============================================
#include "CubeBlock.h"
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
CCubeBlock::CCubeBlock(int nPriority) :CObjmeshCube(nPriority)
{

}

//====================================================================
//デストラクタ
//====================================================================
CCubeBlock::~CCubeBlock()
{

}

//====================================================================
//生成処理
//====================================================================
CCubeBlock* CCubeBlock::Create(void)
{
	CCubeBlock* pObject3D = NULL;

	if (pObject3D == NULL)
	{
		//オブジェクト3Dの生成
		pObject3D = new CCubeBlock();
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
HRESULT CCubeBlock::Init(void)
{	
	CObjmeshCube::Init();

	SetTexture("data\\TEXTURE\\Wood002.png");

	SetType(TYPE_CUBEBLOCK);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCubeBlock::Uninit(void)
{
	CObjmeshCube::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CCubeBlock::Update(void)
{
	CObjmeshCube::Update();
}

//====================================================================
//描画処理
//====================================================================
void CCubeBlock::Draw(void)
{
	CObjmeshCube::Draw();
}

//====================================================================
//ブロックの当たり判定
//====================================================================
bool CCubeBlock::CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ)
{
	D3DXVECTOR3 MyPos = GetPos();
	D3DXVECTOR3 MyPosOld = GetPosOld();
	D3DXVECTOR3 MySize = GetSize();
	D3DXVECTOR3 MyMove = GetMove();

	switch (XYZ)
	{
	case CObject::COLLISION_X:	// X軸

		//＋から－の面
		if (MyPos.x + MySize.x > pPos->x - Size.x &&
			(MyPosOld.x + MySize.x <= pPos->x - Size.x ||
				MyPos.x + MySize.x <= pPosOld.x - Size.x ||
				(MyPos.x + MySize.x >= pPosOld.x - Size.x &&
					MyPosOld.x + MySize.x <= pPosOld.x - Size.x &&
					MyPosOld.x + MySize.x >= pPos->x - Size.x)) &&
			MyPos.z + MySize.z > pPos->z - Size.z &&
			MyPos.z - MySize.z < pPos->z + Size.z &&
			MyPos.y + MySize.y > pPos->y &&
			MyPos.y - MySize.y < pPos->y + Size.y
			)
		{
			if (MyMove.x >= 0.0f)
			{
				pPos->x = pPosOld.x - Size.x + Size.x + MyMove.x;
			}
			else
			{
				pPos->x = MyPos.x + MySize.x + Size.x + MyMove.x;
			}
			pMove->x = 0.0f;
		}

		//－から＋の面
		if (MyPos.x - MySize.x < pPos->x + Size.x &&
			(MyPosOld.x - MySize.x >= pPos->x + Size.x ||
				MyPos.x - MySize.x >= pPosOld.x + Size.x ||
				(MyPos.x - MySize.x <= pPosOld.x + Size.x &&
					MyPosOld.x - MySize.x >= pPosOld.x + Size.x &&
					MyPosOld.x - MySize.x <= pPos->x + Size.x)) &&
			MyPos.z + MySize.z > pPos->z - Size.z &&
			MyPos.z - MySize.z < pPos->z + Size.z &&
			MyPos.y + MySize.y > pPos->y &&
			MyPos.y - MySize.y < pPos->y + Size.y
			)
		{
			if (MyMove.x <= 0.0f)
			{
				pPos->x = pPosOld.x + Size.x - Size.x + MyMove.x;
			}
			else
			{
				pPos->x = MyPos.x - MySize.x - Size.x + MyMove.x;
			}
			pMove->x = 0.0f;
		}

		break;
	case CObject::COLLISION_Y:

		//＋から－の面
		if (MyPos.y + MySize.y > pPos->y &&
			(MyPosOld.y + MySize.y <= pPos->y ||
				MyPos.y + MySize.y <= pPosOld.y ||
				(MyPos.y + MySize.y >= pPosOld.y &&
					MyPosOld.y + MySize.y <= pPosOld.y &&
					MyPosOld.y + MySize.y >= pPos->y)) &&
			MyPos.z + MySize.z > pPos->z - Size.z &&
			MyPos.z - MySize.z < pPos->z + Size.z &&
			MyPos.x + MySize.x > pPos->x - Size.x &&
			MyPos.x - MySize.x < pPos->x + Size.x
			)
		{
			if (MyMove.y >= 0.0f)
			{
				pPos->y = pPosOld.y + MyMove.y;
			}
			else
			{
				pPos->y = MyPos.y + MySize.y + MyMove.y;
			}
			pMove->y = 0.0f;
			*bJump = false;
			pObjMove->x = MyMove.x;
			pObjMove->y = MyMove.y;
			pObjMove->z = MyMove.z;
		}

		//－から＋の面
		if (MyPos.y - MySize.y < pPos->y + Size.y &&
			(MyPosOld.y - MySize.y >= pPos->y + Size.y ||
				MyPos.y - MySize.y >= pPosOld.y + Size.y ||
				(MyPos.y - MySize.y <= pPosOld.y + Size.y &&
					MyPosOld.y - MySize.y >= pPosOld.y + Size.y &&
					MyPosOld.y - MySize.y <= pPos->y + Size.y)) &&
			MyPos.z + MySize.z > pPos->z - Size.z &&
			MyPos.z - MySize.z < pPos->z + Size.z &&
			MyPos.x + MySize.x > pPos->x - Size.x &&
			MyPos.x - MySize.x < pPos->x + Size.x
			)
		{
			pPos->y = MyPos.y - MySize.y - Size.y + MyMove.y;
			pMove->y = MyMove.y;
		}

		break;
	case CObject::COLLISION_Z:

		//＋から－の面
		if (MyPos.z + MySize.z > pPos->z - Size.z &&
			(MyPosOld.z + MySize.z <= pPos->z - Size.z ||
				MyPos.z + MySize.z <= pPosOld.z - Size.z ||
				(MyPos.z + MySize.z >= pPosOld.z - Size.z &&
					MyPosOld.z + MySize.z <= pPosOld.z - Size.z &&
					MyPosOld.z + MySize.z >= pPos->z - Size.z)) &&
			MyPos.x + MySize.x > pPos->x - Size.x &&
			MyPos.x - MySize.x < pPos->x + Size.x &&
			MyPos.y + MySize.y > pPos->y &&
			MyPos.y - MySize.y < pPos->y + Size.y
			)
		{
			if (MyMove.z >= 0.0f)
			{
				pPos->z = pPosOld.z - Size.z + Size.z + MyMove.z;
			}
			else
			{
				pPos->z = MyPos.z + MySize.z + Size.z + MyMove.z;
			}
			pMove->z = 0.0f;
		}

		//－から＋の面
		if (MyPos.z - MySize.z < pPos->z + Size.z &&
			(MyPosOld.z - MySize.z >= pPos->z + Size.z ||
				MyPos.z - MySize.z >= pPosOld.z + Size.z ||
				(MyPos.z - MySize.z <= pPosOld.z + Size.z &&
					MyPosOld.z - MySize.z >= pPosOld.z + Size.z &&
					MyPosOld.z - MySize.z <= pPos->z + Size.z)) &&
			MyPos.x + MySize.x > pPos->x - Size.x &&
			MyPos.x - MySize.x < pPos->x + Size.x &&
			MyPos.y + MySize.y > pPos->y &&
			MyPos.y - MySize.y < pPos->y + Size.y
			)
		{
			if (MyMove.z <= 0.0f)
			{
				pPos->z = pPosOld.z + Size.z - Size.z + MyMove.z;
			}
			else
			{
				pPos->z = MyPos.z - MySize.z - Size.z + MyMove.z;
			}
			pMove->z = 0.0f;
		}

		break;
	}

	return false;
}