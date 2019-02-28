#pragma once
#include <string>
#include <sstream>
#include <iomanip>

class MyUtil
{
public:
	// 末端
	static std::string toString(const char *s)
	{
		std::string ret = "";
		while (*s)
		{
			if (*s == '%' && *(++s) != '%')
				throw std::runtime_error("invalid format string: missing arguments");
			ret += *s++;
		}
		return ret;
	}

	template<typename T, typename... Args>
	static std::string toString(const char *s, T value, Args... args)
	{
		std::string ret = "";
		while (*s)
		{
			if (*s == '%' && *(++s) != '%')
			{
				if (*s == '.')	// 小数点の表記に指定があるか確認
				{
					auto precision = *(++s);
					std::stringstream ss;
					ss << std::fixed << std::setprecision(precision - '0') << value;	//
					ret += ss.str();
					*(++s);
					std::string str = toString(*s ? ++s : s, args...); // さらなる引数を見つけるため、*s == '\0' でも呼び出す
					ret += str;
					return ret;
				}
				else
				{
					ret += std::to_string(value);
					ret += toString(*s ? ++s : s, args...); // さらなる引数を見つけるため、*s == '\0' でも呼び出す
					return ret;
				}
			}
			ret += *s;
			*(++s);
		}
		throw std::runtime_error("extra arguments provided to printf");
		return ret;
	}

	// 可変長引数をstringにして返す
	std::string format(const char* format, ...)
		throw(const char *)
	{
		va_list v;				// 可変長引数型
		int num = 0;			// バイト数
		int res = 0;			// 返り値
		std::string ret;		// 返り値

		va_start(v, format);	// 可変長引数操作開始

		num = _vscprintf(format, v) + 1;				// 書式展開後のバイト数取得
		char* buf = new char[num];						// バッファメモリアロケーション

		res = _vsnprintf_s(buf, num, num, format, v);	// バッファに書式展開後の文字列を設定
		if (res < 0)
		{
			va_end(v);				// 可変長引数操作終了
			delete[] buf;			// バッファ解放
			throw "_vsnprintf_s error";
		}

		ret = buf;				// stringに変換

		va_end(v);				// 可変長引数操作終了
		delete[] buf;			// バッファ解放

		return ret;
	}
};