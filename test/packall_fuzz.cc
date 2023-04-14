#include "packall_test.h"

#include "fuzztest/fuzztest.h"

using namespace fuzztest;
using namespace testing;

namespace {
void test_zigzag(uint64_t v)
{
	EXPECT_EQ(packall::detail::zigzag_decode(packall::detail::zigzag_encode(v)), v);
}
FUZZ_TEST(packall_fuzz, test_zigzag);

template<typename T>
void decode_T(std::span<uint8_t> bytes)
{
	struct s
	{
		T t;
	} a;
	// This should never crash, even with totally invalid inputs
	(void)packall::unpack(a, bytes);
}

#define COMMA ,

#define CONCAT2(x, y) x##y
#define CONCAT(x, y) CONCAT2(x, y)
#define DO_TEST(name, type, n)                             \
	void CONCAT(roundtrip_, name)(type v)                  \
	{                                                      \
		roundtrip_T(v);                                    \
	}                                                      \
	FUZZ_TEST(packall_fuzz, CONCAT(roundtrip_, name));     \
	void CONCAT(decode_, name)(std::vector<uint8_t> bytes) \
	{                                                      \
		decode_T<type>(bytes);                             \
	}                                                      \
	FUZZ_TEST(packall_fuzz, CONCAT(decode_, name))         \
	    .WithDomains(Arbitrary<std::vector<uint8_t>>().WithMinSize(4).WithMaxSize(n + 4 + 1))

#define RT_TEST(name, type)               \
	void CONCAT(roundtrip_, name)(type v) \
	{                                     \
		roundtrip_T(v);                   \
	}                                     \
	FUZZ_TEST(packall_fuzz, CONCAT(roundtrip_, name));

// Integers
DO_TEST(int8, int8_t, 1);
DO_TEST(int16, int16_t, 2);
DO_TEST(int32, int32_t, 5);
DO_TEST(int64, int64_t, 10);
DO_TEST(uint8, uint8_t, 1);
DO_TEST(uint16, uint16_t, 2);
DO_TEST(uint32, uint32_t, 5);
DO_TEST(uint64, uint64_t, 10);
DO_TEST(float, float, 4);
DO_TEST(double, double, 8);
DO_TEST(char, char, 1);

struct foo
{
	int x;

	bool operator==(const foo& o) const = default;
};
struct bar
{
	foo a;
	int b;
	foo c;

	bool operator==(const bar& o) const = default;
};

RT_TEST(test_nested_structs, bar);
RT_TEST(vector_nested, std::vector<bar>);

RT_TEST(everything, everything);

// Containers
DO_TEST(string, std::string, 20);
RT_TEST(vector_string, std::vector<std::string>);
RT_TEST(map_int_string, std::map<int COMMA std::string>);

#define DO_PERMUTE_TEST(name, t0, t1, t2)                      \
	void CONCAT(roundtrip_permute_, name)(t0 v0, t1 v1, t2 v2) \
	{                                                          \
		roundtrip_T(v0, v1, v2);                               \
		roundtrip_T(v0, v2, v1);                               \
		roundtrip_T(v1, v0, v2);                               \
		roundtrip_T(v1, v2, v0);                               \
		roundtrip_T(v2, v1, v0);                               \
		roundtrip_T(v2, v0, v1);                               \
	}                                                          \
	FUZZ_TEST(packall_fuzz, CONCAT(roundtrip_permute_, name))

DO_PERMUTE_TEST(int_string_map, int, std::string, std::map<int COMMA std::string>);

} // namespace
