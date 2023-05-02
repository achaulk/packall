C++20 serialization of arbitrary structs into byte arrays

* `packall` is a header-only serialization library for C++20
* **No external dependencies, zero to minimal source changes required.**
* Builtin serialization to both binary and text.
* Extensible, add new containers and custom type serialization easily
* Supports most custom containers out of the box using concepts

Inspired by [alpaca](https://github.com/p-ranav/alpaca)

### Basic usage
```cpp
#include "packall/packall.h"

struct Config {
  std::string device;
  std::pair<unsigned, unsigned> resolution;
  std::array<double, 9> K_matrix;
  std::vector<float> distortion_coeffients;
  std::map<std::string, std::variant<uint16_t, std::string, bool>> parameters;
};

// Construct the object
Config c{"/dev/video0", {640, 480}, 
	 {223.28249888247538, 0.0, 152.30570853111396,
	  0.0, 223.8756535707556, 124.5606000035353,
	  0.0, 0.0, 1.0},
	 {-0.44158343539568284, 0.23861463831967872, 0.0016338407443826572,
	  0.0034950038632981604, -0.05239245892096022},
	 {{"start_server", bool{true}},
	  {"max_depth", uint16_t{5}},
	  {"model_path", std::string{"foo/bar.pt"}}}};

// Serialize
std::vector<uint8_t> bytes;
packall::pack(c, bytes);

// Deserialize
Config new_c;
if(packall::unpack(new_c, bytes) == packall::status::ok) {
	// ok!
}
```

### API

`packall::pack(object, container)` `packall::pack<options::*>(object, container)`
Packs `object` into `container`. `container` can be any type that looks like `vector` or an `ostream`

`packall::unpack(object, container)` `packall::unpack<options::*>(object, container)`
Unpacks `container` into `object` and returns a status code. `container` can be a vector, span or `istream`


`packall::parse(object, string)`
Parse the given `string` into `object`. `string` must be a string_view

`packall::format(object, string)`
Convert `object` to a text representation.


### Type system

#### Special types
Structs, tuple, variant and all containers are handled in a special way that allows for direct interaction with each other and certain other features like optional and deprecation.

#### Structs & Tuples
Structs, which must be [aggregates](https://en.cppreference.com/w/cpp/types/is_aggregate) are the primary user type.

Structs can contain any other type, including other structs. Structs are normally a logical boundary like messages in protocol buffers. Structs can be efficiently [deprecated](#modifiers) whereas multiple primitive fields cannot be.

Structs operate in one of three compatiblity modes
* By default older structs can be loaded by new code
* Structs can be marked immutable for a more efficient encoding that disallows changes.
* Structs can be marked backwards compatible, to allow older code to load newer data.

`std::tuple<T, U, V, ...>` behaves much like a struct containing those values in sequence, except that `omit<>` **does** change the emitted # of fields for decode purposes and so may break compatibility. Additionally tuples cannot operate in fully backwards compatible decode mode like a struct can.

#### Containers
Concepts are used to not require specific implementations for every container type.
Three containers are implemented
* listlikes must implement `resize(size_t)` and provide `value_type`.
* setlikes must implement `emplace(T&&)` and provide `key_type`.
* maplikes must implement `emplace(T&&, K&&)` and must permit unpacking via `auto& [k, v] : map` and must provide `key_type` and  `mapped_type`.

Additionally, all containers must implement member `size()`, `begin()` & `end()` and typical iterator semantics of incrementing and dereferencing. Any container that satisfies one of these will be automagically supported.

In addition, both `std::array<T, N>` and `T[N]` are supported as if they were fixed size containers. It is not an error to decode fewer values than the size (other members will be default-constructed).

#### Primitives
`char` and `bool` are treated as if they were uint8_t.
All integral and floating point types are supported.

#### Modifiers
##### `std::optional<T>` & `std::unique_ptr<T>`
This is used to signal a maybe-missing object. The format is capable of distinguishing between a missing and an empty object. For [special](#special-types) types the encoding is zero-cost and may freely switch between direct-declared, unique_ptr/optional and deprecated.

Note that `std::optional` requires [explicitly specifying](#manually-specifying-size) the struct size.

##### `packall::deprecated<T>`
If used with a special type this replaces the serialization with a single byte (as if it were an empty vector, empty struct, etc), it also effectively removes this member from the struct. If used with other types, it will serialize a default-constructed object.

If decoding an old buffer with a real object, the contents will be discarded.

##### `packall::omit<T>`
This prevents any encoding or decoding. Additionally in structs these are wholly invisible and do not affect the emitted # of fields meaning adding, removing or moving an omitted field will never affect the serialization.

#### Other
`std::variant<A, B, C, ...>` is supported as a type-safe union as long as each individual type is supported (or is omitted).

`std::pair<K, V>` is supported and encodes as if it were an immutable two element struct

`enum` and `enum class` are supported, encoding as whatever `std::underlying_type_t<T>` is. No validation is performed that the decoded value is in fact valid.

`std::string_view` is supported for encoding only.
`std::vector<bool>` is supported as a special case.

#### Custom encoding
Certain things, like raw or `shared_ptr` are not encodable, largely because this does not handle arbitrary topologies, it's not designed to encode many references to a single object, pointers with cycles and so on.

To support arbitrary encoding, an object can implement the following
```cpp
class ComplexGraph
{
	void pack(packall::bytebuffer& buf)
	{
		packall::serializer_t<packall::options::none> c(buf);
		c.write(x);
	}
	void unpack(packall::bytebuffer& buf)
	{
		packall::serializer_t<packall::options::none> c(buf);
		c.read(x);
	}

	int x;
};
```
packall::serializer_t is a class that provides the following interface
```cpp
struct
{
	// Encode interface
	void write(PrimitiveType);
	void writebuf(const void *buf, size_t sz);
	void write_u8(uint8_t u8);
	void write_sz(size_t sz);
	size_t push();
	void pop(size_t at);

	// Decode interface
	void read(PrimitiveType&);
	void readbuf(void *buf, size_t sz);
	uint8_t peek_u8();
	uint8_t read_u8();
	size_t read_sz();
	size_t enter();
	void leave(size_t at);
};
```

It is invalid to call encode functions on decode or vice-versa.

Each function is mirrored for both encode and decode. `PrimitiveType` is any integer, floating point, char or bool.

Push/pop and enter/leave implement backwards compatible decoding, call push to emit a placeholder and pop to fill it in. Call enter to consume it and leave to jump to where encoding ended.

write_sz / read_sz always use **variable-length integer** encoding. Conversely, write_u8 / read_u8 always write one byte. Plain write and read will use whatever the toplevel options specify.

The exact type of the container (if desiring to implement this in a .cc file) is `packall::container_t<Options, Type>` where `Options` is the options passed in to the pack or unpack API (default `packall::options::none`) and `Type` is the type of the container passed to the toplevel API.

pack & unpack can in turn call the internal functions to encode or decode objects, located at `packall::detail::typeinfo<T>::[un]pack(obj, container)`.


Finally, to integrate wholly custom encoding at the library's level you can implement
```cpp
template<> struct packall::detail::typeinfo<MyType>
{
	template<typename Container>
	void pack(MyType&, Container&);
	template<typename Container>
	void unpack(MyType&, Container&);
};
```
but this should be unnecessary unless extending for custom containers or something similar.

### Changing the schema
All older buffers are always decodable correctly by newer code as long as entries are never removed or changed incompatibally. Additionally, older code can sometimes decode newer buffers.

#### Structs
A struct can provide trait flags to change how it is encoded. Note that immutable is forever!
```cpp
struct MyStruct
{
	static constexpr packall::traits Traits = packall::traits::immutable;
};
```
or
```cpp
template<>
struct packall::detail::traits_impl<MyStruct>
{
	static constexpr packall::traits Traits = packall::traits::immutable;
};
```

A struct that specifies the `immutable` flag forfeits any compatibility options in exchange for slightly more efficient encoding. Use this for structs that will never change, for example `vec3` should never have anything other than 3 entries.

A struct that specifies the `backwards_compatible` flag uses a larger encoding that allows the decoder to skip newer unknown fields.

By default (and with `backwards_compatible`) newer fields may be added to the end of any struct. `backwards_compatible` can be added retroactively the first time a struct definition is changed to preserve compatibility.

In addition, in any struct
* `packall::omit<T>` entries may be added, moved or removed anywhere.
* Any struct member may be replaced by `packall::deprecated<T>`.

If the buffer being decoded has a known size then the toplevel struct is effectively `backwards_compatible` by default as decoding may stop on any struct member boundary.

#### Containers
All non-map containers can be freely interchanged. There is no encoding level difference between a `std::vector` and a `std::list`.

A map can be changed to a non-map containing `std::pair<K, V>` or an immutable two entry struct.

#### Primitives
If a buffer is using variable-length encoding, any non-8bit integer value can be extended, eg `uint16_t` to `uint32_t` but this may break older code if a too-large value is written.

If a buffer is using fixed-length encoding, any integer may change it's sign.

`bool`, `char`, `int8_t` and `uint8_t` can always be interchanged.

#### Deprecation of entries
Any deprecated entry may be replaced with `packall::deprecated<T>`. This is guaranteed to be an empty object (and can have `[[no_unique_address]]` applied) and will simply encode a default-initialized T and decode to a throwaway T.

#### Other
`std::tuple` follows the same rules as structs, except that `omit<T>` counts as a type change and tuples cannot specify compatibility options

`std::variant` can add new types at will and will only cause issues if unknown types are passed to older code.

`std::unique_ptr` and `std::optional` encode identially. Additionally, if T is a struct or tuple or container it can be replaced with the same type inline.

### Output containers
Acceptable containers include:
* `std::vector<uint8_t>`
* `std::span<uint8_t>` for decoding
* `std::ostream` & `std::istream`
Additionally other containers that look similar to a vector (having push_back, data & size) may match the concept and work automatically.

To implement a custom container, you can provide a specialization as follows
```cpp
template<>
struct packall::bytebuffer_impl<MyContainer>
{
	// Read at least n bytes. If n is zero it is not an error to be at the end of the buffer.
	void more_data(size_t n) override;
	// Provide at least n bytes of free space in the buffer (e - p).
	void more_buffer(size_t n) override;
	// Move p to be at the specified byte offset
	void seek_to(size_t at) override;
	// Fixup the 4 bytes at offset at, write n into it
	void fix_offset(size_t at, uint32_t n) override;
	// Write any pending data
	void flush_all() override;
};
```

This may throw a `packall::status` value in more_data() or seek_to() to error out.

### Safety
Serialization is type-safe and fuzz-tested. Invalid input sequences should never crash, but may leave objects partially-initialized or with unexpected values (eg floating point NaNs or bad enum values).

A type hash can be computed at compile time and can be manually stored to verify that the type has not changed. Integrity checking the byte buffer is out of scope for this library.

### Limits
No struct, variant or tuple may contain more than 250 entries (technically some may go all the way to 255 but 250 is a safe limit).
Struct decoding past 50 elements must be provided explicitly (see struct_decompose.inc).

### Manually specifying size
If a struct uses C arrays or `std::optional` then you must specify an Arity member as below.
```cpp
struct MyStruct
{
	static constexpr size_t Arity = ?;
};
```

## Binary Format

### Conventions
#### Variable Integer Encoding
This uses the same scheme as protobufs, base-128 integers and zigzag encoding for signed values. Values are stored little-endian. One byte integers are directly stored always as a single byte.

#### The prefix value
For non-immutable structs this is the number of elements * 4 + 2 that will be omitted with an additional + 1 if this type is backwards compatible.
For tuples this is the number of values stored + 1
This value is always encoded as variable length

### Structs
If non-immutable and not already emitted, emit the prefix value.
If backwards compatible, emit a fixed offset to end of struct (filled in at the end). This is container implemented but currently is a 32-bit fixed value everywhere.
Emit every non-omitted value in order

### Tuples
As structs, but without the backwards compatible offset.

### Containers
Emit the count + 1 as a variable length integer.
If the contained type has a prefix value, emit it and bypass emitting for each value.
Emit every value. If a map, emit key then value.

### Primitives
Integers may be stored as variable or fixed width, depending on encoding settings and usage. Floating point numbers are always fixed width.

### Variant
A single byte containing the field number + 1 being encoded.
Then that field.

### unique_ptr & optional
If the enclosed type has a prefix value or is a container, emit a 0 if that field is not present, otherwise emit it.

Otherwise, emit a 0 if that field is not present, and a 1 if it is, then the value.

### deprecated
If the enclosed type has a prefix value or is a container, emit a 0. Otherwise emit a default-constructed value.


## Text Format
This is simply a [Lua](https://www.lua.org/pil/3.6.html) table constructor.
Keys for tables can either omit the quotes if it is a valid identifier, or be a long string, or be any value enclosed in []

Lists, tuples, nameless structs are stored as `{val1, val2, val3}`
Variants are stored as however their current value is stored.
Maps and structs are stored as `{k1=v1, k2=v2, k3=v3}`
Maps can have complex keys - anything that can be serialized is permitted, enclosed in [].
Strings can use 'single' or "double" quotes, but also [[long strings containing anything]].
Bool is either `true` or `false`
Numbers follow the usual default decimal integers, `0x` prefixed hexadecimal integers, standard and scientific floating point and `0x` prefixed hex float.

To serialize a struct you must provide names.
```cpp
struct MyStruct
{
	float a;
	int b;

	static constexpr kMembers[] = {"a", "b"};
};
```

```cpp
struct Config {
	std::string device;
	std::pair<unsigned, unsigned> resolution;
	std::array<double, 9> K_matrix;
	std::vector<double> distortion_coeffients;
	std::map<std::string, std::variant<uint16_t, std::string, bool>> parameters;

	static constexpr const char *kMembers[] = {
	    "device", "resolution", "K_matrix", "distortion_coeffients", "parameters"};
};

// Construct the object
Config c{"/dev/video0", {640, 480}, 
	 {223.28249888247538, 0.0, 152.30570853111396,
	  0.0, 223.8756535707556, 124.5606000035353,
	  0.0, 0.0, 1.0},
	 {-0.44158343539568284, 0.23861463831967872, 0.0016338407443826572,
	  0.0034950038632981604, -0.05239245892096022},
	 {{"start_server", bool{true}},
	  {"max_depth", uint16_t{5}},
	  {"model_path", std::string{"foo/bar.pt"}}}};

std::string text;
packall::format(c, text);
```
encodes to
```lua
{
        device = [[/dev/video0]],
        resolution = {
                640,
                480
        },
        K_matrix = {
                223.28249888247538,
                0,
                152.30570853111396,
                0,
                223.8756535707556,
                124.5606000035353,
                0,
                0,
                1,
        },
        distortion_coeffients = {
                -0.44158343539568284,
                0.23861463831967872,
                0.0016338407443826572,
                0.0034950038632981604,
                -0.05239245892096022,
        },
        parameters = {
                max_depth = 5,
                model_path = [[foo/bar.pt]],
                start_server = true,
        },
}
```
