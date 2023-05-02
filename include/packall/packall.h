#ifndef PACKALL_H_
#define PACKALL_H_

#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <memory>
#include <optional>
#include <source_location>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include <string.h>

#include "packall_forward.h"

namespace packall {

namespace detail {
template<typename T>
concept has_traits = requires { T::Traits; };
}

template<typename T>
struct struct_traits
{
	static constexpr traits Traits = traits::none;
};

template<detail::has_traits T>
struct struct_traits<T>
{
	static constexpr traits Traits = T::Traits;
};

// Wrapper class to remove an element from a struct without affecting serialization.
template<typename T>
struct deprecated
{
};

// Wrapper class to omit this from serialization/deserialization
template<typename T>
struct omit : public T
{
	omit(const T& o) : T(o) {}
	omit(T&& o) : T(std::move(o)) {}
	using T::T;

	using T::operator=;
};

template<typename T>
constexpr bool operator==(const omit<T>& l, const omit<T>& r) noexcept
{
	return static_cast<const T&>(l) == static_cast<const T&>(r);
}
template<typename T>
constexpr auto operator<=>(const omit<T>& l, const omit<T>& r) noexcept
{
	return static_cast<const T&>(l) <=> static_cast<const T&>(r);
}

// This is the primary API, pack and unpack to/from a Container.
template<options o, typename T, typename Container>
void pack(const T& obj, Container& out);

template<options o, typename T, typename Container>
[[nodiscard]] status unpack(T& obj, Container& in);

template<typename T, typename Container>
void pack(const T& obj, Container& out)
{
	pack<options::none>(obj, out);
}

template<typename T, typename Container>
[[nodiscard]] status unpack(T& obj, Container& in)
{
	return unpack<options::none>(obj, in);
}

// This performs the same type hashing as the pack function
template<typename T>
consteval uint32_t get_type_id();

// This returns the name of T
template<typename T>
constexpr std::string_view get_type_name();

// Reflection entrypoint
// Assumes that Foreach is a type that contains:
// void enter(std::string_view type);
// void leave();
// template<typename T> void visit(size_t index, const char *name, const T& v);
// index is the logical element number
// enter is called before processing any struct elements, leave after all elements.
// visit is called for every element
// This is semi-recursive, if a struct is nested inside a struct then it will be processed in turn,
// otherwise it is expected that the types will be picked apart by visit().
// visit can itself call foreach_member on a sub-value
template<typename T, typename Foreach>
void foreach_member(T& obj, Foreach& c);

template<typename T>
struct bytebuffer_impl;

struct bytebuffer
{
	virtual ~bytebuffer() = default;
	virtual void more_data(size_t n) = 0;
	virtual void more_buffer(size_t n) = 0;
	virtual void seek_to(size_t at) = 0;
	virtual void fix_offset(size_t at, uint32_t n) = 0;
	virtual void flush_all() = 0;

	void write_u8(uint8_t v)
	{
		if(p == e) [[unlikely]] {
			more_buffer(0);
		}
		*p++ = v;
	}
	void write_bytes(const void *v, size_t sz)
	{
		if(e - p < (ptrdiff_t)sz) [[unlikely]] {
			more_buffer(sz);
		}
		memcpy(p, v, sz);
		p += sz;
	}

	size_t push()
	{
		size_t ret = offset + (p - s);
		uint32_t v = 0;
		write_bytes(&v, 4);
		return ret;
	}
	void pop(size_t at)
	{
		uint32_t sz = (uint32_t)(offset + (p - s) - at);
		fix_offset(at, sz);
	}

	size_t enter()
	{
		uint32_t v;
		size_t ret = offset + (p - s);
		read_bytes(&v, 4);
		return ret + v;
	}
	void leave(size_t at)
	{
		seek_to(at);
	}

	uint8_t read_u8()
	{
		if(p == e) [[unlikely]]
			more_data(1);
		uint8_t v = *p++;
		if(p == e) [[unlikely]]
			more_data(0);
		return v;
	}
	uint8_t peek_u8()
	{
		return *p;
	}

	void read_bytes(void *v, size_t sz)
	{
		if(e - p < (ptrdiff_t)sz) [[unlikely]] {
			more_data(sz - (e - p));
		}
		memcpy(v, p, sz);
		p += sz;
		if(p == e) [[unlikely]]
			more_data(0);
	}

	bool end() const
	{
		return p == e;
	}

	bool ok()
	{
		return true;
	}

	size_t offset = 0;
	uint8_t *s = nullptr, *p = nullptr, *e = nullptr;
};

// Everything below this is implementation details
namespace detail {
// Serialization format:
// structs, tuples: one byte # of entries, then all entries. These are considered predecode eligible as the byte is fixed by the type, if this is stored in a container.
// the byte is pulled out in front of all of the entries instead of in front of each one
// Variable length integers (32 & 64 bit): protobuf (signed as zigzag, then base-128 with high bit set to continue)
// Fixed length integers: int16 - LE, int32 and int64 native
// structs in backwards comparible mode, 16-bit non-variable size prefixed
// containers (vector, list, string, deque, set, unordered_set): 32-bit size followed by entries. If the contained type has predecode info that is stored before the size

// This is the root type implementing serialization for T
template<typename T>
struct typeinfo;

struct any
{
	template<typename type>
	operator type();
};

template<class T, class... Args>
struct aggregate_arity
{
	// Approximate the number of initializers required, add one until there are too many.
	// This miscomputes for C arrays (one initializer per element) and optional (stops the sequence)
	static constexpr size_t count(void *)
	{
		return sizeof...(Args) - 1;
	}

