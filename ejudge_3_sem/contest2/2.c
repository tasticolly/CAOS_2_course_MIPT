#include <stdint.h>
typedef enum {
  PlusZero = 0x00,
  MinusZero = 0x01,
  PlusInf = 0xF0,
  MinusInf = 0xF1,
  PlusRegular = 0x10,
  MinusRegular = 0x11,
  PlusDenormal = 0x20,
  MinusDenormal = 0x21,
  SignalingNaN = 0x30,
  QuietNaN = 0x31
} float_class_t;

const int BITS_MANTISS = 52;
const int BITS_EXPONENT = 11;

extern float_class_t
classify(double* value_ptr) {
  union {
    double d;
    uint64_t ui;
  } bits;

  bits.d = *value_ptr;
  uint64_t number = bits.ui;

  uint64_t sign = number >> (BITS_MANTISS + BITS_EXPONENT);
  uint64_t mantissa = number & (((uint64_t) 1 << BITS_MANTISS) - 1);
  uint64_t exponent = (number >> BITS_MANTISS) & ((1 << BITS_EXPONENT) - 1);
  float_class_t ans;
  if (!number) {
    ans = PlusZero;
  } else if (exponent == 0) {
    if (sign) {
      if (!mantissa) {
        ans = MinusZero;
      } else {
        ans = MinusDenormal;
      }
    } else {
      ans = PlusDenormal;
    }
  } else if (exponent + 1 == (1 << BITS_EXPONENT)) {
    if (!mantissa) {
      if (sign) {
        ans = MinusInf;
      } else {
        ans = PlusInf;
      }
    } else if ((mantissa >> (BITS_MANTISS - 1)) == 1) {
      ans = QuietNaN;
    } else {
      ans = SignalingNaN;
    }
  } else if (sign) {
    ans = MinusRegular;
  } else {
    ans = PlusRegular;
  }
  return ans;
}
