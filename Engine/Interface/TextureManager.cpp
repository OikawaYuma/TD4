#include "TextureManager.h"
#include "DirectXCommon.h"
#include "SRVManager.h"
#include "WinAPI.h"
#include <d3dx12.h>
#include <Mesh.h>
//DirectXCommon dirX;

// resourceを保存する場所を設定
// ImGuiが0に格納されているため1から格納する
int TextureManager::kSRVIndexTop = 1;
int TextureManager::kParIndez = 10;

// テクスチャ生成に必要なデータを定義
std::unordered_map<std::string, TextureData> TextureManager::textureDatas_;
//Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::intermediateResource_[kNumDescriptors] = { nullptr };

void TextureManager::Init()
{
	textureDatas_.reserve(SRVManager::kMaXSRVCount);
}
int TextureManager::StoreTexture(const std::string& filePath) {
	// 読み込み済みテクスチャを検索
	if (textureDatas_.contains(filePath)) {
		return textureDatas_[filePath].srvIndex;
	};
	 // テクスチャ枚数上限チェック
	//assert(SRVManager::kMaXSRVCount <= SRVManager::);
	DirectXCommon *sDirectXCommon = DirectXCommon::GetInstance();

	TextureData& textureData = textureDatas_[filePath];
	// Textureを読んで転送する
	// ミップマップの作成
	DirectX::ScratchImage mipImages_ = LoadTexture(filePath);
	textureData.srvIndex = SRVManager::Allocate();
	textureData.metaData = mipImages_.GetMetadata();
	textureData.resource = CreateTextureResource(sDirectXCommon->GetDevice().Get(), textureData.metaData);
	TextureManager::GetInstance()->intermediateResource_[textureData.srvIndex] = UploadTextureData(textureData.resource.Get(), mipImages_);
	// SRVを作成するDescriptorHeapの場所を決める
	textureData.textureSrvHandleCPU = SRVManager::GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.textureSrvHandleGPU = SRVManager::GetGPUDescriptorHandle(textureData.srvIndex);

	SRVManager::CreateSRVforTexture2D(textureData);

	return textureData.srvIndex;
};
int  TextureManager::PlusIndex() {
	
	//int  returnIndex = kParIndez ;
	kParIndez++;

	return kParIndez;
}
void TextureManager::Release() {
	for (auto& pair : textureDatas_) {
		auto& textureData = pair.second;
		textureData.resource.Reset();  // ComPtr は自動的に解放するので、これで十分
		// srvIndex や他のリソースも適切に解放する必要があれば、ここで行う
	}

	// 必要なら、その他のリソースも解放
	for (int i = 0; i < kNumDescriptors; ++i) {
		intermediateResource_[i].Reset();
	}
}



TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

//void TextureManager::SetTexture() {
//	sDirectXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
//};


const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath)
{
	// 範囲外指定違反チェック
	//assert();
	TextureData &textureData= textureDatas_[filePath];
	return  textureData.metaData;
}

DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filePath) {
	////// テクスチャファイルを呼んでプログラムを使えるようにする
	std::wstring filePathW = ConvertString(filePath);
	// テクスチャファイルを呼んでプログラムを使えるようにする
	DirectX::ScratchImage image{};
	// エラー検知用変数

	HRESULT hr_;
	if (filePathW.ends_with(L".dds")) {// .ddsで終わっていたらddsとみなす。より安全な方法はいくらでもあるので余裕があれば対応すると良い
		hr_ = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else {
		hr_ = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}
	// ミップマップの作成
	DirectX::ScratchImage mipImages{};
	if (DirectX::IsCompressed(image.GetMetadata().format)) {// 圧縮フォーマットか調べる
		mipImages = std::move(image);// 圧縮フォーマットならそのまま使うのでmoveする
	}
	else {
		hr_ = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 4, mipImages);
	}
	assert(SUCCEEDED(hr_));
	
	// ミップマップ付きのデータを渡す
	return mipImages;
}

Microsoft::WRL::ComPtr <ID3D12Resource> TextureManager::CreateTextureResource(Microsoft::WRL::ComPtr <ID3D12Device> device, const DirectX::TexMetadata& metadata)
{
	Microsoft::WRL::ComPtr <ID3D12Resource> resource_;
	
	D3D12_RESOURCE_DESC resourceDesc_{};
	// metadataを基にResourceの設定
	resourceDesc_.Width = UINT(metadata.width); // Textureの幅
	resourceDesc_.Height = UINT(metadata.height); // Textureの高さ
	resourceDesc_.MipLevels = UINT16(metadata.mipLevels); // mipmapの数
	resourceDesc_.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行き or 配列Textureの配列数
	resourceDesc_.Format = metadata.format; // TextureのFormat
	resourceDesc_.SampleDesc.Count = 1; // サンプリングカウント。1固定・
	resourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // Textureの次元数。普段使っているのは2次元

	// 利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties_{};
	// 利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	heapProperties_.Type = D3D12_HEAP_TYPE_DEFAULT; // 細かい設定を行う
	//heapProperties_.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	//heapProperties_.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置

	// Rewsourceの生成
	resource_ = nullptr;
	HRESULT hr;
	hr = device->CreateCommittedResource(
		&heapProperties_, // Heapの設定
		D3D12_HEAP_FLAG_NONE, //Heapの特殊な設定。特になし。
		&resourceDesc_, // Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST, // 初回のResourceState。Textureは基本読むだけ
		nullptr, // Clear最適地。使わないのでnullptr
		IID_PPV_ARGS(&resource_));
	assert(SUCCEEDED(hr));

	resource_->SetName(L"AAAAAAA");
	return resource_;

}

[[nodiscard]]
Microsoft::WRL::ComPtr <ID3D12Resource> TextureManager::UploadTextureData(Microsoft::WRL::ComPtr <ID3D12Resource> texture, const DirectX::ScratchImage& mipImages) {
	
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(DirectXCommon::GetInstance()->GetDevice().Get(),
		mipImages.GetImages(), mipImages.GetImageCount(),
		mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = Mesh::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice().Get(), intermediateSize);
	UpdateSubresources(DirectXCommon::GetInstance()->GetCommandList().Get(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	// textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	DirectXCommon::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;
	//// Meta情報を取得
	//const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//// 全MipMapについて
	//for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
	//	// MipMapLevelを指定して書くImageを取得
	//	const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
	//	// Textureに転送
	//	HRESULT hr = texture->WriteToSubresource(
	//		UINT(mipLevel),
	//		nullptr,			   // 全領域へコピー
	//		img->pixels,		   // 元データアクセス
	//		UINT(img->rowPitch),   // 1ラインサイズ
	//		UINT(img->slicePitch)  // 1枚サイズ
	//	);
	//	assert(SUCCEEDED(hr));
	//}
};





