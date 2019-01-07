#include "DirectX11.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "../Window.h"
#include "../Utility.h"

DirectX11::DirectX11() :
	m_DriverType(D3D_DRIVER_TYPE_NULL),
	m_FeatureLevel(D3D_FEATURE_LEVEL_11_0),
	m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_pAdapter(nullptr)
{
}

DirectX11::~DirectX11()
{
}

bool DirectX11::Initialize()
{
	// �A�_�v�^�̍쐬
	if (!CreateAdapter())
		return false;

	// �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	if (!CreateDeviceAndSwapChain())
		return false;

	// �E�B���h�E�̎����؂�ւ��𖳌���
	CreateViewPort();

	return true;
}

void DirectX11::SetRenderTarget(const RenderTarget * renderTarget, DepthStencil * depthStencil)
{
	auto target = renderTarget->Get();

	if (depthStencil)
		m_pDeviceContext->OMSetRenderTargets(1, &target, depthStencil->Get());
	else
		m_pDeviceContext->OMSetRenderTargets(1, &target, nullptr);
}

void DirectX11::ClearRenderTarget(const RenderTarget * renderTarget, float color[])
{
	m_pDeviceContext->ClearRenderTargetView(renderTarget->Get(), color);
}

void DirectX11::ClearDepthStencil(DepthStencil * depthStencil, float depth, float stencil)
{
	m_pDeviceContext->ClearDepthStencilView(depthStencil->Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

ID3D11Device * DirectX11::GetDevice() const
{
	return m_pDevice;
}

ID3D11DeviceContext * DirectX11::GetContext() const
{
	return m_pDeviceContext;
}

IDXGISwapChain * DirectX11::GetSwapChain() const
{
	return m_pSwapChain;
}

bool DirectX11::CreateBuffer(ID3D11Buffer ** ppBuffer, void * resources, size_t size, D3D11_BIND_FLAG bindFlag)
{
	HRESULT					hr;
	D3D11_BUFFER_DESC		bufferDesc;
	D3D11_SUBRESOURCE_DATA* subResoreData = nullptr;

	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = bindFlag;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// �T�u���\�[�X
	if (resources)
	{
		subResoreData = new D3D11_SUBRESOURCE_DATA();
		subResoreData->pSysMem = resources;
		subResoreData->SysMemPitch = 0;
		subResoreData->SysMemSlicePitch = 0;
	}

	hr = m_pDevice->CreateBuffer(&bufferDesc, subResoreData, ppBuffer);

	delete subResoreData;

	return SUCCEEDED(hr);
}

bool DirectX11::SetRasterizer(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC hRasterizerDesc = {};
	CComPtr<ID3D11RasterizerState>	pRasterizerState;

	hRasterizerDesc.FillMode = fillMode;
	hRasterizerDesc.CullMode = cullMode;
	hRasterizerDesc.FrontCounterClockwise = TRUE;
	hRasterizerDesc.DepthClipEnable = TRUE;
	hr = DirectX11::GetInstance()->GetDevice()->CreateRasterizerState(&hRasterizerDesc, &pRasterizerState);

	if (FAILED(hr))
		return false;;

	m_pDeviceContext->RSSetState(pRasterizerState);

	return true;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetNoBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= FALSE;
	RenderTarget.SrcBlend				= D3D11_BLEND_ONE;
	RenderTarget.DestBlend				= D3D11_BLEND_ZERO;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetAlignmentBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= TRUE;
	RenderTarget.SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	RenderTarget.DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetAddBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= TRUE;
	RenderTarget.SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	RenderTarget.DestBlend				= D3D11_BLEND_ONE;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetSubtractBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= TRUE;
	RenderTarget.SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	RenderTarget.DestBlend				= D3D11_BLEND_ONE;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_REV_SUBTRACT;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

D3D11_RENDER_TARGET_BLEND_DESC DirectX11::GetMultipleBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable			= TRUE;
	RenderTarget.SrcBlend				= D3D11_BLEND_ZERO;
	RenderTarget.DestBlend				= D3D11_BLEND_SRC_COLOR;
	RenderTarget.BlendOp				= D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha			= D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha			= D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

bool DirectX11::SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC BlendStateArray[], UINT NumBlendState, bool AlphaToCoverageEnable)
{
	HRESULT hr = E_FAIL;

	CComPtr<ID3D11BlendState> pBlendState;

	if (NumBlendState > 8)
		return false;

	D3D11_BLEND_DESC BlendDesc;
	::ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;
	// TRUE�̏ꍇ�A�}���`�����_�[�^�[�Q�b�g�Ŋe�����_�[�^�[�Q�b�g�̃u�����h�X�e�[�g�̐ݒ���ʂɐݒ�ł���
	// FALSE�̏ꍇ�A0�Ԗڂ݂̂��g�p�����
	BlendDesc.IndependentBlendEnable = TRUE;

	for (UINT i = 0; i<NumBlendState; i++)
	{
		::CopyMemory(&BlendDesc.RenderTarget[i], &BlendStateArray[i], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	}

	hr = m_pDevice->CreateBlendState(&BlendDesc, &pBlendState);
	if (FAILED(hr))
		return false;

	m_pDeviceContext->OMSetBlendState(pBlendState, 0, 0xffffffff);

	return true;
}

void DirectX11::SetAlphaBlend(bool alphaEnable, bool alphaToCoverageEnable)
{
	D3D11_RENDER_TARGET_BLEND_DESC BlendDesc;
	if (alphaEnable)
		BlendDesc = GetAlignmentBlendDesc();
	else
		BlendDesc = GetNoBlendDesc();

	if (alphaToCoverageEnable)BlendDesc = GetNoBlendDesc();
	SetBlendState(&BlendDesc, 1, alphaToCoverageEnable);
}

bool DirectX11::CreateAdapter()
{
	IDXGIFactory* factory = nullptr;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&factory))))
		return false;

	// 0�Ԗڂ̃A�_�v�^���擾
	if (FAILED(factory->EnumAdapters(0, &m_pAdapter)))
		return false;

	return true;
}

