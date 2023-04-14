#include "packall_test.h"

using namespace packall;

template<typename U, typename T>
void CanonicalBinaryTest(std::span<uint8_t> f_bytes, std::span<uint8_t> v_bytes, const T& x)
{
	U f{}, v{}, ex;
	ex.t = x;
	std::vector<uint8_t> f_pack, v_pack;
	pack<options::none>(ex, f_pack);
	EXPECT_EQ(f_pack, std::vector<uint8_t>(f_bytes.data(), f_bytes.data() + f_bytes.size()));
	EXPECT_EQ(unpack<options::none>(f, f_bytes), status::ok);
	EXPECT_EQ(f.t, x);
	if(f.t != x) {
		printf("FIXED BYTES: %s\n", to_bytes(f_bytes).c_str());
		printf("WANT  BYTES: %s\n", to_bytes(f_pack).c_str());
	}
	if(!v_bytes.empty()) {
		pack<options::variable_length_encoding>(ex, v_pack);
		EXPECT_EQ(v_pack, std::vector<uint8_t>(v_bytes.data(), v_bytes.data() + v_bytes.size()));
		EXPECT_EQ(unpack<options::variable_length_encoding>(v, v_bytes), status::ok);
		EXPECT_EQ(v.t, x);
		if(v.t != x) {
			printf("VAR  BYTES: %s\n", to_bytes(v_bytes).c_str());
			printf("WANT BYTES: %s\n", to_bytes(v_pack).c_str());
		}
	}
}

#define COMMA ,

#define B(name, ...) static uint8_t name[] = {__VA_ARGS__}

#define T(name, type, f_bytes, v_bytes, ...)           \
	namespace name {                                   \
	struct S                                           \
	{                                                  \
		static constexpr size_t Arity = 1;             \
		static constexpr traits Traits = traits::none; \
		type t;                                        \
	};                                                 \
	TEST(packall_canonical, name)                      \
	{                                                  \
		f_bytes;                                       \
		v_bytes;                                       \
		CanonicalBinaryTest<S>(f, v, __VA_ARGS__);     \
	}                                                  \
	}

// These are canonical representations, these tests are immutable

// s8/u8 is never var-encoded or zigzag encoded
T(u8, uint8_t, B(f, 2, 0xFF), B(v, 2, 0xFF), 0xFF);
T(s8, int8_t, B(f, 2, 0xFF), B(v, 2, 0xFF), -1);
T(ch, char, B(f, 2, 0xFF), B(v, 2, 0xFF), -1);

T(u16, uint16_t, B(f, 2, 0xFF, 0xFF), B(v, 2, 0xFF, 0xFF, 3), 0xFFFF);
T(s16, int16_t, B(f, 2, 0x18, 0xFC), B(v, 2, 0xCF, 0x0F), -1000);

T(u32, uint32_t, B(f, 2, 0xFF, 0xFF, 0, 0), B(v, 2, 0xFF, 0xFF, 3), 0xFFFF);
T(s32, int32_t, B(f, 2, 0x60, 0x79, 0xFE, 0xFF), B(v, 2, 0xBF, 0x9A, 0xC), -100000);

T(u64, uint64_t, B(f, 2, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0), B(v, 2, 0xFF, 0xFF, 3), 0xFFFF);
T(s64, int64_t, B(f, 2, 0x60, 0x79, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF), B(v, 2, 0xBF, 0x9A, 0xC), -100000);

// Floating point representations don't have variable encodings
T(f32, float, B(f, 2, 0xDB, 0x0F, 0x49, 0x40), B(v, 2, 0xDB, 0x0F, 0x49, 0x40), 3.14159265359f);
T(f64, double, B(f, 2, 0xEA, 0x2E, 0x44, 0x54, 0xFB, 0x21, 0x09, 0x40),
    B(v, 2, 0xEA, 0x2E, 0x44, 0x54, 0xFB, 0x21, 0x09, 0x40), 3.14159265359);

// Simple nested struct
struct two_ints
{
	int a, b;
	bool operator==(const two_ints&) const = default;
};

