#include <stdint.h>
#include <stdio.h>
typedef enum {
  PlusZero      = 1,
  MinusZero     = 2,
  PlusInf       = 3,
  MinusInf      = 4,
  PlusRegular   = 5,
  MinusRegular  = 6,
  PlusDenormal  = 7,
  MinusDenormal = 8,
  SignalingNaN  = 9,
  QuietNaN      = 10
} float_class_t;

extern float_class_t classify(double *value_ptr);

uint64_t* get_int(double *value_ptr) {
  void *void_ptr = value_ptr;
  uint64_t *int_ptr = void_ptr;
  return int_ptr;
}

uint64_t get_mantis(double *value_ptr) {
  uint64_t *p = get_int(value_ptr);
  uint64_t mask = 0xfffffffffffff;
  return *p & mask;
}

uint64_t get_exponent(double *value_ptr) {
  uint64_t *p = get_int(value_ptr);
  uint64_t mask = 0x7ff0000000000000;
  uint64_t result = (*p & mask) >> 52;
  return result;
}

int get_sign(double *value_ptr) {
  uint64_t *p = get_int(value_ptr);
  double plus_zero = +0.0;
  double minus_zero = -0.0;
  uint64_t mask = *get_int(&plus_zero) ^ *get_int(&minus_zero);
  return ((*p & mask) == mask);
}

float_class_t classify(double *value_ptr) {
  uint64_t mantis = get_mantis(value_ptr);
  uint64_t exponent = get_exponent(value_ptr);
  uint64_t sign = get_sign(value_ptr);
  float_class_t res = PlusZero;
  if (exponent == 0 && mantis == 0) {
    if (sign == 0) {
      res = PlusZero;
    } else {
      res = MinusZero;
    }
  } else if (exponent == 0x7FF && mantis == 0) {
    if (sign == 0) {
      res = PlusInf;
    } else {
      res = MinusInf;
    }
  } else if (exponent == 0x7FF && (!(mantis >> 51) & 1)) {
    res = SignalingNaN;
  } else if (exponent == 0x7FF && ((mantis >> 51) & 1)) {
    res = QuietNaN;
  } else if (exponent == 0 && mantis != 0) {
    if (sign == 0) {
      res = PlusDenormal;
    } else {
      res = MinusDenormal;
    }
  } else {
    if (sign == 0) {
      res = PlusRegular;
    } else {
      res = MinusRegular;
    }
  }
  return res;
}

int main(){
  double a = 5.87747175411144e-39;
  printf("%d", classify(&a));
}