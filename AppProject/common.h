
#pragma once

#include <vector>
#include <list>
#include <deque>
#include <assert.h>



// 窗口大小
const int SCREEN_WIDTH	= 700;
const int SCREEN_HEIGHT	= 500;

// 安全删除
#define SafeDelete(ptr) if (ptr) { delete ptr; ptr = 0; }
#define SafeDeleteArray(ptr) if (ptr) { delete [] ptr; ptr = 0; }