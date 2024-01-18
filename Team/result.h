//============================================
//
//	リザルト画面 [result.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "manager.h"

//前方宣言
class CObject2D;
class CRanking;
class CScore;
class CBreak_Block3D;
class CEdit;

//シーンクラス
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static void SetApprear(bool Set) { m_Appear = Set; }
	static bool GetApprear(void) { return m_Appear; }

private:

	CBreak_Block3D *m_pBreakBlock3D;
	static CObject2D *m_pResult;
	static CRanking *m_pRanking;
	static CScore *m_DeathScore;
	static CEdit *m_pEdit;					//エディットモードのポインタ
	static CObject2D *m_ClearText;
	static CObject2D *m_NormalText;
	static CObject2D *m_DeathText;
	static bool m_Appear;
	static int m_AddScoreCount;
	static int m_AddTotalScore;
};
#endif