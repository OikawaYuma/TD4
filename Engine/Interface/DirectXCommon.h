/**
* @ file DirectXCommon.h
* @ brief DirectXの基本設定　Singleton Patternで作成
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include "function.h"
#include <string>
#include <format>
#include <wrl.h>
#include <chrono>

#include "WinAPI.h"
#include <dxcapi.h>
#include "ResourceObject.h"
#include "Vector4.h"
#include "TextureManager.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
/*----------------------------------------------------------
   このクラスはシングルトンパターンのを元に設計する
--------------------------------------------------------------*/
class DirectXCommon final {
public:
	static DirectXCommon* GetInstance();

	DirectXCommon() = default;
	~DirectXCommon() = default;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理、フレーム開始
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void ViewChange();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();


	/// <summary>
	/// deviceの生成
	/// </summary>
	void CreateDevice();

	/// <summary>
	/// command関連の初期化
	/// </summary>
	void CreateCommand();

	/// <summary>
	/// SeapChain生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// DepthBufferの生成
	/// </summary>
	void CreateDepth();

	/// <summary>
	/// 各種Descriptorの生成
	/// </summary>
	void CreateDescriptorHeap();

	/// <summary>
	/// RTVの初期化
	/// </summary>
	void RTVInit();

	/// <summary>
	/// RenderTextureの生成
	/// </summary>
	void CrateRenderTexture();

	/// <summary>
	/// fenceの生成
	/// </summary>
	void CreateFence();

	/// <summary>
	/// Viewportの初期化
	/// </summary>
	void ViewportInit();

	/// <summary>
	/// scissorRectの初期化
	/// </summary>
	void ScissorRectInit();

	/// <summary>
	/// DXCコンパイラの生成
	/// </summary>
	void CreateDXCCompilier();

	void ChangeDepthStatetoRead();
	void ChangeDepthStatetoRender();

	void tempRender();


public: //Getter

	/// <summary>
	/// ディスクリプターハンドルの取得
	/// </summary>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns>デバイス</returns>
	Microsoft::WRL::ComPtr <ID3D12Device> GetDevice() { return device_.Get(); };

	/// <summary>
	/// 描画コマンドの取得
	/// </summary>
	/// <returns>描画コマンドリスト</returns>
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> GetCommandList() { return commandList_.Get(); };

	// Accessor
	IDxcUtils* GetDxcUtils() { return dxcUtils_; };
	IDxcCompiler3* GetDxcCompiler() { return dxcCompiler_; };
	IDxcIncludeHandler* GetIncludeHandler() { return includeHandler_; };

	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc_; };
	D3D12_RENDER_TARGET_VIEW_DESC GetrtvDesc() { return rtvDesc_; };
	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() { return depthStencilDesc_; };

	uint32_t GetRenderIndex() { return renderTexData_.srvIndex; }
	uint32_t GetDepthIndex() { return depthIndex_; }

public: 
	//ReleaseCheck
	struct D3DResourceLeakChecker {
		~D3DResourceLeakChecker()
		{
			// リソースリークチェック
			Microsoft::WRL::ComPtr <IDXGIDebug1> debug;
			if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
				debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			}
		}
	};


	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr < ID3D12Device> device, int32_t width, int32_t height);
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateRenderTextureResource(Microsoft::WRL::ComPtr < ID3D12Device> device, int32_t width, int32_t height,DXGI_FORMAT format,const Vector4& clearColor);
	/*D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		handleCPU.ptr += (descriptorSize * index);
		return handleCPU;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
		D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		handleGPU.ptr += (descriptorSize * index);
		return handleGPU;
	}*/

private:
	// FPS固定初期化
	void InitializeFixFPS();

	// FPS固定更新
	void UpdateFixFPS();



	// 記録時間（FPS固定用）
	std::chrono::steady_clock::time_point reference_;

	// DXGIファクトリーの生成
	Microsoft::WRL::ComPtr < IDXGIFactory7> dxgiFactory;

	// HRESULTはWindows系のエラーコードであり、
	// 関数が成功したかどうかをSUCCEEDEDマクロで判定できる


	// 使用するアダプタ用の変数
	Microsoft::WRL::ComPtr < IDXGIAdapter4> useAdapter_;
	Microsoft::WRL::ComPtr < ID3D12Device> device_;
	Microsoft::WRL::ComPtr < ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList_;

	//スワップチェーン
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	Microsoft::WRL::ComPtr < ID3D12Resource> swapChainResources_[2] = { nullptr };

	// RenderTextureresource
	Microsoft::WRL::ComPtr < ID3D12Resource> renderTextureResource_ = nullptr;


	// ディスクリプタヒープの生成
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr; // DSVようのヒープでディスクリプタの数は1。DSVはShader内で触るものではないので、ShaderVisibleはfalse



	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};

	// ディスクリプタヒープの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;

	// RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[3];

	// これから書き込むバックバッファのインデックスを取得
	//UINT backBufferIndex_;
	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier_{};
	D3D12_RESOURCE_BARRIER barrier2_{};

	// 初期値0でFenceを作る
	Microsoft::WRL::ComPtr < ID3D12Fence> fence_;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;

	// dxCompilerを初期化
	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;

	IDxcIncludeHandler* includeHandler_;


	WinAPI* sWinAPI_ = nullptr;


	// DepthStencilTextureをウィンドウのサイズで作成
	Microsoft::WRL::ComPtr < ID3D12Resource> depthStencilResource_;


	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};

	// 描画先のRTVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	//ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};

	//ビューポート
	D3D12_VIEWPORT tmpViewport{};
	// シザー矩形
	D3D12_RECT tmpScissorRect{};

	TextureData renderTexData_;
	uint32_t depthIndex_;



public: // 共通変数　以下の変数は変更しない
	// ディスクリプターヒープのサイズをあらかじめ設定
	const static uint32_t rtvDescriptorSize_ = 3;
	const static uint32_t dsvDescriptorSize_ = 1;
};

