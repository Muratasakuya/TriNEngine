#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector2.h>
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Vector4.h>

// json
#include <Externals/nlohmann/json.hpp>
// using
using Json = nlohmann::json;

// c++
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cassert>

//============================================================================*/
//	JsonAdapter class
//============================================================================*/
class JsonAdapter {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	JsonAdapter() = default;
	~JsonAdapter() = default;

	static void Save(const std::string& saveDirectoryFilePath, const Json& jsonData);

	static Json Load(const std::string& loadDirectoryFilePath);

	static Json FromVector3(const Vector3& v);
	static Vector3 ToVector3(const Json& data);

	static Json FromVector2(const Vector2& v);
	static Vector2 ToVector2(const Json& data);

	static Json FromColor(const Color& v);
	static Color ToColor(const Json& data);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static const std::string& baseDirectoryFilePath_;

};