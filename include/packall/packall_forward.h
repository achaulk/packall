#ifndef PACKALL_FORWARD_H_
#define PACKALL_FORWARD_H_

#include <stdint.h>

namespace packall {

// Avoid unbounded memory consumption on decode. A million seems like a good default cap.
#ifdef PACKALL_MAX_ELEMENTS
static constexpr size_t kMaximumVectorSize = PACKALL_MAX_ELEMENTS;
#else
static constexpr size_t kMaximumVectorSize = 1000000;
#endif

enum class options : uint8_t
{
	none = 0,
	variable_length_encoding = 1,
};
constexpr options operator|(options l, options r)
{
	return static_cast<options>(static_cast<uint8_t>(l) | static_cast<uint8_t>(r));
}
constexpr bool operator&(options l, options r)
{
	return !!(static_cast<uint8_t>(l) & static_cast<uint8_t>(r));
}

enum class status
{
	ok,
	// Buffer is either incorrect or is a newer version and without decode assists.
	incompatible,
	// Buffer is too small! EOF in the middle of decoding a type other than at a struct member boundary.
	data_underrun,
	// Available to user implementations, not currently thrown.
	bad_data,
	// Data structure exceeds maximum allowable depth.
	stack_overflow,
	// Bad formatted data for parser.
	bad_format,
	bad_variant_value,
	unknown_key,
	out_of_memory,
};

enum class traits : uint8_t
{
	none,
	backwards_compatible = 1,
	immutable = 2,
};
constexpr traits operator|(traits l, traits r)
{
	return static_cast<traits>(static_cast<uint8_t>(l) | static_cast<uint8_t>(r));
}
constexpr bool operator&(traits l, traits r)
{
	return !!(static_cast<uint8_t>(l) & static_cast<uint8_t>(r));
}

}

#endif

