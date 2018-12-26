#include <variant_helpers.h>

bool getVariantBool(const love::Variant &v, bool def)
{
	if (v.getType() == love::Variant::BOOLEAN) {
		return v.getData().boolean;
	}
	return def;
}

std::string getVariantString(const love::Variant &v, const std::string &def)
{
	auto &data = v.getData();
	if (v.getType() == love::Variant::SMALLSTRING) {
		return std::string(data.smallstring.str, data.smallstring.len);
	} else if (v.getType() == love::Variant::STRING) {
		return std::string(data.string->str, data.string->len);
	}
	return def;
}
