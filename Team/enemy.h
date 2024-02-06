//============================================
//
//	敵の処理 [enemy.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "objectX.h"

//前方宣言
class CObjGauge2D;
class CNumber;
class CCubeSpin;
class CObject2D;

//オブジェクトプレイヤークラス
class CEnemy : public CObjectX
{
public:

	CEnemy(int nPriority = 3);
	~CEnemy();

	//ボスの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_MAX,
	};

	//ボスの行動
	enum ACTION
	{
		ACTION_NORMAL = 0,	//通常
		ACTION_ATTACK,		//攻撃
		ACTION_EVENT,		//イベント時
		ACTION_MAX,
	};

	//ボスの攻撃
	enum ATTACK
	{
		ATTACK_NOT = 0,		//攻撃しない
		ATTACK_RUSH,		//突進
		ATTACK_MAX,

	};

	static CEnemy* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool bX);
	void HitDamage(float Damage);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetAction(ACTION Action) { m_Action = Action; }
	ACTION GetAction(void) { return m_Action; }

private:
	void ActionManager(void);	//行動管理
	void StateManager(void);	//状態管理
	void AttackUpdate(void);	//攻撃更新処理
	void AttackRush(void);		//攻撃パターン突進
	void AttackCollision(void);	//オブジェクトとの当たり判定
	bool CollisionBlock(D3DXVECTOR3* pos, COLLISION XYZ);	//オブジェクトとの当たり判定

	int m_nIdxXModel;				//Xモデルの番号
	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	bool m_bCollision;				//当たり判定用の座標
	int m_nIdxTexture;				//テクスチャの番号
	STATE m_State;					//状態
	ACTION m_Action;				//行動
	int m_nStateCount;				//状態管理用変数
	float m_ColorA;					//不透明度
	ATTACK m_AttackPattern;			//攻撃パターン
	int m_AttackCount;				//攻撃に使うカウント
	int m_AttackWave;				//攻撃段階
	int m_AttackCoolTime;			//攻撃に使うカウント
	float m_Scaling;				//大きさ
	bool m_bRevivalColorSwitch;		//蘇生中の色の変化
	D3DXVECTOR3 m_pos;				//位置	
	D3DXVECTOR3 m_posOld;			//過去の位置	
	D3DXVECTOR3 m_move;				//移動量	
	D3DXVECTOR3 m_Objmove;			//オブジェクトから受ける移動量	
	D3DXVECTOR3 m_rot;				//向き	
	bool m_bJump;					//ジャンプしたかどうか
};
#endif