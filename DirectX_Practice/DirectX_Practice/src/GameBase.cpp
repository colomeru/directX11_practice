#include "GameBase.h"
#include "Window.h"
#include "Input/Keyboard.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "DirectX/DirectX11.h"
#include "DirectX/Texture.h"
#include "DirectX/RenderTarget.h"
#include "DirectX/DepthStencil.h"
#include <atlbase.h>

#include "ConstantBuffer/ConstantBuffer.h"

#include "Camera/Camera.h"
#include "Light/Light.h"
#include "Timer/MySleep.h"

// �Ǘ��N���X
#include "Shader/ShaderManager.h"
#include "Graphic/SpriteManager.h"
#include "Graphic/ModelManager.h"
#include "Graphic/Model.h"
#include "Graphic/RenderTexture.h"

#include "Graphic/Font/DrawFont.h"
#include "ShadowMap.h"
#include <Windows.h>

#include "util/math/MathHelper.h"
#include "util/math/Vector2.h"

// ���_�V�F�[�_�[
#include "Shader/vertex/ShadowMeshVertexShader.h"
#include "Shader/vertex/ShadowMapVertexShader.h"
#include "Shader/vertex/TextureVertexShader.h"
#include "Shader/vertex/FontVertexShader.h"
// �s�N�Z���V�F�[�_
#include "Shader/pixel/ShadowMeshPixelShader.h"
#include "Shader/pixel/TexturePixelShader.h"
#include "Shader/pixel/PostEffectPixelShader.h"

#include "Shader/MeshShader.h"
#include "Shader/ShadowMapShader.h"

#include <future>

enum class DRAW_PATTERN
{
	Default		= 0,
	Monochrome	= 1,
	Bloom		= 2,
	Uncharted2	= 3,
};

enum class DRAW_MODEL
{
	None		= 0,
	Mirai_Akari = 1,
	Siro		= 2,
	Kouhai_chan = 3,
	Ichigo		= 4,
	MAX			= 5,
};

GameBase::GameBase() :
	m_MultiSampleCount(1),
	m_MultiSampleQuality(0),
	m_MultiSampleMaxQuality(m_MultiSampleQuality),
	m_SwapChainCount(2),
	m_SwapChainFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
	m_DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT),
	m_Width(1280),
	m_Height(720),
	m_AspectRatio((float)m_Width / (float)m_Height)
{
}

GameBase::~GameBase()
{
}

