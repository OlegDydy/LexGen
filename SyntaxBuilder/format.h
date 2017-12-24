#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <locale>

typedef std::string(*custom_to_string)(const void* const obj);
typedef std::wstring(*custom_to_wstring)(const void* const obj);

struct variant {
	bool free;
public:
	enum variant_t :char {
		v_int8, v_uint8, v_int16, v_uint16, v_int32, v_uint32, v_int64, v_uint64,
		v_float, v_double, v_ldouble,
		v_char, v_wchar, v_string, v_wstring,
		v_pointer,
		v_custom
	} type;

	union
	{
		int8_t i8;
		uint8_t u8;
		int16_t i16;
		uint16_t u16;
		int32_t i32;
		uint32_t u32;
		int64_t i64;
		uint64_t u64;
		float f;
		double d;
		long double ld;
		char ch;
		wchar_t wch;
		const void *ptr;
	} var;
	std::string str;
	std::wstring wstr;
	variant(int8_t p) { free = false; type = v_int8; var.i8 = p; };
	variant(uint8_t p) { free = false; type = v_uint8; var.u8 = p; };
	variant(int16_t p) { free = false; type = v_int16; var.i16 = p; };
	variant(uint16_t p) { free = false; type = v_uint16; var.u16 = p; };
	variant(int32_t p) { free = false; type = v_int32; var.i32 = p; };
	variant(uint32_t p) { free = false; type = v_uint32; var.u32 = p; };
	variant(int64_t p) { free = false; type = v_int64; var.i64 = p; };
	variant(uint64_t p) { free = false; type = v_uint64; var.u64 = p; };

	variant(float p) { free = false; type = v_float; var.f = p; };
	variant(double p) { free = false; type = v_double; var.d = p; };
	variant(long double p) { free = false; type = v_ldouble; var.ld = p; };

	variant(char p) { free = false; type = v_char; var.ch = p; };
	variant(wchar_t p) { free = false; type = v_wchar; var.wch = p; };
	variant(const std::string &p) { free = false; type = v_string; str = p; };
	variant(const std::wstring &p) { free = false; type = v_wstring; wstr = p; };
	variant(const char* p) { 
		free = false; type = v_string; 
		str = std::string(p); };
	variant(const wchar_t* p) { free = false; type = v_wstring; wstr = std::wstring(p); };
	//	variant(const void *p) { free = false; type = v_pointer; var.ptr = p; };

	~variant() {

	}

	custom_to_string ToString;
	custom_to_wstring ToWString;
};

inline
std::basic_ostream<char>& operator<<(
	std::basic_ostream<char>& _Ostr,
	const variant& _var) {
	//uint32_t codepage = (*(__crt_locale_data_public*)_get_current_locale()->locinfo)._locale_lc_codepage;
	//MultiByteToWideChar(Locale)

	switch (_var.type)
	{
	case variant::v_int8: _Ostr << (short)_var.var.i8; break;
	case variant::v_uint8: _Ostr << (short)_var.var.u8; break;
	case variant::v_int16: _Ostr << _var.var.i16; break;
	case variant::v_uint16: _Ostr << _var.var.u16; break;
	case variant::v_int32: _Ostr << _var.var.i32; break;
	case variant::v_uint32: _Ostr << _var.var.u32; break;
	case variant::v_int64: _Ostr << _var.var.i64; break;
	case variant::v_uint64: _Ostr << _var.var.u64; break;

	case variant::v_float: _Ostr << _var.var.f; break;
	case variant::v_double: _Ostr << _var.var.d; break;
	case variant::v_ldouble: _Ostr << _var.var.ld; break;

	case variant::v_char: _Ostr << _var.var.ch; break;
//	case variant::v_wchar: _Ostr << _var.var.wch; break;
	case variant::v_string: _Ostr << _var.str; break;
//	case variant::v_wstring: _Ostr << *_var.var.wstr; break;
	case variant::v_pointer: _Ostr << _var.var.ptr; break;

	case variant::v_custom:
		if (_var.ToString)
			_Ostr << _var.ToString(_var.var.ptr);
		else
			_Ostr << _var.var.ptr;
	}
	return _Ostr;
}

inline
std::basic_ostream<wchar_t>& operator<<(
	std::basic_ostream<wchar_t>& _Ostr,
	const variant& _var) {

	switch (_var.type)
	{
	case variant::v_int8: _Ostr << (short)_var.var.i8; break;
	case variant::v_uint8: _Ostr << (short)_var.var.u8; break;
	case variant::v_int16: _Ostr << _var.var.i16; break;
	case variant::v_uint16: _Ostr << _var.var.u16; break;
	case variant::v_int32: _Ostr << _var.var.i32; break;
	case variant::v_uint32: _Ostr << _var.var.u32; break;
	case variant::v_int64: _Ostr << _var.var.i64; break;
	case variant::v_uint64: _Ostr << _var.var.u64; break;

	case variant::v_float: _Ostr << _var.var.f; break;
	case variant::v_double: _Ostr << _var.var.d; break;
	case variant::v_ldouble: _Ostr << _var.var.ld; break;

//	case variant::v_char: _Ostr << _var.var.ch; break;
	case variant::v_wchar: _Ostr << _var.var.wch; break;
//	case variant::v_string: _Ostr << *_var.var.str; break;
	case variant::v_wstring: _Ostr << _var.wstr; break;
	case variant::v_pointer: _Ostr << _var.var.ptr; break;

	case variant::v_custom:
		if (_var.ToWString)
			_Ostr << _var.ToWString(_var.var.ptr);
		else
			_Ostr << _var.var.ptr;
	}
	std::to_string(12);
	return _Ostr;
}

std::string format(std::string fmt, const std::vector<variant> &par);