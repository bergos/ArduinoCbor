/*
   CBOR float-32 vs double-64 test
   --------------------------------
   • Needs the float overload you added earlier:
       CborObject::set(key, float)  /  CborVariant::asFloat()
   • Uses the existing cn_cbor_double_create() for the 64-bit value.

   Generates:
     - random int          → key "I"
     - random float (32-bit)→ key "F"
     - random bool         → key "B"
     - random double (64-bit)→ key "D"

   Then shows raw CBOR bytes and decodes them back.
*/

#include <Arduino.h>
#include <ArduinoCbor.h>
#include <cn-cbor/cn-cbor.h>          // for cn_cbor_double_create()

constexpr size_t BUF_SIZE = 96;

void loopOnce()
{
  long    rndInt   = random(-1'000'000L, 1'000'000L);
  float   rndFlt   = random(-5000, 5000) / 37.89f;
  bool    rndBool  = random(0, 2);
  double  rndDbl   = random(-10000, 10000) / 13.37;   // 64-bit

  Serial.println(F("\n--- new sample --------------------------------"));
  Serial.print(F("original int     : ")); Serial.println(rndInt);
  Serial.print(F("original float   : ")); Serial.println(rndFlt, 6);
  Serial.print(F("original bool    : ")); Serial.println(rndBool);
  Serial.print(F("original double  : ")); Serial.println(rndDbl, 10);

  // ---------------- encode ----------------
  CborBuffer outBuf(BUF_SIZE);
  CborObject root(outBuf);

  root.set("I", static_cast<CBOR_INT_T>(rndInt));   // integer  (variable bytes)
  root.set("F", rndFlt);                            // float-32 (5 B)

  root.set("B", static_cast<CBOR_INT_T>(rndBool));  // bool as 0 / 1 (1 B)

  // --- double via the low-level call ---
  cn_cbor_errback err;
  cn_cbor* dbl = cn_cbor_double_create(rndDbl, &outBuf.context, &err);
  root.set("D", CborVariant(outBuf, dbl));          // double-64 (9 B)

  // encode to raw buffer
  uint8_t encoded[BUF_SIZE];
  size_t  len = root.encode(encoded, sizeof(encoded));

  // ---------------- dump hex -------------
  Serial.print(F("encoded length   : "));
  Serial.println(len);
  Serial.print(F("raw bytes        : "));
  for (size_t i = 0; i < len; ++i) {
    if (encoded[i] < 0x10) Serial.print('0');
    Serial.print(encoded[i], HEX);
    Serial.print(' ');
  }
  Serial.println();

  // ---------------- decode ---------------
  CborBuffer  inBuf(len);
  CborVariant decoded = inBuf.decode(encoded, len);

  long   outInt  = 0;
  float  outFlt  = 0.0f;
  bool   outBool = false;
  double outDbl  = 0.0;

  if (decoded.isObject()) {
    CborObject obj = decoded.asObject();
    outInt  = obj.get("I").asInteger();
    outFlt  = obj.get("F").asFloat();
    outBool = obj.get("B").asInteger() != 0;
    outDbl  = obj.get("D").asDouble();      // uses new helper
  }

  // --------------- report ---------------
  Serial.print(F("decoded int       : ")); Serial.println(outInt);
  Serial.print(F("decoded float     : ")); Serial.println(outFlt, 6);
  Serial.print(F("decoded bool      : ")); Serial.println(outBool);
  Serial.print(F("decoded double    : ")); Serial.println(outDbl, 10);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  randomSeed(analogRead(A0));
}

void loop() {
  loopOnce();
  delay(5000);
}
