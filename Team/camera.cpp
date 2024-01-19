//============================================
//
//	カメラの処理 [camera.cpp]
//	Author:sakamoto kai
//
//============================================
#include "camera.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"

//マクロ定義
#define CAMERA_DISTANCE (500.0f)//視点と注視点の距離
#define MODEL_DISTANCE (10.0f)	//モデルと注視点の距離
#define CAMERA_SPEED (3.0f)		//カメラの移動スピード
#define CAMERA_VR_SPEED (0.03f)	//カメラの視点スピード
#define CAMERA_HOMING (0.2f)	//カメラの追従スピード
#define POS_HOMING (0.9f)		//位置への追従スピード
#define DOWNVIEW_POSV (D3DXVECTOR3(0.0f, 1000.0f, -5.0f))	//見下ろしの視点
#define DOWNVIEW_POSR (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//見下ろしの注視点
#define SIDEVIEW_POSV (D3DXVECTOR3(0.0f, 200.0f, -1000.0f))	//2Dの視点
#define SIDEVIEW_POSR (D3DXVECTOR3(0.0f, 200.0f, 0.0f))		//2Dの注視点

//====================================================================
//コンストラクタ
//====================================================================
CCamera::CCamera()
{
	m_bBib = false;
	m_fBibPowor = 0.0f;
	m_move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PlayerPos = INITVECTOR3;
	m_DelCameraPos = INITVECTOR3;
	ResetCamera();
	CameraMode = CAMERAMODE_FOLLOW;
	m_FollowTime = 0;
	m_bFollowY = false;
}

//====================================================================
//デストラクタ
//====================================================================
CCamera::~CCamera()
{

}

//====================================================================
//カメラの初期化処理
//====================================================================
HRESULT CCamera::Init(void)
{
	return S_OK;
}

//====================================================================
//カメラの終了処理
//====================================================================
void CCamera::Uninit(void)
{

}

//====================================================================
//カメラの更新処理
//====================================================================
void CCamera::Update(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	//マウスの取得
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	//プレイヤーの取得
	CPlayer* pPlayer = CGame::GetPlayer();

#ifdef _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	{
		if (CameraMode == CAMERAMODE_CONTROL)
		{
			CameraMode = CAMERAMODE_FOLLOW;
			m_FollowTime = 10;
		}
		else if(CameraMode == CAMERAMODE_FOLLOW)
		{
			CameraMode = CAMERAMODE_DOWNVIEW;
		}
		else if (CameraMode == CAMERAMODE_DOWNVIEW)
		{
			CameraMode = CAMERAMODE_2D;
		}
		else if (CameraMode == CAMERAMODE_2D)
		{
			CameraMode = CAMERAMODE_CONTROL;
		}
	}

