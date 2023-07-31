#pragma once

struct D3DEnumOutputInfo
{
	Microsoft::WRL::ComPtr<IDXGIOutput>		output; //����� ���
	DXGI_OUTPUT_DESC						outputDesc;
	UINT									numerator;
	UINT									denominator;
	D3DEnumOutputInfo();
	~D3DEnumOutputInfo() = default;;
};

struct D3DEnumAdapterInfo
{
	UINT									adapterOrdinal;
	Microsoft::WRL::ComPtr<IDXGIAdapter1>	adapter;
	//��� ������ ����ü
	DXGI_ADAPTER_DESC1						adapterDesc;
	std::shared_ptr<D3DEnumOutputInfo>		outputInfo;
	D3DEnumAdapterInfo();
	~D3DEnumAdapterInfo() = default;;
};

class Direct3D11
{
private:
	Direct3D11();

public:
	static Direct3D11& GetInstance()
	{
		static std::unique_ptr<Direct3D11> inst;
		if (!inst)
			inst = std::unique_ptr<Direct3D11>(new Direct3D11());
		return *inst;
	}

public:
	~Direct3D11();

private:
	void CreateBackBuffer(float width, float height);

public:
	void Create();
	void SetRenderTarget(
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr,
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = nullptr) const;
	//
	void Clear(Color color = 
		Color(0.7f,0.7f,0.7f,1.0f), 
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr, 
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = nullptr) const;
	//
	void Present() const;

	void ResizeScreen(float width, float height);
	
public:
	//Getter
	Microsoft::WRL::ComPtr<ID3D11Device>			GetDevice()		{ return device; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		GetDC()			{ return deviceContext; }
	Microsoft::WRL::ComPtr<IDXGISwapChain>			GetSwapChain()	{ return swapChain; }
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	GetRTV()		{ return renderTargetView; }
	bool	GetCreated() const { return isCreated; }
	//ID3D11DepthStencilView* GetDSV() { return depthStencilView; }

private:
	Microsoft::WRL::ComPtr<ID3D11Device>			device; //���� ����
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		deviceContext; //������ ���������� ���ε�
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapChain;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			backBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	//ID3D11DepthStencilView*	depthStencilView;

	std::vector<std::shared_ptr<D3DEnumAdapterInfo>>	adapterInfos{};
	UINT	numerator;
	UINT	denominator;
	bool	isCreated;
};
