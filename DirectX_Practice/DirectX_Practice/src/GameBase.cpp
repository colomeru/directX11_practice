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

// 管理クラス
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

// 頂点シェーダー
#include "Shader/vertex/ShadowMeshVertexShader.h"
#include "Shader/vertex/ShadowMapVertexShader.h"
#include "Shader/vertex/TextureVertexShader.h"
#include "Shader/vertex/FontVertexShader.h"
// ピクセルシェーダ
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

	// ウィンドウの作成
	Window::GetInstance()->Create("DirectX11_Practice", m_Width, m_Height);

	// 初期化
	if (!DirectX11::GetInstance()->Initialize())
		return false;

	// DirectInput初期化
	LPDIRECTINPUT8 dInput;
	hr = DirectInput8Create(hIns, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL);
	if (!SUCCEEDED(hr))
		return false;

	// 入力関係初期化
	Keyboard::GetInstance()->Initialize(Window::GetInstance()->GetHandle(), dInput);

	// マルチサンプルの最大品質を取得
	hr = DirectX11::GetInstance()->GetDevice()->CheckMultisampleQualityLevels(m_SwapChainFormat, m_MultiSampleCount, &m_MultiSampleMaxQuality);
	if (FAILED(hr))
		return false;

	// レンダーターゲットの生成
	Texture backBuffer;
	backBuffer.Create();
	RenderTarget renderTarget;
	renderTarget.Create(&backBuffer);

	// 描画先を裏画面にする
	DirectX11::GetInstance()->SetRenderTarget(&renderTarget, nullptr);

	DXGI_FORMAT textureFormat  = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT resourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// テクスチャとシェーダーリソースビューのフォーマットを適切なものに変更
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
	textureDesc.Width		= m_Width;	// バックバッファーと同じサイズを指定
	textureDesc.Height		= m_Height;	// バックバッファーと同じサイズを指定
	textureDesc.Format		= textureFormat;
	textureDesc.Usage		= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags	= D3D11_BIND_DEPTH_STENCIL;
	textureDesc.MiscFlags	= 0;
	textureDesc.CPUAccessFlags		= 0;
	textureDesc.MipLevels			= 1;
	textureDesc.ArraySize			= 1;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.SampleDesc.Quality	= 0;

	// デプスステンシルの生成
	Texture depthTexture;
	depthTexture.Create(textureDesc);
	DepthStencil depthStencil;
	depthStencil.Create(&depthTexture, textureDesc.Format);

	/* レンダーテクスチャ生成 */
	// 通常用
	RenderTexture rtDefault(&depthStencil);
	rtDefault.Create(m_Width, m_Height);
	// グレイスケール用
	RenderTexture rtGrayScale(nullptr);
	rtGrayScale.Create(m_Width, m_Height);
	// 高輝度抽出用
	RenderTexture rtBright(nullptr);
	rtBright.Create(m_Width, m_Height);
	// 水平方向ブラーシェーダー
	RenderTexture rtBlurH(nullptr);
	rtBlurH.Create(m_Width, m_Height);
	// 垂直方向ブラーシェーダー
	RenderTexture rtBlurVRT(nullptr);
	rtBlurVRT.Create(m_Width, m_Height);
	// ブルーム効果シェーダー
	RenderTexture rtBloomCombine(nullptr);
	rtBloomCombine.Create(m_Width, m_Height);
	// アンチャーテッド２トゥーンマップシェーダー
	RenderTexture rtUncharted2ToneMap(nullptr);
	rtUncharted2ToneMap.Create(m_Width, m_Height);

	// リソースの読み込み
	LoadResources();

	// ミライアカリ
	auto mirai_akari = ModelManager::GetInstance()->Get(MODEL_ID::MIRAI_AKARI_MODEL);
	// 太ももにアルファブレンドが必要なため
	mirai_akari->SetAlphaBlend(true, true);
	// シロ
	auto siro = ModelManager::GetInstance()->Get(MODEL_ID::SIRO_MODEL);
	// はごろもフーズ
	siro->SetAlphaBlend(true, false);
	// 後輩ちゃん
	auto kouhai_chan = ModelManager::GetInstance()->Get(MODEL_ID::KOUHAI_CHAN_MODEL);
	kouhai_chan->SetAlphaBlend(true, false);
	// いちご
	auto ichigo = ModelManager::GetInstance()->Get(MODEL_ID::ICHIGO_MODEL);
	// 額
	ichigo->SetAlphaBlend(true, false);
	// ステージ
	auto stage = ModelManager::GetInstance()->Get(MODEL_ID::STAGE_MODEL);

	// 定数バッファの設定
	CComPtr<ID3D11Buffer> pBloomBuffer;
	DirectX11::GetInstance()->CreateBuffer(&pBloomBuffer, nullptr, sizeof(BloomCB), D3D11_BIND_CONSTANT_BUFFER);

	// シャドウマップ用のテクスチャの設定
	int  index  = 11;
	UINT smSize = MathHelper::Pow(2, index);

	// シャドウマップ生成
	ShadowMap shadowMap;
	if (!shadowMap.Create(smSize))
		return false;

	// 投影データの設定
	Camera::GetInstance()->SetFovAngle(60.0f);
	Camera::GetInstance()->SetAspect(m_AspectRatio);
	Camera::GetInstance()->SetNearFar(1.0f, 1000.0f);
	Camera::GetInstance()->SetUp(Vector3::Up);

	// FPS制御準備
	MySleep sleep;
	sleep.QuerySet();

	float angle	 = 0.0f;
	float light  = 0.0f;
	float cangle = 0.0f;
	float ey	 = 0.0f;
	float cl	 = 50.0f;

	// 描画パターン
	DRAW_PATTERN drawPattern = DRAW_PATTERN::Default;
	DRAW_MODEL	 drawModel	 = DRAW_MODEL::Mirai_Akari;
	bool		 debugDraw   = false;

	float brightPassThreshold = 0.7f;
	float bloomIntensity	= 1.0f;
	float bloomSaturation	= 1.0f;
	float baseIntensity		= 1.0f;
	float baseSaturation	= 1.0f;

	// クリアカラー
	static float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	// シェーダー生成
	Effect effectDefault(VERTEX_SHADER_ID::SHADOW_MESH_SHADER, PIXEL_SHADER_ID::SHADOW_MESH_SHADER);
	Effect effectShadowMap(VERTEX_SHADER_ID::SHADOW_SHADER);
	Effect effectGrayScale(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::GRAYSCALE_SHADER);
	Effect effectBright(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::BRIGHTPASS_SHADER);
	Effect effectBlurH(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::GAUSSIANBLUR_H_SHADER);
	Effect effectBlurV(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::GAUSSIANBLUR_V_SHADER);
	Effect effectBloomCombine(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::BLOOM_SHADER);
	Effect effectUncharted2ToneMap(VERTEX_SHADER_ID::TEXTURE_SHADER, PIXEL_SHADER_ID::UNCHARTED2_TOONMAP_SHADER);

	// ゲームループ
	while (Window::GetInstance()->MessageHandling())
	{
		// 入力関係更新
		Keyboard::GetInstance()->Update();

		// 強制終了
		if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_ESC))
			break;

		/*** ゲームの処理 ***/
		DirectX11::GetInstance()->ClearRenderTarget(&renderTarget, color);

		/* キーボードの入力 */
		{
			// 描画情報の切り替え
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_1))
				drawPattern = DRAW_PATTERN::Default;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_2))
				drawPattern = DRAW_PATTERN::Bloom;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_3))
				drawPattern = DRAW_PATTERN::Uncharted2;
			else if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_4))
				drawPattern = DRAW_PATTERN::Monochrome;

			// シャドウマップの解像度の変更
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

			// 描画モデルの切り替え
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_C))
			{
				int current = static_cast<int>(drawModel);
				current++;
				current   = MathHelper::Mod(current, static_cast<int>(DRAW_MODEL::MAX));
				drawModel = static_cast<DRAW_MODEL>(current);
			}
			// モデル回転
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_Z))
				angle += 0.5f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_X))
				angle -= 0.5f;

			// パラメータ描画をするか？
			if (Keyboard::GetInstance()->IsKeyDown(KEY_CODE::KEY_SPACE))
				debugDraw = !debugDraw;

			// 光源の位置を設定
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_A))
				light += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_D))
				light -= 0.01f;

			// カメラの位置を設定
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_UP))
				cl -= 1.0f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_DOWN))
				cl += 1.0f;
			cl = MathHelper::Clamp(cl, 10.0f, 150.0f);
			// カメラ回転
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_LEFT))
				cangle += 0.2f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_RIGHT))
				cangle -= 0.2f;
		}

		/* ポストエフェクトパラメータの操作 */
		{
			// 輝度の閾値
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_Y))
				brightPassThreshold += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_G))
				brightPassThreshold -= 0.01f;
			brightPassThreshold = MathHelper::Clamp(brightPassThreshold, 0.0f, 1.0f);

			// 元画像のテクスチャの彩度
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_U))
				bloomIntensity += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_H))
				bloomIntensity -= 0.01f;
			bloomIntensity = MathHelper::Clamp(bloomIntensity, 0.0f, 1.0f);

			// 元画像のテクスチャの輝度
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_I))
				bloomSaturation += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_J))
				bloomSaturation -= 0.01f;
			bloomSaturation = MathHelper::Clamp(bloomSaturation, 0.0f, 1.0f);

			// ブルームテクスチャの彩度
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_O))
				baseIntensity += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_K))
				baseIntensity -= 0.01f;
			baseIntensity = MathHelper::Clamp(baseIntensity, 0.0f, 1.0f);

			// ブルームテクスチャの輝度
			if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_P))
				baseSaturation += 0.01f;
			else if (Keyboard::GetInstance()->IsKey(KEY_CODE::KEY_L))
				baseSaturation -= 0.01f;
			baseSaturation = MathHelper::Clamp(baseSaturation, 0.0f, 1.0f);

			// ポストエフェクトのパラメータを初期化
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

		// カメラ情報更新
		Camera::GetInstance()->SetPosition(eyePosMat.Translation());
		Camera::GetInstance()->SetTarget(focus);

		float deglight = MathHelper::ToDegrees(light);
		Light::GetInstance()->SetPosition(Vector3(80.0f * MathHelper::Sin(deglight), 90.0f, -80.0f * MathHelper::Cos(deglight)));

		auto worldMatrix = Matrix::CreateRotationY(angle);

		ShadowMapShader shadowMapShader(effectShadowMap);

		/* 描画処理 */
		// シャドウマップ処理
		shadowMap.Begin(effectShadowMap);

		DirectX11::GetInstance()->SetRasterizer(D3D11_FILL_SOLID, D3D11_CULL_FRONT);

		// モデルの描画
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
		
		// 通常描画
		rtDefault.Begin();
		rtDefault.Clear();

		// 光源情報反映
		Light::GetInstance()->Draw();
		// カメラ情報反映
		Camera::GetInstance()->Draw();

		// 影データセット
		shadowMap.Set();

		/* モデルの描画 */
		MeshShader shader(effectDefault);
		// ステージモデル
		stage->Draw(shader, Matrix::Identity);
		// キャラクターモデル
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

		/* ポストエフェクト開始 */
		// グレイスケールで描画
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
				// ピクセルシェーダにパラメータのセット
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

				// 高輝度抽出
				rtBright.Begin();
				rtBright.Clear();
				effectBright.Begin();
				rtBright.Set(rtDefault);
				rtBright.Draw();
				effectBright.End();
				rtBright.End();

				// 水平方向ブラー
				rtBlurH.Begin();
				rtBlurH.Clear();
				effectBlurH.Begin();
				rtBlurH.Set(rtBright);
				rtBlurH.Draw();
				effectBlurH.End();
				rtBlurH.End();

				// 垂直方向ブラー
				rtBlurVRT.Begin();
				rtBlurVRT.Clear();
				effectBlurV.Begin();
				rtBlurVRT.Set(rtBlurH);
				rtBlurVRT.Draw();
				effectBlurV.End();
				rtBlurVRT.End();

				// 元画像とブラー画像を合成
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
					// アンチャーテッド２トゥーンマップ
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

		/* 最終描画 */
		// 描画情報セット
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

		// FPS描画
		DrawFont::GetInstance()->Draw(Vector2(10.0f, 10.0f), DrawFont::GetInstance()->format("FPS : %.1f", sleep.fps));

		// パラメータ描画
		if (debugDraw)
		{
			DrawFont::GetInstance()->Draw(Vector2(10.0f,  30.0f), DrawFont::GetInstance()->format("ライト回転     : A/D"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f,  50.0f), DrawFont::GetInstance()->format("モデル回転     : Z/X"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f,  70.0f), DrawFont::GetInstance()->format("モデル切り替え   : C"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f,  90.0f), DrawFont::GetInstance()->format("カメラ移動     : 十字キー"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 110.0f), DrawFont::GetInstance()->format("シェーダー切り替え : 1/2/3/4"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 130.0f), DrawFont::GetInstance()->format("影解像度切り替え 　: Q/E"));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 200.0f), DrawFont::GetInstance()->format("輝度の閾値   : Y/G : %.2f", brightPassThreshold));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 220.0f), DrawFont::GetInstance()->format("元画像の彩度  : P/L : %.2f", baseSaturation));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 240.0f), DrawFont::GetInstance()->format("元画像の輝度  : O/K : %.2f", baseIntensity));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 260.0f), DrawFont::GetInstance()->format("ブルームの彩度 : I/J : %.2f", bloomSaturation));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 280.0f), DrawFont::GetInstance()->format("ブルームの輝度 : U/H : %.2f", bloomIntensity));
			DrawFont::GetInstance()->Draw(Vector2(10.0f, 300.0f), DrawFont::GetInstance()->format("パラメータの初期化 : R"));
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

		// FPS制御
		sleep.Wait();

		// 画面の更新
		DirectX11::GetInstance()->GetSwapChain()->Present(0, 0);
	}

	ModelManager::GetInstance()->Clear();
	SpriteManager::GetInstance()->Clear();

	return true;
}

