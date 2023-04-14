#include "packall_test.h"

struct sub_v1
{
	int x;
	int y;
};

struct sub_v2
{
	int x;
	int y;
	float z;
};

struct root_v1
{
	std::string a;
	std::vector<std::string> b;
	sub_v1 c;
	int d;
};

// Change the struct type, add a field
struct root_v2
{
	std::string a;
	std::vector<std::string> b;
	sub_v2 c;
	int d;
	int e;
};

// Deprecate the struct
struct root_v3
{
	std::string a;
	std::vector<std::string> b;
	packall::deprecated<sub_v2> c;
	int d;
	int e;
};

TEST(packall_compat, forward_compatibility)
{
	root_v1 v1{"a", {"b1", "b1"}, {1, 2}, 99};
	std::vector<uint8_t> v1_bytes;

	packall::pack(v1, v1_bytes);

	root_v2 v2{};
	EXPECT_EQ(packall::unpack(v2, v1_bytes), packall::status::ok);

	EXPECT_EQ(v2.a, v1.a);
	EXPECT_EQ(v2.b, v1.b);
	EXPECT_EQ(v2.c.x, v1.c.x);
	EXPECT_EQ(v2.c.y, v1.c.y);
	EXPECT_EQ(v2.c.z, 0); // default initialized
	EXPECT_EQ(v2.d, v1.d);
}

TEST(packall_compat, deprecation)
{
	root_v2 v2{"a", {"b1", "b1"}, {1, 2}, 99, 100};
	std::vector<uint8_t> v2_bytes;

	packall::pack(v2, v2_bytes);

	root_v3 v3{};
	EXPECT_EQ(packall::unpack(v3, v2_bytes), packall::status::ok);

	// All other values are equal
	EXPECT_EQ(v3.a, v2.a);
	EXPECT_EQ(v3.b, v2.b);
	EXPECT_EQ(v3.d, v2.d);
	EXPECT_EQ(v3.e, v2.e);

	std::vector<uint8_t> v3_bytes;
	packall::pack(v3, v3_bytes);

	// A deprecation does not break backwards compatiblility
	root_v2 v2b{};
	EXPECT_EQ(packall::unpack(v2b, v3_bytes), packall::status::ok);
	EXPECT_EQ(v2b.a, v2.a);
	EXPECT_EQ(v2b.b, v2.b);
	EXPECT_EQ(v2b.d, v2.d);
	EXPECT_EQ(v2b.e, v2.e);
}

struct back1
{
	static constexpr packall::traits Traits = packall::traits::backwards_compatible;

	int a;
	int b;
};

struct back2
{
	int a;
	int b;
	int c;
};

template<>
struct packall::struct_traits<back2>
{
	static constexpr packall::traits Traits = packall::traits::backwards_compatible;
};

static_assert(packall::detail::typeinfo<back1>::is_backwards_compatible);
static_assert(packall::detail::typeinfo<back2>::is_backwards_compatible);

struct s1
{
	int w;
	back1 x;
	int y;
};
struct s2
{
	int w;
	back2 x;
	int y;
};

TEST(packall_compat, backwards)
{
	s2 v2{98, {1, 2, 3}, 99};
	std::vector<uint8_t> v2_bytes;
	packall::pack(v2, v2_bytes);

	// If inner structs are marked as backwards compatible, then we can decode newer structs
	s1 v1{};
	EXPECT_EQ(packall::unpack(v1, v2_bytes), packall::status::ok);

	EXPECT_EQ(v1.w, v2.w);
	EXPECT_EQ(v1.x.a, v2.x.a);
	EXPECT_EQ(v1.x.b, v2.x.b);
	EXPECT_EQ(v1.y, v2.y);
}
