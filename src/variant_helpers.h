#pragma once

#include <common/Variant.h>

bool getVariantBool(const love::Variant &v, bool def = false);

template<typename T = double>
T getVariantNumber(const love::Variant &v, T def = 0)
{
	if (v.getType() == love::Variant::NUMBER) {
		return (T)v.getData().number;
	}
	return def;
}

std::string getVariantString(const love::Variant &v, const std::string &def = "");
