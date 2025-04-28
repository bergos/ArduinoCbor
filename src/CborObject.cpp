#include "CborObject.h"

CborObject::CborObject(CborBuffer& buffer, cn_cbor* raw) : buffer(buffer) {
  this->raw = raw;

  if (raw == 0) {
    cn_cbor_errback err;

    this->raw = cn_cbor_map_create(&buffer.context, &err);
  }
}

CborVariant CborObject::get(const char* key) {
  return CborVariant(buffer, cn_cbor_mapget_string(raw, key));
}

void CborObject::set(const char* key, CborVariant value) {
  cn_cbor_errback err;

  cn_cbor_mapput_string(raw, key, value.raw, &buffer.context, &err);
}

void CborObject::set(const char* key, const char* value) {
  set(key, CborVariant(buffer, value));
}

void CborObject::set(const char* key, CBOR_INT_T value) {
  set(key, CborVariant(buffer, value));
}

void CborObject::set(const char* key, float value) {
  cn_cbor_errback err;

  // build a 32-bit float node (major-type 7, AI 26 → 0xFA)
  cn_cbor* f32 = cn_cbor_float_create(value, &buffer.context, &err);
  cn_cbor_mapput_string(raw, key, f32, &buffer.context, &err);
}

void CborObject::set(const char* key, bool value) {
  cn_cbor_errback err;

  // Build a 0-or-1 unsigned-int node (initial byte 0x00 or 0x01)
  cn_cbor* n = cn_cbor_int_create(value ? 1 : 0, &buffer.context, &err);

  cn_cbor_mapput_string(raw, key, n, &buffer.context, &err);
}



void CborObject::set(const char* key, CborObject value) {
  set(key, CborVariant(buffer, value));
}

void CborObject::set(const char* key, CborArray value) {
  set(key, CborVariant(buffer, value));
}

size_t CborObject::encode(uint8_t* data, size_t size) {
  return cn_cbor_encoder_write(data, 0, size, raw);
}