#endif // _DEBUG


	switch (CameraMode)
	{
	case CAMERAMODE_CONTROL:

		if (m_rot.x <= D3DX_PI * 0.5f && m_rot.x >= -(D3DX_PI * 0.5f))
		{//入力
			m_rotOld.x = m_rot.x;

			//キーボード
			if (pInputKeyboard->GetPress(DIK_I) == true)
			{
				m_rot.x += CAMERA_VR_SPEED;
			}
			if (pInputKeyboard->GetPress(DIK_K) == true)
			{
				m_rot.x -= CAMERA_VR_SPEED;
			}
		}

		if (fabsf(m_rot.x) > fabsf(D3DX_PI * 0.5f))
		{//上限に達した時１フレーム前のrotにもどる
			m_rot.x = m_rotOld.x;
		}

		//キーボード
		if (pInputKeyboard->GetPress(DIK_J) == true)
		{
			m_rot.y -= CAMERA_VR_SPEED;

		}
		if (pInputKeyboard->GetPress(DIK_L) == true)
		{
			m_rot.y += CAMERA_VR_SPEED;
		}

		//一周した時の向きの補正
		if (m_rot.y > D3DX_PI * 1.0f)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		else if (m_rot.y < -D3DX_PI * 1.0f)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}


		//キーボード
		if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			m_posV.y += CAMERA_SPEED;
			m_posR.y += CAMERA_SPEED;

		}
		if (pInputKeyboard->GetPress(DIK_LCONTROL) == true)
		{
			m_posV.y -= CAMERA_SPEED;
			m_posR.y -= CAMERA_SPEED;
		}
		if (pInputKeyboard->GetPress(DIK_RSHIFT) == true)
		{
			m_posV.y += CAMERA_SPEED;
			m_posR.y += CAMERA_SPEED;

		}
		if (pInputKeyboard->GetPress(DIK_RCONTROL) == true)
		{
			m_posV.y -= CAMERA_SPEED;
			m_posR.y -= CAMERA_SPEED;
		}

		if (pInputKeyboard->GetTrigger(DIK_4) == true)
		{
			m_CameraDistance -= 500.0f;
		}

		if (pInputKeyboard->GetTrigger(DIK_5) == true)
		{
			m_CameraDistance += 500.0f;
		}

		//キーボード
		if (pInputKeyboard->GetPress(DIK_T) == true)
		{
			m_posR.z += CAMERA_SPEED * cosf(m_rot.y);
			m_posR.x += CAMERA_SPEED * sinf(m_rot.y);

			m_posV.z += CAMERA_SPEED * cosf(m_rot.y);
			m_posV.x += CAMERA_SPEED * sinf(m_rot.y);
		}
		if (pInputKeyboard->GetPress(DIK_G) == true)
		{
			m_posR.z += -CAMERA_SPEED * cosf(m_rot.y);
			m_posR.x += -CAMERA_SPEED * sinf(m_rot.y);

			m_posV.z += -CAMERA_SPEED * cosf(m_rot.y);
			m_posV.x += -CAMERA_SPEED * sinf(m_rot.y);
		}
		if (pInputKeyboard->GetPress(DIK_F) == true)
		{
			m_posR.x += -CAMERA_SPEED * cosf(m_rot.y);
			m_posR.z -= -CAMERA_SPEED * sinf(m_rot.y);

			m_posV.x += -CAMERA_SPEED * cosf(m_rot.y);
			m_posV.z -= -CAMERA_SPEED * sinf(m_rot.y);
		}
		if (pInputKeyboard->GetPress(DIK_H) == true)
		{
			m_posR.x += CAMERA_SPEED * cosf(m_rot.y);
			m_posR.z -= CAMERA_SPEED * sinf(m_rot.y);

			m_posV.x += CAMERA_SPEED * cosf(m_rot.y);
			m_posV.z -= CAMERA_SPEED * sinf(m_rot.y);
		}

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{
			//視点の情報を出力する
			m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
			m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
			m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		}
		break;
	case CAMERAMODE_FOLLOW:

		if (m_rot.x <= D3DX_PI * 0.5f && m_rot.x >= -(D3DX_PI * 0.5f))
		{//入力
			m_rotOld.x = m_rot.x;

			//キーボード
			if (pInputKeyboard->GetPress(DIK_I) == true)
			{
				m_rot.x += CAMERA_VR_SPEED;
			}
			if (pInputKeyboard->GetPress(DIK_K) == true)
			{
				m_rot.x -= CAMERA_VR_SPEED;
			}

			//右スティックの上下視点移動入力
			m_rot.x -= pInputMouse->GetMouseMove().y * CAMERA_VR_SPEED;

		}

		if (fabsf(m_rot.x) > fabsf(D3DX_PI * 0.4f))
		{//上限に達した時１フレーム前のrotにもどる
			m_rot.x = m_rotOld.x;
		}

		//キーボード
		if (pInputKeyboard->GetPress(DIK_J) == true)
		{
			m_rot.y -= CAMERA_VR_SPEED;

		}
		if (pInputKeyboard->GetPress(DIK_L) == true)
		{
			m_rot.y += CAMERA_VR_SPEED;
		}

		//右スティックの左右視点移動入力
		m_rot.y += pInputMouse->GetMouseMove().x * CAMERA_VR_SPEED;


		//一周した時の向きの補正
		if (m_rot.y > D3DX_PI * 1.0f)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		else if (m_rot.y < -D3DX_PI * 1.0f)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		//モデル追従処理
		m_posRDest.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
		m_posRDest.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
		m_posRDest.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE;

		m_posVDest.x = pPlayer->GetPos().x + sinf(-pPlayer->GetRot().y) * MODEL_DISTANCE + sinf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE;
		m_posVDest.z = pPlayer->GetPos().z + cosf(-pPlayer->GetRot().y) * MODEL_DISTANCE + cosf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE;
		m_posVDest.y = pPlayer->GetPos().y + 100.0f + sinf(-m_rot.x) * CAMERA_DISTANCE;

		m_posR.x += (m_posRDest.x - m_posR.x) * CAMERA_HOMING;
		m_posR.z += (m_posRDest.z - m_posR.z) * CAMERA_HOMING;
		m_posV.x += (m_posVDest.x - m_posV.x) * CAMERA_HOMING * 5.0f;
		m_posV.z += (m_posVDest.z - m_posV.z) * CAMERA_HOMING * 5.0f;

		if (pPlayer->GetJump() == false)
		{
			m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_HOMING;
			m_posV.y += (m_posVDest.y - m_posV.y);
		}
		else
		{
			if (m_posR.y > (pPlayer->GetPos().y + 100.0f))
			{
				m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_HOMING;
				m_posV.y += (m_posVDest.y - m_posV.y);
			}
			else
			{
				m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_HOMING * 0.05f;
				m_posV.y += (m_posVDest.y - m_posV.y);
			}
		}

		//モデルが止まった時に正面を向く処理
		float fRotMove, fRotDest, fRotDiff;

		if (m_FollowTime > 0)
		{
			m_FollowTime--;

			fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));	//現在の向き
			fRotDest = atan2f(sinf(pPlayer->GetRot().y + D3DX_PI), cosf(pPlayer->GetRot().y + D3DX_PI));	//目的の向き
			fRotDiff = fRotDest - fRotMove;									 //差分

			if (fRotDiff > D3DX_PI * 1.0f)
			{
				fRotDiff -= D3DX_PI * 2.0f;
			}
			else if (fRotDiff < -D3DX_PI * 1.0f)
			{
				fRotDiff += D3DX_PI * 2.0f;
			}

			fRotMove += fRotDiff * 0.1f;

			if (fRotMove > D3DX_PI * 1.0f)
			{
				fRotMove -= D3DX_PI * 2.0f;
			}
			else if (fRotMove < -D3DX_PI * 1.0f)
			{
				fRotMove += D3DX_PI * 2.0f;
			}
			m_rot.y = fRotMove;
		}

		break;
	case CAMERAMODE_DOWNVIEW:

		m_rot.y = 0.0f;
		m_posV.x += (DOWNVIEW_POSV.x - m_posV.x) * CAMERA_HOMING;
		m_posV.y += (DOWNVIEW_POSV.y - m_posV.y) * CAMERA_HOMING;
		m_posV.z += (DOWNVIEW_POSV.z - m_posV.z) * CAMERA_HOMING;
		m_posR.x += (DOWNVIEW_POSR.x - m_posR.x) * CAMERA_HOMING;
		m_posR.y += (DOWNVIEW_POSR.y - m_posR.y) * CAMERA_HOMING;
		m_posR.z += (DOWNVIEW_POSR.z - m_posR.z) * CAMERA_HOMING;

		break;
	case CAMERAMODE_2D:

		m_rot.y = 0.0f;
		m_posV.x += (SIDEVIEW_POSV.x - m_posV.x) * CAMERA_HOMING;
		m_posV.y += (SIDEVIEW_POSV.y - m_posV.y) * CAMERA_HOMING;
		m_posV.z += (SIDEVIEW_POSV.z - m_posV.z) * CAMERA_HOMING;
		m_posR.x += (SIDEVIEW_POSR.x - m_posR.x) * CAMERA_HOMING;
		m_posR.y += (SIDEVIEW_POSR.y - m_posR.y) * CAMERA_HOMING;
		m_posR.z += (SIDEVIEW_POSR.z - m_posR.z) * CAMERA_HOMING;

		break;
	}

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TITLE)
	{
		//視点の情報を出力する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	}
	else if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_RESULT)
	{
		//視点の情報を出力する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	}

	//デバッグ表示の取得
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("%f:%f:%f\n", m_posV.x, m_posV.y, m_posV.z);
	pDebugProc->Print("%f:%f:%f\n", m_posR.x, m_posR.y, m_posR.z);

}

//====================================================================
//カメラの設定処理
//====================================================================
void CCamera::SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		10000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//====================================================================
//カメラの設定処理
//====================================================================
void CCamera::ResetCamera(void)
{
	m_posV = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rotOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CameraDistance = CAMERA_DISTANCE;
	m_EventPosY = 0.0f;

	if (CScene::MODE_GAME == CScene::GetMode())
	{
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}