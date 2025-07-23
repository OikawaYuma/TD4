/**
* @ Line.h
* @ brief Lineを生成するクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/


#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>

#include "WinAPI.h"
#include "DirectXCommon.h"
#include "Mesh.h"
#include "VertexData.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "Material.h"
#include "DirectionLight.h"
#include "TransformationMatrix.h"
#include "Transform.h"
#include "WorldTransform.h"
#include "Camera.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
// object1個分のデータ
struct LineData {
	std::string objectName;
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
	uint32_t instancingNum;
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

};

struct LineParam {
	bool isAlive;
	WorldTransform worldTransform;
	Vector4 color;

};

struct LineForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
	Vector4 color;
};
class Line
{
public:

	void Initialize();
	void Update();
	void Draw(Camera* camera);
	void Release();
	D3D12_VERTEX_BUFFER_VIEW  CreateBufferView();
	D3D12_RESOURCE_DESC  CreateBufferResourceDesc(size_t sizeInBytes);

public:
	void SetWorldTransform(WorldTransform worldTransform) { worldTransform_ = worldTransform; }
	void SetColor(Vector4 color) {
		materialData->color = color;
	}

	void AddListPram(std::shared_ptr<LineParam > LineParam);
private:

	HRESULT hr;
	/*頂点用*/
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// 頂点リソースにデータを書き込む
	VertexData* vertexData_;

	std::list<std::shared_ptr<LineParam>> lineParams_;
	uint32_t numInstance_ = 0; // インスタンス数
	uint32_t kNumMaxInstance_ = 1000000; // インスタンス数
	/*色用*/
	//頂点リソースの設定
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};
	// 頂点リソースにデータを書き込む
	Material* materialData;
	uint32_t SRVIndex_ = 0;
	/*移動用*/
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr < ID3D12Resource> wvpResource;
	LineForGPU* instancingData_ = nullptr;
	LineData lineData_;
	// データを書き込む
	TransformationMatrix* TransformationData;

	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};


	DirectXCommon* sDirectXCommon_ = nullptr;

	Camera* camera_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
};

