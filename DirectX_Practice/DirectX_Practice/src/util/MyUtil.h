#pragma once
#include <string>
#include <sstream>
#include <iomanip>

class MyUtil
{
public:
	// ���[
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
				if (*s == '.')	// �����_�̕\�L�Ɏw�肪���邩�m�F
				{
					auto precision = *(++s);
					std::stringstream ss;
					ss << std::fixed << std::setprecision(precision - '0') << value;	//
					ret += ss.str();
					*(++s);
					std::string str = toString(*s ? ++s : s, args...); // ����Ȃ�����������邽�߁A*s == '\0' �ł��Ăяo��
					ret += str;
					return ret;
				}
				else
				{
					ret += std::to_string(value);
					ret += toString(*s ? ++s : s, args...); // ����Ȃ�����������邽�߁A*s == '\0' �ł��Ăяo��
					return ret;
				}
			}
			ret += *s;
			*(++s);
		}
		throw std::runtime_error("extra arguments provided to printf");
		return ret;
	}

	// �ϒ�������string�ɂ��ĕԂ�
	std::string format(const char* format, ...)
		throw(const char *)
	{
		va_list v;				// �ϒ������^
		int num = 0;			// �o�C�g��
		int res = 0;			// �Ԃ�l
		std::string ret;		// �Ԃ�l

		va_start(v, format);	// �ϒ���������J�n

		num = _vscprintf(format, v) + 1;				// �����W�J��̃o�C�g���擾
		char* buf = new char[num];						// �o�b�t�@�������A���P�[�V����

		res = _vsnprintf_s(buf, num, num, format, v);	// �o�b�t�@�ɏ����W�J��̕������ݒ�
		if (res < 0)
		{
			va_end(v);				// �ϒ���������I��
			delete[] buf;			// �o�b�t�@���
			throw "_vsnprintf_s error";
		}

		ret = buf;				// string�ɕϊ�

		va_end(v);				// �ϒ���������I��
		delete[] buf;			// �o�b�t�@���

		return ret;
	}
};