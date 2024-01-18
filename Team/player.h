//============================================
//
//	プレイヤーの処理 [playerlevel.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYERLEVEL_H_
#define _PLAYERLEVEL_H_

#include "object.h"

//前方宣言
class CModel;
class CMotion;

//オブジェクトプレイヤークラス
class CPlayer : public CObject
{
public:
	CPlayer(int nPriority = 3);
	~CPlayer();

	//プレイヤーのモーション
	typedef enum
	{
		ACTION_WAIT = 0,
		ACTION_MOVE,
		ACTION_ATTACK1,
		ACTION_ATTACK2,
		ACTION_ATTACK3,
		ACTION_SKYATTACK,
		ACTION_JAMP,
		ACTION_EVASION,
		ACTION_MAX,

	}ACTION_TYPE;

	//プレイヤーの状態
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DEATH,
		STATE_WAIT,
		STATE_MAX,
		
	}STATE;

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CPlayer *Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadLevelData(const char *pFilename);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWight(float Wight) { m_fWight = Wight; }
	float GetWight(void) { return m_fWight; }
	void SetHeight(float Height) { m_fHeight = Height; }
	float GetHeight(void) { return m_fHeight; }
	D3DXVECTOR3 GetReSpownPos(void) { return m_ReSpownPos; }
	STATE GetState(void) { return m_State; }
	bool GetJump(void) { return m_bJump; }
	void HitDamage(void);								//攻撃を受けた時の処理

private:
	void StateManager(void);							//状態管理
	void Move(void);									//移動処理
	void Rot(void);										//移動方向処理
	void Jump(void);									//ジャンプ処理
	void Attack(void);									//攻撃処理
	void Dodge(void);									//回避処理
	void ActionState(void);								//モーションと状態の管理
	bool CollisionBlock(D3DXVECTOR3 *pos, COLLISION XYZ);	//オブジェクトとの当たり判定
	void CollisionBoss(void);							//ボスとの当たり判定
	void DeleteMap(void);								//マップの削除
	
	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//攻撃状態記録用変数
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posOld;		//過去の位置
	D3DXVECTOR3 m_ReSpownPos;	//復活地点
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_Objmove;		//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	bool m_bWallJump;			//壁生成ジャンプ
	bool m_bRight;				//右向きか
	bool m_bAirAttack;			//空中で攻撃をしたかどうか
	bool m_bJump;				//ジャンプをしたかどうか
	int m_nActionCount;			//状態のカウント
	int m_nAttackCount;			//攻撃のカウント
	int m_nDodgeCount;			//回避のカウント
	int m_nDodgeCoolTime;		//回避のクールタイム
	int m_nAttackChainFrame;	//連続攻撃の猶予フレーム
	int m_nWallMoveCount;		//移動のカウント
	int m_nActionNotCount;		//行動不能のカウント
	float m_fWight;				//幅
	float m_fHeight;			//高さ
	STATE m_State;				//状態
	int m_nStateCount;			//状態管理用カウント
	bool m_bHit;				//攻撃をくらったかどうか
	bool m_GameEnd;				//ゲームが終わったかどうか

	CModel *m_apModel[64];
	CMotion *m_pMotion;
	char *m_aModelName[64];
	int m_nNumModel;
};

#endif