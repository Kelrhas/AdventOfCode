#pragma once

#include <cstdint>

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;
using S64 = int64_t;

const U8 CONSOLE_WHITE = 15;
const U8 CONSOLE_GREEN = 10;
const U8 CONSOLE_RED = 12;

#include <D:\Prog\Plop\Externals\glm\glm\glm.hpp>
#include <D:\Prog\Plop\Externals\glm\glm\gtc\matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <D:\Prog\Plop\Externals\glm\glm\/gtx/hash.hpp>


template <typename cT>
std::basic_ostream<cT>& operator<< (std::basic_ostream<cT>& out,
	glm::vec3 const& _vec) {
	out << _vec.x << "," << _vec.y << "," << _vec.z;
	return out;
}
template <typename cT>
std::basic_ostream<cT>& operator<< (std::basic_ostream<cT>& out,
	glm::ivec3 const& _vec) {
	out << _vec.x << "," << _vec.y << "," << _vec.z;
	return out;
}