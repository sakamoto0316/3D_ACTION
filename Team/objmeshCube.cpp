//============================================
//
//	オブジェクトメッシュキューブ [objmeshCube.cpp]
//	Author:sakamoto kai
//
//============================================
#include "objmeshCube.h"
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
CObjmeshCube::CObjmeshCube(int nPriority) :CObject(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	g_pIdxBuff = NULL;
	m_pos = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Size = INITVECTOR3;
	m_move = INITVECTOR3;
	m_TestCount = 0.0f;
}

//====================================================================
//デストラクタ
//====================================================================
CObjmeshCube::~CObjmeshCube()
{

}

//====================================================================
//生成処理
//====================================================================
CObjmeshCube* CObjmeshCube::Create(void)
{
	CObjmeshCube* pObject3D = NULL;

	if (pObject3D == NULL)
	{
		//オブジェクト3Dの生成
		pObject3D = new CObjmeshCube();
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
HRESULT CObjmeshCube::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	SetTexture("data\\TEXTURE\\Wood002.png");

	SetType(TYPE_BLOCK);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 24,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定 
	pVtx[0].pos.x	= -m_Size.x;
	pVtx[1].pos.x	= m_Size.x;
	pVtx[2].pos.x	= -m_Size.x;
	pVtx[3].pos.x	= m_Size.x;
	pVtx[4].pos.x	= m_Size.x;
	pVtx[5].pos.x	= m_Size.x;
	pVtx[6].pos.x	= m_Size.x;
	pVtx[7].pos.x	= -m_Size.x;
	pVtx[8].pos.x	= -m_Size.x;
	pVtx[9].pos.x	= -m_Size.x;
	pVtx[10].pos.x	= -m_Size.x;
	pVtx[11].pos.x	= m_Size.x;
	pVtx[12].pos.x	= m_Size.x;
	pVtx[13].pos.x	= m_Size.x;
	pVtx[14].pos.x	= m_Size.x;
	pVtx[15].pos.x	= -m_Size.x;
	pVtx[16].pos.x	= -m_Size.x;
	pVtx[17].pos.x	= -m_Size.x;
	pVtx[18].pos.x	= -m_Size.x;
	pVtx[19].pos.x	= m_Size.x;
	pVtx[20].pos.x	= m_Size.x;
	pVtx[21].pos.x	= m_Size.x;
	pVtx[22].pos.x	= -m_Size.x;
	pVtx[23].pos.x	= -m_Size.x;

	pVtx[0].pos.y	= m_Size.y;
	pVtx[1].pos.y	= m_Size.y;
	pVtx[2].pos.y	= m_Size.y;
	pVtx[3].pos.y	= m_Size.y;
	pVtx[4].pos.y	= m_Size.y;
	pVtx[5].pos.y	= m_Size.y;
	pVtx[6].pos.y	= m_Size.y;
	pVtx[7].pos.y	= m_Size.y;
	pVtx[8].pos.y	= m_Size.y;
	pVtx[9].pos.y	= m_Size.y;
	pVtx[10].pos.y	= m_Size.y;
	pVtx[11].pos.y	= m_Size.y;
	pVtx[12].pos.y	= -m_Size.y;
	pVtx[13].pos.y	= -m_Size.y;
	pVtx[14].pos.y	= -m_Size.y;
	pVtx[15].pos.y	= -m_Size.y;
	pVtx[16].pos.y	= -m_Size.y;
	pVtx[17].pos.y	= -m_Size.y;
	pVtx[18].pos.y	= -m_Size.y;
	pVtx[19].pos.y	= -m_Size.y;
	pVtx[20].pos.y	= -m_Size.y;
	pVtx[21].pos.y	= -m_Size.y;
	pVtx[22].pos.y	= -m_Size.y;
	pVtx[23].pos.y	= -m_Size.y;

	pVtx[0].pos.z	= m_Size.z;
	pVtx[1].pos.z	= m_Size.z;
	pVtx[2].pos.z	= -m_Size.z;
	pVtx[3].pos.z	= -m_Size.z;
	pVtx[4].pos.z	= -m_Size.z;
	pVtx[5].pos.z	= m_Size.z;
	pVtx[6].pos.z	= m_Size.z;
	pVtx[7].pos.z	= m_Size.z;
	pVtx[8].pos.z	= m_Size.z;
	pVtx[9].pos.z	= -m_Size.z;
	pVtx[10].pos.z	= -m_Size.z;
	pVtx[11].pos.z	= -m_Size.z;
	pVtx[12].pos.z	= -m_Size.z;
	pVtx[13].pos.z	= m_Size.z;
	pVtx[14].pos.z	= m_Size.z;
	pVtx[15].pos.z	= m_Size.z;
	pVtx[16].pos.z	= m_Size.z;
	pVtx[17].pos.z	= -m_Size.z;
	pVtx[18].pos.z	= -m_Size.z;
	pVtx[19].pos.z	= -m_Size.z;
	pVtx[20].pos.z	= -m_Size.z;
	pVtx[21].pos.z	= m_Size.z;
	pVtx[22].pos.z	= -m_Size.z;
	pVtx[23].pos.z	= m_Size.z;

		//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[8].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[12].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[13].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[14].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[15].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[16].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	pVtx[17].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	pVtx[18].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[19].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[20].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[21].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[22].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[23].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	for (int nCnt = 0; nCnt < 24; nCnt++)
	{
		//頂点カラーの設定
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[12].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[16].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[22].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[23].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (28),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuff,
		NULL);

	WORD* pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを所得
	g_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0]  = 2;
	pIdx[1]  = 0;
	pIdx[2]  = 3;
	pIdx[3]  = 1;
	pIdx[4]  = 1;
	pIdx[5]  = 12;
	pIdx[6]  = 12;
	pIdx[7]  = 4;
	pIdx[8]  = 13;
	pIdx[9]  = 5;
	pIdx[10] = 14;
	pIdx[11] = 6;
	pIdx[12] = 15;
	pIdx[13] = 7;
	pIdx[14] = 16;
	pIdx[15] = 8;
	pIdx[16] = 17;
	pIdx[17] = 9;
	pIdx[18] = 18;
	pIdx[19] = 10;
	pIdx[20] = 19;
	pIdx[21] = 11;
	pIdx[22] = 11;
	pIdx[23] = 22;
	pIdx[24] = 22;
	pIdx[25] = 20;
	pIdx[26] = 23;
	pIdx[27] = 21;

	//インデックスバッファをアンロックする
	g_pIdxBuff->Unlock();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CObjmeshCube::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuff != NULL)
	{
		g_pIdxBuff->Release();
		g_pIdxBuff = NULL;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CObjmeshCube::Update(void)
{
	m_posOld = m_pos;

	//m_TestCount += 0.1f;

	//m_move.z = sinf(m_TestCount) * 10.0f;

	m_pos += m_move;
}

//====================================================================
//描画処理
//====================================================================
void CObjmeshCube::Draw(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

		//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	assert(GetIdx() != -1);		//テクスチャの番号を入れ忘れた場合エラーを吐く

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(GetIdx()));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		0,//用意した頂点の数
		0,
		(28) - 2);//描画するプリミティブの数
}

