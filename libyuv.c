#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define clip(v) v 

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

#pragma pack(push, 1)

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

typedef struct tagBITMAPFILEHEADER {
	U16 bfType;
	U32 bfSize;
	U16 bfReserved1;
	U16 bfReserved2;
	U32 bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	U32 biSize;
	U32 biWidth;
	U32 biHeight;
	U16 biPlanes;
	U16 biBitCount;
	U32 biCompression;
	U32 biSizeImage;
	U32 biXPelsPerMeter;
	U32 biYPelsPerMeter;
	U32 biClrUsed;
	U32 biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	U8 rgbBlue;
	U8 rgbGreen;
	U8 rgbRed;
	U8 rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[1];
} BITMAPINFO;

typedef struct tagBITMAP {
	BITMAPFILEHEADER bfHeader;
	BITMAPINFO biInfo;
}BITMAPFILE;

#pragma pack(pop)

int GenBmpFile(U8 *pData, U8 bitCountPerPix, U32 width, U32 height, const char *filename) { 
    FILE *fp = fopen(filename, "wb"); 
    if (!fp) { 
        printf("fopen failed : %s, %d\n", __FILE__, __LINE__); 
        return 0; 
    } 

    U32 bmppitch = ((width*bitCountPerPix + 31) >> 5) << 2; 
    U32 filesize = bmppitch*height; 
    BITMAPFILE bmpfile;

    bmpfile.bfHeader.bfType = 0x4D42; 
    bmpfile.bfHeader.bfSize = filesize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
    bmpfile.bfHeader.bfReserved1 = 0; 
    bmpfile.bfHeader.bfReserved2 = 0; 
    bmpfile.bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
    bmpfile.biInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    bmpfile.biInfo.bmiHeader.biWidth = width; 
    bmpfile.biInfo.bmiHeader.biHeight = height; 
    bmpfile.biInfo.bmiHeader.biPlanes = 1; 
    bmpfile.biInfo.bmiHeader.biBitCount = bitCountPerPix; 
    bmpfile.biInfo.bmiHeader.biCompression = 0; 
    bmpfile.biInfo.bmiHeader.biSizeImage = 0; 
    bmpfile.biInfo.bmiHeader.biXPelsPerMeter = 0; 
    bmpfile.biInfo.bmiHeader.biYPelsPerMeter = 0; 
    bmpfile.biInfo.bmiHeader.biClrUsed = 0; 
    bmpfile.biInfo.bmiHeader.biClrImportant = 0; 
    fwrite(&(bmpfile.bfHeader), sizeof(BITMAPFILEHEADER), 1, fp); 
    fwrite(&(bmpfile.biInfo.bmiHeader), sizeof(BITMAPINFOHEADER), 1, fp); 
    U8 *pEachLinBuf = (U8*)malloc(bmppitch); 
    memset(pEachLinBuf, 0, bmppitch); 
    U8 BytePerPix = bitCountPerPix >> 3; 
    U32 pitch = width * BytePerPix; 
    if (pEachLinBuf) { 
        int h,w; 
        for(h = height-1; h >= 0; h--) { 
            for(w = 0; w < width; w++) { 
                //copy by a pixel 
                pEachLinBuf[w*BytePerPix+0] = pData[h*pitch + w*BytePerPix + 0]; 
                pEachLinBuf[w*BytePerPix+1] = pData[h*pitch + w*BytePerPix + 1]; 
                pEachLinBuf[w*BytePerPix+2] = pData[h*pitch + w*BytePerPix + 2]; 
            } 
            fwrite(pEachLinBuf, bmppitch, 1, fp); 
        } 
        free(pEachLinBuf); 
    } 
    fclose(fp); 
    return 1; 
} 

