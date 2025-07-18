#include "Object3d.h"
#include "Modelmanager.h"
#include "Object3dCommon.h"
#include <SRVManager.h>
void Object3d::Init()
{
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	worldTransform_.Initialize();

	// カメラ用
	cameraForGPUData_ = nullptr;
	cameraForGPUResource_ = Mesh::CreateBufferResource(directXCommon->GetDevice(), sizeof(CameraForGPU));
	//// 書き込むためのアドレスを取得
	cameraForGPUResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPUData_));

	cameraForGPUData_->worldPosition = { 1.0f,1.0f,-5.0f };

	// 実際に頂点リソースを作る
	materialResource = Mesh::CreateBufferResource(directXCommon->GetDevice(), sizeof(Material));

	/*materialBufferView = CreateBufferView();;*/
	// 頂点リソースにデータを書き込む
	materialData_ = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 色のデータを変数から読み込み
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = true;
	WorldTransform uvW;
	uvW.Initialize();
	uvW.scale_ = { 1.0f , 1.0f,1.0f };
	uvW.UpdateMatrix();
	materialData_->uvTransform = uvW.matWorld_;
	materialData_->shininess = 60.0f;

	directionalLightData = nullptr;
	directionalLightResource = Mesh::CreateBufferResource(directXCommon->GetDevice(), sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;

	spotLightResource_ = nullptr;
	spotLightResource_ = Mesh::CreateBufferResource(directXCommon->GetDevice(), sizeof(SpotLight));
	// 書き込むためのアドレスを取得
	spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));


	spotlight.color = { 0.0f,0.0f,0.0f,1.0f };
	spotlight.position = { 2.0f,1.25f,0.0f };
	spotlight.distance = 7.0f;
	spotlight.direction =
		Normalize(Vector3{ -1.0f,-1.0f,0.0f });
	spotlight.intensity = 4.0f;
	spotlight.dacya = 2.0f;
	spotlight.cosAngle =
		std::cos(std::numbers::pi_v<float> / 3.0f);

	spotLightData_;

	// instancing
	// Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	object3dData_.instancingResource = Mesh::CreateBufferResource(directXCommon->GetDevice(), sizeof(Object3dForGPU) * kNumMaxInstance_);
	// 書き込むためのアドレスを取得
	object3dData_.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	// 単位行列を書き込んでおく
	SRVIndex_ = SRVManager::Allocate();
	SRVManager::CreateSRVforStructuredBuffer(SRVIndex_, object3dData_.instancingResource.Get(), kNumMaxInstance_, sizeof(Object3dForGPU));
	object3dData_.instancingSrvHandleCPU = SRVManager::GetCPUDescriptorHandle(SRVIndex_);
	object3dData_.instancingSrvHandleGPU = SRVManager::GetGPUDescriptorHandle(SRVIndex_);
	//for (uint32_t index = 0; index < kNumMaxInstance_; ++index) {
	instancingData_[objectNum_].WVP = MakeIdentity4x4();
	instancingData_[objectNum_].World = MakeIdentity4x4();
	instancingData_[objectNum_].WorldInverseTranspose = MakeIdentity4x4();
	instancingData_[objectNum_].color = { 1.0f,1.0f,1.0f,1.0f };
	//}

}

void Object3d::Update()
{
	// 3Dモデルの更新処理を行う
	numInstance_ = 0;
	
	for (std::list<std::shared_ptr<ObjectPram>>::iterator objectPtamIterator = objectParms_.begin(); objectPtamIterator != objectParms_.end();) {
		if ((*objectPtamIterator)->isAlive == false) {
			objectPtamIterator = objectParms_.erase(objectPtamIterator);
			continue;
		}
		// インスタンス数が最大数を超えないようにする
		if (numInstance_ < kNumMaxInstance_) {
			instancingData_[numInstance_].World = (*objectPtamIterator)->worldTransform.matWorld_;
			instancingData_[numInstance_].WVP = instancingData_[numInstance_].World;
			instancingData_[numInstance_].WorldInverseTranspose = MakeIdentity4x4();
			instancingData_[numInstance_].color = (*objectPtamIterator)->color;
		}
		numInstance_++;
		++objectPtamIterator;
	}


	worldTransform_.UpdateMatrix();
	if (animationModel_) {
		animationModel_->Update();
	}
	else if (model_) {
		model_->Update();
	}


}