//====================================================================
//テクスチャの割り当て
//====================================================================
void CObjmeshCube::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//====================================================================
//頂点座標の設定
//====================================================================
void CObjmeshCube::SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = Pos0;
	pVtx[1].pos = Pos1;
	pVtx[2].pos = Pos2;
	pVtx[3].pos = Pos3;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点座標の設定
//====================================================================
void CObjmeshCube::SetVerSize(void)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos.x = -m_Size.x;
	pVtx[1].pos.x = m_Size.x;
	pVtx[2].pos.x = -m_Size.x;
	pVtx[3].pos.x = m_Size.x;
	pVtx[4].pos.x = m_Size.x;
	pVtx[5].pos.x = m_Size.x;
	pVtx[6].pos.x = m_Size.x;
	pVtx[7].pos.x = -m_Size.x;
	pVtx[8].pos.x = -m_Size.x;
	pVtx[9].pos.x = -m_Size.x;
	pVtx[10].pos.x = -m_Size.x;
	pVtx[11].pos.x = m_Size.x;
	pVtx[12].pos.x = m_Size.x;
	pVtx[13].pos.x = m_Size.x;
	pVtx[14].pos.x = m_Size.x;
	pVtx[15].pos.x = -m_Size.x;
	pVtx[16].pos.x = -m_Size.x;
	pVtx[17].pos.x = -m_Size.x;
	pVtx[18].pos.x = -m_Size.x;
	pVtx[19].pos.x = m_Size.x;
	pVtx[20].pos.x = m_Size.x;
	pVtx[21].pos.x = m_Size.x;
	pVtx[22].pos.x = -m_Size.x;
	pVtx[23].pos.x = -m_Size.x;

	pVtx[0].pos.y = m_Size.y;
	pVtx[1].pos.y = m_Size.y;
	pVtx[2].pos.y = m_Size.y;
	pVtx[3].pos.y = m_Size.y;
	pVtx[4].pos.y = m_Size.y;
	pVtx[5].pos.y = m_Size.y;
	pVtx[6].pos.y = m_Size.y;
	pVtx[7].pos.y = m_Size.y;
	pVtx[8].pos.y = m_Size.y;
	pVtx[9].pos.y = m_Size.y;
	pVtx[10].pos.y = m_Size.y;
	pVtx[11].pos.y = m_Size.y;
	pVtx[12].pos.y = -m_Size.y;
	pVtx[13].pos.y = -m_Size.y;
	pVtx[14].pos.y = -m_Size.y;
	pVtx[15].pos.y = -m_Size.y;
	pVtx[16].pos.y = -m_Size.y;
	pVtx[17].pos.y = -m_Size.y;
	pVtx[18].pos.y = -m_Size.y;
	pVtx[19].pos.y = -m_Size.y;
	pVtx[20].pos.y = -m_Size.y;
	pVtx[21].pos.y = -m_Size.y;
	pVtx[22].pos.y = -m_Size.y;
	pVtx[23].pos.y = -m_Size.y;

	pVtx[0].pos.z = m_Size.z;
	pVtx[1].pos.z = m_Size.z;
	pVtx[2].pos.z = -m_Size.z;
	pVtx[3].pos.z = -m_Size.z;
	pVtx[4].pos.z = -m_Size.z;
	pVtx[5].pos.z = m_Size.z;
	pVtx[6].pos.z = m_Size.z;
	pVtx[7].pos.z = m_Size.z;
	pVtx[8].pos.z = m_Size.z;
	pVtx[9].pos.z = -m_Size.z;
	pVtx[10].pos.z = -m_Size.z;
	pVtx[11].pos.z = -m_Size.z;
	pVtx[12].pos.z = -m_Size.z;
	pVtx[13].pos.z = m_Size.z;
	pVtx[14].pos.z = m_Size.z;
	pVtx[15].pos.z = m_Size.z;
	pVtx[16].pos.z = m_Size.z;
	pVtx[17].pos.z = -m_Size.z;
	pVtx[18].pos.z = -m_Size.z;
	pVtx[19].pos.z = -m_Size.z;
	pVtx[20].pos.z = -m_Size.z;
	pVtx[21].pos.z = m_Size.z;
	pVtx[22].pos.z = -m_Size.z;
	pVtx[23].pos.z = m_Size.z;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャ座標の設定
