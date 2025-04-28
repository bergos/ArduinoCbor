# ArduinoCbor &nbsp;v0.0.2  
**Floats • Doubles • Bools • Fast Buffer Reset**

This patch upgrades Benoît Blanchon’s original **ArduinoCbor** wrapper with four extra, fully-backward-compatible features.

| New feature | CBOR encoding | Wire cost | Public API |
|-------------|---------------|-----------|------------|
| **Float-32** | `0xFA … 4-byte payload` | **5 bytes** | `set(key, float)` / `asFloat()` |
| **Double-64** | `0xFB … 8-byte payload` | **9 bytes** | `set(key, double)` / `asDouble()` |
| **Bool** | unsigned-int `1` / `0` | **1 byte** | `set(key, bool)` / `asBool()` |
| **Arena reset** | — | O(1) | `CborBuffer::clear()` |

> ✅ Sketches written for v0.0.1 still compile unchanged.

---

## Quick start

```cpp
#include <ArduinoCbor.h>

CborBuffer arena(128);          // single reusable arena
CborObject root(arena);

/* ------------ encode ------------ */
root.set("temp",   23.75f);     // 5 B float-32
root.set("energy", 1.234567e6); // 9 B double-64
root.set("ready",  true);       // 1 B bool
root.set("cnt",    42);         // variable-length int

uint8_t packet[128];
size_t  len = root.encode(packet, sizeof(packet));

/* ------------ decode ------------ */
CborBuffer inBuf(len);
CborObject obj = inBuf.decode(packet, len).asObject();

float  temp   = obj.get("temp").asFloat();
double energy = obj.get("energy").asDouble();
bool   ready  = obj.get("ready").asBool();
int    cnt    = obj.get("cnt").asInteger();

/* ------------ reuse arena ------- */
arena.clear();                  // zero-alloc reset
new (&root) CborObject(arena);  // placement-new

```

## API additions

```cpp
// ---------- CborObject.h ----------
void set(const char* key, float  value);   // 0xFA
void set(const char* key, double value);   // 0xFB
void set(const char* key, bool   value);   // 0 or 1

// ---------- CborVariant.h ----------
bool   isFloat();   float  asFloat();
bool   isDouble();  double asDouble();
bool   isBool();    bool   asBool();

// ---------- CborBuffer.h ----------
void clear();       // rewind bump allocator
Implementation adds ~120 LOC and does not modify existing behaviour.

```
