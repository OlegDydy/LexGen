#include "format.h"
#include <sstream>

std::string format(std::string fmt, const std::vector<variant> &par) {
	std::istringstream ss;
	std::ostringstream result;
	size_t prev = 0;
	size_t j = 0;
	size_t id;
	for (size_t i = 0; i < fmt.size(); i++) {
		if (fmt[i] == L'{') {
			// screen
			if (i == j)
				j = 0;
			if (j != 0)
				throw std::exception("Left bracket { need to be screened with additional one {");
			j = i + 1;
		}
		if (fmt[i] == L'}' && j) {
			result << fmt.substr(prev, j - prev - 1);
			ss = std::istringstream(fmt.substr(j,i-j));
			ss >> id;
			if (id > par.size())
				throw std::exception("Left bracket { need to be screened with additional one {");

			result << par[id];

			prev = i + 1;
			j = 0;
		}
	}
	std::string s = result.str();
	while ((j = s.find("{{")) != s.npos) {
		s.replace(j, 2, 1, '{');
	}
	return s;
}


const char TwoDigitLookup[100][3] =
{ "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
"10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
"20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
"30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
"40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
"50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
"60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
"70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
"80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
"90", "91", "92", "93", "94", "95", "96", "97", "98", "99" };

std::string _IntToStr32(uint32_t Value, bool Negative) {
	uint32_t I, J, K;
	int32_t Digits;
	char* P;
	int32_t NewLen;
	char result[12];

	I = Value;
	if (I >= 10000)
		if (I >= 1000000)
			if (I >= 100000000)
				Digits = 9 + (char)(I >= 1000000000);
			else
				Digits = 7 + (char)(I >= 10000000);
		else
			Digits = 5 + (char)(I >= 100000);
	else
		if (I >= 100)
			Digits = 3 + (char)(I >= 1000);
		else
			Digits = 1 + (char)(I >= 10);
	NewLen = Digits + (char)(Negative);

	P = result;
	*P = '-';
	P += char(Negative);
	if (Digits > 2) {
		do {
			J = I / 100;	/*/idend / 100*/
			K = J * 100;
			K = I - K;		/*dividend mod 100*/
			I = J;			/*Next dividend*/
			Digits -= 2;
			*(uint16_t*)(P + Digits) = *(uint16_t*)(TwoDigitLookup[K]);
		} while (Digits > 2);
	}
	if (Digits == 2)
		*	(uint16_t*)(P + Digits - 2) = *(uint16_t*)(TwoDigitLookup[I]);
	else
		*P = I | '0';

	return result;
}

std::string _IntToStr64(uint64_t Value, bool Negative) {
	uint64_t I64, J64, K64;
	uint32_t I32, J32, K32, L32;
	uint8_t Digits;
	char *P;
	int32_t NewLen;
	char result[22];

	/*Within int32_t Range - Use Faster int32_t Version*/
	if (Negative && Value <= 0x7fffffff ||
		!Negative && Value <= 0xffffffff)
		return _IntToStr32((uint32_t)Value, Negative);

	I64 = Value;
	if (I64 >= 100000000000000)
		if (I64 >= 10000000000000000)
			if (I64 >= 1000000000000000000)
				if (I64 >= 10000000000000000000)
					Digits = 20;
				else
					Digits = 19;
			else
				Digits = 17 + (char)(I64 >= 100000000000000000);
		else
			Digits = 15 + (char)(I64 >= 1000000000000000);
	else
		if (I64 >= 1000000000000)
			Digits = 13 + (char)(I64 >= 10000000000000);
		else
			if (I64 >= 10000000000)
				Digits = 11 + (char)(I64 >= 100000000000);
			else
				Digits = 10;
	NewLen = Digits + (char)Negative;

	P = result;
	*P = '-';
	P += char(Negative);
	if (Digits == 20)
	{
		*P = '1';
		P++;
		I64 -= 10000000000000000000;
		Digits--;
	}
	if (Digits > 17)
	{ /*18 or 19 Digits*/
		if (Digits == 19)
		{
			*P = '0';
			while (I64 >= 1000000000000000000)
			{
				I64 -= 1000000000000000000;
				*P++;
			}
			P++;
		}
		*P = '0';
		while (I64 >= 100000000000000000)
		{
			I64 -= 100000000000000000;
			*P++;
		}
		P++;
		Digits = 17;
	}
	J64 = I64 / 100000000;
	K64 = I64 - (J64 * 100000000); /*Remainder == 0..99999999*/
	I32 = (uint32_t)K64;
	J32 = I32 / 100;
	K32 = J32 * 100;
	K32 = I32 - K32;
	*(uint16_t*)(P + Digits - 2) = *(uint16_t*)(TwoDigitLookup[K32]);
	I32 = J32 / 100;
	L32 = I32 * 100;
	L32 = J32 - L32;
	*(uint16_t*)(P + Digits - 4) = *(uint16_t*)(TwoDigitLookup[L32]);
	J32 = I32 / 100;
	K32 = J32 * 100;
	K32 = I32 - K32;
	*(uint16_t*)(P + Digits - 6) = *(uint16_t*)(TwoDigitLookup[K32]);
	*(uint16_t*)(P + Digits - 8) = *(uint16_t*)(TwoDigitLookup[J32]);
	Digits -= 8;
	I32 = (uint32_t)J64; /* dividend now Fits within int32_t - Use Faster Version*/
	if (Digits > 2) {
		do {
			J32 = I32 / 100;
			K32 = J32 * 100;
			K32 = I32 - K32;
			I32 = J32;
			Digits -= 2;
			*(uint16_t*)(P + Digits) = *(uint16_t*)(TwoDigitLookup[K32]);
		} while (!(Digits <= 2));
	}
	if (Digits == 2)
		*(uint16_t*)(P + Digits - 2) = *(uint16_t*)(TwoDigitLookup[I32]);
	else
		*P = I32 | '0';

	return result;
}

std::string IntToStr(int32_t Value) {
	if (Value < 0)
		return _IntToStr32(-Value, true);
	else
		return _IntToStr32(Value, false);
}

std::string IntToStr(int64_t Value) {
	if (Value < 0)
		return _IntToStr64(-Value, true);
	else
		return _IntToStr64(Value, false);
}

std::string IntToStr(uint32_t Value) {
	return _IntToStr32(Value, false);
}

std::string IntToStr(uint64_t Value) {
	return _IntToStr64(Value, false);
}