	template<typename U = T, typename = decltype(U{Args{}...})>
	static constexpr size_t count(std::nullptr_t)
	{
		return aggregate_arity<T, Args..., any>::value;
	}

	static constexpr size_t value = count(nullptr);
};

template<size_t>
struct decompose;

// Decomposition of structs up to 50 elements
#include "struct_decompose.inc"

template<typename T>
struct aggregate_arity_calc
{
	static constexpr size_t Arity = aggregate_arity<std::remove_cv_t<T>>::value;
};

// aggregate_arity doesn't catch everything, so if the struct declares an Arity member, use that.
template<typename T>
concept has_arity = requires(T t) { T::Arity; };

template<has_arity T>
struct aggregate_arity_calc<T>
{
	static constexpr size_t Arity = T::Arity;
};

template<typename T>
struct is_array_type : std::false_type
{
};

template<typename T, size_t N>
struct is_array_type<std::array<T, N>> : std::true_type
{
	using type = T;
	static constexpr size_t kN = N;
};

// protobuf zigzag encoding is efficient for signed numbers
// T is always unsigned here
template<typename T>
constexpr T zigzag_encode(T v)
{
	T x = (v >> ((8 * sizeof(T)) - 1)) ? ~(T)0 : (T)0;
	return (v << 1) ^ x;
}

template<typename T>
constexpr T zigzag_decode(T v)
{
	return (v >> 1) ^ (~(v & 1) + 1);
}

template<bool VariableEncoding>
struct bytes_converter
{
	bytes_converter(bytebuffer& wrap) : wrap(wrap) {}

	void write(int8_t v)
	{
		wrap.write_u8(std::bit_cast<uint8_t>(v));
	}

	void write(int16_t v)
	{
		if constexpr(VariableEncoding) {
			// proto encoding
			write(zigzag_encode(std::bit_cast<uint16_t>(v)));
		} else {
			wrap.write_bytes(&v, sizeof(v));
		}
	}

	void write(int32_t v)
	{
		if constexpr(VariableEncoding) {
			// proto encoding
			write(zigzag_encode(std::bit_cast<uint32_t>(v)));
		} else {
			wrap.write_bytes(&v, sizeof(v));
		}
	}

	void write(int64_t v)
	{
		if constexpr(VariableEncoding) {
			// proto encoding
			write(zigzag_encode(std::bit_cast<uint64_t>(v)));
		} else {
			wrap.write_bytes(&v, sizeof(v));
		}
	}

	void write(uint8_t v)
	{
		wrap.write_u8(v);
	}

	void write(uint16_t v)
	{
		if constexpr(VariableEncoding) {
			do {
				wrap.write_u8((uint8_t)v | ((v > 127) ? 0x80 : 0));
				v >>= 7;
			} while(v);
		} else {
			wrap.write_bytes(&v, sizeof(v));
		}
	}

	void write(uint32_t v)
	{
		if constexpr(VariableEncoding) {
			do {
				wrap.write_u8((uint8_t)v | ((v > 127) ? 0x80 : 0));
				v >>= 7;
			} while(v);
		} else {
			wrap.write_bytes(&v, sizeof(v));
		}
	}

	void write(uint64_t v)
	{
		if constexpr(VariableEncoding) {
			do {
				wrap.write_u8((uint8_t)v | ((v > 127) ? 0x80 : 0));
				v >>= 7;
			} while(v);
		} else {
			wrap.write_bytes(&v, sizeof(v));
		}
	}

	template<std::floating_point U>
	void write(U v)
	{
		wrap.write_bytes(&v, sizeof(v));
	}

	void write(bool v)
	{
		write((uint8_t)(v ? 1 : 0));
	}

	void write_sz(size_t v)
	{
		do {
			wrap.write_u8((uint8_t)(v | ((v > 0x7F) ? 0x80 : 0)));
			v >>= 7;
		} while(v);
	}
	size_t read_sz()
	{
		size_t v = 0;
		uint8_t ofs = 0;
		for(int i = 0; i < 10; i++, ofs += 7) {
			uint8_t b = wrap.read_u8();
			v |= (uint64_t)(b & 0x7F) << ofs;
			if(!(b & 0x80))
				break;
		}
		return v;
	}

	void read(int8_t& v)
	{
		v = std::bit_cast<int8_t>(wrap.read_u8());
	}

	void read(int16_t& v)
	{
		if constexpr(VariableEncoding) {
			uint16_t u = 0;
			read(u);
			v = std::bit_cast<int16_t>(zigzag_decode(u));
		} else {
			wrap.read_bytes(&v, sizeof(v));
		}
	}

	void read(int32_t& v)
	{
		if constexpr(VariableEncoding) {
			uint32_t u = 0;
			read(u);
			v = std::bit_cast<int32_t>(zigzag_decode(u));
		} else {
			wrap.read_bytes(&v, sizeof(v));
		}
	}

	void read(int64_t& v)
	{
		if constexpr(VariableEncoding) {
			uint64_t u = 0;
			read(u);
			v = std::bit_cast<int64_t>(zigzag_decode(u));
		} else {
			wrap.read_bytes(&v, sizeof(v));
		}
	}

	void read(uint8_t& v)
	{
		v = wrap.read_u8();
	}

	void read(uint16_t& v)
	{
		if constexpr(VariableEncoding) {
			v = 0;
			uint8_t ofs = 0;
			for(int i = 0; i < 3; i++, ofs += 7) {
				uint8_t b = wrap.read_u8();
				v |= (uint16_t)(b & 0x7F) << ofs;
				if(!(b & 0x80))
					break;
			}
		} else {
			wrap.read_bytes(&v, sizeof(v));
		}
	}

