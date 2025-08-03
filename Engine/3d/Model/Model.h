/**
* @ file Model
* @ brief gltfとobjでの通常モデルの読むクラス
* @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "PSOModel.h"
#include "Mesh.h"
#include "TextureManager.h"
//#include "ModelManager.h"

#include "Transform.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "vertexData.h"
#include "ModelData.h"
#include "MaterialData.h"
#include "mathFunction.h"
#include "Material.h"
#include "TransformationMatrix.h"


#include "Animation.h"
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")



class Model
{
public:
	
	
	Model();
	~Model();
	void Initialize(const std::string& directoryPath, const std::string& filePath);
	void Update();
	void Draw(uint32_t texture, uint32_t instance);


	void SetTextureManager(TextureManager* textureManager) {
		textureManager_ = textureManager;
	}
	ModelData LoadObjFile(const std::string& directoryPath, const std::string&filePath);
	ModelData LoadGLTFFile(const std::string& directoryPath, const std::string& filePath);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath,const std::string& filename);

	Matrix4x4 GetAniMatrix() { return aniMatrix_; }
	Matrix4x4 GetSkeMatrix() { return skeMatrix_; }
	// アニメーション読み込み
	AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& filePath);

	Node ReadNode(aiNode* node);
	void ApplyAnimation(SkeletonData& skeleton, const AnimationData& animation, float animationTime);

public: //Getter
	// Particleにモデルのデータを貸し出す用
	ModelData GetModelData() { return modelData_; }

private:


	
	HRESULT hr;
	// RootSignature作成
	//ModelManager* modelManager = nullptr;

	// 頂点リソースにデータを書き込む
	

	VertexData* vertexData_;
	ModelData modelData_;
	AnimationData animation_;
	SkeletonData skeleton_;
	SkinCluster skinCluster_;


	DirectXCommon* directXCommon_;
	WinAPI* sWinAPI_;
	TextureManager* textureManager_ = nullptr;
	PSO* pso_ = nullptr;
	
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_VERTEX_BUFFER_VIEW vbvs[2]{};

	/*色用*/
//頂点リソースの設定
// 実際に頂点リソースを作る
	
	


	
	

	// IndexBuffer用リソース

	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW	indexBufferView_{};
	

	

	Transform transformUv;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};
	Camera* camera_ = nullptr;

	Matrix4x4 aniMatrix_;
	Matrix4x4 skeMatrix_;
	float animationTime_ = 0.0f;

	
};

constexpr float EPSILON = 0.0001f;

inline bool NearlyEqual(float a, float b) {
	return std::fabs(a - b) < EPSILON;
}

inline bool operator==(const VertexData& lhs, const VertexData& rhs) {
	return
		NearlyEqual(lhs.position.x, rhs.position.x) &&
		NearlyEqual(lhs.position.y, rhs.position.y) &&
		NearlyEqual(lhs.position.z, rhs.position.z) &&
		NearlyEqual(lhs.position.w, rhs.position.w) &&

		NearlyEqual(lhs.normal.x, rhs.normal.x) &&
		NearlyEqual(lhs.normal.y, rhs.normal.y) &&
		NearlyEqual(lhs.normal.z, rhs.normal.z) &&

		NearlyEqual(lhs.texcorrd.x, rhs.texcorrd.x) &&
		NearlyEqual(lhs.texcorrd.y, rhs.texcorrd.y);
}

struct VertexDataHasher {
	size_t operator()(const VertexData& v) const {
		size_t h = 0;
		auto hash_f = [](float f) {
			return std::hash<int>()(static_cast<int>(f * 10000)); // 精度調整
			};

		h ^= hash_f(v.position.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= hash_f(v.position.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= hash_f(v.position.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= hash_f(v.position.w) + 0x9e3779b9 + (h << 6) + (h >> 2);

		h ^= hash_f(v.normal.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= hash_f(v.normal.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= hash_f(v.normal.z) + 0x9e3779b9 + (h << 6) + (h >> 2);

		h ^= hash_f(v.texcorrd.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= hash_f(v.texcorrd.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
		return h;
	}
};

