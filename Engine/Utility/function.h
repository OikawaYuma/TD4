/**
* @ file function.h
* @ brief エンジン機能用の関数をまとめたクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>

#include<dxgidebug.h>
#include<dxcapi.h>

#include "Matrix4x4.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

std::wstring ConvertString(const std::string& str);
std::string ConvertString(const std::wstring& str);

void ConsoleLog(const std::string& message);

IDxcBlob* CompileShader(
	// CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	// Compileerに使用するProfile
	const wchar_t* profile,
	// 初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler);
void MatrixScreenPrintf(const Matrix4x4& matrix, const char* label);