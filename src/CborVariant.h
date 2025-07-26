#ifndef CBOR_VARIANT_H_
#define CBOR_VARIANT_H_

#include "ArduinoCbor.h"

class CborVariant {
 friend class CborArray;
 friend class CborObject;

 public:
  CborVariant(CborBuffer& buffer, cn_cbor* raw);
  CborVariant(CborBuffer& buffer, bool value);
  CborVariant(CborBuffer& buffer, const char* value);
  CborVariant(CborBuffer& buffer, double value);
  CborVariant(CborBuffer& buffer, float value);
  CborVariant(CborBuffer& buffer, int value);
  CborVariant(CborBuffer& buffer, long value);
  CborVariant(CborBuffer& buffer, unsigned int value);
  CborVariant(CborBuffer& buffer, CborObject& value);
  CborVariant(CborBuffer& buffer, CborArray& value);

  int length();

  bool isValid();
  bool isBool();
  bool isString();
  bool isDouble();
  bool isFloat();
  bool isInteger();
  bool isObject();
  bool isArray();

  bool asBool();
  const char* asString();
  double asDouble();
  float asFloat();
  long asInteger();
  CborObject asObject();
  CborArray asArray();

  size_t encode(uint8_t* data, size_t size);

 protected:
  CborBuffer& buffer;
  cn_cbor* raw;
};

#endif // CBOR_VARIANT_H_
