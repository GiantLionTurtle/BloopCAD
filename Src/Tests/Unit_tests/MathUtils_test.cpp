
#include <catch2/catch.hpp>

#include <Utils/Maths_util.hpp>

TEST_CASE("Map function is tested", "[maths]")
{
	REQUIRE(map(0.5, 0.0, 1.0, -10.0, 10.0) == 0.0);
	REQUIRE(map(0.0, 0.0, 1.0, -10.0, 10.0) == -10.0);
	REQUIRE(map(7.0, 0.0, 10.0, 5.0, 25.0) == 19.0);
	REQUIRE(map(-5.0, -10.0, 10.0, -10.0, 10.0) == -5.0);
}