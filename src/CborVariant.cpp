#include "CborVariant.h"

CborVariant::CborVariant(CborBuffer& buffer, cn_cbor* raw) : buffer(buffer) {
  this->raw = raw;
}

CborVariant::CborVariant(CborBuffer& buffer, bool value) : buffer(buffer) {
  cn_cbor_errback err;

  raw = cn_cbor_int_create(value ? 1 : 0, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, const char* value) : buffer(buffer) {
  cn_cbor_errback err;

  raw = cn_cbor_string_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, double value) : buffer(buffer) {
  cn_cbor_errback err;

  raw = cn_cbor_double_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, float value) : buffer(buffer) {
  cn_cbor_errback err;

  raw = cn_cbor_float_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, int value) : buffer(buffer) {
  cn_cbor_errback err;

  raw = cn_cbor_int_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, long value) : buffer(buffer) {
  cn_cbor_errback err;

  raw = cn_cbor_int_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, unsigned int value) : buffer(buffer) {
  cn_cbor_errback err;

  raw = cn_cbor_int_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, CborObject& value) : buffer(buffer) {
  this->raw = value.raw;
}

CborVariant::CborVariant(CborBuffer& buffer, CborArray& value) : buffer(buffer) {
  this->raw = value.raw;
}

int CborVariant::length() {
  if (raw == 0) {
    return -1;
  }

  return raw->length;
}

bool CborVariant::isValid() {
  return raw != 0;
}

bool CborVariant::isBool() {
  return isValid() && (raw->type == CN_CBOR_TRUE || raw->type == CN_CBOR_FALSE);
}

bool CborVariant::isString() {
  return isValid() && (raw->type == CN_CBOR_BYTES || raw->type == CN_CBOR_TEXT);
}

bool CborVariant::isDouble() {
  return isValid() && raw->type == CN_CBOR_DOUBLE;
}

bool CborVariant::isFloat() {
  return isValid() && raw->type == CN_CBOR_FLOAT;
}

bool CborVariant::isInteger() {
  return isValid() && (raw->type == CN_CBOR_UINT || raw->type == CN_CBOR_INT);
}

bool CborVariant::isObject() {
  return isValid() && raw->type == CN_CBOR_MAP;
}

bool CborVariant::isArray() {
  return isValid() && raw->type == CN_CBOR_ARRAY;
}

bool CborVariant::asBool() {
  if (!isValid() || !isBool()) {
    return false;
  }

  return raw->type == CN_CBOR_TRUE;
}

const char* CborVariant::asString() {
  if (!isValid()) {
    return 0;
  }

  if (raw->type != CN_CBOR_BYTES && raw->type != CN_CBOR_TEXT) {
    return 0;
  }

  if (raw->v.str[raw->length] != 0) {
    char* tmp = (char*)buffer.alloc(raw->length + 1);

    for (int i = 0; i < raw->length; i++) {
      tmp[i] = raw->v.str[i];
    }

    return tmp;
  }

  return raw->v.str;
}

double CborVariant::asDouble() {
  if (!isValid()) {
    return NAN;
  }

  if (isDouble()) {
    return raw->v.dbl;
  }

  if (isFloat()) {
    return raw->v.f;
  }

  return NAN;
}

float CborVariant::asFloat() {
  if (!isValid()) {
    return NAN;
  }

  if (isDouble()) {
    return raw->v.dbl;
  }

  if (isFloat()) {
    return raw->v.f;
  }

  return NAN;
}

long CborVariant::asInteger() {
  if (!isValid()) {
    return 0;
  }

  if (raw->type == CN_CBOR_UINT) {
    return raw->v.uint;
  }

  if (raw->type == CN_CBOR_INT) {
    return raw->v.sint;
  }

  return 0;
}

CborObject CborVariant::asObject() {
  if (isObject()) {
    return CborObject(buffer, raw);
  }

  return CborObject(buffer);
}

CborArray CborVariant::asArray() {
  if (isArray()) {
    return CborArray(buffer, raw);
  }

  return CborArray(buffer);
}

size_t CborVariant::encode(uint8_t* data, size_t size) {
  return cn_cbor_encoder_write(data, 0, size, raw);
}
