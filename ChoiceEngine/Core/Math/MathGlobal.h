// 数学常量
#define PI 3.141592654f
#define PI2 (PI * 2.0f)
#define PI_DIV_2 (PI * 0.5f)
#define PI_DIV_4 (PI * 0.25f)
#define PI_INV 0.318309886f

// 计算精度
#define EPSILON_E6 ((float)(1E-6))

//角度变弧度转换, 弧度便角度转换
//#define Degree_TO_Radian(deg) ((float)deg * PI / 180.0f)
//#define Radian_To_Degree(rad) ((float)rad * 180.0f / PI)
#define Degree_TO_Radian(deg) (deg * 0.017453f)
#define Radian_To_Degree(rad) (rad * 57.295780f)