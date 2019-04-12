#include <ArduinoCbor.h>

void setup() {
  Serial.begin(115200);

  while(!Serial) {
    delay(10);
  }
}


void loop() {
  CborBuffer buffer(200);

  CborObject object(buffer);
  object.set("string", "value");
  object.set("integer", -1234);

  CborObject child(buffer);
  child.set("key", "value");
  object.set("object", child);

  CborArray array(buffer);
  array.add(4321);
  object.set("array", array);

  uint8_t encoded[200];
  size_t len = object.encode(encoded, sizeof(encoded));
  Serial.print("encoded length: ");
  Serial.println(len);

  CborVariant decoded = buffer.decode(encoded, len);
  CborObject obj = decoded.asObject();

  Serial.print("string value: ");
  Serial.println(obj.get("string").asString());

  Serial.print("integer value: ");
  Serial.println(obj.get("integer").asInteger());

  Serial.print("child string value: ");
  Serial.println(obj.get("object").asObject().get("key").asString());

  Serial.print("array integer value: ");
  Serial.println(obj.get("array").asArray().get(0).asInteger());

  delay(1000);
}
