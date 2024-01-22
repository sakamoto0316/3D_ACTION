//============================================
//
//	�Q�[����� [game.cpp]
//	Author:sakamoto kai
//
//============================================
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "objmeshField.h"
#include "objmeshWall.h"
#include "objmeshCylinder.h"
#include "objmeshDome.h"
#include "CubeBlock.h"
#include "CubeDamage.h"
#include "player.h"
#include "boss.h"
#include "Pause.h"
#include "camera.h"
#include "game.h"
#include "sound.h"
#include "time.h"
#include "debugproc.h"
#include "Xmodel.h"
#include "texture.h"

//�ÓI�����o�ϐ��錾
CTutorialUI *CGame::m_pTutorialUI = NULL;
CEdit *CGame::m_pEdit = NULL;
CPause *CGame::m_pPause = NULL;
CScore *CGame::m_pScore = NULL;
CTime *CGame::m_pTime = NULL;
CObject2D *CGame::m_p2DSample = NULL;
CObject3D *CGame::m_p3DSample = NULL;
CObjectBillboard* CGame::m_pBillboardSample = NULL;
CObjectX* CGame::m_pXModelSample = NULL;
CObjmeshField* CGame::m_pMeshFieldSample = NULL;
CObjmeshWall* CGame::m_pMeshWallSample = NULL;
CObjmeshCylinder* CGame::m_pMeshCylinderSample = NULL;
CObjmeshDome* CGame::m_pMeshDomeSample = NULL;
CCubeBlock* CGame::m_pCubeBlock = NULL;
CPlayer* CGame::m_pPlayer = NULL;
CBoss*CGame::m_pBoss = NULL;
bool CGame::m_bGameEnd = false;

//====================================================================
//�R���X�g���N�^
//====================================================================
CGame::CGame()
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CGame::~CGame()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CGame::Init(void)
{
	//�Q�[����BGM���Đ�����
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME);

	//m_p2DSample = CObject2D::Create();
	//m_p2DSample->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	//m_p2DSample->SetWight(100.0f);
	//m_p2DSample->SetHeight(100.0f);
	//m_p2DSample->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	m_p3DSample = CObject3D::Create();
	m_p3DSample->SetPos(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	m_p3DSample->SetRot(D3DXVECTOR3(1.57f, 0.0f, 0.0f));
	m_p3DSample->SetWight(100.0f);
	m_p3DSample->SetHeight(100.0f);

	m_pBillboardSample = CObjectBillboard::Create();
	m_pBillboardSample->SetPos(INITVECTOR3);

	m_pXModelSample = CObjectX::Create("data\\MODEL\\player00.x");
	m_pXModelSample->SetPos(D3DXVECTOR3(-200.0f, 0.0f, 0.0f));

	//m_pMeshFieldSample = CObjmeshField::Create();
	//m_pMeshFieldSample->SetPos(D3DXVECTOR3(0.0f, 0.0f, 200.0f));

	//m_pMeshWallSample = CObjmeshWall::Create();
	//m_pMeshWallSample->SetPos(D3DXVECTOR3(0.0f, 0.0f, 300.0f));

	//m_pMeshCylinderSample = CObjmeshCylinder::Create();

	//m_pMeshDomeSample = CObjmeshDome::Create();

	m_pCubeBlock = CCubeBlock::Create();
	m_pCubeBlock->SetPos(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	m_pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 10.0f, 500.0f));
	m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pCubeBlock = CCubeBlock::Create();
	m_pCubeBlock->SetPos(D3DXVECTOR3(150.0f, 250.0f, -150.0f));
	m_pCubeBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
	m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pCubeBlock = CCubeBlock::Create();
	m_pCubeBlock->SetPos(D3DXVECTOR3(-150.0f, 250.0f, 150.0f));
	m_pCubeBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
	m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 150.0f, 0.0f));

	m_pBoss = CBoss::Create("data\\MODEL\\boss.x");
	m_pBoss->SetPos(D3DXVECTOR3(100.0f, 300.0f, 100.0f));

	if (m_pPause == NULL)
	{
		m_pPause = CPause::Create();
	}

	m_bGameEnd = false;
	CManager::GetInstance()->GetInstance()->SetStop(false);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CGame::Uninit(void)
{
	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	if (m_pPause != NULL)
	{
		delete m_pPause;
		m_pPause = NULL;
	}
}

//====================================================================
//�I������
//====================================================================
void CGame::DeleteTutorial(void)
{

}

//====================================================================
//�X�V����
//====================================================================
void CGame::Update(void)
{
#if _DEBUG
	//���Z�b�g����
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_R) == true)
	{
		ReSetGame();
	}

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		CFade::SetFade(CScene::MODE_RESULT);
	}

	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		m_pCubeBlock = CCubeBlock::Create();
		m_pCubeBlock->SetPos(D3DXVECTOR3(-100.0f, -60.0f, 0.0f));
		m_pCubeBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
		m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	}
	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		m_pCubeBlock = CCubeBlock::Create();
		m_pCubeBlock->SetPos(D3DXVECTOR3(100.0f, 500.0f, 0.0f));
		m_pCubeBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
		m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, -5.0f, 0.0f));
	}

	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{
		CCubeDamage *pCubeDamage = CCubeDamage::Create();
		pCubeDamage->SetPos(D3DXVECTOR3(-300.0f, 150.0f, 0.0f));
		pCubeDamage->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
		pCubeDamage->SetMove(D3DXVECTOR3(20.0f, 0.0f, 0.0f));
		pCubeDamage->SetBreak(false);
	}

	if (pInputKeyboard->GetTrigger(DIK_4) == true)
	{
		CCubeDamage* pCubeDamage = CCubeDamage::Create();
		pCubeDamage->SetPos(D3DXVECTOR3(-300.0f, 150.0f, 0.0f));
		pCubeDamage->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
		pCubeDamage->SetMove(D3DXVECTOR3(20.0f, 0.0f, 0.0f));
		pCubeDamage->SetBreak(true);
	}

#endif

	//�|�[�Y�̍X�V����
	m_pPause->Update();
}

//====================================================================
//���Z�b�g����
//====================================================================
void CGame::ReSetGame(void)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CGame::Draw(void)
{

}