T(twoints, two_ints, B(f, 2, 3, 0xFF, 0xFF, 0xFF, 0xFF, 0xE8, 3, 0, 0), B(v, 2, 3, 1, 0xD0, 0xF), two_ints{-1, 1000});

struct S
{
	deprecated<two_ints> v;
};
struct S2
{
	std::optional<two_ints> v;
	static constexpr size_t Arity = 1;
};
struct S3
{
	std::unique_ptr<two_ints> v;
};
struct S4
{
	deprecated<std::vector<int>> v;
};
struct S5
{
	std::optional<std::vector<int>> v;
	static constexpr size_t Arity = 1;
};
struct S6
{
	std::unique_ptr<std::vector<int>> v;
};
TEST(packall_canonical, deprecated_and_optional_and_ptr)
{
	S s;
	S2 s2;
	S3 s3;
	S4 s4;
	S5 s5;
	S6 s6;
	std::vector<uint8_t> bytes;

	// deprecated<T> replaces T's length with 0
	pack(s, bytes);
	EXPECT_EQ(bytes, (std::vector<uint8_t>{2, 0}));

	// optional<T> for structs should encode the same way
	bytes.clear();
	pack(s2, bytes);
	EXPECT_EQ(bytes, (std::vector<uint8_t>{2, 0}));

	// unique_ptr<T> for structs should encode the same way
	bytes.clear();
	pack(s3, bytes);
	EXPECT_EQ(bytes, (std::vector<uint8_t>{2, 0}));

	// deprecated<T> replaces T's length with 0
	bytes.clear();
	pack(s4, bytes);
	EXPECT_EQ(bytes, (std::vector<uint8_t>{2, 0}));

	// optional<T> for containers should encode the same way
	bytes.clear();
	pack(s5, bytes);
	EXPECT_EQ(bytes, (std::vector<uint8_t>{2, 0}));

	// unique_ptr<T> for containers should encode the same way
	bytes.clear();
	pack(s6, bytes);
	EXPECT_EQ(bytes, (std::vector<uint8_t>{2, 0}));

	// Full structs/containers can still be "unpacked" into a deprecated<T>, also the same for optional and unique_ptr
	std::vector<uint8_t> full{2, 3, 0xFF, 0xFF, 0xFF, 0xFF, 0xE8, 3, 0, 0};
	EXPECT_EQ(unpack(s, full), status::ok);
	EXPECT_EQ(unpack(s2, full), status::ok);
	EXPECT_EQ(unpack(s3, full), status::ok);
	EXPECT_EQ(unpack(s4, full), status::ok);
	EXPECT_EQ(unpack(s5, full), status::ok);
	EXPECT_EQ(unpack(s6, full), status::ok);
}

// Omitted elements can be added anywhere
struct two_ints_omit
{
	omit<std::string> c;
	int a, b;
	bool operator==(const two_ints_omit& o) const = default;
};

// Adding an omit<t> should not change the output
T(twoints_omitthird, two_ints_omit, B(f, 2, 3, 0xFF, 0xFF, 0xFF, 0xFF, 0xE8, 3, 0, 0), B(v, 2, 3, 1, 0xD0, 0xF),
    two_ints_omit{std::string(), -1, 1000});

// Immutable structs are stored inline!
struct two_ints_inline
{
	int a, b;
	bool operator==(const two_ints_inline&) const = default;
	static constexpr traits Traits = traits::immutable;
};

static_assert(detail::typeinfo<two_ints_inline>::use_predecode == false);

T(twoints_imm, two_ints_inline, B(f, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0xE8, 3, 0, 0), B(v, 2, 1, 0xD0, 0xF),
    two_ints_inline{-1, 1000});

struct two_ints_inline_omit
{
	omit<std::string> c;
	int a, b;
	bool operator==(const two_ints_inline_omit&) const = default;
	static constexpr traits Traits = traits::immutable;
};

T(twoints_imm_omit, two_ints_inline_omit, B(f, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0xE8, 3, 0, 0), B(v, 2, 1, 0xD0, 0xF),
    two_ints_inline_omit{std::string(), -1, 1000});

