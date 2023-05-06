// This is an optional text parser. It requires that each struct be set up for reflection with member names provided.

#ifndef PACKALL_TEXT_H_
#define PACKALL_TEXT_H_

#include "packall.h"

#include <charconv>

namespace packall {
struct parse_options
{
	// Maximum parse depth allowed. Returns stack_overflow is this is exceeded.
	uint32_t max_depth = 256;
	// Allow unknown names for structs
	bool allow_unknown_keys = true;
	// Leaves the variant default-constructed if no types can parse the value
	bool allow_unknown_variant_values = true;
	// Ignore extra entries for tuples
	bool allow_unknown_tuple_elements = true;
	// Ignore extra entries for std::array & C arrays
	bool allow_extra_array_entries = true;

	bool skip_initial_scope = false;
};

struct format_options
{
	// If set, omit any value that is a default value
	bool omit_default = false;
	// If set, omit all struct keys
	bool omit_names = false;

	bool skip_initial_scope = false;
};

template<typename T>
status parse(T& obj, std::string_view text, const parse_options& opts);

namespace detail {
template<typename T>
concept is_custom_text_serialized = requires(T t) {
	                                    t.parse;
	                                    t.format;
                                    };

template<typename T>
constexpr bool is_default_value(const T&)
{
	return false;
}

template<std::equality_comparable T>
constexpr bool is_default_value(const T& o)
{
	return o == T{};
}

template<typename T>
struct parser;

struct parse_state
{
	parse_options opts;
	const char *s, *e;
	std::string_view token;
	uint32_t depth = 0;

	bool maybe_nil()
	{
		if(e - s >= 3 && s[0] == 'n' && s[1] == 'i' && s[2] == 'l') {
			s += 3;
			return true;
		}
		return false;
	}