	void read(uint32_t& v)
	{
		if constexpr(VariableEncoding) {
			v = 0;
			uint8_t ofs = 0;
			for(int i = 0; i < 5; i++, ofs += 7) {
				uint8_t b = wrap.read_u8();
				v |= (uint32_t)(b & 0x7F) << ofs;
				if(!(b & 0x80))
					break;
			}
		} else {
			wrap.read_bytes(&v, sizeof(v));
		}
	}

	void read(uint64_t& v)
	{
		if constexpr(VariableEncoding) {
			v = 0;
			uint8_t ofs = 0;
			for(int i = 0; i < 10; i++, ofs += 7) {
				uint8_t b = wrap.read_u8();
				v |= (uint64_t)(b & 0x7F) << ofs;
				if(!(b & 0x80))
					break;
			}
		} else {
			wrap.read_bytes(&v, sizeof(v));
		}
	}

	template<std::floating_point U>
	void read(U& v)
	{
		wrap.read_bytes(&v, sizeof(v));
	}

	void readbuf(void *buf, size_t sz)
	{
		wrap.read_bytes(buf, sz);
	}
	void writebuf(const void *buf, size_t sz)
	{
		wrap.write_bytes(buf, sz);
	}

	uint8_t peek_u8()
	{
		return wrap.peek_u8();
	}
	uint8_t read_u8()
	{
		return wrap.read_u8();
	}
	void write_u8(uint8_t v)
	{
		write(v);
	}

	bool done() const
	{
		return wrap.end();
	}

	// Backwards compatibility
	size_t push()
	{
		return wrap.push();
	}
	void pop(size_t at)
	{
		wrap.pop(at);
	}

	size_t enter()
	{
		return wrap.enter();
	}
	void leave(size_t at)
	{
		wrap.leave(at);
	}

	bytebuffer& get_custom_buffer()
	{
		return wrap;
	}

	bytebuffer& wrap;

};

template<typename T>
struct get_member_index_helper
{
	static constexpr bool helper2(uint32_t& out, uint32_t index, std::string_view name)
	{
		if(name == T::kMembers[index]) {
			out = index;
			return true;
		}
		return false;
	}

	template<size_t... Index>
	static constexpr uint32_t helper(std::string_view name, std::index_sequence<Index...>)
	{
		uint32_t o = ~0u;
		(helper2(o, Index, name) || ...);
		return o;
	}
};

template<typename T>
constexpr uint32_t get_member_index(std::string_view name)
{
	return get_member_index_helper<T>::helper(name, std::make_index_sequence<typeinfo<T>::Arity>());
}

template<typename T>
inline consteval std::string_view get_t_name()
{
#ifdef _MSC_VER
	// Works @ 1934
	std::string_view str = __FUNCSIG__;
	auto start = str.find_first_of('<', str.find("get_t_name")) + 1;
	auto end = str.find_last_of('>');
	auto ret = str.substr(start, end - start);
	if(ret.starts_with("struct "))
		ret.remove_prefix(7);
	if(ret.starts_with("class "))
		ret.remove_prefix(6);
	return ret;
#elif __clang__
	std::string_view str = __PRETTY_FUNCTION__;
	auto start = str.find("[T = ", str.find("get_t_name")) + 5;
	auto end = str.find_last_of(']');
	return str.substr(start, end - start);
#else
#error "Missing typename implementation!"
#endif
}

struct type_list
{
	std::vector<uint8_t> types;
	struct aggregate
	{
		std::string_view name;
		uint8_t n;
	};
	std::vector<aggregate> aggregates;

	template<typename T>
	constexpr std::pair<uint8_t, bool> add_type()
	{
		std::string_view this_name = get_t_name<std::remove_cv_t<T>>();
		auto it =
		    std::find_if(aggregates.begin(), aggregates.end(), [&](const auto& e) { return e.name == this_name; });
		// don't recurse indefinitely
		if(it == aggregates.end()) {
			uint8_t id = (uint8_t)aggregates.size();
			if(id == 255)
				throw;
			aggregates.push_back(type_list::aggregate{this_name, id});
			return std::make_pair(id, true);
		} else {
			return std::make_pair(it->n, false);
		}
	}
};

enum class type_id : uint8_t
{
	// These are carefully ordered
	uint8,  // +1-1
	uint16, // +2-1
	bool_,
	uint32, // +4-1
	char_,
	int8,   // +1-1
	int16,  // +2-2
	uint64, // +8-1
	int32,  // +4-1
	float32,
	float64,
	enum_class,
	int64, // +8-1

	string,
	array,
	listlike,
	maplike,
	setlike,
	optional,
	pair,
	tuple,
	variant,
	unique_ptr,
	struct_,

	user_type,
};

// custom
template<typename T>
concept is_custom_serialized = requires(T t) {
	                               t.pack;
	                               t.unpack;
                               };

template<is_custom_serialized T>
struct typeinfo<T>
{
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::struct_);

	template<typename Container>
	static void pack(T& obj, Container& out)
	{
		obj.pack(out.get_custom_buffer());
	}
	template<typename Container>
	static void unpack(T& obj, Container& in)
	{
		obj.unpack(in.get_custom_buffer());
	}
	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(static_cast<uint8_t>(type_id::struct_));
		t.types.push_back(t.add_type<T>().first);
	}

