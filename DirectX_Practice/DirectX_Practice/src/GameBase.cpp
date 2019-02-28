#include "GameBase.h"
#include "Window.h"
#include "Input/Keyboard.h"
#include "Timer/MySleep.h"
#include "Util.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <atlbase.h>
#include "DirectX/DirectX11.h"
#include "DirectX/Texture.h"
#include "DirectX/RenderTarget.h"
#include "DirectX/DepthStencil.h"

#include "ConstantBuffer/ConstantBuffer.h"

#include "Camera/FreeCamera.h"
#include "Light/Light.h"

#include "Shader/ShaderManager.h"
#include "Graphic/SpriteManager.h"
#include "Graphic/Sprite.h"
#include "Graphic/ModelManager.h"
#include "Graphic/Model.h"
#include "Graphic/Font/FontManager.h"

#include "Graphic/RenderTexture.h"

#include "ShadowMap.h"
#include <Windows.h>

#include "util/math/MathHelper.h"
#include "util/math/Vector2.h"
#include "util/MyUtil.h"

#include "Shader/vertex/MeshVertexShader.h"
#include "Shader/vertex/ShadowMeshVertexShader.h"
#include "Shader/vertex/SkinnedMeshVertexShader.h"
#include "Shader/vertex/ShadowMapVertexShader.h"
#include "Shader/vertex/TextureVertexShader.h"
#include "Shader/vertex/FontVertexShader.h"

#include "Shader/pixel/MeshPixelShader.h"
#include "Shader/pixel/ShadowMeshPixelShader.h"
#include "Shader/pixel/TexturePixelShader.h"
#include "Shader/pixel/PostEffectPixelShader.h"

#include "Shader/MeshShader.h"
#include "Shader/ShadowMapShader.h"
#include "Shader/SkinnedMeshShader.h"

#include "SphereModel.h"

#include "Graphic\Model\Animation.h"

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
	Soldier		= 5,
	MAX			= 6,
};

GameBase::GameBase() :
	m_MultiSampleCount(1),
	m_MultiSampleQuality(0),
	m_MultiSampleMaxQuality(m_MultiSampleQuality),
	m_SwapChainCount(2),
	m_SwapChainFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
	m_DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT),
	m_Width(WINDOW_WIDTH),
	m_Height(WINDOW_HEIGHT),
	m_AspectRatio((float)m_Width / (float)m_Height)
{
}

GameBase::~GameBase()
{
}

