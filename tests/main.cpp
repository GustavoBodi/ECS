#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include <iostream>

int main(int argc, char **argv) {
  int result = Catch::Session().run(argc, argv);
}
