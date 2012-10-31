
#pragma once

#include <vector>
#include <list>
#include <deque>
#include <assert.h>

#include <string>
#include <sstream>
#include <Windows.h>

// ���ڴ�С
const int SCREEN_WIDTH	= 700;
const int SCREEN_HEIGHT	= 500;

// ��ȫɾ��
#define SafeDelete(ptr) if (ptr) { delete ptr; ptr = 0; }
#define SafeDeleteArray(ptr) if (ptr) { delete [] ptr; ptr = 0; }

inline std::string GetFilePath(const std::string& filename)
{
	return "3DS/" + filename;
}

inline std::string IntToString(float num)
{
	std::stringstream ss;
	ss << num;
	std::string re;
	ss >> re;
	return re;
}