	template<typename Foreach>
	static void for_each(const char *name, T& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<typename T>
struct has_predecode_info : public std::false_type
{
};

template<typename T>
concept has_predecode_bool = requires(T t) {
	                             typeinfo<T>::predecode_info;
	                             typeinfo<T>::use_predecode;
                             };

template<has_predecode_bool T>
struct has_predecode_info<T>
{
	static constexpr bool value = typeinfo<T>::use_predecode;
};

// Pick apart various containers

template<typename T>
concept is_container = !
is_array_type<T>::value&& requires(T t) {
	                          t.begin();
	                          t.end();
	                          t.size();
                          };

template<typename T>
concept is_setlike = is_container<T> && requires { typename T::key_type; };

template<typename T>
concept is_maplike = is_setlike<T> && requires { typename T::mapped_type; };

template<typename T>
concept is_listlike = is_container<T> && requires(T t, size_t sz) {
	                                         typename T::value_type;
	                                         t.resize(sz);
                                         };

template<typename T>
concept is_aggregate_struct = std::is_aggregate_v<T> && !
is_array_type<T>::value && !is_custom_serialized<T> && std::is_class_v<T>;

template<typename T>
struct emit_element
{
	static constexpr uint8_t value = 1;
};
template<typename T>
struct emit_element<omit<T>>
{
	static constexpr uint8_t value = 0;
};

template<typename T, size_t Arity, size_t... Index>
static consteval uint8_t calculate_predecode(std::index_sequence<Index...>)
{
	return (
	    emit_element<std::remove_cvref_t<decltype(decompose<Arity>::template get<Index>(std::declval<T>()))>>::value +
	    ...);
}

template<typename T>
concept has_postdecode_check = requires(T t) { t.post_decode(); };

template<typename T>
constexpr void maybe_postdecode(T& v)
{
}

template<has_postdecode_check T>
void maybe_postdecode(T& v)
{
	v.post_decode();
}

// struct / class
template<is_aggregate_struct T>
struct typeinfo<T>
{
	using type = T;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::struct_);
	static constexpr size_t Arity = aggregate_arity_calc<T>::Arity;
	static_assert(Arity < 250);

	static constexpr bool is_backwards_compatible = struct_traits<T>::Traits & traits::backwards_compatible;
	static constexpr size_t predecode_info =
	    (Arity > 0 ? calculate_predecode<T, Arity>(std::make_index_sequence<Arity>()) : 0) * 4 + 2 +
		(is_backwards_compatible ? 1 : 0);
	static constexpr bool use_predecode = !(struct_traits<T>::Traits & traits::immutable);


	template<typename Container>
	static void pack(T& obj, Container& out)
	{
		if constexpr(use_predecode)
			out.write_sz(predecode_info);
		pack_predecoded(obj, out);
	}

	template<typename Container>
	static void pack_predecoded(T& obj, Container& out)
	{
		pack_helper(obj, out, std::make_index_sequence<Arity>());
	}

	template<typename Container, size_t... Index>
	static void pack_helper(T& obj, Container& out, std::index_sequence<Index...>)
	{
		if constexpr(Arity > 0) {
			size_t at;
			if constexpr(is_backwards_compatible) {
				at = out.push();
			}
			(typeinfo<std::remove_cvref_t<decltype(decompose<Arity>::template get<Index>(std::declval<T>()))>>::pack(
			     decompose<Arity>::template get<Index>(obj), out),
			    ...);
			if constexpr(is_backwards_compatible) {
				out.pop(at);
			}
		}
	}

	template<typename Container>
	static void unpack(T& obj, Container& in)
	{
		size_t n = predecode_info;
		if constexpr(use_predecode)
			n = in.read_sz();
		unpack_helper(obj, n, in, std::make_index_sequence<Arity>());
	}

	template<typename Container>
	static void unpack_predecoded(T& obj, Container& in, size_t n)
	{
		unpack_helper(obj, n, in, std::make_index_sequence<Arity>());
	}

	template<typename Container, size_t... Index>
	static void unpack_helper(T& obj, size_t n, Container& in, std::index_sequence<Index...>)
	{
		// If no elements were written, this is either an empty struct, or a deprecated<T>, in any case no size was written
		if(n == 0)
			return;
		bool bc = n & 1;
		n >>= 2;

		size_t at = 0;
		if(bc) {
			at = in.enter();
		} else if(n > Arity) [[unlikely]] {
			throw status::incompatible;
		}
		(maybe_unpack<Index>(obj, n, in) && ...);
		if(bc)
			in.leave(at);
		maybe_postdecode(obj);
	}

	template<size_t I, typename Container>
	static bool maybe_unpack(T& obj, size_t& n, Container& in)
	{
		if constexpr(!emit_element<
		                 std::remove_cvref_t<decltype(decompose<Arity>::template get<I>(std::declval<T>()))>>::value)
			return true;
		if(n == 0 || in.done())
			return false;
		n--;
		typeinfo<std::remove_cvref_t<decltype(decompose<Arity>::template get<I>(std::declval<T>()))>>::unpack(
		    decompose<Arity>::template get<I>(obj), in);
		return true;
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		auto r = t.add_type<T>();
		t.types.push_back(r.first);
		if(r.second)
			get_types_helper(t, std::make_index_sequence<Arity>());
	}

	template<size_t... Index>
	static constexpr void get_types_helper(type_list& t, std::index_sequence<Index...>)
	{
		(typeinfo<std::remove_cvref_t<decltype(decompose<Arity>::template get<Index>(std::declval<T>()))>>::get_types(
		     t),
		    ...);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.enter(get_t_name<type>());
		foreach_helper(obj, c, std::make_index_sequence<Arity>());
		c.leave();
	}
	template<typename Foreach, size_t... Index>
	static void foreach_helper(type& obj, Foreach& c, std::index_sequence<Index...>)
	{
		(c.visit(Index, type::kMembers[Index],
		     decompose<Arity>::template get<Index>(static_cast<std::remove_reference_t<T>&&>(obj))),
		    ...);
	}
};

// array<T, N>
template<typename T>
    requires(std::is_aggregate_v<T> && is_array_type<T>::value)
struct typeinfo<T>
{
	using type = typename is_array_type<T>::type;
	static constexpr size_t N = is_array_type<T>::kN;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::array);

