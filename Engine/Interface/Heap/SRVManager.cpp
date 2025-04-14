#include "SRVManager.h"
#include "DirectXCommon.h"

const uint32_t SRVManager::kMaXSRVCount = 10000;

uint32_t SRVManager::useIndex_ = 1;

// SRV用のデスクリプタサイズ
uint32_t SRVManager::descriptorSize_;

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SRVManager::descriptorHeap_;
void SRVManager::Init() {
	// デスクリプタヒープの生成
	descriptorHeap_ = DirectXCommon::GetInstance()->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaXSRVCount, true);
	// デスクリプタ1個分のサイズを取得して記録
	descriptorSize_ = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

uint32_t SRVManager::Allocate()
{
	// return する番号をいったん記録しておく
	int index = useIndex_;
	// 次回のために番号を1進める
	useIndex_++;
	// 上で記録した番号をreturn
	return index;
}

SRVManager* SRVManager::GetInstance()
{
	static SRVManager instance;
	return &instance;
}

D3D12_CPU_DESCRIPTOR_HANDLE SRVManager::GetCPUDescriptorHandle(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::GetGPUDescriptorHandle(uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

void SRVManager::CreateSRVforTexture2D(const TextureData& textureData)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// metaDataを基にSRVの設定
	srvDesc.Format = textureData.metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	if(textureData.metaData.IsCubemap()){
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0; // unionがTextureCubeになったが、内部パラメータの意味はTexture2Dと変わらない
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャfi
		srvDesc.Texture2D.MipLevels = UINT(textureData.metaData.mipLevels);
	}
	// SRVの生成
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, GetCPUDescriptorHandle(textureData.srvIndex));

}

void SRVManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = sizeof(structureByteStride);
	// SRVの生成
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SRVManager::CreateSRVDepth(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// metaDataを基にSRVの設定
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャfi
	srvDesc.Texture2D.MipLevels = UINT(MipLevels);
	// SRVの生成
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SRVManager::CreateSRVRenderTexture(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// metaDataを基にSRVの設定
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャfi
	srvDesc.Texture2D.MipLevels = UINT(MipLevels);
	// SRVの生成
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SRVManager::PreDraw()
{
	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };
	DirectXCommon::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void SRVManager::SetGraphicsRootDescriptortable(UINT rootParameterIndex, uint32_t srvIndex)
{
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(rootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}