void GameBase::LoadResources()
{
	// リソース関係初期化

	/* シェーダーの読み込み */
	// 頂点シェーダの読み込み
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::SHADOW_MESH_SHADER, std::make_shared<ShadowMeshVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::SHADOW_SHADER, std::make_shared<ShadowMapVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::TEXTURE_SHADER, std::make_shared<TextureVertexShader>());
	ShaderManager::GetInstance()->Add(VERTEX_SHADER_ID::FONT_SHADER, std::make_shared<FontVertexShader>());

	// ピクセルシェーダの読み込み
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::SHADOW_MESH_SHADER, std::make_shared<ShadowMeshPixelShader>());
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::TEXTURE_SHADER, std::make_shared<TexturePixelShader>());
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::GRAYSCALE_SHADER, std::make_shared<PostEffectPixelShader>("GrayScale.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::BRIGHTPASS_SHADER, std::make_shared<PostEffectPixelShader>("BrightPass.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::GAUSSIANBLUR_H_SHADER, std::make_shared<PostEffectPixelShader>("GaussianBlurH.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::GAUSSIANBLUR_V_SHADER, std::make_shared<PostEffectPixelShader>("GaussianBlurV.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::BLOOM_SHADER, std::make_shared<PostEffectPixelShader>("BloomCombine.hlsl"));
	ShaderManager::GetInstance()->Add(PIXEL_SHADER_ID::UNCHARTED2_TOONMAP_SHADER, std::make_shared<PostEffectPixelShader>("Uncharted2Tonemap.hlsl"));

	// モデルの読み込み
	ModelManager::GetInstance()->Load("MiraiAkari/MiraiAkari_v1.0.pmx", MODEL_ID::MIRAI_AKARI_MODEL);
	ModelManager::GetInstance()->Load("Siro/siro_dance_costume_v1.0.pmx", MODEL_ID::SIRO_MODEL);
	ModelManager::GetInstance()->Load("Kouhai_chan/Kouhai_chan.pmd", MODEL_ID::KOUHAI_CHAN_MODEL);
	ModelManager::GetInstance()->Load("Ichigo/Ichigo.pmx", MODEL_ID::ICHIGO_MODEL);
	ModelManager::GetInstance()->Load("Stage/Stage.pmx", MODEL_ID::STAGE_MODEL);

	// スプライトの読み込み
	SpriteManager::GetInstance()->Initialize(m_Width, m_Height);
	SpriteManager::GetInstance()->Load("numakuro.png", SPRITE_ID::TEST_SPRITE);
	//SpriteManager::GetInstance()->Load("yuingo.jpg", SPRITE_ID::TEST_SPRITE);
}
