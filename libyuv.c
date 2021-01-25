#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <Windows.h>
#else
typedef unsigned char byte;
#endif

#define DBG_MSG_MAX 4096
#define PrintCtx(fmt, ...)               \
	do {                                 \
		char dbgMsg[DBG_MSG_MAX];        \
		snprintf(dbgMsg, sizeof(dbgMsg), "[%s:%d]%s", __FUNCTION__, __LINE__, fmt);  \
		printf(dbgMsg, ##__VA_ARGS__);   \
	} while(0)
#define TOSTR(x) #x

void SetCmdRed()
{
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
#else
	printf("\033[31m");
#endif
}

void SetCmdGreen()
{
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
#else
	printf("\033[32m");
#endif
}

void SetCmdBlue()
{
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
#else
	printf("\033[34m");
#endif
}

void SetCmdDefault()
{
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
	printf("\033[0m");
#endif
}

#define clip(min, max, v)   \
  do {                      \
    v = v < min ? min : v;  \
    v = v > max ? max : v;  \
  }

#define Rgb888ToYuv(R, G, B, Y, U, V)                                \
	do {                                                             \
		Y = ( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16 ;\
		U = ( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128 ;\
		V = ( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128 ;\
	} while(0);

#define YuvToRgb888(R, G, B, Y, U, V)                       \
	do {                                                    \
		R = clip(( 298 * ((Y) - 16) + 409 * ((V) - 128) + 128) >> 8) ;\
		G = clip(( 298 * ((Y) - 16) - 100 * ((U) - 128) - 208 * ((V) - 128) + 128) >> 8) ;\
		B = clip(( 298 * ((Y) - 16) + 516 * ((U) - 128) + 128) >> 8) ;\
	} while(0);

typedef enum {
	YUV420_NONE,
	YUV420_YU12,
	YUV420_YV12,
	YUV420_NV12,
	YUV420_NV21,
}YuvFmt;

typedef enum{
	PIX_NONE,
	PIX_RED,
	PIX_GREEN,
	PIX_BLUE,
}PixColor;

const char *YuvFmtStr(YuvFmt fmt)
{
	const char *str = NULL;
	switch (fmt)
	{
	case YUV420_YU12:
		str = TOSTR(YUV420_YU12);
		break;
	case YUV420_YV12:
		str = TOSTR(YUV420_YV12);
		break;
	case YUV420_NV12:
		str = TOSTR(YUV420_NV12);
		break;
	case YUV420_NV21:
		str = TOSTR(YUV420_NV21);
		break;
	default:
		str = TOSTR(YUV420_NONE);
		break;
	}
	return str;
}
void PrintPix(PixColor color, byte b)
{
	char prefix = 0;
	switch (color)
	{
	case PIX_RED:
		SetCmdRed();
		prefix = 'Y';
		break;
	
	case PIX_GREEN:
		SetCmdGreen();
		prefix = 'U';
		break;
	
	case PIX_BLUE:
		SetCmdBlue();
		prefix = 'V';
		break;
	default:
		SetCmdDefault();
		prefix = 'N';
		break;
	}
	printf("%C%02X ", prefix, b);
	SetCmdDefault();
	return;
}

void PrintEnd()
{
	printf("\n");
}

void PrintByte(PixColor color, byte *b, size_t width, size_t height)
{
	size_t idxRow;
	size_t idxCol;
	for (idxRow = 0; idxRow < height; ++idxRow){
		for (idxCol = 0; idxCol < width; ++idxCol){
			PrintPix(color, b[idxRow * width + idxCol]);
		}
		PrintEnd();
	}
}

void PrintCrossByte(YuvFmt fmt, byte *b, size_t width, size_t height)
{
	size_t idxRow;
	size_t idxCol;
	PixColor color = PIX_NONE;
	for (idxRow = 0; idxRow < height; ++idxRow){
		for (idxCol = 0; idxCol < width; ++idxCol){
			switch (fmt)
			{
			case YUV420_NV12:
				color = idxCol % 2 ? PIX_BLUE : PIX_GREEN;
				break;
			case YUV420_NV21:
				color = idxCol % 2 ? PIX_GREEN : PIX_BLUE;
				break;
			default:
				color = PIX_NONE;
				break;
			}
			PrintPix(color, b[idxRow * width + idxCol]);
		}
		PrintEnd();
	}
}

void PrintYuv(YuvFmt fmt, byte *yuv, size_t width, size_t height)
{
	PrintCtx(" fmt:%s\n", YuvFmtStr(fmt));
	byte *y = yuv;
	byte *uv = yuv + width * height;
	byte *v = NULL;
	PrintByte(PIX_RED, y, width, height);
	switch (fmt)
	{
	case YUV420_YU12:
		PrintByte(PIX_GREEN, uv, width / 2, height / 2);
		v = uv + width / 2 * height / 2;
		PrintByte(PIX_BLUE, v, width / 2, height / 2);
		break;	
	case YUV420_YV12:
		PrintByte(PIX_BLUE, uv, width / 2, height / 2);
		v = uv + width / 2 * height / 2;
		PrintByte(PIX_GREEN, v, width / 2, height / 2);
		break;	
	case YUV420_NV12:
		PrintCrossByte(fmt, uv, width, height / 2);
		break;
	case YUV420_NV21:
		PrintCrossByte(fmt, uv, width, height / 2);
		break;
	default:
		break;
	}
}

int yuv_fill(byte *b, size_t bsz, size_t width, size_t height, int cross)
{
	size_t idxRow;
	size_t idxCol;
	for (idxRow = 0; idxRow < height; ++idxRow){
		for (idxCol = 0; idxCol < width; ++idxCol){
			cross = (cross == 0) ? 1 : cross;			
			b[idxRow * width + idxCol] = (idxRow * width + idxCol) / cross;
		}
	}
	return 0;
}

int yuv_gen_440(byte *b, size_t bsz, size_t width, size_t height)
{
	if (bsz < (width * height * 3)) {
		return -1;
	}
	byte *y = b;
	yuv_fill(y, bsz, width, height, 0);
	byte *u = y + width * height;
	yuv_fill(u, bsz, width, height, 0);
	byte *v = u + width * height;
	yuv_fill(v, bsz, width, height, 0);
	return 0;
}

int yuv_gen_420(int cross, byte *b, size_t bsz, size_t width, size_t height)
{
	if (bsz < (width * height * 2)) {
		return -1;
	}
	byte *y = b;


	if (cross == 2) {
		yuv_fill(y, bsz, width, height, 1);
		byte *uv = y + width * height;
		yuv_fill(uv, bsz, width, height / 2, cross);
	} else if (cross == 1){
		yuv_fill(y, bsz, width, height, cross);
		byte *u = y + width * height;
		width /= 2;
		height /= 2;
		yuv_fill(u, bsz, width, height, cross);
		byte *v = u + width * height;
		yuv_fill(v, bsz, width, height, cross);
	}
	return 0;
}

int bin_save(byte *yuv, size_t yuvSize, const char *fileName)
{
	PrintCtx("fileName%s\n", fileName);
	FILE *fp = fopen(fileName, "wb");
	fwrite(yuv, yuvSize, 1, fp);
	fclose(fp);
	return 0;
}

int bin_gen(int yuv444, int cross, const char *fileName)
{
	size_t width = 8;
	size_t height = 8;
	size_t dim = yuv444 ? 3 : 2;
	size_t yuvSize = width * height * dim;
	byte *yuv = (byte*)malloc(yuvSize * sizeof(byte));
	if (yuv444) {
		yuv_gen_440(yuv, yuvSize, width, height);
	} else {
		yuv_gen_420(cross, yuv, yuvSize, width, height);
	}
	bin_save(yuv, yuvSize, fileName);
	free(yuv);
	return 0;
}

int bin_load(byte **b, size_t *bsz, const char *fileName)
{
	FILE *fp = fopen(fileName, "rb");
	fseek(fp, 0, SEEK_END);
	*bsz = ftell(fp);
	*b = (byte*)malloc(*bsz * sizeof(byte));
	fseek(fp, 0, SEEK_SET);
	fread(*b, *bsz, 1, fp);
	fclose(fp);
	return 0;
}

int bin_unload(byte **b)
{
	free(*b);
	*b = NULL;
	return 0;
}


int yuv_usage(int argc, char** argv)
{
	PrintCtx("%s <img> <fmt> <width> <height>\n", argv[0]);
	return 0;
}

int yuv_dump(int argc, char** argv)
{
	byte *yuv = NULL;
	size_t yuvSize = 0;
	if (argc != 5) {
		yuv_usage(argc, argv);	
		return 1;
	}
	const char *fileName = argv[1];
	int fmt = atoi(argv[2]);
	size_t width = atoi(argv[3]);
	size_t height = atoi(argv[4]);	
	bin_load(&yuv, &yuvSize, fileName);
	PrintYuv(fmt, yuv, width, height);
	bin_unload(&yuv);
	return 0;
}

int yuv_main(int argc, char** argv)
{
	return yuv_dump(argc, argv);
	byte *yuv = NULL;
	size_t yuvSize = 0;
	if (argc != 4) {
		yuv_usage(argc, argv);
		return 1;
	}

	const char *fileName = argv[1];
	size_t width = atoi(argv[2]);
	size_t height = atoi(argv[3]);
	bin_gen(0, 1, fileName);
	for (int fmt = YUV420_YU12; fmt <= YUV420_YV12; ++fmt) {
		bin_load(&yuv, &yuvSize, fileName);
		PrintYuv(fmt, yuv, width, height);
		bin_unload(&yuv);
	}
	bin_gen(0, 2, fileName);
	for (int fmt = YUV420_NV12; fmt <= YUV420_NV21; ++fmt) {
		bin_load(&yuv, &yuvSize, fileName);
		PrintYuv(fmt, yuv, width, height);
		bin_unload(&yuv);
	}
	return 0;
}
