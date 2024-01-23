//============================================
//
//	キューブダメージ [CubeDamage.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CUBEDAMAGE_H_
#define _CUBEDAMAGE_H_

#include "main.h"
#include "objmeshCube.h"

//オブジェクトメッシュフィールドクラス
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
	float m_fDamage;	//ダメージ量
	bool m_bBreak;	//一度で消えるかどうか

};
#endif