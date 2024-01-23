//============================================
//
//	ボスの処理 [boss.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "objectX.h"

//前方宣言
class CObjGauge2D;
class CNumber;

//オブジェクトプレイヤークラス
class CBoss : public CObjectX
{
public:

	CBoss(int nPriority = 3);
	~CBoss();

	//ボスの状態
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX,
	}STATE;

	//ボスの行動
	typedef enum
	{
		ACTION_NORMAL = 0,
		ACTION_ATTACK,
		ACTION_WARP,
		ACTION_MAX,
	}ACTION;

	//ボスの攻撃
	typedef enum
	{
		ATTACK_NOT = 0,
		ATTACK_BULLET,
		ATTACK_RUSH,
		ATTACK_BLOCKRUN,
		ATTACK_MAX,

	}ATTACK;

	static CBoss* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool bX);
	void HitDamage(float Damage);
	void SetLifeUI(void);
	ACTION GetAction(void) { return m_Action; }

private:
	void StateManager(void);				//状態管理
	void WarpUpdate(D3DXVECTOR3* pos);		//ワープ更新処理
	void Warp(ATTACK Pattern);				//ワープ位置指定処理
	void AttackUpdate(D3DXVECTOR3* pos);	//攻撃更新処理
	void AttackBullet(D3DXVECTOR3* pos);	//攻撃パターン射撃
	void AttackRush(D3DXVECTOR3* pos);		//攻撃パターン突進
	void AttackBlockRun(D3DXVECTOR3* pos);	//攻撃パターンブロックラン

	int m_nIdxXModel;				//Xモデルの番号
	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	D3DXVECTOR3 m_WarpPos;			//ワープ先の位置
	bool m_bCollision;				//当たり判定用の座標
	int m_nIdxTexture;				//テクスチャの番号
	float m_MoveCount;				//待機中の浮遊用変数
	STATE m_State;					//状態
	ACTION m_Action;				//行動
	int m_nStateCount;				//状態管理用変数
	bool m_nWarpOK;					//ワープ前かワープ後か
	float m_ColorA;					//不透明度
	ATTACK m_AttackPattern;			//攻撃パターン
	int m_AttackCount;				//攻撃に使うカウント
	int m_AttackWave;				//攻撃段階

	D3DXVECTOR3 m_move;				//移動量	
	D3DXVECTOR3 m_rot;				//向き	
	float m_fLife;					//ボスのライフ
	float m_fLifeMax;				//ボスのライフの最大値
	CObjGauge2D* m_pLifeGauge;		//ライフゲージのポインタ
	CNumber* m_pLifeNumber[5];		//ライフ用UI
};
#endif