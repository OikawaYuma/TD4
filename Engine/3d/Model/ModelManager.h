/**
* @ file ModelManager.h
* @ brief 読み込んだModelを格納するクラス
* @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <vector>
#include <map>
#include "Model.h"
#include "AnimationModel.h"
class ModelManager
{
private:
	ModelManager() = default;
	~ModelManager() = default;
	const ModelManager& operator=(const ModelManager&) = delete;
public:
	static ModelManager* GetInstance();
public:
	void init();
	void Update();
	void Release();
	void Finalize();
private:
	DirectXCommon* dxCommon_;
private:
	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models_;
	// モデルデータ アニメーション
	std::map<std::string, std::unique_ptr<AnimationModel>> animationModels_;
public:
	/// <summary>
	/// モデルファイルの読み込み
	/// </summary>
	void LoadModel(const std::string& directoryPath, const std::string& filePath);
	/// <summary>
	/// モデルファイルの読み込み
	/// </summary>
	void LoadAnimationModel(const std::string& directoryPath, const std::string& filePath);
	/// <summary>
	/// モデルの検索
	/// </summary>
	Model* FindModel(const std::string& filePath);

	AnimationModel* FindAnimationModel(const std::string& filePath);


public:	//Gettrer
	//DirectXCommon* GetDxCommon() const { return dxCommon_; }

};

