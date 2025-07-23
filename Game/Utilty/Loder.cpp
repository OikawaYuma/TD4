#include "Loder.h"
#pragma warning(push)
// 外部ライブラリの警告を無視する
#pragma warning(disable:26819)
#include  <json.hpp>
#pragma warning(pop)
#include <fstream>
#include <cassert>
#include "ModelManager.h"
#include <iostream>


LevelData Loder::LoadJsonFile(const std::string kDefaultBaseDirectory, const std::string fileName)
{
	LevelData levelData;
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// Json文字列から回答したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	// 正しいレベルデータファイル化チェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	//LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData.objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData.objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.filename = object["file_name"];
				// トランスフォームのパラメータ読み込み
				nlohmann::json& transform = object["transform"];
				// 平行移動
				objectData.transform.translate.x = (float)transform["translation"][0];
				objectData.transform.translate.y = (float)transform["translation"][2];
				objectData.transform.translate.z = (float)transform["translation"][1];
				// 回転角
				objectData.transform.rotate.x = -(float)transform["rotation"][0];
				objectData.transform.rotate.y = -(float)transform["rotation"][2];
				objectData.transform.rotate.z = -(float)transform["rotation"][1];
				// スケーリングa
				objectData.transform.scale.x = (float)transform["scaling"][0];
				objectData.transform.scale.y = (float)transform["scaling"][2];
				objectData.transform.scale.z = (float)transform["scaling"][1];

				if (object.contains("collider")) {
					nlohmann::json& collider = object["collider"];
				
					objectData.collisionMode = collider["type"];
					objectData.collisionSize.x = collider["size"][0].get<float>();
					objectData.collisionSize.y = collider["size"][1].get<float>();
					objectData.collisionSize.z = collider["size"][2].get<float>();
				}

				continue;
			}

			
		}

		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
		
	}
	return levelData;
}