bool GameBase::Run(HINSTANCE hIns)
{
	HRESULT hr;

	// �E�B���h�E�̍쐬
	Window::GetInstance()->Create("DirectX11_Practice", m_Width, m_Height);

	// ������
	if (!DirectX11::GetInstance()->Initialize())
		return false;

	// DirectInput������
	LPDIRECTINPUT8 dInput;
	hr = DirectInput8Create(hIns, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL);
	if (!SUCCEEDED(hr))
		return false;

	// ���͊֌W������
	Keyboard::GetInstance()->Initialize(Window::GetInstance()->GetHandle(), dInput);

	// �}���`�T���v���̍ő�i�����擾
	hr = DirectX11::GetInstance()->GetDevice()->CheckMultisampleQualityLevels(m_SwapChainFormat, m_MultiSampleCount, &m_MultiSampleMaxQuality);
	if (FAILED(hr))
		return false;

	// �����_�[�^�[�Q�b�g�̐���
	Texture backBuffer;
	backBuffer.Create();
	RenderTarget renderTarget;
	renderTarget.Create(&backBuffer);

	// �`���𗠉�ʂɂ���
	DirectX11::GetInstance()->SetRenderTarget(&renderTarget, nullptr);

	DXGI_FORMAT textureFormat  = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT resourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// �e�N�X�`���ƃV�F�[�_�[���\�[�X�r���[�̃t�H�[�}�b�g��K�؂Ȃ��̂ɕύX
	switch (m_DepthStencilFormat)
	{
	case DXGI_FORMAT_D16_UNORM:
	{
		textureFormat  = DXGI_FORMAT_R16_TYPELESS;
		resourceFormat = DXGI_FORMAT_R16_UNORM;
	}
	break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	{
		textureFormat  = DXGI_FORMAT_D24_UNORM_S8_UINT;
		resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	}
	break;
	case DXGI_FORMAT_D32_FLOAT:
	{
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT;
	}
	break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	{
		textureFormat  = DXGI_FORMAT_R32G8X24_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
	}
	break;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width		= m_Width;	// �o�b�N�o�b�t�@�[�Ɠ����T�C�Y���w��
	textureDesc.Height		= m_Height;	// �o�b�N�o�b�t�@�[�Ɠ����T�C�Y���w��
	textureDesc.Format		= textureFormat;
	textureDesc.Usage		= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags	= D3D11_BIND_DEPTH_STENCIL;
	textureDesc.MiscFlags	= 0;
	textureDesc.CPUAccessFlags		= 0;
	textureDesc.MipLevels			= 1;
	textureDesc.ArraySize			= 1;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.SampleDesc.Quality	= 0;

	// �f�v�X�X�e���V���̐���
	Texture depthTexture;
	depthTexture.Create(textureDesc);
	DepthStencil depthStencil;
	depthStencil.Create(&depthTexture, textureDesc.Format);

	/* �����_�[�e�N�X�`������ */
	// �ʏ�p
	RenderTexture rtDefault(&depthStencil);
	rtDefault.Create(m_Width, m_Height);
	// �O���C�X�P�[���p
	RenderTexture rtGrayScale(nullptr);
	rtGrayScale.Create(m_Width, m_Height);
	// ���P�x���o�p
	RenderTexture rtBright(nullptr);
	rtBright.Create(m_Width, m_Height);
	// ���������u���[�V�F�[�_�[
	RenderTexture rtBlurH(nullptr);
	rtBlurH.Create(m_Width, m_Height);
	// ���������u���[�V�F�[�_�[
	RenderTexture rtBlurVRT(nullptr);
	rtBlurVRT.Create(m_Width, m_Height);
	// �u���[�����ʃV�F�[�_�[
	RenderTexture rtBloomCombine(nullptr);
	rtBloomCombine.Create(m_Width, m_Height);
	// �A���`���[�e�b�h�Q�g�D�[���}�b�v�V�F�[�_�[
	RenderTexture rtUncharted2ToneMap(nullptr);
	rtUncharted2ToneMap.Create(m_Width, m_Height);

	// ���\�[�X�̓ǂݍ���
	LoadResources();

	// �~���C�A�J��
	auto mirai_akari = ModelManager::GetInstance()->Get(MODEL_ID::MIRAI_AKARI_MODEL);
	// �������ɃA���t�@�u�����h���K�v�Ȃ���
	mirai_akari->SetAlphaBlend(true, true);
	// �V��
	auto siro = ModelManager::GetInstance()->Get(MODEL_ID::SIRO_MODEL);
	// �͂�����t�[�Y
	siro->SetAlphaBlend(true, false);
	// ��y�����
	auto kouhai_chan = ModelManager::GetInstance()->Get(MODEL_ID::KOUHAI_CHAN_MODEL);
	kouhai_chan->SetAlphaBlend(true, false);
	// ������
	auto ichigo = ModelManager::GetInstance()->Get(MODEL_ID::ICHIGO_MODEL);
	// �z
	ichigo->SetAlphaBlend(true, false);
	// �X�e�[�W
	auto stage = ModelManager::GetInstance()->Get(MODEL_ID::STAGE_MODEL);

	// �萔�o�b�t�@�̐ݒ�
	CComPtr<ID3D11Buffer> pBloomBuffer;
	DirectX11::GetInstance()->CreateBuffer(&pBloomBuffer, nullptr, sizeof(BloomCB), D3D11_BIND_CONSTANT_BUFFER);

	// �V���h�E�}�b�v�p�̃e�N�X�`���̐ݒ�
	int  index  = 11;
	UINT smSize = MathHelper::Pow(2, index);

	// �V���h�E�}�b�v����
	ShadowMap shadowMap;
	if (!shadowMap.Create(smSize))
		return false;

	// ���e�f�[�^�̐ݒ�
	Camera::GetInstance()->SetFovAngle(60.0f);
	Camera::GetInstance()->SetAspect(m_AspectRatio);
	Camera::GetInstance()->SetNearFar(1.0f, 1000.0f);
	Camera::GetInstance()->SetUp(Vector3::Up);

	// FPS���䏀��
	MySleep sleep;
	sleep.QuerySet();

	float angle	 = 0.0f;
	float light  = 0.0f;
	float cangle = 0.0f;
	float ey	 = 0.0f;
	float cl	 = 50.0f;

	// �`��p�^�[��
	DRAW_PATTERN drawPattern = DRAW_PATTERN::Default;
	DRAW_MODEL	 drawModel	 = DRAW_MODEL::Mirai_Akari;
	bool		 debugDraw   = false;

	float brightPassThreshold = 0.7f;
	float bloomIntensity	= 1.0f;
	float bloomSaturation	= 1.0f;
	float baseIntensity		= 1.0f;
	float baseSaturation	= 1.0f;

	// �N���A�J���[
	static float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	// �V�F�[�_�[����
	Effect effectDefault(VERTEX_SHADER_ID::SHADOW_MESH_SHADER, PIXEL_SHADER_ID::SHADOW_MESH_SHADER);
	Effect effectShadowMap(VERTEX_SHADER_ID::SHADOW_SHADER);
	Effect effectGrayScale(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::GRAYSCALE_SHADER);
	Effect effectBright(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::BRIGHTPASS_SHADER);
	Effect effectBlurH(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::GAUSSIANBLUR_H_SHADER);
	Effect effectBlurV(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::GAUSSIANBLUR_V_SHADER);
	Effect effectBloomCombine(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::BLOOM_SHADER);
	Effect effectUncharted2ToneMap(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::UNCHARTED2_TOONMAP_SHADER);

	// �Q�[�����[�v
	while (Window::GetInstance()->MessageHandling())
	{
		// ���͊֌W�X�V
		Keyboard::GetInstance()->Update();

		// �����I��
		if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_ESC))
			break;

		/*** �Q�[���̏��� ***/
		DirectX11::GetInstance()->ClearRenderTarget(&renderTarget, color);

		/* �L�[�{�[�h�̓��� */
		{
			// �`����̐؂�ւ�
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_1))
				drawPattern = DRAW_PATTERN::Default;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_2))
				drawPattern = DRAW_PATTERN::Bloom;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_3))
				drawPattern = DRAW_PATTERN::Uncharted2;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_4))
				drawPattern = DRAW_PATTERN::Monochrome;

			// �V���h�E�}�b�v�̉𑜓x�̕ύX
			int prevIndex = index;
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_Q))
				index--;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_E))
				index++;
			if (index != prevIndex)
			{
				index = MathHelper::Clamp(index, 9, 12);
				shadowMap.Create(MathHelper::Pow(2, index));
			}

			// �`�惂�f���̐؂�ւ�
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_C))
			{
				int current = static_cast<int>(drawModel);
				current++;
				current   = MathHelper::Mod(current, static_cast<int>(DRAW_MODEL::MAX));
				drawModel = static_cast<DRAW_MODEL>(current);
			}
			// ���f����]
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_Z))
				angle += 0.5f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_X))
				angle -= 0.5f;

			// �p�����[�^�`������邩�H
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_SPACE))
				debugDraw = !debugDraw;

			// �����̈ʒu��ݒ�
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_A))
				light += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_D))
				light -= 0.01f;

			// �J�����̈ʒu��ݒ�
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_UP))
				cl -= 1.0f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_DOWN))
				cl += 1.0f;
			cl = MathHelper::Clamp(cl, 10.0f, 150.0f);
			// �J������]
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_LEFT))
				cangle += 0.2f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_RIGHT))
				cangle -= 0.2f;
		}

		/* �|�X�g�G�t�F�N�g�p�����[�^�̑��� */
		{
			// �P�x��臒l
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_Y))
				brightPassThreshold += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_G))
				brightPassThreshold -= 0.01f;
			brightPassThreshold = MathHelper::Clamp(brightPassThreshold, 0.0f, 1.0f);

			// ���摜�̃e�N�X�`���̍ʓx
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_U))
				bloomIntensity += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_H))
				bloomIntensity -= 0.01f;
			bloomIntensity = MathHelper::Clamp(bloomIntensity, 0.0f, 1.0f);

			// ���摜�̃e�N�X�`���̋P�x
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_I))
				bloomSaturation += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_J))
				bloomSaturation -= 0.01f;
			bloomSaturation = MathHelper::Clamp(bloomSaturation, 0.0f, 1.0f);

			// �u���[���e�N�X�`���̍ʓx
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_O))
				baseIntensity += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_K))
				baseIntensity -= 0.01f;
			baseIntensity = MathHelper::Clamp(baseIntensity, 0.0f, 1.0f);

			// �u���[���e�N�X�`���̋P�x
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_P))
				baseSaturation += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_L))
				baseSaturation -= 0.01f;
			baseSaturation = MathHelper::Clamp(baseSaturation, 0.0f, 1.0f);

			// �|�X�g�G�t�F�N�g�̃p�����[�^��������
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_R))
			{
				brightPassThreshold = 0.7f;
				bloomIntensity		= 1.0f;
				bloomSaturation		= 1.0f;
				baseIntensity		= 1.0f;
				baseSaturation		= 1.0f;
			}
		}

		//shadowMap.Update();

		Vector3 eye		= Vector3(0.0f, 13.0f,  -cl);
		Vector3 focus	= Vector3(0.0f, 13.0f, 0.0f);

		auto eyePosMat = Matrix::CreateTranslation(eye) * Matrix::CreateRotationY(cangle);

		// �J�������X�V
		Camera::GetInstance()->SetPosition(eyePosMat.Translation());
		Camera::GetInstance()->SetTarget(focus);

		float deglight = MathHelper::ToDegrees(light);
		Light::GetInstance()->SetPosition(Vector3(80.0f * MathHelper::Sin(deglight), 90.0f, -80.0f * MathHelper::Cos(deglight)));

		auto worldMatrix = Matrix::CreateRotationY(angle);

		ShadowMapShader shadowMapShader(effectShadowMap);

		/* �`�揈�� */
		// �V���h�E�}�b�v����
		shadowMap.Begin(effectShadowMap);

		DirectX11::GetInstance()->SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_FRONT);

		// ���f���̕`��
		stage->DrawForShadow(shadowMapShader, Matrix::Identity);
		switch (drawModel)
		{
		case DRAW_MODEL::Mirai_Akari: 
			mirai_akari->DrawForShadow(shadowMapShader, worldMatrix); break;
		case DRAW_MODEL::Siro:  	 
			siro->DrawForShadow(shadowMapShader, worldMatrix);	break;
		case DRAW_MODEL::Kouhai_chan:
			kouhai_chan->DrawForShadow(shadowMapShader, worldMatrix); break;
		case DRAW_MODEL::Ichigo:
			ichigo->DrawForShadow(shadowMapShader, worldMatrix); break;
		default: break;
		}

		shadowMap.End(effectShadowMap);
		
		// �ʏ�`��
		rtDefault.Begin();
		rtDefault.Clear();

		// ������񔽉f
		Light::GetInstance()->Draw();
		// �J������񔽉f
		Camera::GetInstance()->Draw();

		// �e�f�[�^�Z�b�g
		shadowMap.Set();

		/* ���f���̕`�� */
		MeshShader shader(effectDefault);
		// �X�e�[�W���f��
		stage->Draw(shader, Matrix::Identity);
		// �L�����N�^�[���f��
		switch (drawModel)
		{
		case DRAW_MODEL::Mirai_Akari:
			DirectX11::GetInstance()->SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_FRONT);
			mirai_akari->Draw(shader, worldMatrix); break;
		case DRAW_MODEL::Siro:
			DirectX11::GetInstance()->SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_NONE);
			siro->Draw(shader, worldMatrix); break;
		case DRAW_MODEL::Kouhai_chan:
			DirectX11::GetInstance()->SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_FRONT);
			kouhai_chan->Draw(shader, worldMatrix); break;
		case DRAW_MODEL::Ichigo:	
			DirectX11::GetInstance()->SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_NONE);
			ichigo->Draw(shader, worldMatrix);	break;
		default: break;
		}

		shadowMap.Clear();
		rtDefault.End();

		/* �|�X�g�G�t�F�N�g�J�n */
		// �O���C�X�P�[���ŕ`��
		if (drawPattern == DRAW_PATTERN::Monochrome)
		{
			rtGrayScale.Begin();
			rtGrayScale.Clear();
			effectGrayScale.Begin();
			rtGrayScale.Set(rtDefault);
			rtGrayScale.Draw();
			effectDefault.End();
			rtGrayScale.End();
		}
		else
		{
			if (drawPattern != DRAW_PATTERN::Default)
			{
				// �s�N�Z���V�F�[�_�Ƀp�����[�^�̃Z�b�g
				{
					BloomCB bloomCB;
					bloomCB.g_BrightPassThreshold = brightPassThreshold;
					bloomCB.g_BloomIntensity = bloomIntensity;
					bloomCB.g_BloomSaturation = bloomSaturation;
					bloomCB.g_BaseIntensity = baseIntensity;
					bloomCB.g_BaseSaturation = baseSaturation;

					DirectX11::GetInstance()->GetContext()->UpdateSubresource(pBloomBuffer, 0, NULL, &bloomCB, 0, 0);
					DirectX11::GetInstance()->GetContext()->PSSetConstantBuffers(0, 1, &pBloomBuffer.p);
				}

				// ���P�x���o
				rtBright.Begin();
				rtBright.Clear();
				effectBright.Begin();
				rtBright.Set(rtDefault);
				rtBright.Draw();
				effectBright.End();
				rtBright.End();

				// ���������u���[
				rtBlurH.Begin();
				rtBlurH.Clear();
				effectBlurH.Begin();
				rtBlurH.Set(rtBright);
				rtBlurH.Draw();
				effectBlurH.End();
				rtBlurH.End();

				// ���������u���[
				rtBlurVRT.Begin();
				rtBlurVRT.Clear();
				effectBlurV.Begin();
				rtBlurVRT.Set(rtBlurH);
				rtBlurVRT.Draw();
				effectBlurV.End();
				rtBlurVRT.End();

				// ���摜�ƃu���[�摜������
				rtBloomCombine.Begin();
				rtBloomCombine.Clear();
				effectBloomCombine.Begin();
				rtBloomCombine.Set(rtDefault);
				rtBloomCombine.Set(rtBlurVRT, 1);
				rtBloomCombine.Draw();
				effectBloomCombine.End();
				rtBloomCombine.End(1);
				rtBloomCombine.End();

				if (drawPattern == DRAW_PATTERN::Uncharted2)
				{
					// �A���`���[�e�b�h�Q�g�D�[���}�b�v
					rtUncharted2ToneMap.Begin();
					rtUncharted2ToneMap.Clear();
					effectUncharted2ToneMap.Begin();
					rtUncharted2ToneMap.Set(rtBloomCombine);
					rtUncharted2ToneMap.Draw();
					effectUncharted2ToneMap.End();
					rtUncharted2ToneMap.End();
				}
			}
		}

		/* �ŏI�`�� */
		// �`����Z�b�g
		switch (drawPattern)
		{
		case DRAW_PATTERN::Default:	   SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, rtDefault); break;
		case DRAW_PATTERN::Monochrome: SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, rtGrayScale);	break;
		case DRAW_PATTERN::Bloom:
		{
			//SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, effectBloomCombineRT); break;
			SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, rtBright.pSRV.p, rtBright.pSampler.p, rtBright.GetWidth() / 2, rtBright.GetHeight() / 2);
			SpriteManager::GetInstance()->DrawGraph(Vector2(0, m_Height / 2), rtBloomCombine.pSRV.p, rtBloomCombine.pSampler.p, rtBloomCombine.GetWidth() / 2, rtBloomCombine.GetHeight() / 2);
			SpriteManager::GetInstance()->DrawGraph(Vector2(m_Width / 2, 0), rtBlurH.pSRV.p, rtBlurH.pSampler.p, rtBlurH.GetWidth() / 2, rtBlurH.GetHeight() / 2); 
			SpriteManager::GetInstance()->DrawGraph(Vector2(m_Width / 2, m_Height / 2), rtBlurVRT.pSRV.p, rtBlurVRT.pSampler.p, rtBlurVRT.GetWidth() / 2, rtBlurVRT.GetHeight() / 2);
			break;
		}
		case DRAW_PATTERN::Uncharted2: SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, rtUncharted2ToneMap);	break;
		default: break;
		}

		// FPS�`��
		DrawFont::GetInstance()->Draw(Vector2(10.0f, 10.0f), DrawFont::GetInstance()->format("FPS : %.1f", sleep.fps));

		// �p�����[�^�`��
		if (debugDraw)
		{
			DrawFont::GetInstance()->Draw(Vector2(10.0f,  30.0f), DrawFont::GetInstance()->format("���C�g��]     : A/D"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f,  50.0f), DrawFont::GetInstance()->format("���f����]     : Z/X"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f,  70.0f), DrawFont::GetInstance()->format("���f���؂�ւ�   : C"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f,  90.0f), DrawFont::GetInstance()->format("�J�����ړ�     : �\���L�["));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 110.0f), DrawFont::GetInstance()->format("�V�F�[�_�[�؂�ւ� : 1/2/3/4"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 130.0f), DrawFont::GetInstance()->format("�e�𑜓x�؂�ւ� �@: Q/E"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 200.0f), DrawFont::GetInstance()->format("�P�x��臒l   : Y/G : %.2f", brightPassThreshold));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 220.0f), DrawFont::GetInstance()->format("���摜�̍ʓx  : P/L : %.2f", baseSaturation));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 240.0f), DrawFont::GetInstance()->format("���摜�̋P�x  : O/K : %.2f", baseIntensity));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 260.0f), DrawFont::GetInstance()->format("�u���[���̍ʓx : I/J : %.2f", bloomSaturation));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 280.0f), DrawFont::GetInstance()->format("�u���[���̋P�x : U/H : %.2f", bloomIntensity));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 300.0f), DrawFont::GetInstance()->format("�p�����[�^�̏����� : R"));
		}
		else
		{
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 30.0f), DrawFont::GetInstance()->format("DebugText : SPACE"));
		}

		
		//SpriteManager::GetInstance()->DrawGraph(
		//	Vector2(100, 100), 
		//	DrawFont::GetInstance()->m_DebugFont.m_FontTexture.pSRV.p,
		//	DrawFont::GetInstance()->m_DebugFont.m_FontTexture.pSampler.p,
		//	DrawFont::GetInstance()->m_DebugFont.m_FontTexture.GetWidth(),
		//	DrawFont::GetInstance()->m_DebugFont.m_FontTexture.GetHeight());

		//shadowMap.Draw();
		//SpriteManager::GetInstance()->Draw(SPRITE_ID::TEST_SPRITE, Vector2(100, 100));

		// FPS����
		sleep.Wait();

		// ��ʂ̍X�V
		DirectX11::GetInstance()->GetSwapChain()->Present(0, 0);
	}

	ModelManager::GetInstance()->Clear();
	SpriteManager::GetInstance()->Clear();

	return true;
}

