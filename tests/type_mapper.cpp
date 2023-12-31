#include <boost/hana/fwd/integral_constant.hpp>
#include <boost/hana/fwd/ordering.hpp>
#include <boost/hana/fwd/type.hpp>
#include <boost/mp11/algorithm.hpp>
#include <catch2/catch_test_macros.hpp>
#include <boost/hana.hpp>
#include "TypeMapper.hpp"
#include "Types.hpp"

struct Velocity {
  int x;
  int y;
};

struct Acceleration {
  int x;
  int y;
};

TEST_CASE("Type mapping", "[mapper]") {
  auto mapper = TypeMapper<uint32_t>();
  ComponentId vel_id = mapper.put<Velocity>(10);
  mapper.put<Acceleration>(7);
  REQUIRE(mapper.find<Acceleration>()->second != mapper.find<Velocity>()->second);
  REQUIRE(mapper.find<Velocity>()->second == 10);
  REQUIRE(mapper.find<Acceleration>()->second == 7);
  REQUIRE(mapper.find<Velocity>()->first == vel_id);
  REQUIRE(mapper.find<Acceleration>()->second == mapper.find<Acceleration>()->second);
}

TEST_CASE("Ovewrite value", "[ovewrite]") {
  auto mapper = TypeMapper<uint32_t>();
  mapper.put<Acceleration>(7);
  mapper.put<Acceleration>(8);
  REQUIRE(mapper.find<Acceleration>()->second == 8);
}

TEST_CASE("Ovewrite composite type", "[ovewrite_composite]") {
  auto mapper = TypeMapper<uint32_t>();
  mapper.put<Acceleration, Velocity>(7);
  mapper.put<Acceleration, Velocity>(8);
  REQUIRE(mapper.find<Acceleration, Velocity>()->second == 8);
  mapper.put<Velocity, Acceleration>(9);
  REQUIRE(mapper.find<Velocity, Acceleration>()->second == 9);
  //REQUIRE(mapper.find<Acceleration, Velocity>()->second == 9);
}

TEST_CASE("Multiple type mapping", "[multiple_mapper]") {
  auto mapper = TypeMapper<uint32_t>();
  mapper.put<Acceleration, Velocity>(5);
  mapper.put<Velocity, Acceleration>(11);
  //REQUIRE(mapper.find<Acceleration, Velocity>()->second == 11);
  //REQUIRE(mapper.find<Velocity, Acceleration>()->second == 11);
  //REQUIRE(mapper.find<Velocity, Acceleration>()->second == mapper.find<Acceleration, Velocity>()->second);
}

TEST_CASE("Erasing from type mapping", "[erase]") {
  TypeMapper<uint32_t> mapper {};
  uint64_t value_to_insert = 5;
  uint64_t id = mapper.put<Acceleration, Velocity>(5);
  uint32_t remove_value = mapper.remove<Acceleration, Velocity>();
  REQUIRE(5 == remove_value);
}

TEST_CASE("Check contains", "[contains]") {
  TypeMapper<uint32_t> mapper {};
  uint64_t id = mapper.put<Acceleration>(2);
  REQUIRE( mapper.contains<Velocity>() == false );
  REQUIRE( mapper.contains<Acceleration>() == true );
  mapper.remove<Acceleration>();
  REQUIRE( mapper.contains<Acceleration>() == false );
}