// Containers
// All simple containers are created equal
TEST(packall_canonical, linear_containers)
{
	std::vector<uint8_t> bytes = {5, 1, 2, 3, 4};
	std::list<int> list;
	std::vector<int> vector;
	std::deque<int> deque;
	std::set<int> set;
	std::unordered_set<int> uset;
	std::multiset<int> mset;
	std::unordered_multiset<int> umset;

	EXPECT_EQ(unpack<options::variable_length_encoding>(list, bytes), status::ok);
	EXPECT_EQ(list, (std::list<int>{-1, 1, -2, 2}));

	EXPECT_EQ(unpack<options::variable_length_encoding>(vector, bytes), status::ok);
	EXPECT_EQ(vector, (std::vector<int>{-1, 1, -2, 2}));

	EXPECT_EQ(unpack<options::variable_length_encoding>(deque, bytes), status::ok);
	EXPECT_EQ(deque, (std::deque<int>{-1, 1, -2, 2}));

	EXPECT_EQ(unpack<options::variable_length_encoding>(set, bytes), status::ok);
	EXPECT_EQ(set, (std::set<int>{-1, 1, -2, 2}));

	EXPECT_EQ(unpack<options::variable_length_encoding>(uset, bytes), status::ok);
	EXPECT_EQ(uset, (std::unordered_set<int>{-1, 1, -2, 2}));

	EXPECT_EQ(unpack<options::variable_length_encoding>(mset, bytes), status::ok);
	EXPECT_EQ(mset, (std::multiset<int>{-1, 1, -2, 2}));

	EXPECT_EQ(unpack<options::variable_length_encoding>(umset, bytes), status::ok);
	EXPECT_EQ(umset, (std::unordered_multiset<int>{-1, 1, -2, 2}));

	std::array<int, 9> larger{};
	std::array<int, 3> smaller{};

	int larger_raw[9] = {};
	int smaller_raw[3] = {};

	// Arrays can decode fewer elements but must be large enough to store everything
	EXPECT_EQ(unpack<options::variable_length_encoding>(larger, bytes), status::ok);
	EXPECT_EQ(unpack<options::variable_length_encoding>(smaller, bytes), status::incompatible);
	EXPECT_EQ(unpack<options::variable_length_encoding>(larger_raw, bytes), status::ok);
	EXPECT_EQ(unpack<options::variable_length_encoding>(smaller_raw, bytes), status::incompatible);
}

// All maps are created equal
TEST(packall_canonical, mapped_containers)
{
	std::vector<uint8_t> bytes = {3, 1, 2, 3, 4};
	std::map<int, int> map;
	std::unordered_map<int, int> umap;
	std::multimap<int, int> mmap;
	std::unordered_multimap<int, int> ummap;

	EXPECT_EQ(unpack<options::variable_length_encoding>(map, bytes), status::ok);
	EXPECT_EQ(map, (std::map<int, int>{{-1, 1}, {-2, 2}}));

	EXPECT_EQ(unpack<options::variable_length_encoding>(umap, bytes), status::ok);
	EXPECT_EQ(umap, (std::unordered_map<int, int>{{-1, 1}, {-2, 2}}));

	EXPECT_EQ(unpack<options::variable_length_encoding>(mmap, bytes), status::ok);
	EXPECT_EQ(mmap, (std::multimap<int, int>{{-1, 1}, {-2, 2}}));

	EXPECT_EQ(unpack<options::variable_length_encoding>(ummap, bytes), status::ok);
	EXPECT_EQ(ummap, (std::unordered_multimap<int, int>{{-1, 1}, {-2, 2}}));

	// Canonically, a map can also be accessed as a list of pair<K, V>
	std::vector<std::pair<int, int>> vector;
	EXPECT_EQ(unpack<options::variable_length_encoding>(vector, bytes), status::ok);
	EXPECT_EQ(vector, (std::vector<std::pair<int, int>>{{-1, 1}, {-2, 2}}));
}
