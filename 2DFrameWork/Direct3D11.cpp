#include "Framework.h"
using Microsoft::WRL::ComPtr;

void Direct3D11::Create()
{
	//EnumerateAdapters
	{
		//IDXGIFactory1 DXGI ��ü�� �����ϱ� ���� �޼ҵ带 ����
		ComPtr<IDXGIFactory1> factory;
		FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factory.GetAddressOf())));

		UINT index = 0;
		while (true)
		{
			//��͵��� ��Ƶ� ������
			ComPtr<IDXGIAdapter1> adapter;
			//IDXGIFactory1 ��ü�� ���� ��͸� ����
			HRESULT hr = factory->EnumAdapters1(index, adapter.GetAddressOf());

			//���� index�� �ý��ۿ� �����ϴ� ����� ������ ���ų� �� ũ�ٸ� ����
			if (hr == DXGI_ERROR_NOT_FOUND)
				break;

			Check(hr);

			//���� �־�� ��͵��� ������ ����ü�� ���� �ִ� ����
			shared_ptr<D3DEnumAdapterInfo> adapterInfo = make_shared< D3DEnumAdapterInfo>();
			ZeroMemory(adapterInfo.get(), sizeof(D3DEnumAdapterInfo));
			adapterInfo->adapterOrdinal = index;
			adapter->GetDesc1(&adapterInfo->adapterDesc);
			adapterInfo->adapter = adapter;

			ComPtr<IDXGIOutput> output = nullptr;

			hr = adapterInfo->adapter->EnumOutputs(0, output.GetAddressOf());
			if (DXGI_ERROR_NOT_FOUND == hr)
				break;

			shared_ptr<D3DEnumOutputInfo> outputInfo = make_shared<D3DEnumOutputInfo>();
			ZeroMemory(outputInfo.get(), sizeof(D3DEnumOutputInfo));

			hr = output->GetDesc(&outputInfo->outputDesc);
			Check(hr);

			outputInfo->output = output;

			UINT num_modes = 0;
			//���ĸ� �����Ͽ� ä�δ� 8��Ʈ�� �����ϴ� 4����, 32��Ʈ ��ȣ ���� ����ȭ ���� ����.
			DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

			//��û�� ���� �� ��Ÿ �Է� �ɼǰ� ��ġ�ϴ� ǥ�� ��带 ������
			hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &num_modes, nullptr); // ��� ���� ��������
			//DXGI_ENUM_MODES_INTERLACED INTERLACE��� : ��ü �̹����� ���� �� �纻�� �� �� �ֵ��� ��Ʈ �� �̹����� ���ڵ��ϴ� ���
			Check(hr);

			//���÷��� ���
			shared_ptr<DXGI_MODE_DESC[]> displayModes{ new DXGI_MODE_DESC[num_modes] };

			hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &num_modes, displayModes.get()); // ���÷��� ��� ��������
			Check(hr);

			for (UINT i = 0; i < num_modes; i++)
			{
				bool isCheck = true;
				isCheck &= displayModes[i].Width == app.width;
				isCheck &= displayModes[i].Height == app.height;

				if (isCheck == true)
				{
					//										ȭ�� �ֻ���
					outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
					outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
				}
			}

			adapterInfo->outputInfo = outputInfo;

			adapterInfos.push_back(adapterInfo);

			index++;
		}
	}

	//CreateSwapChainAndDevice
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = 0;
		swapChainDesc.BufferDesc.Height = 0;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		if (app.vSync == true)
		{
			//�󵵼� �����?						����
			swapChainDesc.BufferDesc.RefreshRate.Numerator = adapterInfos[0]->outputInfo->numerator;
			//									�и�
			swapChainDesc.BufferDesc.RefreshRate.Denominator = adapterInfos[0]->outputInfo->denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = app.handle;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = !app.fullScreen;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
//#if defined(_DEBUG)
//		creationFlags = D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
//#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
		};

		UINT maxVideoMemory = 0;
		int selectedAdapterIndex = -1;
		for (UINT i = 0; i < adapterInfos.size(); ++i)
		{
			if (adapterInfos[i]->adapterDesc.DedicatedVideoMemory > maxVideoMemory)
			{
				selectedAdapterIndex = i;
				maxVideoMemory = static_cast<UINT>(adapterInfos[i]->adapterDesc.DedicatedVideoMemory);
			}
		}

		numerator = adapterInfos[0]->outputInfo->numerator;
		denominator = adapterInfos[0]->outputInfo->denominator;

		HRESULT hr = D3D11CreateDeviceAndSwapChain
		(
			adapterInfos[selectedAdapterIndex]->adapter.Get()
			, D3D_DRIVER_TYPE_UNKNOWN
			, nullptr
			, creationFlags
			, featureLevels
			, 1
			, D3D11_SDK_VERSION
			, &swapChainDesc
			, swapChain.GetAddressOf()
			, device.GetAddressOf()
			, nullptr
			, deviceContext.GetAddressOf()
		);
		assert(SUCCEEDED(hr));
	}

	CreateBackBuffer(app.width, app.height);

	//����������� ����
	//Disable DepthStencil
	{ 
		ComPtr<ID3D11DepthStencilState> depthStencilState;
		D3D11_DEPTH_STENCIL_DESC desc = { 0 };
		desc.DepthEnable = false;
		desc.StencilEnable = false;
		device->CreateDepthStencilState(&desc, depthStencilState.GetAddressOf());
		//���ε�
		deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0xFF);
	}
	//CullNone, FillSolid
	{
		ComPtr<ID3D11RasterizerState> rasterizerState;
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE; 
		desc.DepthBias = 0;
		desc.FrontCounterClockwise = false; 
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = false;
		desc.ScissorEnable = false; 
		desc.MultisampleEnable = false; 
		desc.AntialiasedLineEnable = false; 
		device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
		deviceContext->RSSetState(rasterizerState.Get());
	}
	//���İ� ó��
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		desc.RenderTarget[0].BlendEnable = true;
		//rgb
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //1 - 0.3 -> 0.7
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 0.3
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;// 0.7 * dest + 0.3 * src
		//a
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; // 1
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; // 0
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // 0 + 1
		device->CreateBlendState(&desc, blendState.GetAddressOf());
		deviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xFF);
	}
	isCreated = true;
}

