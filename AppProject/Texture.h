#include "Pre.h"

class FTexture
{
public:
	int mWidth, mHeight;

	FTexture(const std::string &filename);
	~FTexture();

	inline std::string GetName() const { return mName; }
	inline bool IsValid() const { return mValid;}
	CColor GetPixel(int x, int y);
	CColor GetPixelUV(float u, float v);
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
