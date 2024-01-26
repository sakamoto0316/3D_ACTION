//============================================
//
//	ゲーム画面 [game.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "manager.h"

//前方宣言
class CTutorialUI;
class CPlayer;
class CMap2D;
class CEdit;
class CTutorialUI;
class CObject2D;
class CObject3D;
class CObjectBillboard;
class CObjectX;
class CObjmeshField;
class CObjmeshWall;
class CObjmeshCylinder;
class CObjmeshDome;
class CCubeBlock;
class CPause;
class CScore;
class CTime;
class CBoss;

//シーンクラス
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void ReSetGame(void);
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CBoss*GetBoss(void) { return m_pBoss; }
	static CPause *GetPause(void) { return m_pPause; }
	static CScore *GetScore(void) { return m_pScore; }
	static CEdit *GetEdit(void) { return m_pEdit; }
	static CTime *GetTime(void) { return m_pTime; }
	static void DeleteTutorial(void);

private:
	static bool m_bGameEnd;					//ゲーム終了状態かどうか
	static CTutorialUI *m_pTutorialUI;		//チュートリアルUIのポインタ
	static CPlayer *m_pPlayer;				//プレイヤーのポインタ
	static CEdit *m_pEdit;					//エディットモードのポインタ
	static CPause *m_pPause;				//ポーズのポインタ
	static CScore *m_pScore;				//スコアのポインタ
	static CTime *m_pTime;
	static CObject2D* m_p2DSample;			//2Dポリゴンのサンプル
	static CObject2D* m_p2DUI_Attack;		//攻撃の2DUI
	static CObject2D* m_p2DUI_Jump;			//ジャンプの2DUI
	static CObject2D* m_p2DUI_Dodge;		//回避の2DUI
	static CObject2D* m_p2DUI_Attention;	//注目の2DUI
	static CObject2D *m_p2DUI_AttentionOK;	//注目の2DUI
	static CObject3D *m_p3DSample;			//3Dポリゴンのサンプル
	static CObjectBillboard* m_pBillboardSample; //Billboardポリゴンのサンプル
	static CObjectX* m_pXModelSample;		//Xモデルのサンプル
	static CObjmeshField* m_pMeshFieldSample;//メッシュフィールドのサンプル
	static CObjmeshWall* m_pMeshWallSample;//メッシュウォールのサンプル
	static CObjmeshCylinder* m_pMeshCylinderSample;//メッシュシリンダーのサンプル
	static CObjmeshDome* m_pMeshDome;		//メッシュドーム
	static CCubeBlock* m_pCubeBlock;		//キューブブロック
	static CBoss* m_pBoss;					//ボス
};
#endif