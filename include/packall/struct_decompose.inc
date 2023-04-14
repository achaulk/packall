template<> struct decompose<1> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0] = o;
		if constexpr(I == 0) return _0;
	}
};
template<> struct decompose<2> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
	}
};
template<> struct decompose<3> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
	}
};
template<> struct decompose<4> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
	}
};
template<> struct decompose<5> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
	}
};
template<> struct decompose<6> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
	}
};
template<> struct decompose<7> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
	}
};
template<> struct decompose<8> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
	}
};
template<> struct decompose<9> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
	}
};
template<> struct decompose<10> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
	}
};
template<> struct decompose<11> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
	}
};
template<> struct decompose<12> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
	}
};
template<> struct decompose<13> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
	}
};
template<> struct decompose<14> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
	}
};
template<> struct decompose<15> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
	}
};
template<> struct decompose<16> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
	}
};
template<> struct decompose<17> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
	}
};
template<> struct decompose<18> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
	}
};
template<> struct decompose<19> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
	}
};
template<> struct decompose<20> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
	}
};
template<> struct decompose<21> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
	}
};
template<> struct decompose<22> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
	}
};
template<> struct decompose<23> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
	}
};
template<> struct decompose<24> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
	}
};
template<> struct decompose<25> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
	}
};
template<> struct decompose<26> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
	}
};
template<> struct decompose<27> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
	}
};
template<> struct decompose<28> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
	}
};
template<> struct decompose<29> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
	}
};
template<> struct decompose<30> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
	}
};
template<> struct decompose<31> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
	}
};
template<> struct decompose<32> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
	}
};
template<> struct decompose<33> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
	}
};
template<> struct decompose<34> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
	}
};
template<> struct decompose<35> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
	}
};
template<> struct decompose<36> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
	}
};
template<> struct decompose<37> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
	}
};
template<> struct decompose<38> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
	}
};
template<> struct decompose<39> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
	}
};
template<> struct decompose<40> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
	}
};
template<> struct decompose<41> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
	}
};
template<> struct decompose<42> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
		if constexpr(I == 41) return _41;
	}
};
template<> struct decompose<43> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
		if constexpr(I == 41) return _41;
		if constexpr(I == 42) return _42;
	}
};
template<> struct decompose<44> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
		if constexpr(I == 41) return _41;
		if constexpr(I == 42) return _42;
		if constexpr(I == 43) return _43;
	}
};
template<> struct decompose<45> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
		if constexpr(I == 41) return _41;
		if constexpr(I == 42) return _42;
		if constexpr(I == 43) return _43;
		if constexpr(I == 44) return _44;
	}
};
template<> struct decompose<46> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
		if constexpr(I == 41) return _41;
		if constexpr(I == 42) return _42;
		if constexpr(I == 43) return _43;
		if constexpr(I == 44) return _44;
		if constexpr(I == 45) return _45;
	}
};
template<> struct decompose<47> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
		if constexpr(I == 41) return _41;
		if constexpr(I == 42) return _42;
		if constexpr(I == 43) return _43;
		if constexpr(I == 44) return _44;
		if constexpr(I == 45) return _45;
		if constexpr(I == 46) return _46;
	}
};
template<> struct decompose<48> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
		if constexpr(I == 41) return _41;
		if constexpr(I == 42) return _42;
		if constexpr(I == 43) return _43;
		if constexpr(I == 44) return _44;
		if constexpr(I == 45) return _45;
		if constexpr(I == 46) return _46;
		if constexpr(I == 47) return _47;
	}
};
template<> struct decompose<49> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
		if constexpr(I == 41) return _41;
		if constexpr(I == 42) return _42;
		if constexpr(I == 43) return _43;
		if constexpr(I == 44) return _44;
		if constexpr(I == 45) return _45;
		if constexpr(I == 46) return _46;
		if constexpr(I == 47) return _47;
		if constexpr(I == 48) return _48;
	}
};
template<> struct decompose<50> {
	template<size_t I, class T>
	constexpr static auto& get(T&& o) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49] = o;
		if constexpr(I == 0) return _0;
		if constexpr(I == 1) return _1;
		if constexpr(I == 2) return _2;
		if constexpr(I == 3) return _3;
		if constexpr(I == 4) return _4;
		if constexpr(I == 5) return _5;
		if constexpr(I == 6) return _6;
		if constexpr(I == 7) return _7;
		if constexpr(I == 8) return _8;
		if constexpr(I == 9) return _9;
		if constexpr(I == 10) return _10;
		if constexpr(I == 11) return _11;
		if constexpr(I == 12) return _12;
		if constexpr(I == 13) return _13;
		if constexpr(I == 14) return _14;
		if constexpr(I == 15) return _15;
		if constexpr(I == 16) return _16;
		if constexpr(I == 17) return _17;
		if constexpr(I == 18) return _18;
		if constexpr(I == 19) return _19;
		if constexpr(I == 20) return _20;
		if constexpr(I == 21) return _21;
		if constexpr(I == 22) return _22;
		if constexpr(I == 23) return _23;
		if constexpr(I == 24) return _24;
		if constexpr(I == 25) return _25;
		if constexpr(I == 26) return _26;
		if constexpr(I == 27) return _27;
		if constexpr(I == 28) return _28;
		if constexpr(I == 29) return _29;
		if constexpr(I == 30) return _30;
		if constexpr(I == 31) return _31;
		if constexpr(I == 32) return _32;
		if constexpr(I == 33) return _33;
		if constexpr(I == 34) return _34;
		if constexpr(I == 35) return _35;
		if constexpr(I == 36) return _36;
		if constexpr(I == 37) return _37;
		if constexpr(I == 38) return _38;
		if constexpr(I == 39) return _39;
		if constexpr(I == 40) return _40;
		if constexpr(I == 41) return _41;
		if constexpr(I == 42) return _42;
		if constexpr(I == 43) return _43;
		if constexpr(I == 44) return _44;
		if constexpr(I == 45) return _45;
		if constexpr(I == 46) return _46;
		if constexpr(I == 47) return _47;
		if constexpr(I == 48) return _48;
		if constexpr(I == 49) return _49;
	}
};