Direct3D11::Direct3D11()
:numerator(0), denominator(1), isCreated(0), backBuffer(nullptr), device(nullptr)
, deviceContext(nullptr), renderTargetView(nullptr), swapChain(nullptr), blendState(nullptr)
{

}

Direct3D11::~Direct3D11()
{
	if (swapChain != nullptr)
		swapChain->SetFullscreenState(false, NULL);
}

void Direct3D11::SetRenderTarget(ComPtr<ID3D11RenderTargetView> rtv, ComPtr<ID3D11DepthStencilView> dsv) const
{
	if (rtv == nullptr)
		rtv = renderTargetView;

	//if (dsv == nullptr)
	//	dsv = depthStencilView;

	deviceContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
}

void Direct3D11::Clear(Color color, ComPtr<ID3D11RenderTargetView> rtv, ComPtr<ID3D11DepthStencilView> dsv) const
{
	if (rtv == nullptr)
		rtv = renderTargetView;

	//if (dsv == nullptr)
	//	dsv = depthStencilView;

	deviceContext->ClearRenderTargetView(rtv.Get(), color);
	//deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void Direct3D11::Present() const
{
	swapChain->Present(app.vSync == true ? 1 : 0, 0);
}

void Direct3D11::ResizeScreen(const float width, const float height)
{
	if (width < 1 || height < 1)
		return;

	app.width = width;
	app.height = height;

	DWRITE.DeleteBackBuffer();
	{
		const HRESULT hr = 
			swapChain->ResizeBuffers(
				0, 
				static_cast<UINT>(width), 
				static_cast<UINT>(height), 
				DXGI_FORMAT_UNKNOWN, 
				0);
		assert(SUCCEEDED(hr));
	}
	CreateBackBuffer(width, height);
	DWRITE.CreateBackBuffer(width, height);
}

void Direct3D11::CreateBackBuffer(float width, float height)
{
	//Create RTV - System BackBuffer
	{
		HRESULT hr;
		ComPtr<ID3D11Texture2D> backbufferPointer;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backbufferPointer.GetAddressOf()));
		Check(hr);

		hr = device->CreateRenderTargetView(backbufferPointer.Get(), nullptr, renderTargetView.GetAddressOf());
		Check(hr);
	}
}

D3DEnumAdapterInfo::D3DEnumAdapterInfo()
	: adapterOrdinal(0), adapter(nullptr), adapterDesc(DXGI_ADAPTER_DESC1()), outputInfo(nullptr)
{
}

D3DEnumOutputInfo::D3DEnumOutputInfo()
	: output(nullptr), outputDesc(DXGI_OUTPUT_DESC()), numerator(0), denominator(1)
{
}
