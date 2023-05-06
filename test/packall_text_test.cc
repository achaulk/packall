#include "packall_test.h"
#include "../include/packall/packall_text.h"

TEST(packall_text, Config)
{
	static const char kStr[] = R"({
	device = "/dev/video0",
	resolution = {640, 480},
	K_matrix = {223.28249888247538, 0.0, 152.30570853111396, 0.0, 223.8756535707556, 124.5606000035353, 0.0, 0.0, 1.0},
	distortion_coeffients = {-0.44158343539568284, 0.23861463831967872, 0.0016338407443826572, 0.0034950038632981604, -0.05239245892096022},
	parameters = {start_server = true, max_depth = 5, model_path = "foo/bar.pt"},
})";
	Config c{"/dev/video0", {640, 480},
	    {223.28249888247538, 0.0, 152.30570853111396, 0.0, 223.8756535707556, 124.5606000035353, 0.0, 0.0, 1.0},
	    {-0.44158343539568284, 0.23861463831967872, 0.0016338407443826572, 0.0034950038632981604, -0.05239245892096022},
	    {{"start_server", bool{true}}, {"max_depth", uint16_t{5}}, {"model_path", std::string{"foo/bar.pt"}}}};

	// Check that parsing the known good version matches hardcoding
	Config new_c;
	EXPECT_EQ(packall::parse(new_c, kStr), packall::status::ok);
	EXPECT_EQ(new_c.device, c.device);
	EXPECT_EQ(new_c.resolution, c.resolution);
	EXPECT_EQ(new_c.K_matrix, c.K_matrix);
	EXPECT_EQ(new_c.distortion_coeffients, c.distortion_coeffients);
	EXPECT_EQ(new_c.parameters, c.parameters);

	std::string text;
	packall::format(c, text);

	printf("%s\n", packall::prettyprint(text).c_str());

	// Check that parsing the formatted version works correctly
	Config text_c;
	EXPECT_EQ(packall::parse(text_c, text), packall::status::ok);
	EXPECT_EQ(text_c.device, c.device);
	EXPECT_EQ(text_c.resolution, c.resolution);
	EXPECT_EQ(text_c.K_matrix, c.K_matrix);
	EXPECT_EQ(text_c.distortion_coeffients, c.distortion_coeffients);
	EXPECT_EQ(text_c.parameters, c.parameters);
}

TEST(packall_text, no_members)
{
	struct point
	{
		float x, y, z;
		bool operator==(const point&) const = default;
	} pt{1.1f, 2.0, 3.0};

	std::string text;
	packall::format(pt, text);

	printf("%s\n", packall::prettyprint(text).c_str());

	point pt2{};
	packall::parse(pt2, text);

	EXPECT_EQ(pt2, pt);
}