void Object3d::Draw(Camera* camera)
{
	numInstance_ = 0;


	for (std::list<std::shared_ptr<ObjectPram>>::iterator objectPtamIterator = objectParms_.begin(); objectPtamIterator != objectParms_.end();) {
		if ((*objectPtamIterator)->isAlive == false) {
			objectPtamIterator = objectParms_.erase(objectPtamIterator);
			continue;
		}
		if (numInstance_ < kNumMaxInstance_) {
			instancingData_[numInstance_].World = (*objectPtamIterator)->worldTransform.matWorld_;
			instancingData_[numInstance_].WVP = Multiply(instancingData_[numInstance_].World, camera->GetViewprojectionMatrix());;
			instancingData_[numInstance_].WorldInverseTranspose = Inverse(Transpose((*objectPtamIterator)->worldTransform.matWorld_));
			instancingData_[numInstance_].color = (*objectPtamIterator)->color;
			instancingData_[numInstance_].color.w = 1.0f;
		}
		numInstance_++;
		++objectPtamIterator;
	}

	cameraForGPUData_->worldPosition = camera->GetTransform().translate;
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	if (animationModel_) {
		PSOAnimationModel* pso = PSOAnimationModel::GatInstance();
		//directionalLightData->direction =  Normalize(directionalLightData->direction);
		directXCommon->GetCommandList()->SetGraphicsRootSignature(pso->GetProperty().rootSignature.Get());
		directXCommon->GetCommandList()->SetPipelineState(pso->GetProperty().graphicsPipelineState.Get());    //PSOを設定

	}
	else if (model_) {
		PSO* pso = PSO::GatInstance();
		//directionalLightData->direction =  Normalize(directionalLightData->direction);
		directXCommon->GetCommandList()->SetGraphicsRootSignature(pso->GetProperty().rootSignature.Get());
		directXCommon->GetCommandList()->SetPipelineState(pso->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	}
	else if (skybox_) {
		PSOSkybox* pso = PSOSkybox::GatInstance();
		//directionalLightData->direction =  Normalize(directionalLightData->direction);
		directXCommon->GetCommandList()->SetGraphicsRootSignature(pso->GetProperty().rootSignature.Get());
		directXCommon->GetCommandList()->SetPipelineState(pso->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	}







	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	directXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, object3dData_.instancingSrvHandleGPU);
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraForGPUResource_->GetGPUVirtualAddress());
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(5, spotLightResource_->GetGPUVirtualAddress());

	// 3Dモデルが割り当てられていれば描画する
	if (animationModel_) {

		animationModel_->Draw(skinTex_, { { 1.0f,1.0f,1.0f,1.0f },true
			}, { { 1.0f,1.0,1.0,1.0f } ,{ 0.0f,-1.0f,0.0f },0.5f });
	}
	else if (model_) {

		model_->Draw(skinTex_, numInstance_);

	}
	else if (skybox_) {

		skybox_->Draw(skinTex_, { { 1.0f,1.0f,1.0f,1.0f },false
			}, { { 1.0f,1.0,1.0,1.0f } ,{ 0.0f,-1.0f,0.0f },0.2f });
	}
}

void Object3d::Release()
{
	//for (auto& pair : textureDatas_) {
	//	auto& textureData = pair.second;
	//	textureData.resource.Reset();  // ComPtr は自動的に解放するので、これで十分
	//	// srvIndex や他のリソースも適切に解放する必要があれば、ここで行う
	//}

	//// 必要なら、その他のリソースも解放
	//for (int i = 0; i < kNumDescriptors; ++i) {
	//	intermediateResource_[i].Reset();
	//}

	object3dData_.instancingResource.Reset();

}

void Object3d::SetModel(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Object3d::SetAnimationModel(const std::string& filePath)
{
	animationModel_ = ModelManager::GetInstance()->FindAnimationModel(filePath);
}

void Object3d::SetTransform(Transform transform)
{
	worldTransform_.translation_ = transform.translate;
	worldTransform_.rotation_ = transform.rotate;
	worldTransform_.scale_ = transform.scale;

}

ModelData Object3d::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData; // 構築するMataData
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line; // ファイルから読んだ1行を格納するもの

	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		// identifierに応じた処理
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			position.z *= -1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;

			texcoord.y *= -1.0f;// -texcoord.y; //- texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.z *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// 区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);

				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//position.x *= -1.0f;
				//texcoord.y = 1.0f - texcoord.y;
				//normal.x *= -1.0f;

				VertexData vertex = { position, texcoord, normal };
				modelData.vertices.push_back(vertex);

				triangle[faceVertex] = { position,texcoord,normal };

			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);

		}
		else if (identifier == "mtllib") {
			// materialtemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}

	}
	return modelData;
}

MaterialData Object3d::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData materialData;// 構築するMaterialData
	std::string line; // ファイルから読んだ1行をかくのうするもの
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identiferに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}

	}

	return materialData;
}

void Object3d::AddListPram(std::shared_ptr<ObjectPram > objectParm)
{
	objectParms_.push_back(objectParm);
}


void Object3d::ModelDebug() {
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.0f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.3f, 0.5f));
	ImGui::Begin("ModelDebug");

	// 位置・回転・スケールの調整
	float translate[3] = { worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z };
	bool changed = false;
	if (ImGui::DragFloat3("transform", translate, 0.01f)) {
		worldTransform_.translation_.x = translate[0];
		worldTransform_.translation_.y = translate[1];
		worldTransform_.translation_.z = translate[2];
		changed = true;
	}
	float rotate[3] = { worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z };
	if (ImGui::DragFloat3("rotate", rotate, 0.01f)) {
		worldTransform_.rotation_.x = rotate[0];
		worldTransform_.rotation_.y = rotate[1];
		worldTransform_.rotation_.z = rotate[2];
		changed = true;
	}
	float scale[3] = { worldTransform_.scale_.x, worldTransform_.scale_.y, worldTransform_.scale_.z };
	if (ImGui::DragFloat3("scale", scale, 0.01f)) {
		worldTransform_.scale_.x = scale[0];
		worldTransform_.scale_.y = scale[1];
		worldTransform_.scale_.z = scale[2];
		changed = true;
	}

	// 値が変わったら行列を更新
	if (changed) {
		worldTransform_.UpdateMatrix();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}