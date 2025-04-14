#include "PostProcess.h"
#include "PSOPostEffect.h"
#include "Mesh.h"
#include <SRVManager.h>
PostProcess::PostProcess()
{
}
PostProcess::~PostProcess()
{
	vertexResourceSprite_.Reset();
	transformationMatrixResouceSprite.Reset();
	indexResourceSprite.Reset();
	materialResource.Reset();
	directionalLightResource.Reset();
	depthStencilResource.Reset();


}
void PostProcess::Init()
{
	noiseTexture_ = TextureManager::StoreTexture("Resources/noise2.png");
	
}

void PostProcess::Update()
{
}

void PostProcess::Draw(){
	//materialData->projectionInverse = Inverse(camera_->GetProjectionMatrix());
	PSOPostEffect* pso_ = PSOPostEffect::GatInstance();
	pso_->EffectChangeCheck(effectNo_);
	DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();
	
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	sDirectXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pso_->SetCommand(this,effectNo_);
	// 描画（DrawCall/ドローコール）
	//sDirectXCommon->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	sDirectXCommon->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void PostProcess::Release()
{
}