//====================================================================
void CObjmeshCube::SetAnim(D3DXVECTOR2 Tex)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

					//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(Tex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, Tex.y);
	pVtx[3].tex = D3DXVECTOR2(Tex.x, Tex.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点カラーの設定
//====================================================================
void CObjmeshCube::SetColor(D3DXCOLOR col)
{
	m_Color = col;

	VERTEX_3D* pVtx;	//頂点ポインタを所得

					//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点カラーの設定
//====================================================================
void CObjmeshCube::SetTexture(const char* name)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	m_nIdxTexture = pTexture->Regist(name);
}

//====================================================================
//ブロックの当たり判定
//====================================================================
bool CObjmeshCube::CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ)
{
	switch (XYZ)	
	{
	case CObject::COLLISION_X:	// X軸

		//＋から－の面
		if (m_pos.x + m_Size.x > pPos->x - Size.x &&
			(m_posOld.x + m_Size.x <= pPos->x - Size.x ||
			m_pos.x + m_Size.x <= pPosOld.x - Size.x ||
			(m_pos.x + m_Size.x >= pPosOld.x - Size.x &&
			m_posOld.x + m_Size.x <= pPosOld.x - Size.x &&
			m_posOld.x + m_Size.x >= pPos->x - Size.x)) &&
			m_pos.z + m_Size.z > pPos->z - Size.z &&
			m_pos.z - m_Size.z < pPos->z + Size.z &&
			m_pos.y + m_Size.y > pPos->y &&
			m_pos.y - m_Size.y < pPos->y + Size.y
			)
		{
			if (m_move.x >= 0.0f)
			{
				pPos->x = pPosOld.x - Size.x + Size.x + m_move.x;
			}
			else
			{
				pPos->x = m_pos.x + m_Size.x + Size.x + m_move.x;
			}
			pMove->x = 0.0f;
		}

		//－から＋の面
		if (m_pos.x - m_Size.x < pPos->x + Size.x &&
			(m_posOld.x - m_Size.x >= pPos->x + Size.x ||
			m_pos.x - m_Size.x >= pPosOld.x + Size.x ||
			(m_pos.x - m_Size.x <= pPosOld.x + Size.x &&
			m_posOld.x - m_Size.x >= pPosOld.x + Size.x &&
			m_posOld.x - m_Size.x <= pPos->x + Size.x)) &&
			m_pos.z + m_Size.z > pPos->z - Size.z &&
			m_pos.z - m_Size.z < pPos->z + Size.z &&
			m_pos.y + m_Size.y > pPos->y &&
			m_pos.y - m_Size.y < pPos->y + Size.y
			)
		{
			if (m_move.x <= 0.0f)
			{
				pPos->x = pPosOld.x + Size.x - Size.x + m_move.x;
			}
			else
			{
				pPos->x = m_pos.x - m_Size.x - Size.x + m_move.x;
			}
			pMove->x = 0.0f;
		}

		break;
	case CObject::COLLISION_Y:

		//＋から－の面
		if (m_pos.y + m_Size.y > pPos->y &&
			(m_posOld.y + m_Size.y <= pPos->y ||
			m_pos.y + m_Size.y <= pPosOld.y ||
			(m_pos.y + m_Size.y >= pPosOld.y &&
			m_posOld.y + m_Size.y <= pPosOld.y &&
			m_posOld.y + m_Size.y >= pPos->y)) &&
			m_pos.z + m_Size.z > pPos->z - Size.z &&
			m_pos.z - m_Size.z < pPos->z + Size.z &&
			m_pos.x + m_Size.x > pPos->x - Size.x &&
			m_pos.x - m_Size.x < pPos->x + Size.x
			)
		{
			if (m_move.y >= 0.0f)
			{
				pPos->y = pPosOld.y + m_move.y;
			}
			else
			{
				pPos->y = m_pos.y + m_Size.y + m_move.y;
			}
			pMove->y = 0.0f;
			*bJump = false;
			pObjMove->x = m_move.x;
			pObjMove->y = m_move.y;
			pObjMove->z = m_move.z;
		}

		//－から＋の面
		if (m_pos.y - m_Size.y < pPos->y + Size.y &&
			(m_posOld.y - m_Size.y >= pPos->y + Size.y ||
			m_pos.y - m_Size.y >= pPosOld.y + Size.y ||
			(m_pos.y - m_Size.y <= pPosOld.y + Size.y &&
			m_posOld.y - m_Size.y >= pPosOld.y + Size.y &&
			m_posOld.y - m_Size.y <= pPos->y + Size.y)) &&
			m_pos.z + m_Size.z > pPos->z - Size.z &&
			m_pos.z - m_Size.z < pPos->z + Size.z &&
			m_pos.x + m_Size.x > pPos->x - Size.x &&
			m_pos.x - m_Size.x < pPos->x + Size.x
			)
		{
			pPos->y = m_pos.y - m_Size.y - Size.y + m_move.y;
			pMove->y = m_move.y;
		}

		break;
	case CObject::COLLISION_Z:

		//＋から－の面
		if (m_pos.z + m_Size.z > pPos->z - Size.z &&
			(m_posOld.z + m_Size.z <= pPos->z - Size.z ||
			m_pos.z + m_Size.z <= pPosOld.z - Size.z ||
			(m_pos.z + m_Size.z >= pPosOld.z - Size.z &&
			m_posOld.z + m_Size.z <= pPosOld.z - Size.z &&
			m_posOld.z + m_Size.z >= pPos->z - Size.z)) &&
			m_pos.x + m_Size.x > pPos->x - Size.x &&
			m_pos.x - m_Size.x < pPos->x + Size.x &&
			m_pos.y + m_Size.y > pPos->y &&
			m_pos.y - m_Size.y < pPos->y + Size.y
			)
		{
			if (m_move.z >= 0.0f)
			{
				pPos->z = pPosOld.z - Size.z + Size.z + m_move.z;
			}
			else
			{
				pPos->z = m_pos.z + m_Size.z + Size.z + m_move.z;
			}
			pMove->z = 0.0f;
		}

		//－から＋の面
		if (m_pos.z - m_Size.z < pPos->z + Size.z &&
			(m_posOld.z - m_Size.z >= pPos->z + Size.z ||
			m_pos.z - m_Size.z >= pPosOld.z + Size.z ||
			(m_pos.z - m_Size.z <= pPosOld.z + Size.z &&
			m_posOld.z - m_Size.z >= pPosOld.z + Size.z &&
			m_posOld.z - m_Size.z <= pPos->z + Size.z)) &&
			m_pos.x + m_Size.x > pPos->x - Size.x &&
			m_pos.x - m_Size.x < pPos->x + Size.x &&
			m_pos.y + m_Size.y > pPos->y &&
			m_pos.y - m_Size.y < pPos->y + Size.y
			)
		{
			if (m_move.z <= 0.0f)
			{
				pPos->z = pPosOld.z + Size.z - Size.z + m_move.z;
			}
			else
			{
				pPos->z = m_pos.z - m_Size.z - Size.z + m_move.z;
			}
			pMove->z = 0.0f;
		}

		break;
	}

	return false;
}