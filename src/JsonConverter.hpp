#ifndef JSON_CONVERTER_HPP_
#define JSON_CONVERTER_HPP_

#include <nlohmann/json.hpp>
#include <Utils/CubismJson.hpp>

class JsonConverter {
public:
	// Read a nlohmann-style json object, allocate and return a CubismJson object that stores in $outbuffer.
	// The buffer size will be stored in $outsize. 
	// [Memory] $outBuffer should be deallocate by calling LAppPal::ReleaseBytes() manually.
	// [Memory] The returned CubismJson object should be deallocated by calling CubismJson::Delete() mannually.
	// [Contract] The input nlohmann-style json $src must have a NON-NULL and valid "data" field
	static Csm::Utils::CubismJson* convert(const nlohmann::json* src, Csm::csmByte** outBuffer, Csm::csmSizeInt* outSize);

private:
	static void rewrite(nlohmann::json& j, const nlohmann::json* src);
	static nlohmann::json modify(const nlohmann::json* src);
	static Csm::csmByte* loadNJsonAsBytes(const nlohmann::json* data, Csm::csmSizeInt* outSize);

private:
	JsonConverter() = delete;
	JsonConverter(const JsonConverter&) = delete;
};

#endif
