
#pragma once

#include <vector>
#include <list>
#include <deque>
#include <assert.h>



// ���ڴ�С
const int SCREEN_WIDTH	= 700;
const int SCREEN_HEIGHT	= 500;

// ��ȫɾ��
#define SafeDelete(ptr) if (ptr) { delete ptr; ptr = 0; }
#define SafeDeleteArray(ptr) if (ptr) { delete [] ptr; ptr = 0; }