	template<typename Container>
	static void pack(std::array<type, N>& obj, Container& out)
	{
		out.write_sz(N + 1);
		for(size_t i = 0; i < N; i++) {
			typeinfo<type>::pack(obj[i], out);
		}
	}

	template<typename Container>
	static void unpack(std::array<type, N>& obj, Container& in)
	{
		size_t n = in.read_sz();
		if(n == 0)
			return;
		n--;
		if(n > N)
			throw status::incompatible;
		for(size_t i = 0; i < n; i++) {
			typeinfo<type>::unpack(obj[i], in);
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		size_t v = N;
		while(v > 0) {
			t.types.push_back((uint8_t)v);
			v >>= 8;
		}
		typeinfo<type>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, T *obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
	template<typename Foreach>
	static void for_each(const char *name, std::array<type, N>& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<typename T>
    requires(std::is_aggregate_v<T> && std::is_array_v<T>)
struct typeinfo<T>
{
	using type = std::remove_cvref_t<decltype(std::declval<T>()[0])>;
	static constexpr size_t N = std::extent_v<T>;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::array);

	template<typename Container>
	static void pack(T& obj, Container& out)
	{
		out.write_sz(N + 1);
		for(size_t i = 0; i < N; i++) {
			typeinfo<type>::pack(obj[i], out);
		}
	}

	template<typename Container>
	static void unpack(T& obj, Container& in)
	{
		size_t n = in.read_sz();
		if(n == 0)
			return;
		n--;
		if(n > N)
			throw status::incompatible;
		for(size_t i = 0; i < n; i++) {
			typeinfo<type>::unpack(obj[i], in);
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		size_t v = N;
		while(v > 0) {
			t.types.push_back((uint8_t)v);
			v >>= 8;
		}
		typeinfo<type>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, T *obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
	template<typename Foreach>
	static void for_each(const char *name, std::array<type, N>& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<typename T>
struct typeinfo<omit<T>>
{
	template<typename Container>
	static void pack(T& obj, Container& out)
	{
	}
	template<typename Container>
	static void unpack(T& obj, Container& in)
	{
	}
};

template<typename T>
struct typeinfo<deprecated<T>>
{
	using type = deprecated<T>;
	static constexpr uint8_t predecode_info = 0;
	static constexpr bool use_predecode = has_predecode_info<T>::value;

	template<typename Container>
	static void pack(type obj, Container& out)
	{
		out.write_u8(0);
	}
	template<typename U = T, typename Container>
	static void pack_predecoded(type obj, Container& out)
	{
	}
	template<typename Container>
	static void unpack(type obj, Container& in)
	{
		if(in.peek_u8()) {
			T _;
			typeinfo<T>::unpack(_, in);
		} else {
			in.read_u8();
		}
	}
	template<typename U = T, typename Container>
	static void unpack_predecoded(type obj, Container& out, size_t v)
	{
		if(v < UINT_MAX) {
			U _{};
			typeinfo<T>::unpack_predecoded(_, out, v);
		}
	}
	static constexpr void get_types(type_list& t)
	{
		typeinfo<T>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
	}
};

template<typename T>
    requires(std::is_enum_v<T>)
struct typeinfo<T>
{
	using type = T;
	using underlying = std::underlying_type_t<T>;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::enum_class);

	template<typename Container>
	static void pack(type obj, Container& out)
	{
		typeinfo<underlying>::pack(static_cast<underlying>(obj), out);
	}

	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		underlying v{};
		typeinfo<underlying>::unpack(v, in);
		obj = static_cast<type>(v);
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		auto v = static_cast<uint8_t>(obj);
		c.visit(0, name, v);
	}
};

template<>
struct typeinfo<char>
{
	using type = char;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::uint8);
	template<typename Container>
	static void pack(char obj, Container& out)
	{
		out.write_u8(static_cast<uint8_t>(obj));
	}
	template<typename Container>
	static void unpack(char& obj, Container& in)
	{
		obj = static_cast<char>(in.read_u8());
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<std::integral T>
struct typeinfo<T>
{
	using type = T;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::int8) + sizeof(T) - 1;
	template<typename Container>
	static void pack(T obj, Container& out)
	{
		out.write(obj);
	}
	template<typename Container>
	static void unpack(T& obj, Container& in)
	{
		in.read(obj);
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};
template<std::floating_point T>
struct typeinfo<T>
{
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::float32) + ((sizeof(T) == sizeof(double)) ? 1 : 0);
	template<typename Container>
	static void pack(T obj, Container& out)
	{
		out.write(obj);
	}
	template<typename Container>
	static void unpack(T& obj, Container& in)
	{
		in.read(obj);
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
	}

	template<typename Foreach>
	static void for_each(const char *name, T& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<>
struct typeinfo<bool>
{
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::bool_);
	template<typename Container>
	static void pack(bool obj, Container& out)
	{
		out.write((uint8_t)(obj ? 1 : 0));
	}
	template<typename Container>
	static void unpack(bool& obj, Container& in)
	{
		obj = !!in.read_u8();
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
	}

	template<typename Foreach>
	static void for_each(const char *name, bool& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<typename T>
struct typeinfo<std::optional<T>>
{
	using type = std::optional<T>;

	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::optional);
	template<typename Container>
	static void pack(type& obj, Container& out)
	{
		if(obj) {
			out.write((uint8_t)1);
			typeinfo<T>::pack(*obj, out);
		} else {
			out.write((uint8_t)0);
		}
	}
	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		if(in.read_u8()) {
			obj.emplace();
			typeinfo<T>::unpack(*obj, in);
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<T>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, T& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<is_container T>
struct typeinfo<std::optional<T>>
{
	using type = std::optional<T>;

	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::optional);
	template<typename Container>
	static void pack(type& obj, Container& out)
	{
		if(obj) {
			typeinfo<T>::pack(*obj, out);
		} else {
			out.write((uint8_t)0);
		}
	}
	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		if(in.peek_u8()) {
			obj.emplace();
			typeinfo<T>::unpack(*obj, in);
		} else {
			in.read_u8();
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<T>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, T& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<is_aggregate_struct T>
struct typeinfo<std::optional<T>>
{
	using type = std::optional<T>;

	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::optional);
	template<typename Container>
	static void pack(type& obj, Container& out)
	{
		if(obj) {
			out.write((uint8_t)1);
			typeinfo<T>::pack(*obj, out);
		} else {
			out.write((uint8_t)0);
		}
	}
	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		if(in.peek_u8()) {
			obj.emplace();
			typeinfo<T>::unpack(*obj, in);
		} else {
			in.read_u8();
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<T>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, T& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<typename T, typename Traits, typename Alloc>
struct typeinfo<std::basic_string<T, Traits, Alloc>>
{
	using type = std::basic_string<T>;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::string);
	static_assert(std::is_fundamental_v<T>);

	template<typename Container>
	static void pack(const type& obj, Container& out)
	{
		out.write_sz(obj.size() + 1);
		out.writebuf(obj.data(), obj.size() * sizeof(T));
	}

	template<typename Container>
	static void unpack(type& v, Container& in)
	{
		size_t sz = in.read_sz();
		if(sz == 0)
			return;
		sz--;
		if(sz > kMaximumVectorSize) [[unlikely]]
			throw status::out_of_memory;
		v.resize(sz);
		in.readbuf(v.data(), sz * sizeof(T));
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<T>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<typename T, typename Traits>
struct typeinfo<std::basic_string_view<T, Traits>>
{
	using type = std::basic_string_view<T>;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::string);
	static_assert(std::is_fundamental_v<T>);

	template<typename Container>
	static void pack(const type& obj, Container& out)
	{
		out.write_sz(obj.size() + 1);
		out.writebuf(obj.data(), obj.size() * sizeof(T));
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<T>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<is_listlike T>
struct typeinfo<T>
{
	using type = T;
	using V = typename T::value_type;

	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::listlike);

	template<typename Container>
	static void pack(type& obj, Container& out)
	{
		out.write_sz(obj.size() + 1);
		if constexpr(has_predecode_info<V>::value) {
			out.write_sz(typeinfo<V>::predecode_info);
			for(auto it = obj.begin(); it != obj.end(); ++it) typeinfo<V>::pack_predecoded(*it, out);
		} else {
			for(auto it = obj.begin(); it != obj.end(); ++it) typeinfo<V>::pack(*it, out);
		}
	}

	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		size_t sz = in.read_sz();
		if(sz == 0)
			return;
		sz--;
		if(sz > kMaximumVectorSize) [[unlikely]]
			throw status::out_of_memory;
		obj.resize(sz);
		if constexpr(has_predecode_info<V>::value) {
			size_t pd = in.read_sz();
			for(auto it = obj.begin(); it != obj.end(); ++it) typeinfo<V>::unpack_predecoded(*it, in, pd);
		} else {
			for(auto it = obj.begin(); it != obj.end(); ++it) typeinfo<V>::unpack(*it, in);
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<V>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<typename A>
struct typeinfo<std::vector<bool, A>>
{
	using type = std::vector<bool>;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::listlike);

	template<typename Container>
	static void pack(type& obj, Container& out)
	{
		out.write_sz(obj.size() + 1);
		for(auto it = obj.begin(); it != obj.end(); ++it) typeinfo<bool>::pack(*it, out);
	}

	template<typename Container>
	static void unpack(type& v, Container& in)
	{
		size_t sz = in.read_sz();
		if(sz == 0)
			return;
		sz--;
		v.reserve(sz);
		for(uint32_t i = 0; i < sz; i++) {
			bool b;
			typeinfo<bool>::unpack(b, in);
			v.push_back(b);
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<bool>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<is_container T, typename D>
struct typeinfo<std::unique_ptr<T, D>>
{
	using type = std::unique_ptr<T>;

	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::unique_ptr);
	template<typename Container>
	static void pack(const type& obj, Container& out)
	{
		if(obj) {
			typeinfo<T>::pack(*obj, out);
		} else {
			out.write((uint8_t)0);
		}
	}
	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		if(in.peek_u8()) {
			obj = std::make_unique<T>();
			typeinfo<T>::unpack(*obj.get(), in);
		} else {
			in.read_u8();
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<T>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<is_aggregate_struct T, typename D>
struct typeinfo<std::unique_ptr<T, D>>
{
	using type = std::unique_ptr<T>;

	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::unique_ptr);
	template<typename Container>
	static void pack(const type& obj, Container& out)
	{
		if(obj) {
			typeinfo<T>::pack(*obj, out);
		} else {
			out.write((uint8_t)0);
		}
	}
	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		if(in.peek_u8()) {
			obj = std::make_unique<T>();
			typeinfo<T>::unpack(*obj.get(), in);
		} else {
			in.read_u8();
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<T>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<typename T, typename U>
struct typeinfo<std::pair<T, U>>
{
	using type = std::pair<T, U>;

	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::pair);
	template<typename Container>
	static void pack(const type& obj, Container& out)
	{
		typeinfo<T>::pack(obj.first, out);
		typeinfo<U>::pack(obj.second, out);
	}
	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		typeinfo<T>::unpack(obj.first, in);
		typeinfo<U>::unpack(obj.second, in);
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<T>::get_types(t);
		typeinfo<U>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<is_maplike T>
struct typeinfo<T>
{
	using type = T;
	using K = typename T::key_type;
	using V = typename T::mapped_type;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::maplike);

	template<typename Container>
	static void pack(type& obj, Container& out)
	{
		out.write_sz(obj.size() + 1);
		for(auto& [k, v] : obj) {
			typeinfo<K>::pack(k, out);
			typeinfo<V>::pack(v, out);
		}
	}
	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		size_t n = in.read_sz();
		if(n == 0)
			return;
		n--;
		for(size_t i = 0; i < n; i++) {
			K k{};
			V v{};
			typeinfo<K>::unpack(k, in);
			typeinfo<V>::unpack(v, in);
			obj.emplace(std::move(k), std::move(v));
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<K>::get_types(t);
		typeinfo<V>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<is_setlike T>
struct typeinfo<T>
{
	using type = T;
	using K = typename T::key_type;
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::setlike);

	template<typename Container>
	static void pack(const type& obj, Container& out)
	{
		out.write_sz(obj.size() + 1);
		if constexpr(has_predecode_info<K>::value) {
			out.write(typeinfo<K>::predecode_info);
			for(auto it = obj.begin(); it != obj.end(); ++it) typeinfo<K>::pack_predecoded(*it, out);
		} else {
			for(auto it = obj.begin(); it != obj.end(); ++it) typeinfo<K>::pack(*it, out);
		}
	}
	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		size_t n = in.read_sz();
		if(n == 0)
			return;
		n--;
		if constexpr(has_predecode_info<K>::value) {
			std::remove_cvref_t<decltype(typeinfo<K>::predecode_info)> pd;
			in.read(pd);
			for(size_t i = 0; i < n; i++) {
				K k;
				typeinfo<K>::unpack_predecoded(k, in, pd);
				obj.emplace(std::move(k));
			}
		} else {
			for(size_t i = 0; i < n; i++) {
				K k;
				typeinfo<K>::unpack(k, in);
				obj.emplace(std::move(k));
			}
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		typeinfo<K>::get_types(t);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

template<typename... V>
struct typeinfo<std::variant<V...>>
{
	using type = std::variant<V...>;

	static_assert(sizeof...(V) < 255);
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::variant);

	template<typename Container>
	static void pack(type& obj, Container& out)
	{
		out.write_sz(obj.index() + 1);
		pack_helper(obj, out, std::make_index_sequence<sizeof...(V)>());
	}

	template<typename Container, size_t... Index>
	static void pack_helper(type& obj, Container& out, std::index_sequence<Index...>)
	{
		(maybe_pack<Index>(obj, out) || ...);
	}

	template<size_t I, typename Container>
	static bool maybe_pack(type& obj, Container& out)
	{
		if(I == obj.index()) {
			typeinfo<std::variant_alternative_t<I, type>>::pack(std::get<I>(obj), out);
			return true;
		}
		return false;
	}

	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		unpack_helper(obj, in.read_sz(), in, std::make_index_sequence<sizeof...(V)>());
	}

	template<typename Container, size_t... Index>
	static void unpack_helper(type& obj, size_t n, Container& in, std::index_sequence<Index...>)
	{
		if(n == 0)
			return;
		n--;

		if(!(maybe_unpack<Index>(obj, n, in) || ...))
			throw status::incompatible;
	}

	template<size_t I, typename Container>
	static bool maybe_unpack(type& obj, size_t n, Container& in)
	{
		if(I == n) {
			std::variant_alternative_t<I, type> v;
			typeinfo<std::variant_alternative_t<I, type>>::unpack(v, in);
			obj = std::move(v);
			return true;
		}
		return false;
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		(typeinfo<V>::get_types(t), ...);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};
template<typename... V>
struct typeinfo<std::tuple<V...>>
{
	using type = std::tuple<V...>;

	static_assert(sizeof...(V) < 255);
	static constexpr uint8_t type_id = static_cast<uint8_t>(type_id::tuple);

	static constexpr size_t predecode_info = sizeof...(V) + 1;
	static constexpr bool use_predecode = true;

	template<typename Container>
	static void pack(const type& obj, Container& out)
	{
		out.write_sz(predecode_info);
		pack_helper(obj, out, std::make_index_sequence<sizeof...(V)>());
	}

	template<typename Container>
	static void pack_predecoded(const type& obj, Container& out)
	{
		pack_helper(obj, out, std::make_index_sequence<sizeof...(V)>());
	}

	template<typename Container, size_t... Index>
	static void pack_helper(const type& obj, Container& out, std::index_sequence<Index...>)
	{
		(typeinfo<std::tuple_element_t<Index, type>>::pack(std::get<Index>(obj), out), ...);
	}

	template<typename Container>
	static void unpack(type& obj, Container& in)
	{
		unpack_helper(obj, in.read_sz(), in, std::make_index_sequence<sizeof...(V)>());
	}

	template<typename Container>
	static void unpack_predecoded(type& obj, Container& in, size_t n)
	{
		unpack_helper(obj, n, in, std::make_index_sequence<sizeof...(V)>());
	}

	template<typename Container, size_t... Index>
	static void unpack_helper(type& obj, size_t n, Container& in, std::index_sequence<Index...>)
	{
		if(n == 0)
			return;
		n--;
		if(n > sizeof...(V)) [[unlikely]]
			throw status::incompatible;
		(maybe_unpack<Index>(obj, n, in), ...);
	}

	template<size_t I, typename Container>
	static void maybe_unpack(type& obj, size_t n, Container& in)
	{
		if(I < n) {
			std::tuple_element_t<I, type> v;
			typeinfo<decltype(v)>::unpack(v, in);
			std::get<I>(obj) = v;
		}
	}

	static constexpr void get_types(type_list& t)
	{
		t.types.push_back(type_id);
		(typeinfo<V>::get_types(t), ...);
	}

	template<typename Foreach>
	static void for_each(const char *name, type& obj, Foreach& c)
	{
		c.visit(0, name, obj);
	}
};

consteval unsigned int ct_crc32(const uint8_t *bytes, size_t n)
{
	uint32_t crc = 0xFFFFFFFF;
	for(size_t i = 0; i < n; i++) {
		crc ^= bytes[i];
		for(int j = 0; j < 8; j++) {
			crc = (crc >> 1) ^ (0xEDB88320 & ~((crc & 1) - 1));
		}
	}
	return ~crc;
}

} // namespace detail

// This returns a hashed version of the type
template<typename T>
consteval uint32_t get_type_id()
{
	detail::type_list l;
	detail::typeinfo<T>::get_types(l);
	return detail::ct_crc32(l.types.data(), l.types.size());
}

// This returns the name of T
template<typename T>
constexpr std::string_view get_type_name()
{
	return detail::get_t_name<T>();
}

template<options o, typename T, typename Container>
inline void pack(const T& obj, Container& out)
{
	bytebuffer_impl<Container> wrap(out, true);
	detail::bytes_converter<o & options::variable_length_encoding> bc(wrap);
	detail::typeinfo<T>::pack(const_cast<T&>(obj), bc);
}

template<options o, typename T, typename Container>
[[nodiscard]] inline status unpack(T& obj, Container& in)
{
	try {
		bytebuffer_impl<Container> wrap(in, false);
		detail::bytes_converter<o & options::variable_length_encoding> bc(wrap);
		detail::typeinfo<T>::unpack(obj, bc);
		return wrap.ok() ? status::ok : status::data_underrun;
	} catch(status s) {
		return s;
	}
}

template<typename T, typename Foreach>
inline void foreach_member(T& obj, Foreach& c)
{
	detail::typeinfo<T>::for_each(nullptr, obj, c);
}

static constexpr uint8_t kFirstUserType = static_cast<uint8_t>(detail::type_id::user_type);

template<typename T>
concept is_vectorlike_container = requires(T t, uint8_t v) {
	                                  t.push_back(v);
	                                  t.data();
	                                  t.size();
                                  };



template<is_vectorlike_container T>
struct bytebuffer_impl<T> : public bytebuffer
{
	bytebuffer_impl(T& o, bool write) : o(o), write(write)
	{
		// If you're not writing bytes then what?
		static_assert(sizeof(*o.data()) == sizeof(uint8_t));

		if(write)
			o.resize(256);
		s = p = o.data();
		e = s + o.size();
	}

	~bytebuffer_impl()
	{
		if(write)
			o.resize(p - s);
	}

	void more_data(size_t n) override
	{
		if(n > 0)
			throw status::data_underrun;
	}
	void more_buffer(size_t n) override
	{
		o.resize(o.size() + n + 256);
		ptrdiff_t delta = o.data() - s;
		s += delta;
		p += delta;
		e += delta;
	}
	void seek_to(size_t at) override
	{
		if(at >= o.size())
			throw status::data_underrun;
		p = s + at;
	}
	void fix_offset(size_t at, uint32_t n) override
	{
		memcpy(o.data() + at, &n, 4);
	}
	void flush_all() override {}

	T& o;
	bool write;
};

template<>
struct bytebuffer_impl<std::span<uint8_t>> : public bytebuffer
{
	bytebuffer_impl(std::span<uint8_t> o, bool write) : o(o)
	{
		if(write)
			throw status::write_disallowed;
		s = p = o.data();
		e = s + o.size();
	}

	~bytebuffer_impl()
	{
	}

	void more_data(size_t n) override
	{
		if(n > 0)
			throw status::data_underrun;
	}
	void more_buffer(size_t n) override
	{
	}
	void seek_to(size_t at) override
	{
		if(at >= o.size())
			throw status::data_underrun;
		p = s + at;
	}
	void fix_offset(size_t at, uint32_t n) override
	{
	}
	void flush_all() override {}

	std::span<uint8_t> o;
};

template<typename C, typename T>
struct bytebuffer_impl<std::basic_ostream<C, T>> : public bytebuffer
{
	bytebuffer_impl(std::basic_ostream<C, T>& o, bool write) : o(o)
	{
		if(!write)
			throw status::read_disallowed;

		vec.resize(1024);
		s = p = vec.data();
		e = s + vec.size();
	}

	~bytebuffer_impl()
	{
		o.write(s, p - s);
	}

	void more_data(size_t n) override
	{
	}
	void more_buffer(size_t n) override
	{
		o.write(s, p - s);
		p = s;
		if(n > vec.size()) {
			vec.resize(n);
			p = s = vec.data();
			e = p + n;
		}
	}
	void seek_to(size_t at) override
	{
	}
	void fix_offset(size_t at, uint32_t n) override
	{
		memcpy(o.data() + at, &n, 4);
	}
	void flush_all() override {}

	std::basic_ostream<C, T>& o;
	std::vector<uint8_t> vec;
};

template<typename C, typename T>
struct bytebuffer_impl<std::basic_istream<C, T>>
{
	bytebuffer_impl(std::basic_istream<C, T>& o) : o(o)
	{
	}
	~bytebuffer_impl()
	{
	}

	std::basic_istream<C, T>& o;
};

template<options opts = options::none>
using serializer_t = detail::bytes_converter<opts & options::variable_length_encoding>;

} // namespace packall

#endif