bool GameBase::Run(HINSTANCE hIns)
{
	HRESULT hr;

	Window::GetInstance()->Create("DirectX11_Practice", m_Width, m_Height);

	if (!DirectX11::GetInstance()->Initialize())
		return false;

	// DirectInput生成
	LPDIRECTINPUT8 dInput;
	hr = DirectInput8Create(hIns, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL);
	if (!SUCCEEDED(hr))
		return false;

	Keyboard::GetInstance()->Initialize(Window::GetInstance()->GetHandle(), dInput);

	hr = DirectX11::GetInstance()->GetDevice()->CheckMultisampleQualityLevels(m_SwapChainFormat, m_MultiSampleCount, &m_MultiSampleMaxQuality);
	if (FAILED(hr))
		return false;

	// バックバッファの生成
	Texture backBufferTex;
	backBufferTex.Create();
	RenderTarget backBuffer;
	backBuffer.Create(&backBufferTex);

	DirectX11::GetInstance()->SetRenderTarget(&backBuffer, nullptr);

	DXGI_FORMAT textureFormat  = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT resourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 深度ステンシルのクオリティ変更
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
	textureDesc.Width		= m_Width;	// スクリーン横幅
	textureDesc.Height		= m_Height;	// スクリーン縦幅
	textureDesc.Format		= textureFormat;
	textureDesc.Usage		= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags	= D3D11_BIND_DEPTH_STENCIL;
	textureDesc.MiscFlags	= 0;
	textureDesc.CPUAccessFlags		= 0;
	textureDesc.MipLevels			= 1;
	textureDesc.ArraySize			= 1;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.SampleDesc.Quality	= 0;

	// 深度ステンシルの生成
	Texture depthTexture;
	depthTexture.Create(textureDesc);
	DepthStencil depthStencil;
	depthStencil.Create(&depthTexture, textureDesc.Format);

	/* ポストエフェクト用レンダーテクスチャ生成 */
	// 初期レンダーターゲット
	RenderTexture rTexDefault(&depthStencil);
	rTexDefault.Create(m_Width, m_Height);
	RenderTexture rTexGrayScale(nullptr);
	rTexGrayScale.Create(m_Width, m_Height);
	RenderTexture rTexBright(nullptr);
	rTexBright.Create(m_Width, m_Height);
	RenderTexture rTexBlurH(nullptr);
	rTexBlurH.Create(m_Width, m_Height);
	RenderTexture rTexBlurVRT(nullptr);
	rTexBlurVRT.Create(m_Width, m_Height);
	RenderTexture rTexBloomCombine(nullptr);
	rTexBloomCombine.Create(m_Width, m_Height);
	RenderTexture rTexUncharted2ToneMap(nullptr);
	rTexUncharted2ToneMap.Create(m_Width, m_Height);

	// リソース読み込み
	LoadResources();

	// ミライアカリモデル
	auto mirai_akari = ModelManager::GetInstance()->Get(MODEL_ID::MIRAI_AKARI_MODEL);
	mirai_akari->SetAlphaBlend(true, true);
	// 電脳少女シロモデル
	auto siro = ModelManager::GetInstance()->Get(MODEL_ID::SIRO_MODEL);
	siro->SetAlphaBlend(true, false);
	// 後輩ちゃんモデル
	auto kouhai_chan = ModelManager::GetInstance()->Get(MODEL_ID::KOUHAI_CHAN_MODEL);
	kouhai_chan->SetAlphaBlend(true, false);
	// いちごちゃんモデル
	auto ichigo = ModelManager::GetInstance()->Get(MODEL_ID::ICHIGO_MODEL);
	ichigo->SetAlphaBlend(true, false);
	// アリスモデル
	auto soldier = ModelManager::GetInstance()->Get(MODEL_ID::SOLDIER_MODEL);
	soldier->SetAlphaBlend(true, false);
	// ステージモデル
	auto stage = ModelManager::GetInstance()->Get(MODEL_ID::STAGE_MODEL);

	// ポストエフェクト用バッファ
	CComPtr<ID3D11Buffer> pBloomBuffer;
	DirectX11::GetInstance()->CreateBuffer(&pBloomBuffer, nullptr, sizeof(BloomCB), D3D11_BIND_CONSTANT_BUFFER);

	// シャドウマップのテクスチャサイズの冪数
	int  exp    = 11;
	UINT smSize = MathHelper::Pow(2, exp);

	ShadowMap shadowMap;
	if (!shadowMap.Create(smSize))
		return false;

	FreeCamera camera;

	MySleep sleep;
	sleep.QuerySet();

	float angle	 = 0.0f;
	float light  = 0.0f;

	// �`��p�^�[��
	DRAW_PATTERN drawPattern = DRAW_PATTERN::Default;
	DRAW_MODEL	 drawModel	 = DRAW_MODEL::Mirai_Akari;
	bool		 debugDraw   = false;

	float brightPassThreshold = 0.7f;
	float bloomIntensity	= 1.0f;
	float bloomSaturation	= 1.0f;
	float baseIntensity		= 1.0f;
	float baseSaturation	= 1.0f;

	// 背景カラー
	static float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	// �V�F�[�_�[����
	Effect effectMesh(VERTEX_SHADER_ID::MESH_SHADER, PIXEL_SHADER_ID::MESH_SHADER);
	Effect effectSkinnedMesh(VERTEX_SHADER_ID::SKINNED_MESH_SHADER, PIXEL_SHADER_ID::MESH_SHADER);
	Effect effectShadowMap(VERTEX_SHADER_ID::SHADOW_SHADER);
	Effect effectShadow(VERTEX_SHADER_ID::SHADOW_MESH_SHADER, PIXEL_SHADER_ID::SHADOW_MESH_SHADER);
	Effect effectGrayScale(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::GRAYSCALE_SHADER);
	Effect effectBright(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::BRIGHTPASS_SHADER);
	Effect effectBlurH(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::GAUSSIANBLUR_H_SHADER);
	Effect effectBlurV(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::GAUSSIANBLUR_V_SHADER);
	Effect effectBloomCombine(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::BLOOM_SHADER);
	Effect effectUncharted2ToneMap(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::UNCHARTED2_TOONMAP_SHADER);

	SphereModel sphere;

	Animation anim;
	//anim.Load("res/VMD/sweetmagic-left.vmd");
	anim.Load("res/VMD/FUCM_04_0001_RHiKick.vmd");

	float timer = 0.0f;
	float animEndFrame = anim.EndFrame();

	SkinnedMesh	skinnedMesh(soldier->mesh, soldier->skeleton, anim);
	
	// メインループ
	while (Window::GetInstance()->MessageHandling())
	{
		Keyboard::GetInstance()->Update();

		// 強制終了処理
		if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_ESC))
			break;

		/*** 以下ゲームメイン処理 ***/
		DirectX11::GetInstance()->ClearRenderTarget(&backBuffer, color);

		/* 入力処理 */
		{
			// モデル切り替え
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_1))
				drawPattern = DRAW_PATTERN::Default;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_2))
				drawPattern = DRAW_PATTERN::Bloom;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_3))
				drawPattern = DRAW_PATTERN::Uncharted2;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_4))
				drawPattern = DRAW_PATTERN::Monochrome;

			// 影解像度切り替え
			int prevIndex = exp;
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_Q))
				exp--;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_E))
				exp++;
			if (exp != prevIndex)
			{
				exp = MathHelper::Clamp(exp, 9, 12);
				shadowMap.Create(MathHelper::Pow(2, exp));
			}

			// 描画モデル切り替え
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_C))
			{
				int currentModelNum = static_cast<int>(drawModel);
				currentModelNum = MathHelper::Mod(++currentModelNum, static_cast<int>(DRAW_MODEL::MAX));
				drawModel = static_cast<DRAW_MODEL>(currentModelNum);
			}

			// モデルY軸回転
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_Z))
				angle += 0.5f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_X))
				angle -= 0.5f;

			// デバッグ表記切り替え
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_SPACE))
				debugDraw = !debugDraw;

			// �����̈ʒu��ݒ�
			//if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_A))
			//	light += 0.01f;
			//else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_D))
			//	light -= 0.01f;
		}

		/* ポストエフェクトパラメータ変更 */
		{
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_Y))
				brightPassThreshold += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_G))
				brightPassThreshold -= 0.01f;
			brightPassThreshold = MathHelper::Clamp(brightPassThreshold, 0.0f, 1.0f);

			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_U))
				bloomIntensity += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_H))
				bloomIntensity -= 0.01f;
			bloomIntensity = MathHelper::Clamp(bloomIntensity, 0.0f, 1.0f);

			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_I))
				bloomSaturation += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_J))
				bloomSaturation -= 0.01f;
			bloomSaturation = MathHelper::Clamp(bloomSaturation, 0.0f, 1.0f);

			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_O))
				baseIntensity += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_K))
				baseIntensity -= 0.01f;
			baseIntensity = MathHelper::Clamp(baseIntensity, 0.0f, 1.0f);

			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_P))
				baseSaturation += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_L))
				baseSaturation -= 0.01f;
			baseSaturation = MathHelper::Clamp(baseSaturation, 0.0f, 1.0f);

			// ポストエフェクトパラメータリセット
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

		camera.Update();

		float deglight = MathHelper::ToDegrees(light);
		Light::GetInstance()->SetPosition(Vector3(80.0f * MathHelper::Sin(deglight), 90.0f, -80.0f * MathHelper::Cos(deglight)));
		Light::GetInstance()->SetDirection(Vector3(1.0f, -1.0f, 0.0f).Normalize());

		auto worldMatrix = Matrix::CreateRotationY(angle);

		ShadowMapShader shadowMapShader(effectShadowMap);

		/* 影用描画 */
		shadowMap.Begin(effectShadowMap);

		stage->Draw(shadowMapShader, Matrix::Identity);
		switch (drawModel)
		{
		case DRAW_MODEL::Mirai_Akari: 
			mirai_akari->Draw(shadowMapShader, worldMatrix); break;
		case DRAW_MODEL::Siro:
			siro->Draw(shadowMapShader, worldMatrix);	break;
		case DRAW_MODEL::Kouhai_chan:
			kouhai_chan->Draw(shadowMapShader, worldMatrix); break;
		case DRAW_MODEL::Ichigo:
			ichigo->Draw(shadowMapShader, worldMatrix); break;
		case DRAW_MODEL::Soldier:
			soldier->Draw(shadowMapShader, worldMatrix); break;
		default: break;
		}

		shadowMap.End(effectShadowMap);
		
		/* 通常描画 */
		rTexDefault.Begin();
		rTexDefault.Clear();

		Light::GetInstance()->Draw();

		//Camera::GetInstance()->SetShader();
		//camera.Draw();

		// 影データ反映描画
		shadowMap.Set();
		MeshShader shadowShader(effectShadow);
		camera.Draw();
		stage->Draw(shadowShader, Matrix::Identity);
		shadowMap.Clear();


		// �L�����N�^�[���f��
		MeshShader meshShader(effectMesh);
		SkinnedMeshShader skinnedMeshShader(effectSkinnedMesh);

		skinnedMesh.Calculate(worldMatrix, timer);
		if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_9))
			timer += 0.1;
		if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_0))
			timer -= 0.1;
		//timer = MathHelper::Clamp(timer, 0.0f, anim.EndFrame());
		timer = MathHelper::Mod(timer + 1, anim.EndFrame());

		camera.Draw();
		switch (drawModel)
		{
		case DRAW_MODEL::Mirai_Akari:
			mirai_akari->Draw(meshShader, worldMatrix); break;
		case DRAW_MODEL::Siro:
			siro->Draw(meshShader, worldMatrix); break;
		case DRAW_MODEL::Kouhai_chan:
			kouhai_chan->Draw(meshShader, worldMatrix); break;
		case DRAW_MODEL::Ichigo:
			ichigo->Draw(meshShader, worldMatrix);	break;
		case DRAW_MODEL::Soldier:
			skinnedMesh.Draw(skinnedMeshShader); break;
		default:
			break;
		}

		//DirectX11::GetInstance()->SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_NONE);
		//effectMesh.Begin();
		//auto sprite = SpriteManager::GetInstance()->Get(SPRITE_ID::SKY_SPRITE);
		//sprite->Begin();
		//sphere.Draw();
		//effectMesh.End();

		shadowMap.Clear();
		rTexDefault.End();

		/* ポストエフェクト */
		// グレイスケール
		if (drawPattern == DRAW_PATTERN::Monochrome)
		{
			rTexGrayScale.Begin();
			rTexGrayScale.Clear();
			effectGrayScale.Begin();
			rTexGrayScale.Set(rTexDefault);
			rTexGrayScale.Draw();
			effectShadow.End();
			rTexGrayScale.End();
		}
		else
		{
			if (drawPattern != DRAW_PATTERN::Default)
			{
				// シェーダー用パラメータ
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

				// ブライトエフェクト
				rTexBright.Begin();
				rTexBright.Clear();
				effectBright.Begin();
				rTexBright.Set(rTexDefault);
				rTexBright.Draw();
				effectBright.End();
				rTexBright.End();

				// Hブラーエフェクト
				rTexBlurH.Begin();
				rTexBlurH.Clear();
				effectBlurH.Begin();
				rTexBlurH.Set(rTexBright);
				rTexBlurH.Draw();
				effectBlurH.End();
				rTexBlurH.End();

				// Vブラーエフェクト
				rTexBlurVRT.Begin();
				rTexBlurVRT.Clear();
				effectBlurV.Begin();
				rTexBlurVRT.Set(rTexBlurH);
				rTexBlurVRT.Draw();
				effectBlurV.End();
				rTexBlurVRT.End();

				// ブルームエフェクト
				rTexBloomCombine.Begin();
				rTexBloomCombine.Clear();
				effectBloomCombine.Begin();
				rTexBloomCombine.Set(rTexDefault);
				rTexBloomCombine.Set(rTexBlurVRT, 1);
				rTexBloomCombine.Draw();
				effectBloomCombine.End();
				rTexBloomCombine.End(1);
				rTexBloomCombine.End();

				if (drawPattern == DRAW_PATTERN::Uncharted2)
				{
					// アンチャーテッドエフェクト
					rTexUncharted2ToneMap.Begin();
					rTexUncharted2ToneMap.Clear();
					effectUncharted2ToneMap.Begin();
					rTexUncharted2ToneMap.Set(rTexBloomCombine);
					rTexUncharted2ToneMap.Draw();
					effectUncharted2ToneMap.End();
					rTexUncharted2ToneMap.End();
				}
			}
		}

		// 最終描画パターン分岐
		switch (drawPattern)
		{
		case DRAW_PATTERN::Default:	   SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, rTexDefault); break;
		case DRAW_PATTERN::Monochrome: SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, rTexGrayScale);	break;
		case DRAW_PATTERN::Bloom:
		{
			//SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, effectBloomCombineRT); break;
			SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, rTexBright.GetShaderResource(), rTexBright.GetWidth() / 2, rTexBright.GetHeight() / 2);
			SpriteManager::GetInstance()->DrawGraph(Vector2(0, m_Height / 2), rTexBloomCombine.GetShaderResource(), rTexBloomCombine.GetWidth() / 2, rTexBloomCombine.GetHeight() / 2);
			SpriteManager::GetInstance()->DrawGraph(Vector2(m_Width / 2, 0), rTexBlurH.GetShaderResource(), rTexBlurH.GetWidth() / 2, rTexBlurH.GetHeight() / 2); 
			SpriteManager::GetInstance()->DrawGraph(Vector2(m_Width / 2, m_Height / 2), rTexBlurVRT.GetShaderResource(), rTexBlurVRT.GetWidth() / 2, rTexBlurVRT.GetHeight() / 2);
			break;
		}
		case DRAW_PATTERN::Uncharted2: SpriteManager::GetInstance()->DrawGraph(Vector2::Zero, rTexUncharted2ToneMap);	break;
		default: break;
		}

		// FPS描画
		//FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 10.0f), MyUtil::toString("FPS : %.1f", sleep.fps));

		// デバッグ表示
		if (debugDraw)
		{
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f,  30.0f), MyUtil::toString("���C�g��]     : A/D"));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f,  50.0f), MyUtil::toString("���f����]     : Z/X"));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f,  70.0f), MyUtil::toString("���f���؂�ւ�   : C"));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f,  90.0f), MyUtil::toString("�J�����ړ�     : "));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 110.0f), MyUtil::toString("�V�F�[�_�[�؂�ւ� : 1/2/3/4"));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 130.0f), MyUtil::toString("�e�𑜓x�؂�ւ� �@: Q/E"));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 200.0f), MyUtil::toString("�P�x��臒l   : Y/G : %.2f", brightPassThreshold));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 220.0f), MyUtil::toString("���摜�̍ʓx  : P/L : %.2f", baseSaturation));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 240.0f), MyUtil::toString("���摜�̋P�x  : O/K : %.2f", baseIntensity));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 260.0f), MyUtil::toString("�u���[���̍ʓx : I/J : %.2f", bloomSaturation));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 280.0f), MyUtil::toString("�u���[���̋P�x : U/H : %.2f", bloomIntensity));
			FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 300.0f), MyUtil::toString("�p�����[�^�̏����� : R"));
		}
		else
		{
			//FontManager::GetInstance()->DebugDraw(Vector2(10.0f, 30.0f), MyUtil::toString("DebugText : SPACE"));
		}
		
		//SpriteManager::GetInstance()->DrawGraph(Vector2(100, 100), FontManager::GetInstance()->GetDebugFont().GetRenderTexture());

		//shadowMap.DebugDraw();
		//SpriteManager::GetInstance()->Draw(SPRITE_ID::TEST_SPRITE, Vector2(100, 100));

		// 描画
		DirectX11::GetInstance()->GetSwapChain()->Present(1, 0);

		// FPS待機
		//sleep.Wait();
	}

	ModelManager::GetInstance()->Clear();
	SpriteManager::GetInstance()->Clear();

	return true;
}

