//
//    FILE: unit_test_001.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 2021-01-01
// PURPOSE: unit tests for the ML8511 UV sensor
//          https://github.com/RobTillaart/ML8511
//          https://github.com/Arduino-CI/arduino_ci/blob/master/REFERENCE.md
//

// supported assertions
// ----------------------------
// assertEqual(expected, actual);               // a == b
// assertNotEqual(unwanted, actual);            // a != b
// assertComparativeEquivalent(expected, actual);    // abs(a - b) == 0 or (!(a > b) && !(a < b))
// assertComparativeNotEquivalent(unwanted, actual); // abs(a - b) > 0  or ((a > b) || (a < b))
// assertLess(upperBound, actual);              // a < b
// assertMore(lowerBound, actual);              // a > b
// assertLessOrEqual(upperBound, actual);       // a <= b
// assertMoreOrEqual(lowerBound, actual);       // a >= b
// assertTrue(actual);
// assertFalse(actual);
// assertNull(actual);

// // special cases for floats
// assertEqualFloat(expected, actual, epsilon);    // fabs(a - b) <= epsilon
// assertNotEqualFloat(unwanted, actual, epsilon); // fabs(a - b) >= epsilon
// assertInfinity(actual);                         // isinf(a)
// assertNotInfinity(actual);                      // !isinf(a)
// assertNAN(arg);                                 // isnan(a)
// assertNotNAN(arg);                              // !isnan(a)

#include <ArduinoUnitTests.h>


#include "Arduino.h"
#include "ML8511.h"



unittest_setup()
{
}


unittest_teardown()
{
}

/*
unittest(test_new_operator)
{
  assertEqualINF(exp(800));
  assertEqualINF(0.0/0.0);
  assertEqualINF(42);
  
  assertEqualNAN(INFINITY - INFINITY);
  assertEqualNAN(0.0/0.0);
  assertEqualNAN(42);
}
*/


#define ANALOGPIN         0

unittest(test_constructor)
{
  fprintf(stderr, "VERSION: %s\n", ML8511_LIB_VERSION);

  ML8511 light(ANALOGPIN);

  assertEqualFloat(5.0/1023, light.getVoltsPerStep(), 0.0001);
  light.setVoltsPerStep(3.3, 4095);
  assertEqualFloat(3.3/4095, light.getVoltsPerStep(), 0.0001);

  reset()
  assertEqualFloat(5.0/1023, light.getVoltsPerStep(), 0.0001);


  assertTrue(light.isEnabled());
  light.disable();
  assertFalse(light.isEnabled());
  light.enable();
  assertTrue(light.isEnabled());
}


unittest(test_getUV)
{
  ML8511 light(ANALOGPIN);

  assertEqualFloat(0, light.getUV(), 0.0001);
  assertEqualFloat(0, light.getUV(LOW), 0.0001);

  light.enable();
  assertEqualFloat(0, light.getUV(HIGH), 0.0001);

}


unittest(test_setDUVfactor)
{
  ML8511 light(ANALOGPIN);

  light.enable();

  for (float factor = 0.10; factor < 2.01; factor += 0.1)
  {
    light.setDUVfactor(factor);
    assertEqualFloat(factor, getDUVfactor(), 0.0001);
  }

  fprintf(stderr, "\nOUT OF RANGE\n");
  assertTrue(light.setDUVfactor(0.577));
  assertEqualFloat(0.577, getDUVfactor(), 0.0001);

  assertFalse(light.setDUVfactor(0));
  assertEqualFloat(0.577, getDUVfactor(), 0.0001);

  assertFalse(light.setDUVfactor(-1.0));
  assertEqualFloat(0.577, getDUVfactor(), 0.0001);
  
  reset();
  assertEqualFloat(1.61, getDUVfactor(), 0.0001);
}


unittest(test_estimateDUVindex)
{
  ML8511 light(ANALOGPIN);

  light.enable();

  for (float mW = 0; mW < 10; mW += 0.1)
  {
    fprintf(stderr, "%f\t", mW);
    fprintf(stderr, "%f\n", light.estimateDUVindex(mW));
    assertEqualFloat(1.61 * mW, light.estimateDUVindex(mW), 0.0001);
  }

  light.setDUVfactor(1.0)
  for (float mW = 0; mW < 10; mW += 0.1)
  {
    fprintf(stderr, "%f\t", mW);
    fprintf(stderr, "%f\n", light.estimateDUVindex(mW));
    assertEqualFloat(1.0 * mW, light.estimateDUVindex(mW), 0.0001);
  }

}


unittest_main()


// --------