U8* GetBmpData(U8 *bitCountPerPix, U32 *width, U32 *height, const char* filename) { 
    FILE *pf = fopen(filename, "rb"); 
    if(!pf) { 
        printf("fopen failed : %s, %d\n", __FILE__, __LINE__); 
        return NULL; 
    } 

    BITMAPFILE bmpfile; 
    fread(&(bmpfile.bfHeader), sizeof(BITMAPFILEHEADER), 1, pf); 
    fread(&(bmpfile.biInfo.bmiHeader), sizeof(BITMAPINFOHEADER), 1, pf); 

    if(bitCountPerPix) { 
        *bitCountPerPix = bmpfile.biInfo.bmiHeader.biBitCount; 
    } 

    if(width) { 
        *width = bmpfile.biInfo.bmiHeader.biWidth; 
    } 

    if(height) { 
        *height = bmpfile.biInfo.bmiHeader.biHeight; 
    } 

    U32 bmppicth = (((*width)*(*bitCountPerPix) + 31) >> 5) << 2; 
    U8 *pdata = (U8*)malloc((*height)*bmppicth); 
    U8 *pEachLinBuf = (U8*)malloc(bmppicth); 
    memset(pEachLinBuf, 0, bmppicth); 
    U8 BytePerPix = (*bitCountPerPix) >> 3; 
    U32 pitch = (*width) * BytePerPix; 
    if(pdata && pEachLinBuf) { 
        int w, h; 
        for(h = (*height) - 1; h >= 0; h--) { 
            fread(pEachLinBuf, bmppicth, 1, pf); 
            for(w = 0; w < (*width); w++) { 
                pdata[h*pitch + w*BytePerPix + 0] = pEachLinBuf[w*BytePerPix+0]; 
                pdata[h*pitch + w*BytePerPix + 1] = pEachLinBuf[w*BytePerPix+1]; 
                pdata[h*pitch + w*BytePerPix + 2] = pEachLinBuf[w*BytePerPix+2]; 
            } 
        } 
        free(pEachLinBuf); 
    } 
    fclose(pf); 
    return pdata; 
} 

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

void TransYuvToRgb(YuvFmt fmt, byte *yuv, size_t width, size_t height,
	byte *rgb, unsigned int rgbPix)
{
	PrintCtx(" fmt:%s\n", YuvFmtStr(fmt));
	byte *ybase = yuv;
	byte *uvbase = yuv + width * height;
	byte *y = NULL;
	byte *u = NULL;
	byte *v = NULL;
	byte *r = NULL;
	byte *g = NULL;
	byte *b = NULL;
	size_t idxRow;
	size_t idxCol;
	for (idxRow = 0; idxRow < height; ++idxRow) {
		for (idxCol = 0; idxCol < width; ++idxCol) {
			y = ybase + (idxRow * width + idxCol);	 
			switch (fmt)
			{
			case YUV420_YU12:
				u = uvbase + idxRow / 2 * width / 2 + idxCol / 2;
				v = u + width / 2 * height / 2;
				break;	
			case YUV420_YV12:
				v = uvbase + width / 2 * idxRow / 2 + idxCol / 2;
				u = v + width / 2 * height / 2;
				break;	
			case YUV420_NV12:
				if (idxCol % 2 == 0) {
					u = uvbase + width * idxRow / 4 + idxCol;
					v = u + 1;
				} else {
					v = uvbase + width * idxRow / 4 + idxCol;
					u = v - 1;
				}
				break;
			case YUV420_NV21:
				if (idxCol % 2 == 0) {
					v = uvbase + width * idxRow / 4 + idxCol;
					u = v + 1;
				} else {
					u = uvbase + width * idxRow / 4 + idxCol;
					v = u - 1;
				}
				break;
			default:
				break;
			}
			r = rgb + idxRow * width * rgbPix + idxCol * rgbPix;
			g = r + 1;
			b = g + 1;
			YuvToRgb888((*r), (*g), (*b), (*y), (*u), (*v));
		}
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
	// return yuv_dump(argc, argv);
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
		unsigned int rgbPix = 3;
		byte *rgb = (byte*)malloc(width * height * rgbPix);
		TransYuvToRgb(YUV420_NV21, yuv, width, height, rgb, rgbPix);
		GenBmpFile(rgb, 24, width, height, "test.bmp");
		free(rgb);
		bin_unload(&yuv);
	}

	return 0;
}
