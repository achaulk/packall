#pragma once

#include <charconv>
#include <deque>
#include <map>
#include <list>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "../include/packall/packall.h"

#include "gtest/gtest.h"

template<typename T>
void test(T a, T b)
{
	EXPECT_EQ(a, b);
}

inline void test(float a, float b)
{
	EXPECT_TRUE(!memcmp(&a, &b, sizeof(a)));
}

inline void test(double a, double b)
{
	EXPECT_TRUE(!memcmp(&a, &b, sizeof(a)));
}

template<packall::options O, typename T>
void roundtrip_with_options(const T& v)
{
	std::vector<uint8_t> bytes;
	struct s
	{
		T t;
	} a{v}, b{};
	packall::pack<O>(a, bytes);
	EXPECT_EQ(packall::unpack<O>(b, bytes), packall::status::ok);
	test(b.t, v);
}

template<typename T>
void roundtrip_T(const T& v)
{
	roundtrip_with_options<packall::options::none>(v);
	roundtrip_with_options<packall::options::variable_length_encoding>(v);
}

template<packall::options O, typename A0, typename A1, typename A2>
void roundtrip_with_options(const A0& v0, const A1& v1, const A2& v2)
{
	std::vector<uint8_t> bytes;
	struct s
	{
		A0 a0;
		A1 a1;
		A2 a2;
	} a{v0, v1, v2}, b{};
	packall::pack<O>(a, bytes);
	EXPECT_EQ(packall::unpack<O>(b, bytes), packall::status::ok);
	test(b.a0, v0);
	test(b.a1, v1);
	test(b.a2, v2);
}

template<typename A0, typename A1, typename A2>
void roundtrip_T(const A0& v0, const A1& v1, const A2& v2)
{
	roundtrip_with_options<packall::options::none>(v0, v1, v2);
	roundtrip_with_options<packall::options::variable_length_encoding>(v0, v1, v2);
}

struct Config
{
	std::string device;
	std::pair<unsigned, unsigned> resolution;
	std::array<double, 9> K_matrix;
	std::vector<double> distortion_coeffients;
	std::map<std::string, std::variant<uint16_t, std::string, bool>> parameters;

	static constexpr const char *kMembers[] = {
	    "device", "resolution", "K_matrix", "distortion_coeffients", "parameters"};
};

struct everything
{
	static constexpr size_t Arity = 16;
	bool operator==(const everything& o) const = default;

	bool _1;
	int8_t _2;
	int16_t _3;
	int32_t _4;
	int64_t _5;
	uint8_t _6;
	uint16_t _7;
	uint32_t _8;
	uint64_t _9;
	std::map<int, std::string> _10;
	std::map<std::string, int> _11;
	char _12;
	std::string _13;
	std::vector<std::string> _14;
	std::list<std::string> _15;
	std::deque<std::vector<std::string>> _16;
};

inline std::string to_bytes(std::span<uint8_t> bytes)
{
	std::string s;
	for(size_t i = 0; i < bytes.size(); i++) {
		s.append("0x");
		char ch[2];
		std::to_chars(ch, ch + 2, bytes[i], 16);
		if(bytes[i] < 16) {
			s.push_back('0');
			s.push_back(ch[0]);
		} else {
			s.push_back(ch[0]);
			s.push_back(ch[1]);
		}
		s.append(", ");
		if((i & 15) == 15)
			s.append("\n");
	}
	return s;
}