void GameBase::LoadResources()
{
	// 頂点シェーダー読み込み
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::MESH_SHADER, std::make_shared<MeshVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::SHADOW_MESH_SHADER, std::make_shared<ShadowMeshVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::SKINNED_MESH_SHADER, std::make_shared<SkinnedMeshVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::SHADOW_SHADER, std::make_shared<ShadowMapVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::TEXTURE_SHADER, std::make_shared<TextureVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::FONT_SHADER, std::make_shared<FontVertexShader>());

	// ピクセルシェーダー読み込み
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::MESH_SHADER, std::make_shared<MeshPixelShader>());
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::SHADOW_MESH_SHADER, std::make_shared<ShadowMeshPixelShader>());
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::TEXTURE_SHADER, std::make_shared<TexturePixelShader>());
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::GRAYSCALE_SHADER, std::make_shared<PostEffectPixelShader>("GrayScale.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::BRIGHTPASS_SHADER, std::make_shared<PostEffectPixelShader>("BrightPass.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::GAUSSIANBLUR_H_SHADER, std::make_shared<PostEffectPixelShader>("GaussianBlurH.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::GAUSSIANBLUR_V_SHADER, std::make_shared<PostEffectPixelShader>("GaussianBlurV.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::BLOOM_SHADER, std::make_shared<PostEffectPixelShader>("BloomCombine.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::UNCHARTED2_TOONMAP_SHADER, std::make_shared<PostEffectPixelShader>("Uncharted2Tonemap.hlsl"));

	// モデル読み込み
	ModelManager::GetInstance()->Load("MiraiAkari/MiraiAkari_v1.0.pmx", MODEL_ID::MIRAI_AKARI_MODEL);
	ModelManager::GetInstance()->Load("Siro/siro_dance_costume_v1.0.pmx", MODEL_ID::SIRO_MODEL);
	ModelManager::GetInstance()->Load("Kouhai_chan/Kouhai_chan.pmd", MODEL_ID::KOUHAI_CHAN_MODEL);
	ModelManager::GetInstance()->Load("Ichigo/Ichigo.pmx", MODEL_ID::ICHIGO_MODEL);
	//ModelManager::GetInstance()->Load("Alice/alice.pmd", MODEL_ID::ALICE_MODEL);
	ModelManager::GetInstance()->Load("Soldier/Soldier.pmx", MODEL_ID::SOLDIER_MODEL);
	ModelManager::GetInstance()->Load("Stage/Stage.pmx", MODEL_ID::STAGE_MODEL);

	// テクスチャー読み込み
	SpriteManager::GetInstance()->Initialize(m_Width, m_Height);
	SpriteManager::GetInstance()->Load("numakuro.png", SPRITE_ID::TEST_SPRITE);
	SpriteManager::GetInstance()->Load("yuingo.jpg", SPRITE_ID::TEST_SPRITE);
	SpriteManager::GetInstance()->Load("sky.png", SPRITE_ID::SKY_SPRITE);
}