	void next()
	{
		skip_ws();
	}
	void skip_ws()
	{
		while(s < e && (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n')) s++;
		if(s + 1 < e && s[0] == '-' && s[1] == '-') {
			// comment!
			s += 2;
			skip_comment();
			skip_ws();
		}
	}
	void skip_comment()
	{
		if(s + 2 < e && *s == '[') {
			int n = 0;
			auto p = s;
			while(p < e && *p == '=') n++, p++;
			if(e != p && *p == '[') {
				s = p + 1;
				parse_long_string_impl(n);
				return;
			}
		}
		while(s < e && *s != '\n') s++;
	}
	void expect(char ch)
	{
		if(s == e || *s != ch)
			throw status::bad_format;
		s++;
		skip_ws();
	}
	void consume(char ch)
	{
		if(s != e && *s == ch) {
			s++;
			skip_ws();
		}
	}
	bool maybe(char ch)
	{
		if(*s == ch) {
			s++;
			skip_ws();
			return true;
		}
		return false;
	}
	std::string_view parse_ident()
	{
		if((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || *s == '_') {
			const char *start = s++;
			while(
			    s < e && (*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || *s == '_' || (*s >= '0' && *s <= '9'))
				s++;
			const char *end = s;
			skip_ws();
			return std::string_view(start, end);
		}
		throw status::bad_format;
	}

	void skip_element()
	{
		throw status::bad_data;
	}

	status finish()
	{
		return status::ok;
	}

	std::string_view table_key;
	bool table_kv;

	std::string_view parse_immediate_short_string()
	{
		char ch = *s;
		const char *start = ++s;
		while(s < e && *s != ch) s++;
		if(s == e)
			throw status::bad_format;
		table_key = std::string_view(start, s);
		s++;
		skip_ws();
		return std::string_view(start, s - 1);
	}
	static bool check_long_string_end(const char *at, int n)
	{
		if(*at != ']')
			return false;
		at--;
		for(int i = 0; i < n; i++)
			if(*at != '=')
				return false;
		return *at == ']';
	}
	std::string_view parse_long_string()
	{
		int n = 0;
		while(s < e && *s == '=') n++, s++;
		if(e - s < 2 || *s != '[')
			throw status::bad_format;
		s++;
		return parse_long_string_impl(n);
	}

	std::string_view parse_long_string_impl(int n)
	{
		const char *start = s;
		while(s < e && !check_long_string_end(s, n)) s++;

		const char *end = s - 1 - n;
		s++;
		skip_ws();
		return std::string_view(start, end);
	}

	void table_begin()
	{
		expect('{');
	}
	void table_end()
	{
		expect('}');
	}
	bool table_array_implicit_key()
	{
		return !maybe('}');
	}
	bool table_literal_key()
	{
		if(maybe('}'))
			return false;
		if((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || *s == '_') {
			const char *start = s++;
			while(
			    s < e && (*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || *s == '_' || (*s >= '0' && *s <= '9'))
				s++;
			table_key = std::string_view(start, s);
			skip_ws();
			if(s == e || *s != '=')
				return false;
			s++;
			skip_ws();
			return true;
		}
		if(*s == '[') {
			const char *o = s;
			s++;
			skip_ws();
			if(*s == '"' || *s == '\'') {
				// short string
				table_key = parse_immediate_short_string();
				expect(']');
				return true;
			}
			table_key = parse_long_string();
			skip_ws();
			if(*s == '=') {
				return true;
			}
			s = o;
		}
		return false;
	}
	bool table_next()
	{
		if(*s == ';' || *s == ',') {
			s++;
			skip_ws();
			return true;
		}
		return false;
	}

	template<typename T>
	void parse_primitive(T& v)
	{
		auto r = std::from_chars(s, e, v);
		if(r.ec == std::errc::invalid_argument || r.ec == std::errc::result_out_of_range)
			throw status::bad_format;
		s = r.ptr;
	}
	void parse_primitive(bool& v)
	{
		auto str = parse_ident();
		if(str == "true")
			v = true;
		else if(str == "false")
			v = false;
		else
			throw status::bad_format;
	}

	std::string parse_string()
	{
		if(*s == '"' || *s == '\'') {
			std::string str;
			char ch = *s;
			const char *start = ++s;
			while(s < e && *s != ch) {
				str.push_back(*s++);
			}
			if(s == e)
				throw status::bad_format;
			s++;
			skip_ws();
			return str;
		}

		if(*s == '[') {
			s++;
			return std::string(parse_long_string());
		}

		throw status::bad_format;
	}
};

template<typename K>
struct key_parser
{
	static bool parse_key(K& obj, parse_state& s, size_t& i)
	{
		if(s.maybe('}'))
			return false;
		s.expect('[');
		parser<K>::parse(obj, s);
		s.expect(']');
		s.expect('=');
		return true;
	}
};

template<>
struct key_parser<std::string>
{
	static bool parse_key(std::string& obj, parse_state& s, size_t& i)
	{
		if(s.maybe('}'))
			return false;
		// Strings are the most diverse, allowing for unquoted, single quoted and double quoted
		if(*s.s == '[' || *s.s == '"' || *s.s == '\'') {
			obj = s.parse_string();
		} else {
			obj = s.parse_ident();
		}
		s.expect('=');
		return true;
	}
};

template<std::integral K>
struct key_parser<K>
{
	static bool parse_key(K& obj, parse_state& s, size_t& i)
	{
		if(s.maybe('}'))
			return false;
		if(s.maybe('[')) {
			parser<K>::parse(obj, s);
			s.expect(']');
			s.expect('=');
			return true;
		}
		// If integers are allowed, then we can autoassign
		obj = static_cast<K>(i++);
		return true;
	}
};

template<typename... V>
struct key_parser<std::variant<V...>>
{
	using type = std::variant<V...>;

	// Variant keys are tricky because they might accept strings and/or integers
	static bool parse_key(type& obj, parse_state& s, size_t& i)
	{
		abort();
	}
};

// Mirror the main set of structs

struct seqparser
{
	parse_state& s;
	bool done = false;

	constexpr void enter(std::string_view name) {}
	constexpr void leave() {}

	template<typename U>
	void visit(uint32_t index, const char *name, U& obj)
	{
		if(!done) {
			parser<U>::parse(obj, s);
			if(!s.table_next()) {
				s.table_end();
				done = true;
			}
		}
	}
};

struct nameparser
{
	parse_state& s;
	uint32_t target;

	constexpr void enter(std::string_view name) {}
	constexpr void leave() {}

	template<typename U>
	void visit(uint32_t index, const char *name, U& obj)
	{
		if(index == target) {
			parser<U>::parse(obj, s);
		}
	}
};

template<is_aggregate_struct T>
struct parser<T>
{
	static void parse(T& obj, parse_state& s)
	{
		if(s.depth++ > s.opts.max_depth) [[unlikely]] {
			throw status::stack_overflow;
		}
		bool skip = s.opts.skip_initial_scope;
		s.opts.skip_initial_scope = false;
		if(!skip) {
			[[likely]] s.table_begin();
			if(s.maybe('}'))
				return;
		}
		if(s.table_literal_key()) {
			do {
				uint32_t index = get_member_index<T>(s.table_key);
				if(index > typeinfo<T>::Arity) [[unlikely]] {
					if(!s.opts.allow_unknown_keys) [[unlikely]] {
						throw status::unknown_key;
					}
					// Need to skip
					s.skip_element();
				} else {
					nameparser p{s, index};
					foreach_member(obj, p);
				}
				if(!s.table_next()) {
					if(!skip) [[likely]]
						s.table_end();
					break;
				}
			} while(s.table_literal_key());
		} else {
			seqparser seq{s};
			foreach_member(obj, seq);
			if(!skip) [[likely]]
				s.table_end();
		}
		s.depth--;
	}

	// This checks if it's worth maybe trying to parse this
	static bool precheck_parse(const char *s, const char *e)
	{
		return *s == '{';
	}
};

// T[N] & array<T, N>
template<typename T>
    requires(std::is_aggregate_v<T> && is_array_type<T>::value)
struct parser<T>
{
	using type = typename is_array_type<T>::type;
	static constexpr uint32_t N = is_array_type<T>::kN;

	static void parse(T *obj, parse_state& s)
	{
		s.table_begin();
		size_t i = 0;
		while(s.table_array_implicit_key()) {
			if(i < N) {
				parser<type>::parse(obj[i++], s);
			} else {
				type _;
				parser<type>::parse(_, s);
			}
			if(!s.table_next()) {
				s.table_end();
				break;
			}
		}
	}

	static void parse(std::array<type, N>& obj, parse_state& s)
	{
		s.table_begin();
		size_t i = 0;
		while(s.table_array_implicit_key()) {
			if(i < N) {
				parser<type>::parse(obj[i++], s);
			} else {
				type _;
				parser<type>::parse(_, s);
			}
			if(!s.table_next()) {
				s.table_end();
				break;
			}
		}
	}
	static bool precheck_parse(const char *s, const char *e)
	{
		return *s == '{';
	}
};

template<>
struct parser<bool>
{
	static void parse(bool& obj, parse_state& s)
	{
		s.parse_primitive(obj);
	}
	static bool precheck_parse(const char *s, const char *e)
	{
		return *s == 't' || *s == 'f';
	}
};

template<std::integral T>
struct parser<T>
{
	static void parse(T& obj, parse_state& s)
	{
		int base = 10;
		if(s.s[0] == '0' && s.s[1] == 'x') {
			s.s += 2;
			base = 16;
		}
		auto r = std::from_chars(s.s, s.e, obj, base);
		if(r.ec == std::errc::invalid_argument || r.ec == std::errc::result_out_of_range)
			throw status::bad_format;
		s.s = r.ptr;
	}
	static bool precheck_parse(const char *s, const char *e)
	{
		// Hex & octal start with 0
		return *s == '-' || (*s >= '0' && *s <= '9');
	}
};

template<std::floating_point T>
struct parser<T>
{
	static void parse(T& obj, parse_state& s)
	{
		s.parse_primitive(obj);
	}
	static bool precheck_parse(const char *s, const char *e)
	{
		// Hex & octal start with 0
		return *s == '-' || *s == '.' || (*s >= '0' && *s <= '9');
	}
};

template<>
struct parser<std::string>
{
	static void parse(std::string& obj, parse_state& s)
	{
		obj = s.parse_string();
	}
	static bool precheck_parse(const char *s, const char *e)
	{
		return *s == '\'' || *s == '"' || *s == '[';
	}
};

template<is_listlike T>
struct parser<T>
{
	static void parse(T& obj, parse_state& s)
	{
		s.table_begin();
		while(s.table_array_implicit_key()) {
			obj.emplace_back();
			parser<typename T::value_type>::parse(obj.back(), s);
			if(!s.table_next()) {
				s.table_end();
				break;
			}
		}
	}
	static bool precheck_parse(const char *s, const char *e)
	{
		return *s == '{';
	}
};

template<typename T, typename U>
struct parser<std::pair<T, U>>
{
	using type = std::pair<T, U>;

	static void parse(type& obj, parse_state& s)
	{
		s.table_begin();
		if(!s.table_array_implicit_key())
			throw status::bad_format;
		parser<T>::parse(obj.first, s);
		if(!s.table_next())
			throw status::bad_format;
		if(!s.table_array_implicit_key())
			throw status::bad_format;
		parser<T>::parse(obj.second, s);
		s.table_next();
		s.table_end();
	}
	static bool precheck_parse(const char *s, const char *e)
	{
		return *s == '{';
	}
};

template<is_maplike T>
struct parser<T>
{
	using K = typename T::key_type;
	static void parse(T& obj, parse_state& s)
	{
		s.table_begin();
		size_t implicit = 0;
		K k;
		while(key_parser<K>::parse_key(k, s, implicit)) {
			parser<typename T::mapped_type>::parse(obj[k], s);
			s.table_next();
		}
	}
	static bool precheck_parse(const char *s, const char *e)
	{
		return *s == '{';
	}
};

template<typename... V>
struct parser<std::variant<V...>>
{
	using type = std::variant<V...>;

	static void parse(type& obj, parse_state& s)
	{
		parse_helper(obj, s, std::make_index_sequence<sizeof...(V)>());
	}
	template<size_t... Index>
	static void parse_helper(type& obj, parse_state& s, std::index_sequence<Index...>)
	{
		if(!(maybe_parse<Index>(obj, s) || ...)) {
			if(!s.opts.allow_unknown_variant_values) [[unlikely]]
				throw status::bad_variant_value;
			s.skip_element();
		}
	}
	template<size_t I>
	static bool maybe_parse(type& obj, parse_state& s)
	{
		if(!parser<std::variant_alternative_t<I, type>>::precheck_parse(s.s, s.e))
			return false;
		const char *at = s.s;
		try {
			std::variant_alternative_t<I, type> v{};
			parser<std::variant_alternative_t<I, type>>::parse(v, s);
			obj = v;
			return true;
		} catch(status) {
			s.s = at;
			return false;
		}
	}
};

template<typename... V>
struct parser<std::tuple<V...>>
{
	using type = std::tuple<V...>;

	static void parse(type& obj, parse_state& s)
	{
		parse_helper(obj, s, std::make_index_sequence<sizeof...(V)>());
	}
	template<size_t... Index>
	static void parse_helper(type& obj, parse_state& s, std::index_sequence<Index...>)
	{
		s.table_begin();
		(maybe_parse<Index>(obj, s) && ...);
		while(s.table_array_implicit_key()) s.skip_element();
	}

	template<size_t Index>
	static bool maybe_parse(type& obj, parse_state& s)
	{
		if(!s.table_array_implicit_key())
			return false;
		parser<std::tuple_element_t<Index, type>>::parse(std::get<Index>(obj), s);
		return true;
	}
};

struct writer_state
{
	format_options opts;
	std::string o;

	void newscope()
	{
		o.push_back('{');
	}
	void endscope()
	{
		o.push_back('}');
	}
	void next()
	{
		o.push_back(',');
	}
	void prefix() {}

	void writestr(std::string_view s)
	{
		size_t at = o.size();
		o.reserve(at + s.size() + 8);
		o.append("  [[");
		int n = 0;
		int level = -1;
		const char *raw = s.data();
		for(size_t i = 0; i < s.size(); i++) {
			o.push_back(raw[i]);
			if(raw[i] == ']') {
				if(level == -1) {
					level = 0;
				} else if(level > n) {
					n = level;
					level = -1;
				}
			} else if(raw[i] == '=' && level >= 0) {
				level++;
			} else {
				level = -1;
			}
		}

		if(n > 2) [[unlikely]] {
			o[at] = '=';
			o[at + 1] = '=';
			o[at + 2] = '=';
			std::string extra;
			for(int i = 3; i < n; i++) extra.push_back('=');
			extra.push_back('[');
			o.insert(at, extra);
		} else if(n > 0) [[unlikely]] {
			o[at + 2] = '=';
			o[at + 1] = '=';
			o[at + 2 - n] = '[';
		}

		o.push_back(']');
		for(int i = 0; i < n; i++) o.push_back('=');
		o.push_back(']');
	}
};

template<typename T>
struct writer;

template<typename T>
struct key_writer
{
	static void write(const T& obj, writer_state& s)
	{
		s.o.push_back('[');
		writer<T>::write(obj, s);
		s.o.push_back(']');
	}
};

template<>
struct key_writer<std::string>
{
	static void write(const std::string& obj, writer_state& s)
	{
		s.writestr(obj);
	}
};

template<is_maplike T>
struct writer<T>
{
	static void write(const T& obj, writer_state& s)
	{
		s.newscope();
		for(const auto& [k, v] : obj) {
			s.prefix();
			key_writer<typename T::key_type>::write(k, s);
			s.o.push_back('=');
			writer<typename T::mapped_type>::write(v, s);
			s.next();
		}
		s.endscope();
	}
};

template<is_setlike T>
struct writer<T>
{
	static void write(const T& obj, writer_state& s)
	{
		s.newscope();
		for(const auto& k : obj) {
			s.prefix();
			key_writer<typename T::key_type>::write(*k, s);
			s.o.append("=true");
			s.next();
		}
		s.endscope();
	}
};

template<is_aggregate_struct T>
struct writer<T>
{
	writer_state& s;
	uint32_t n = 0;

	static void write(const T& obj, writer_state& s)
	{
		size_t at = s.o.size();
		bool skip = s.opts.skip_initial_scope;
		s.opts.skip_initial_scope = false;
		if(!skip) [[likely]]
			s.newscope();
		writer<T> wr{s};
		foreach_member(const_cast<T&>(obj), wr);
		if(!skip) [[likely]]
			s.endscope();
		if(wr.n == 0 && s.opts.omit_default)
			s.o.resize(at);
	}

	constexpr void enter(std::string_view name) {}
	constexpr void leave() {}

	template<typename U>
	void visit(uint32_t index, const char *name, U& obj)
	{
		if(s.opts.omit_default && is_default_value(obj))
			return;
		n++;
		// Generally a C++ name can also be a valid Lua name with the only difference being keywords
		// and break do else false for goto if not or return true while are C++ keywords too
		// elseif end function in local nil repeat then until are not
		s.prefix();
		if(name && !s.opts.omit_names) {
			s.o.append(name);
			s.o.push_back('=');
		}
		writer<U>::write(obj, s);
		s.next();
	}
};

// T[N] & array<T, N>
template<typename T>
    requires(std::is_aggregate_v<T> && is_array_type<T>::value)
struct writer<T>
{
	using type = typename is_array_type<T>::type;
	static constexpr uint32_t N = is_array_type<T>::kN;

	static void write(const T *obj, writer_state& s)
	{
		s.newscope();
		for(uint32_t i = 0; i < N; i++) {
			s.prefix();
			writer<T>::write(obj[i], s);
			s.next();
		}
		s.endscope();
	}
	static void write(const std::array<type, N>& obj, writer_state& s)
	{
		s.newscope();
		for(uint32_t i = 0; i < N; i++) {
			s.prefix();
			writer<type>::write(obj[i], s);
			s.next();
		}
		s.endscope();
	}
};

template<is_container T>
struct writer<T>
{
	using type = T;
	static void write(const type& obj, writer_state& s)
	{
		s.newscope();
		for(auto& v : obj) {
			s.prefix();
			writer<typename T::value_type>::write(v, s);
			s.next();
		}
		s.endscope();
	}
};

template<std::integral T>
struct writer<T>
{
	static void write(T obj, writer_state& s)
	{
		char temp[32];
		auto r = std::to_chars(temp, temp + sizeof(temp), obj);
		s.o.append(temp, r.ptr);
	}
};

template<std::floating_point T>
struct writer<T>
{
	static void write(T obj, writer_state& s)
	{
		char temp[32];
		auto r = std::to_chars(temp, temp + sizeof(temp), obj);
		s.o.append(temp, r.ptr);
	}
};

template<>
struct writer<bool>
{
	static void write(bool obj, writer_state& s)
	{
		s.o.append(obj ? "true" : "false");
	}
};

template<>
struct writer<std::string>
{
	using type = std::string;
	static void write(const type& obj, writer_state& s)
	{
		s.writestr(obj);
	}
};

template<typename T, typename U>
struct writer<std::pair<T, U>>
{
	using type = std::pair<T, U>;
	static void write(const type& obj, writer_state& s)
	{
		s.o.push_back('{');
		writer<T>::write(obj.first, s);
		s.o.push_back(',');
		writer<U>::write(obj.second, s);
		s.o.push_back('}');
	}
};

template<typename... V>
struct writer<std::variant<V...>>
{
	using type = std::variant<V...>;

	static void write(const type& obj, writer_state& s)
	{
		write_helper(obj, s, std::make_index_sequence<sizeof...(V)>());
	}

	template<size_t... Index>
	static void write_helper(const type& obj, writer_state& s, std::index_sequence<Index...>)
	{
		size_t index = obj.index();
		(maybe_write<Index>(obj, s, index) || ...);
	}
	template<size_t I>
	static bool maybe_write(const type& obj, writer_state& s, size_t index)
	{
		if(I == index) {
			writer<std::variant_alternative_t<I, type>>::write(std::get<I>(obj), s);
			return true;
		}
		return false;
	}
};

template<typename... V>
struct writer<std::tuple<V...>>
{
	using type = std::tuple<V...>;

	static bool write(const type& obj, writer_state& s)
	{
		size_t at = s.o.size();
		s.newscope();
		if(write_helper(obj, s, std::make_index_sequence<sizeof...(V)>()) || !s.opts.omit_default) {
			s.endscope();
			return true;
		}
		s.o.resize(at);
		return false;
	}

	template<size_t... Index>
	static size_t write_helper(const type& obj, writer_state& s, std::index_sequence<Index...>)
	{
		return (write_one<Index>(obj, s) + ...);
	}

	template<size_t I>
	static size_t write_one(const type& obj, writer_state& s)
	{
		if(s.opts.omit_default && is_default_value(std::get<I>(obj)))
			return 0;
		writer<std::tuple_element_t<I, type>>::write(std::get<I>(obj), s);
		s.next();
		return 1;
	}
};

template<is_custom_text_serialized T>
struct writer<T>
{
	using type = T;
	static void write(type& obj, writer_state& s)
	{
		obj.format(s);
	}
};

template<is_custom_text_serialized T>
struct writer<T *>
{
	using type = T;
	static void write(type *obj, writer_state& s)
	{
		if(obj)
			obj->format(s);
		else
			s.o.append("nil");
	}
};

template<is_custom_text_serialized T>
struct parser<T>
{
	using type = T;
	static void parse(type& obj, parse_state& s)
	{
		obj.parse(s);
	}
};

template<is_custom_text_serialized T>
struct parser<T *>
{
	using type = T;
	static void parse(type *obj, parse_state& s)
	{
		if(s.maybe_nil())
			return;
		if(obj)
			obj->parse(s);
		else
			throw status::bad_data;
	}
};

template<typename T>
struct parser<std::unique_ptr<T>>
{
	using type = T;
	static void parse(type& obj, parse_state& s)
	{
		if(s.maybe_nil())
			return;
		obj = std::make_unique<T>();
		parser<T>::parser(*obj, s);
	}
};

template<typename T>
struct writer<std::unique_ptr<T>>
{
	using type = T;
	static void parse(type& obj, writer_state& s)
	{
		if(!obj) {
			s.o.append("nil");
			return;
		}
		writer<T>::write(*obj, s);
	}
};

template<typename T>
struct parser<std::optional<T>>
{
	using type = T;
	static void parse(type& obj, parse_state& s)
	{
		if(s.maybe_nil())
			return;
		obj.emplace();
		parser<T>::parser(*obj, s);
	}
};

template<typename T>
struct writer<std::optional<T>>
{
	using type = T;
	static void parse(type& obj, writer_state& s)
	{
		if(!obj) {
			s.o.append("nil");
			return;
		}
		writer<T>::write(*obj, s);
	}
};

struct prettyprint_state
{
	std::string s;
	int scope = 0;
};

inline const char *pp_shortstr(std::string& s, const char *p, const char *e)
{
	char ch = *p++;
	s.push_back(ch);
	while(p < e) {
		if(*p == ch) {
			break;
		}
		s.push_back(*p);
	}
	s.push_back(ch);
	return p + 1;
}

inline const char *pp_longstr(std::string& s, const char *p, const char *e, uint32_t& info)
{
	const char *start = p;

	p++;
	int level = 0;
	while(p < e && *p == '=') {
		p++, level++;
	}
	if(*p != '[')
		return e;
	p++;

	int match = -1;

	bool nonident = !((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_');
	bool needs_escapes = false;
	bool sq = false;
	bool dq = false;

	while(p < e) {
		if(*p == '=' && match >= 0) {
			match++;
		} else if(*p == ']') {
			if(match == level) {
				p++;
				break;
			}
			match++;
		} else {
			nonident = nonident ||
			           !((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_');
			needs_escapes = needs_escapes || (*p == '\n' || *p == '\r' || *p == '\\');
			sq = sq || *p == '\'';
			dq = dq || *p == '"';
			match = -1;
		}
		p++;
	}

	s.append(start, p);

	info = ((uint32_t)level << 5) | (needs_escapes ? 1 : 0) | (sq ? 2 : 0) | (dq ? 4 : 0) | (nonident ? 8 : 0) | 0x10;

	return p;
}

inline const char *prettyprint(std::string& s, const char *p, const char *e, int scope = 0)
{
	int original_scope = scope;
	int state = 1; // 0 = after newline, 1 = after data, 2 = after assign,
	uint32_t longstrinfo = 0;
	size_t longstrstart = 0;
	while(p < e) {
		if(*p == '{') {
			scope++;
			s.push_back('{');
			if(original_scope == 0 || scope != original_scope + 1) {
				s.push_back('\n');
				s.append(scope, '\t');
			}
			state = 0;
			longstrinfo = 0;
		} else if(*p == '}') {
			if(state == 1) {
				s.push_back('\n');
				s.append(scope, '\t');
			}
			if(s.back() == '\t')
				s.pop_back();
			scope--;
			s.push_back('}');
			longstrinfo = 0;
		} else if(*p == ',') {
			s.push_back(',');
			s.push_back('\n');
			s.append(scope, '\t');
			state = 0;
			longstrinfo = 0;
		} else if(*p == '\'' || *p == '"') {
			// short string
			state = 1;
			p = pp_shortstr(s, p, e);
			longstrinfo = 0;
			continue;
		} else if(*p == ']' && scope == original_scope) {
			s.push_back(']');
			return p + 1;
		} else if(*p == '[') {
			// enclosed expr or long string
			state = 1;
			if(p[1] == '=' || p[1] == '[') {
				longstrstart = s.size();
				p = pp_longstr(s, p, e, longstrinfo);
			} else {
				s.push_back('[');
				p = prettyprint(s, p + 1, e, scope);
			}
			continue;
		} else if(*p == '=') {
			if((longstrinfo & 0x1F) == 0x10) {
				uint32_t level = longstrinfo >> 5;
				s.resize(s.size() - 2 - level);
				s.erase(s.begin() + longstrstart, s.begin() + longstrstart + 2 + level);
			}
			state = 2;
			s.append(" = ");
			p++;
			longstrinfo = 0;
			continue;
		} else if(*p > ' ') {
			// non-whitespace
			state = 1;
			s.push_back(*p);
		}
		p++;
	}
	return p;
}

} // namespace detail

template<typename T>
inline status parse(T& obj, std::string_view text, const parse_options& opts = parse_options())
{
	detail::parse_state s{opts, text.data(), text.data() + text.size()};
	try {
		s.skip_ws();
		detail::parser<T>::parse(obj, s);
		return s.finish();
	} catch(status s) {
		return s;
	}
}

template<typename T>
inline void format(const T& obj, std::string& text, const format_options& opts = format_options())
{
	detail::writer_state s{opts};
	detail::writer<T>::write(obj, s);
	text.swap(s.o);
}

inline std::string prettyprint(std::string_view in)
{
	std::string s;
	s.reserve((in.size() * 3) / 2);
	detail::prettyprint(s, in.data(), in.data() + in.size());
	return s;
}

} // namespace packall

#endif
