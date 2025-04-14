/**
* @ file Mesh.h
* @ brief Meshに関する関数
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include <dxcapi.h>

#include "DirectionLight.h"
#include "TransformationMatrix.h"
#include "wrl.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")


class Mesh
{
public:
	Mesh();
	~Mesh();
	static Microsoft::WRL::ComPtr <ID3D12Resource> CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device,size_t sizeInBytes);	
};

