#include "Line.h"
#include <mathFunction.h>
#include "SRVManager.h"
#include "TextureManager.h"
#include "PSO/PSOLine.h"
void Line::Initialize(Vector3 startPos, Vector3 endPos) {

	sDirectXCommon_ = DirectXCommon::GetInstance();
	//バッファリソース

	// 実際に頂点リソースを作る
	vertexResource = Mesh::CreateBufferResource(sDirectXCommon_->GetDevice(), sizeof(VertexData) * 6);

	vertexBufferView = CreateBufferView();


	// 頂点リソースにデータを書き込む
	vertexData_ = nullptr;
	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//左下
	vertexData_[0].position = { startPos.x,startPos.y,startPos.z,1.0f };
	vertexData_[0].normal = {
		vertexData_[0].position.x,
		vertexData_[0].position.y,
		vertexData_[0].position.z };
	vertexData_[0].texcorrd = { 0.0f,1.0f };
	//上
	vertexData_[1].position = { endPos.x,endPos.y,endPos.z,1.0f };
	vertexData_[1].normal = {
		vertexData_[1].position.x,
		vertexData_[1].position.y,
		vertexData_[1].position.z };
	vertexData_[1].texcorrd = { 0.5f,0.0f };
	

	// 実際に頂点リソースを作る
	materialResource = Mesh::CreateBufferResource(sDirectXCommon_->GetDevice(), sizeof(Material));

	materialBufferView = CreateBufferView();;
	// 頂点リソースにデータを書き込む
	materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 色のデータを変数から読み込み
	materialData->color = {1.0f,1.0f,1.0f,1.0f};
	materialData->enableLighting = false;
	materialData->uvTransform = MakeIdentity4x4();


	//バッファリソース
	// データを書き込む
	TransformationData = nullptr;
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource = Mesh::CreateBufferResource(sDirectXCommon_->GetDevice(), sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&TransformationData));
	//単位行列を書き込んでいく
	TransformationData->World = MakeIdentity4x4();

	// instancing
	// Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	lineData_.instancingResource = Mesh::CreateBufferResource(sDirectXCommon_->GetDevice(), sizeof(LineForGPU));
	// 書き込むためのアドレスを取得
	lineData_.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	// 単位行列を書き込んでおく
	SRVIndex_ = SRVManager::Allocate();
	SRVManager::CreateSRVforStructuredBuffer(SRVIndex_, lineData_.instancingResource.Get(), 1, sizeof(LineForGPU));
	lineData_.instancingSrvHandleCPU = SRVManager::GetCPUDescriptorHandle(SRVIndex_);
	lineData_.instancingSrvHandleGPU = SRVManager::GetGPUDescriptorHandle(SRVIndex_);
	//for (uint32_t index = 0; index < kNumMaxInstance_; ++index) {
	instancingData_[0].WVP = MakeIdentity4x4();
	instancingData_[0].World = MakeIdentity4x4();
	instancingData_[0].WorldInverseTranspose = MakeIdentity4x4();
	instancingData_[0].color = { 1.0f,1.0f,1.0f,1.0f };

};

void Line::Update() {
};


void Line::Draw(Camera* camera) {
	PSOLine* pso = PSOLine::GatInstance();

	camera_ = camera;
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldTransform_.matWorld_, camera->GetViewprojectionMatrix());

	instancingData_[0].WVP = worldViewProjectionMatrix;
	instancingData_[0].World = MakeIdentity4x4();
	instancingData_[0].WorldInverseTranspose = MakeIdentity4x4();
	instancingData_[0].color = { 1.0f,1.0f,1.0f,1.0f };
	// 色のデータを変数から読み込み
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	sDirectXCommon_->GetCommandList()->SetGraphicsRootSignature(pso->GetProperty().rootSignature.Get());
	sDirectXCommon_->GetCommandList()->SetPipelineState(pso->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	sDirectXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);    //VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	sDirectXCommon_->GetCommandList()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	// マテリアルCBufferの場所を設定
	sDirectXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	sDirectXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, lineData_.instancingSrvHandleGPU);

	// SRV のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	sDirectXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetGPUDescriptorHandle(TextureManager::GetInstance()->StoreTexture("Resources/kusa2.png")));
	// 描画！（DrawCall/ドローコール）・3頂点で1つのインスタンス。インスタンスについては今後
	sDirectXCommon_->GetCommandList()->DrawInstanced(2, 1, 0, 0);
};

D3D12_VERTEX_BUFFER_VIEW Line::CreateBufferView() {
	D3D12_VERTEX_BUFFER_VIEW view{};

	//リソースの先頭のアドレスから使う
	view.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	view.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点あたりのサイズ
	view.StrideInBytes = sizeof(VertexData);


	return view;
};