void GameBase::LoadResources()
{
	// ���\�[�X�֌W������

	/* �V�F�[�_�[�̓ǂݍ��� */
	// ���_�V�F�[�_�̓ǂݍ���
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::SHADOW_MESH_SHADER, std::make_shared<ShadowMeshVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::SHADOW_SHADER, std::make_shared<ShadowMapVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::TEXTURE_SHADER, std::make_shared<TextureVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::FONT_SHADER, std::make_shared<FontVertexShader>());

	// �s�N�Z���V�F�[�_�̓ǂݍ���
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::SHADOW_MESH_SHADER, std::make_shared<ShadowMeshPixelShader>());
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::TEXTURE_SHADER, std::make_shared<TexturePixelShader>());
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::GRAYSCALE_SHADER, std::make_shared<PostEffectPixelShader>("GrayScale.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::BRIGHTPASS_SHADER, std::make_shared<PostEffectPixelShader>("BrightPass.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::GAUSSIANBLUR_H_SHADER, std::make_shared<PostEffectPixelShader>("GaussianBlurH.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::GAUSSIANBLUR_V_SHADER, std::make_shared<PostEffectPixelShader>("GaussianBlurV.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::BLOOM_SHADER, std::make_shared<PostEffectPixelShader>("BloomCombine.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::UNCHARTED2_TOONMAP_SHADER, std::make_shared<PostEffectPixelShader>("Uncharted2Tonemap.hlsl"));

	// ���f���̓ǂݍ���
	ModelManager::GetInstance()->Load("MiraiAkari/MiraiAkari_v1.0.pmx", MODEL_ID::MIRAI_AKARI_MODEL);
	ModelManager::GetInstance()->Load("Siro/siro_dance_costume_v1.0.pmx", MODEL_ID::SIRO_MODEL);
	ModelManager::GetInstance()->Load("Kouhai_chan/Kouhai_chan.pmd", MODEL_ID::KOUHAI_CHAN_MODEL);
	ModelManager::GetInstance()->Load("Ichigo/Ichigo.pmx", MODEL_ID::ICHIGO_MODEL);
	ModelManager::GetInstance()->Load("Stage/Stage.pmx", MODEL_ID::STAGE_MODEL);

	// �X�v���C�g�̓ǂݍ���
	SpriteManager::GetInstance()->Initialize(m_Width, m_Height);
	SpriteManager::GetInstance()->Load("numakuro.png", SPRITE_ID::TEST_SPRITE);
	//SpriteManager::GetInstance()->Load("yuingo.jpg", SPRITE_ID::TEST_SPRITE);
}
