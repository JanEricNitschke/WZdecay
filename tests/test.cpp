#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <math.h>

#include "../rotation.h"
#include "../threevector.h"

namespace WZdecay {
  TEST_CASE("Threevector - Crossproduct") {
    SECTION("Zero for same vector") {
      REQUIRE(Crossproduct(CThreeVector(1,0,0), CThreeVector(1,0,0)) == CThreeVector(0,0,0));
      REQUIRE(Crossproduct(CThreeVector(0,1,0), CThreeVector(0,1,0)) == CThreeVector(0,0,0));
      REQUIRE(Crossproduct(CThreeVector(0,0,1), CThreeVector(0,0,1)) == CThreeVector(0,0,0));
    }
    SECTION("unit vectors") {
      REQUIRE(Crossproduct(CThreeVector(1,0,0), CThreeVector(0,1,0)) == CThreeVector(0,0,1));
    }
    SECTION("Random vectors") {
      REQUIRE(Crossproduct(CThreeVector(2,4,3), CThreeVector(6,5,1)) == CThreeVector(-11,16,-14));
    }
  }

  TEST_CASE("Threevector - Unit") {
    SECTION("Same for unit vector") {
      REQUIRE(CThreeVector(1,0,0).Unit() == CThreeVector(1,0,0));
      REQUIRE(CThreeVector(0,1,0).Unit() == CThreeVector(0,1,0));
      REQUIRE(CThreeVector(0,0,1).Unit() == CThreeVector(0,0,1));
    }
    SECTION("Adjusts Length") {
      REQUIRE(CThreeVector(0,0,8).Unit() == CThreeVector(0,0,1));
      REQUIRE(CThreeVector(2,0,0).Unit() == CThreeVector(1,0,0));
    }
  }

  TEST_CASE("Threevector - combination Crossproduct and Unit") {
    REQUIRE(Crossproduct(CThreeVector(2,0,0), CThreeVector(0,4,0)).Unit() == CThreeVector(0,0,1));
    REQUIRE(Crossproduct(CThreeVector(2,0,0).Unit(), CThreeVector(0,4,0).Unit()) == CThreeVector(0,0,1));
  }

  TEST_CASE( "Rotations of unit axis") {
    CRotation rot90DegAroundZAxis(CThreeVector(1,0,0), CThreeVector(0,1,0));
    SECTION( "Is Constructor correct?" ) {
      
      const CThreeVector v3Axis = rot90DegAroundZAxis.Axis();
      REQUIRE( v3Axis == CThreeVector(0,0,1) );
      REQUIRE( rot90DegAroundZAxis.Rotate(CThreeVector(1,0,0)) == CThreeVector(0,1,0));
      REQUIRE( rot90DegAroundZAxis.Rotate(CThreeVector(0,0,1)) == CThreeVector(0,0,1));
    }
  }

  TEST_CASE( "Rotations of axis") {
    CRotation rot90DegAroundZAxis(CThreeVector(1,0,0), CThreeVector(0,2,0));
    SECTION( "Is Constructor correct?" ) {
      
      const CThreeVector v3Axis = rot90DegAroundZAxis.Axis();
      REQUIRE( v3Axis == CThreeVector(0,0,1) );
      REQUIRE( rot90DegAroundZAxis.Rotate(CThreeVector(1,0,0)) == CThreeVector(0,1,0));
      REQUIRE( rot90DegAroundZAxis.Rotate(CThreeVector(0,0,1)) == CThreeVector(0,0,1));
    }
  }

  TEST_CASE("Arbitrary Rotation I") {
    CRotation rot90DegAroundZAxis(CThreeVector(0,0,1), CThreeVector(4.0/5,3.0/5,0));
    REQUIRE( CThreeVector(rot90DegAroundZAxis.Axis()) == CThreeVector(-3.0/5,4.0/5,0) );
    REQUIRE_FALSE( CThreeVector(0,0,1) == CThreeVector(0,0,1.1));
    REQUIRE( rot90DegAroundZAxis.Rotate(CThreeVector(0,0,1)) == CThreeVector(4.0/5,3.0/5,0));
  }

  TEST_CASE("Arbitrary Rotation II") {
    CRotation rot90DegAroundZAxis(CThreeVector(0,0,1), CThreeVector(0.07,-0.15,0.98));
    SECTION(" Built correctly?"){
      REQUIRE( rot90DegAroundZAxis.Axis() == CThreeVector(0.906183,0.422885,0));
      double angle = rot90DegAroundZAxis.Angle();
      REQUIRE( fabs(angle - 0.1673) < 0.001);
    }
    REQUIRE( rot90DegAroundZAxis.Rotate(CThreeVector(0,0,1)) == CThreeVector(0.07043,-0.15092,0.98603));
  }
}; // end of namespace WZdecay