bool DirectX11::CreateDeviceAndSwapChain()
{
	// �h���C�o�[�^�C�v
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	// �r�f�I�J�[�h�̐��\��񋓂��ăf�o�C�X���쎞�ɏォ��̏��Ɍ������Ďg�p�\�Ȃ��̂��I�������
	const int FEATURE_LEVEL_NUM = 6;
	D3D_FEATURE_LEVEL featureLevels[FEATURE_LEVEL_NUM] =
	{
		D3D_FEATURE_LEVEL_11_0,  // Direct3D 11.0 SM 5
		D3D_FEATURE_LEVEL_10_1,  // Direct3D 10.1 SM 4
		D3D_FEATURE_LEVEL_10_0,  // Direct3D 10.0 SM 4
		D3D_FEATURE_LEVEL_9_3,   // Direct3D 9.3  SM 3
		D3D_FEATURE_LEVEL_9_2,   // Direct3D 9.2  SM 2
		D3D_FEATURE_LEVEL_9_1,   // Direct3D 9.1  SM 2
	};

	unsigned int createDeviceFlags = 0;

#ifndef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !_DEBUG

	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);

	// �E�B���h�E�̉�ʃT�C�Y���擾
	RECT rect;
	GetClientRect(Window::GetInstance()->GetHandle(), &rect);
	UINT width  = rect.right  - rect.left;
	UINT height = rect.bottom - rect.top;

	// �X���b�v�`�F�C���̐ݒ�
	DXGI_SWAP_CHAIN_DESC scDesc;
	ZeroMemory(&scDesc, sizeof(scDesc));								// �\���̂��O�N���A
	scDesc.BufferCount = 1;												// �X���b�v�`�F�[���̃o�b�t�@��
	scDesc.BufferDesc.Width		= width;								// �X���b�v�`�F�[���̃o�b�t�@�T�C�Y
	scDesc.BufferDesc.Height	= height;								// �X���b�v�`�F�[���̃o�b�t�@�T�C�Y
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// �X���b�v�`�F�[���̃o�b�t�@�t�H�[�}�b�g
	scDesc.BufferDesc.RefreshRate.Numerator	  = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;						// 1 / 60 = 60fps
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;	// �o�b�t�@�������_�[�^�[�Q�b�g�Ƃ��Ďg�p
	scDesc.OutputWindow			= Window::GetInstance()->GetHandle();	// HWND�n���h��
	scDesc.SampleDesc.Count		= 1;									// �}���`�T���v�����O�̃s�N�Z���P��
	scDesc.SampleDesc.Quality	= 0;									// �}���`�T���v�����O�̕i��
	scDesc.Windowed				= TRUE;									// �E�C���h�E���[�h

	HRESULT hr  = S_OK;
	bool result = false;

	for (UINT idx = 0; idx < numDriverTypes; ++idx)
	{
		// �h���C�o�[�^�C�v�ݒ�
		m_DriverType = driverTypes[idx];

		// �f�o�C�X�ƃX���b�v�`�F�C���̐���
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,				// �A�_�v�^�|�̃|�C���^�BNULL���ƋK��̃A�_�v�^
			m_DriverType,		// �n�[�h�E�F�A���g��D3D_DRIVER_TYPE_HARDWARE���w��
			NULL,				// DeviceType���n�[�h�E�F�A�̏ꍇNULL
			createDeviceFlags,	// �L���ɂ��郉���^�C�����C���[
			featureLevels,		// �쐬�����݂�@�\���x���̏������w�肷��z��
			numFeatureLevels,	// �쐬�����݂�@�\���x���̏������w�肷��z��̐�
			D3D11_SDK_VERSION,	// D3D11_SDK_VERSION�̐ݒ�
			&scDesc,			// �X���b�v�`�F�[���̏������p�����[�^�[
			&m_pSwapChain,		// �쐬�����X���b�v�`�F�[��
			&m_pDevice,			// �쐬�����f�o�C�X
			&m_FeatureLevel,	// �쐬�����f�o�C�X�̋@�\���x��
			&m_pDeviceContext	// �쐬�����f�o�C�X�R���e�L�X�g
			);

		// ���������烋�[�v�E�o
		if (SUCCEEDED(hr))
		{
			result = true;
			break;
		}
	}

	return result;
}

bool DirectX11::SetWindowAssociation()
{
	IDXGIFactory* factory = nullptr;

	if (FAILED(m_pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)(&factory))))
		return false;

	// �f�o�C�X�쐬�̂Ƃ���m_pAdapter��ݒ肵�Ă��Ȃ��ꍇ���̏������s���Ă�Alt + Enter�Ńt���X�N���[���؂�ւ��������ɂȂ�Ȃ�
	if (FAILED(factory->MakeWindowAssociation(Window::GetInstance()->GetHandle(), DXGI_MWA_NO_WINDOW_CHANGES)))
		return false;

	return true;
}

void DirectX11::CreateViewPort()
{
	DXGI_SWAP_CHAIN_DESC scDesc;

	m_pSwapChain->GetDesc(&scDesc);

	D3D11_VIEWPORT vp;
	vp.Width  = (FLOAT)scDesc.BufferDesc.Width;
	vp.Height = (FLOAT)scDesc.BufferDesc.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pDeviceContext->RSSetViewports(1, &vp);
}
