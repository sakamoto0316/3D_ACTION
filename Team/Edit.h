//============================================
//
//	エディット [Edit.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "object.h"

#define DATA_NAME ("data\\TXT\\Stage.txt")

//前方宣言
class CObjmeshCube;

//マップクラス
class CEdit
{
public:
	CEdit();
	~CEdit();

	static CEdit* Create();
	void Uninit(void);
	void Update(void);

protected:

private:
	void CollisionBlock(void);	//オブジェクトとの当たり判定
	void SaveData(void);

	float MoveWidth;		//移動幅
	CObjmeshCube* m_pEditBlock;
};
#endif