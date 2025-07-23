#include "WorldTransform.h"
#include "mathFunction.h"

void WorldTransform::Initialize() {
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}
}

void WorldTransform::CreateConstBuffer() {

}

void WorldTransform::Map() {

}

void WorldTransform::TransferMatrix() {

}

void WorldTransform::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 親があれば親のワールド行列掛ける

	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}


	// 定数バッファに転送する
	//TransferMatrix();

}

void WorldTransform::TransferMatrixToSRT() {
    // スケールの抽出
    scale_.x = Length({ matWorld_.m[0][0], matWorld_.m[0][1], matWorld_.m[0][2] });
    scale_.y = Length({ matWorld_.m[1][0], matWorld_.m[1][1], matWorld_.m[1][2] });
    scale_.z = Length({ matWorld_.m[2][0], matWorld_.m[2][1], matWorld_.m[2][2] });

    // 回転部分だけの行列を取り出し、スケーリングを除去
    Matrix4x4 rotMat = matWorld_;
    for (int i = 0; i < 3; ++i) {
        rotMat.m[0][i] /= scale_.x;
        rotMat.m[1][i] /= scale_.y;
        rotMat.m[2][i] /= scale_.z;
    }

    // 回転行列 → オイラー角
    rotation_ = ExtractEulerAngles(rotMat);

    // 平行移動
    translation_.x = matWorld_.m[3][0];
    translation_.y = matWorld_.m[3][1];
    translation_.z = matWorld_.m[3][2];
}