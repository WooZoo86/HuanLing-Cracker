#pragma once

typedef unsigned char byte;
/* 同16位颜色模式的接口 */
typedef union
{
	unsigned short DATA;
	struct
	{
		unsigned B:5;
		unsigned G:6;
		unsigned R:5;
	}RGB565;
	struct
	{
		unsigned B:5;
		unsigned G:5;
		unsigned R:6;
	}RGB655;
	struct
	{
		unsigned B:6;
		unsigned G:5;
		unsigned R:5;
	}RGB556;
	struct
	{
		unsigned B:5;
		unsigned G:5;
		unsigned R:5;
		unsigned A:1;
	}RGB1555;
	struct
	{
		unsigned A:1;
		unsigned B:5;
		unsigned G:5;
		unsigned R:5;		
	}RGB5551;
	struct
	{
		unsigned A:4;
		unsigned B:4;
		unsigned G:4;
		unsigned R:4;		
	}RGB4444;
}RGB_16;

typedef union
{
	unsigned long DATA;
	struct
	{
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char A;
	}RGBA;
}RGB_32;

/* CMYK
rgb.rgbRed = 255*(100-NC)*(100-NK)/10000;
			rgb.rgbGreen = 255*(100-NM)*(100-NK)/10000;
			rgb.rgbBlue = 255*(100-NY)*(100-NK)/10000;
*/

#define BIG_FIRST 1
#define SMALL_FIRST 2

class MyBits
{
public:
	MyBits();
	virtual ~MyBits();

	/* 指定字节流编码方式：大头在前/小头在前 */
	void SetCode(unsigned int type);
	
	/* 从字节流得到一个指定类型的值 */
	unsigned short GetShort(unsigned char string[2]);
	unsigned long GetLong(unsigned char string[4]);

	/* 将整型数据写入文件 */
	void PutLong(unsigned long value, CFile out);
	void PutShort(unsigned short value, CFile out);

	/* char型与CString类型互转 */
	int CString2Char(CString input, char *output);	
	CString Char2CString(char *input);

	/* 文件操作函数 */
	ULONGLONG FileSize(CString FileName);
	bool FileExist(CString strFileName);
	bool SuperMkDir(CString P);
	bool FolderExists(CString s);
	void ProcessMessages();
	bool ProcFile(CString pTo,CString pFrom, int ProcMode = FO_COPY);

private:
	unsigned int CodeType;
	TCHAR buf[500];
};
