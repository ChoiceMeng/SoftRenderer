#include "Pre.h"

class CTexture
{
public:
	int mWidth, mHeight;

	CTexture(const std::string &filename);
	~CTexture();

	inline std::string GetName() const { return mName; }
	inline bool IsValid() const { return mValid;}
	CColor GetPixel(int x, int y){static CColor defaultColor(255, 255, 255); return defaultColor;}
	CColor GetPixelUV(float u, float v)
	{
		static CColor defaultColor(255, 255, 255);
		if (IsValid())
		{
			// 这是一个更精确的方法, 但是效率低一点
			//EInt x = 0.5f + std::abs(u - (EInt)u) * bitmap->getWidth();
			//x %= bitmap->getWidth();
			//EInt y = 0.5f + std::abs(v - (EInt)v) * bitmap->getHeight();
			//y %= bitmap->getHeight();
			//return bitmap->getPixel(x, y);

			//EInt x = Abs(u - (EInt)u) * bitmap->getWidth();
			//EInt y = Abs(v - (EInt)v) * bitmap->getHeight();

			// u,v值域[0,1]
			int x = Abs(u - (int)u) * mWidth;
			int y = Abs(v - (int)v) * mHeight;
			return GetPixel(x, y)/*mPixels[y * mPitch + x]*/;
		}
		else
			return defaultColor;
	}
	inline int GetHeight() const { return mHeight;}
	inline int GetWidth() const { return mWidth;}

private:
	std::string				mName;

	HBITMAP					hBitmap;
	BITMAP					bitmap;
	HDC						bitmapHDC;

public:
	PBYTE					mPixels;		

	int						mPitch;
	int						mColorBit;

	bool					mValid;
};
