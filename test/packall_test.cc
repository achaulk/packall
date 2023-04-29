#include "packall_test.h"

#include <limits>

static_assert(packall::detail::has_predecode_info<Config>::value);
static_assert(packall::detail::has_predecode_info<packall::deprecated<Config>>::value);
static_assert(packall::detail::has_predecode_info<std::tuple<int, bool>>::value);
static_assert(packall::detail::has_predecode_info<packall::deprecated<std::tuple<int, bool>>>::value);

TEST(packall, Config)
{

	Config c{"/dev/video0", {640, 480},
	    {223.28249888247538, 0.0, 152.30570853111396, 0.0, 223.8756535707556, 124.5606000035353, 0.0, 0.0, 1.0},
	    {-0.44158343539568284, 0.23861463831967872, 0.0016338407443826572, 0.0034950038632981604, -0.05239245892096022},
	    {{"start_server", bool{true}}, {"max_depth", uint16_t{5}}, {"model_path", std::string{"foo/bar.pt"}}}};

	std::vector<uint8_t> bytes;
	packall::pack(c, bytes);

	Config new_c;
	EXPECT_EQ(packall::unpack(new_c, bytes), packall::status::ok);
	EXPECT_EQ(new_c.device, c.device);
	EXPECT_EQ(new_c.resolution, c.resolution);
	EXPECT_EQ(new_c.K_matrix, c.K_matrix);
	EXPECT_EQ(new_c.distortion_coeffients, c.distortion_coeffients);
	EXPECT_EQ(new_c.parameters, c.parameters);
}

struct optional_limits
{
	static constexpr size_t Arity = 2;

	int x;
	std::optional<int> y;
};
static_assert(packall::detail::aggregate_arity<optional_limits>::value == 1);
static_assert(packall::detail::aggregate_arity_calc<optional_limits>::Arity == 2);

TEST(packall, explicit_arity)
{
	optional_limits a{1, 2};
	optional_limits b;

	std::vector<uint8_t> bytes;
	packall::pack(a, bytes);

	EXPECT_EQ(packall::unpack(b, bytes), packall::status::ok);
	EXPECT_EQ(b.x, a.x);
	EXPECT_EQ(b.y, a.y);
}

struct custom_struct
{
	template<typename C>
	void pack(C& c)
	{
		c.write(v);
	}
	template<typename C>
	void unpack(C& c)
	{
		c.read(v);
	}

	bool operator==(const custom_struct& o) const
	{
		return v == o.v;
	}

	uint32_t v;
};
TEST(packall, custom_struct)
{
	custom_struct c;
	c.v = 0x11223344;
	roundtrip_T(c);
}

struct custom_buffer
{
	std::vector<uint8_t> buf;
};
template<>
struct packall::container_wrapper<custom_buffer>
{
	container_wrapper(custom_buffer& o) : o(o) {}

	void write_u8(uint8_t v)
	{
		o.buf.push_back(v);
	}
	void write_bytes(const void *v, size_t sz)
	{
		auto ptr = static_cast<const uint8_t *>(v);
		for(size_t i = 0; i < sz; i++) o.buf.push_back(ptr[i]);
	}

	uint8_t read_u8()
	{
		if(rd == o.buf.size())
			throw status::data_underrun;
		return o.buf[rd++];
	}

	void read_bytes(void *v, size_t sz)
	{
		if(rd + sz > o.buf.size())
			throw status::data_underrun;
		auto ptr = static_cast<uint8_t *>(v);
		memcpy(v, &o.buf[rd], sz);
		rd += sz;
	}

	bool end() const
	{
		return rd == o.buf.size();
	}

	bool ok() const
	{
		return true;
	}

	size_t enter()
	{
		uint32_t v;
		size_t at = rd;
		read_bytes(&v, 4);
		rd += 4;
		return at + v;
	}
	void leave(size_t at)
	{
		rd = at;
	}

	custom_buffer& o;
	size_t rd = 0;
};
TEST(packall, custom_buffer)
{
	Config c{"/dev/video0", {640, 480},
	    {223.28249888247538, 0.0, 152.30570853111396, 0.0, 223.8756535707556, 124.5606000035353, 0.0, 0.0, 1.0},
	    {-0.44158343539568284, 0.23861463831967872, 0.0016338407443826572, 0.0034950038632981604, -0.05239245892096022},
	    {{"start_server", bool{true}}, {"max_depth", uint16_t{5}}, {"model_path", std::string{"foo/bar.pt"}}}};

	custom_buffer bytes;
	packall::pack(c, bytes);

	Config new_c;
	EXPECT_EQ(packall::unpack(new_c, bytes), packall::status::ok);
	EXPECT_EQ(new_c.device, c.device);
	EXPECT_EQ(new_c.resolution, c.resolution);
	EXPECT_EQ(new_c.K_matrix, c.K_matrix);
	EXPECT_EQ(new_c.distortion_coeffients, c.distortion_coeffients);
	EXPECT_EQ(new_c.parameters, c.parameters);
}

struct some_struct
{
};
TEST(packall, typenames)
{
	EXPECT_EQ(packall::get_type_name<int>(), "int");
	EXPECT_EQ(packall::get_type_name<some_struct>(), "some_struct");
}

template<typename T>
void test_limits()
{
	roundtrip_T(std::numeric_limits<T>::lowest());
	roundtrip_T(std::numeric_limits<T>::max());
}

TEST(packall, limits)
{
	test_limits<int8_t>();
	test_limits<int16_t>();
	test_limits<int32_t>();
	test_limits<int64_t>();
	test_limits<uint8_t>();
	test_limits<uint16_t>();
	test_limits<uint32_t>();
	test_limits<uint64_t>();
}
