//============================================
//
//	オブジェクト [object.cpp]
//	Author:sakamoto kai
//
//============================================
#include "object.h"
#include "manager.h"
#include "debugproc.h"

//マクロ定義
#define POLYGON_WIGHT (300.0f)	//横幅
#define POLYGON_HEIGHT (200.0f)	//縦幅

//静的メンバ変数宣言
CObject *CObject::m_pTop[PRIORITY_MAX] = {};
CObject *CObject::m_pCur[PRIORITY_MAX] = {};
int CObject::m_nNumAll = 0;					//オブジェクト総数
bool CObject::m_bLevelStop = false;			//オブジェクト総数

//====================================================================
//コンストラクタ
//====================================================================
CObject::CObject(int nPriority)
{
	if (m_pTop[nPriority] == NULL)
	{//他のオブジェクトがひとつもない時
		m_pTop[nPriority] = this;
	}
	else
	{
		m_pPrev = m_pCur[nPriority];
		m_pPrev->m_pNext = this;
	}

	m_Priority = nPriority;
	m_pCur[nPriority] = this;

	m_bLevelUI = false;
	m_Appear = true;
	m_nNumAll++;					//総数をカウントアップ
}

//====================================================================
//デストラクタ
//====================================================================
CObject::~CObject()
{
	
}

//====================================================================
//全てのオブジェクトの破棄
//====================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			//終了処理
			pObject->Uninit();

			//削除処理
			pObject->SetDeathFlag(true);

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			//削除処理
			pObject->Release();

			pObject = pObjectNext;
		}
	}
}

//====================================================================
//全てのオブジェクトの更新
//====================================================================
void CObject::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			if ((m_bLevelStop == false && pObject->m_bLevelUI == false) ||
				(m_bLevelStop == true && pObject->m_bLevelUI == true))
			{
				if (pObject->m_Appear == true)
				{
					if (CManager::GetInstance()->GetEdit() == true)
					{
						if (pObject->GetType() == TYPE_EFFECT)
						{
							//更新処理
							pObject->Update();
						}
					}
					else if (CManager::GetInstance()->GetPause() == true)
					{
						if (pObject->GetType() == TYPE_TUTORIALUI ||
							pObject->GetType() == TYPE_EXPLOSION ||
							pObject->GetType() == TYPE_FG)
						{
							//更新処理
							pObject->Update();
						}
					}
					else
					{
						//更新処理
						pObject->Update();
					}
				}
			}

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			//削除処理
			pObject->Release();

			pObject = pObjectNext;
		}
	}

	CManager::GetInstance()->GetDebugProc()->Print("オブジェクトの総数:[%d]\n", m_nNumAll);
}

//====================================================================
//全てのオブジェクトの描画
//====================================================================
void CObject::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			if (pObject->m_bLevelUI == false || (m_bLevelStop == true && pObject->m_bLevelUI == true))
			{
				if (pObject->m_Appear == true)
				{
					//描画処理
					pObject->Draw();
				}
			}

			pObject = pObjectNext;
		}
	}
}

//====================================================================
//オブジェクトの破棄
//====================================================================
void CObject::Release(void)
{
	if (m_bDeath == true)
	{
		if (m_pTop[m_Priority] == m_pCur[m_Priority])
		{//オブジェクトが１個しかない時
			m_pTop[m_Priority] = NULL;
			m_pCur[m_Priority] = NULL;
		}
		else if (this == m_pCur[m_Priority])
		{//自分が最後尾の時
			m_pPrev->m_pNext = NULL;
			m_pCur[m_Priority] = m_pPrev;
		}
		else if (this == m_pTop[m_Priority])
		{//自分が先頭の時
			m_pNext->m_pPrev = NULL;
			m_pTop[m_Priority] = m_pNext;
		}
		else
		{//先頭でも最後尾でもない時
			m_pPrev->m_pNext = m_pNext;
			m_pNext->m_pPrev = m_pPrev;
		}

		this->SetNULL();
		delete this;
		m_nNumAll--;				//総数をカウントダウン
	}
}

//====================================================================
//オブジェクトの破棄
//====================================================================
void CObject::ResetObjectMap(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			if (
				pObject->m_type == TYPE_PLAYER3D ||
				pObject->m_type == TYPE_BOSSLEVEL ||
				pObject->m_type == TYPE_BREAKBLOCK3D ||
				pObject->m_type == TYPE_ENEMY3D
				)
			{
				pObject->Uninit();

				pObject->SetDeathFlag(true);
			}

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			//削除処理
			pObject->Release();

			pObject = pObjectNext;
		}
	}
}

//====================================================================
//敵の削除
//====================================================================
void CObject::ReleaseEnemy(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			if (
				pObject->m_type == TYPE_ENEMY3D
				)
			{
				pObject->Uninit();

				pObject->SetDeathFlag(true);
			}

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			//削除処理
			pObject->Release();

			pObject = pObjectNext;
		}
	}
}


//====================================================================
//ブロックの破棄
//====================================================================
void CObject::DeleteBlock(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			if (
				pObject->m_type == TYPE_BREAKBLOCK3D
				)
			{
				pObject->Uninit();

				pObject->SetDeathFlag(true);
			}

			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_pTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != NULL)
		{
			CObject *pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			//削除処理
			pObject->Release();

			pObject = pObjectNext;
		}
	}
}