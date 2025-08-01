#ifndef CBOR_OBJECT_H_
#define CBOR_OBJECT_H_

#include "ArduinoCbor.h"

class CborObject {
 friend class CborVariant;

 public:
  CborObject(CborBuffer& buffer, cn_cbor* raw=0);
  ~CborObject();

  CborVariant get(const char* key);

  void set(const char* key, CborVariant value);
  void set(const char* key, bool  value);
  void set(const char* key, const char* value);
  void set(const char* key, double value);
  void set(const char* key, float value);
  void set(const char* key, int value);
  void set(const char* key, long value);
  void set(const char* key, unsigned int value);
  void set(const char* key, CborObject value);
  void set(const char* key, CborArray value);

  size_t encode(uint8_t* data, size_t size);

 protected:
  CborBuffer& buffer;
  cn_cbor* raw;
};

#endif // CBOR_OBJECT_H_
