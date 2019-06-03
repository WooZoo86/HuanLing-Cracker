#include "StdAfx.h"
#include "MyBits.h"
#include <memory.h>

MyBits::MyBits(void)
{
	CodeType = SMALL_FIRST;
	TCHAR path[1024];
	GetModuleFileName(NULL, path, 1024);
	CString Root = path;
	Root = Root.Left(Root.ReverseFind('\\') + 1);    
}

/* 获取文件大小 */
ULONGLONG MyBits::FileSize(CString FileName)
{
	CFileStatus status; 
	CFile::GetStatus(FileName,status); 
	return(status.m_size);
}

/* 检测文件是否存在，为上面函数的备用函数 */
bool MyBits::FileExist(CString strFileName)
{
    CFileFind fFind;
    return fFind.FindFile(strFileName);
}

/* 获得指定的值 */
unsigned short MyBits::GetShort(unsigned char string[2])
{
	unsigned short vol;
	if(CodeType == BIG_FIRST)
	{
		vol = ((unsigned int)string[0]<<8) + (unsigned int)string[1];
	}
	else
	{
		vol = ((unsigned int)string[1]<<8) + (unsigned int)string[0];
	}
	return(vol);
}


/* 获得指定的值 */
unsigned long MyBits::GetLong(unsigned char string[4])
{
	unsigned int vol;
	if(CodeType == BIG_FIRST)
	{
		vol = ((unsigned int)string[0]<<24) + ((unsigned int)string[1]<<16) + ((unsigned int)string[2]<<8) + ((unsigned int)string[3]);
	}
	else
	{
		vol =  ((unsigned int)string[3]<<24) + ((unsigned int)string[2]<<16) + ((unsigned int)string[1]<<8) + ((unsigned int)string[0]);
	}
	return(vol);
}

/* 将unsigned long型数字写入文件 */
void MyBits::PutLong(unsigned long value, CFile out)
{
        unsigned char buf[4];
        if(CodeType == BIG_FIRST)
        {
                buf[0] = (unsigned char)((value & 0xff000000) >> 24);
                buf[1] = (unsigned char)((value & 0x00ff0000) >> 16);
                buf[2] = (unsigned char)((value & 0x0000ff00) >> 8);
                buf[3] = (unsigned char)((value & 0x000000ff));
        }
        else
        {       buf[0] = (unsigned char)((value & 0x000000ff));
                buf[1] = (unsigned char)((value & 0x0000ff00) >> 8);
                buf[2] = (unsigned char)((value & 0x00ff0000) >> 16);
                buf[3] = (unsigned char)((value & 0xff000000) >> 24);
        }
        out.Write(buf, 4);
}

/* 将unsigned short型数字写入文件 */
void MyBits::PutShort(unsigned short value, CFile out)
{
        unsigned char buf[2];
        if(CodeType == BIG_FIRST)
        {
                buf[0] = (unsigned char)((value & 0xff00) >> 8);
                buf[1] = (unsigned char)((value & 0x00ff));
        }
        else
        {       buf[0] = (unsigned char)((value & 0x00ff));
                buf[1] = (unsigned char)((value & 0xff00) >> 8);
        }
        out.Write(buf, 2);
}

void MyBits::SetCode(unsigned int type)
{
	if(type == BIG_FIRST)
	{
		CodeType = BIG_FIRST;
	}
	else
	{
		CodeType = SMALL_FIRST;
	}
}

MyBits::~MyBits(void)
{
	;
}

/* 将CString类型转换为char类型 */
int MyBits::CString2Char(CString input, char *output)
{
	BSTR un = input.AllocSysString(); 
	int size; 
	size = WideCharToMultiByte(CP_ACP, 0, un, -1, NULL, 0, NULL, NULL); 
	WideCharToMultiByte(CP_ACP, 0, input, -1, output, size, NULL, NULL); 
	SysFreeString(un); 
	return(size);
}

/* 将char类型转换为CString类型 */
CString MyBits::Char2CString(char *input)
{
	LPCSTR temp = (LPCSTR)(LPSTR)input;
	int len = MultiByteToWideChar(CP_ACP, 0, temp, -1, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, temp, -1, buf, len);        
	buf[len] = 0;
	return(buf);
}

// 判断目录是否存在
bool MyBits::FolderExists(CString s)
{
    DWORD attr; 
    attr = GetFileAttributes(s); 
    return (attr != (DWORD)(-1) ) &&
        ( attr & FILE_ATTRIBUTE_DIRECTORY); 
}

// 递归创建目录， 如果目录已经存在或者创建成功返回TRUE
bool MyBits::SuperMkDir(CString P)
{
    int len=P.GetLength();
    if ( len <2 ) 
	{
		return false; 
	}

    if('\\'==P[len-1])
    {
        P=P.Left(len-1);
        len=P.GetLength();
    }
    if ( len <=0 ) 
	{
		return false;
	}

    if (len <=3) 
    {
        if (FolderExists(P))
		{
			return true;
		}
		else 
		{
			return false; 
		}
	}
    
    if (FolderExists(P))
	{
		return true;
	}
    
    
    CString Parent;
    Parent=P.Left(P.ReverseFind(_T('\\')));
    
    if(Parent.GetLength()<=0)
	{
		return false; 
	}

    bool Ret=SuperMkDir(Parent); 
    
    if(Ret) 
    {
        SECURITY_ATTRIBUTES sa;
        sa.nLength=sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor=NULL;
        sa.bInheritHandle=0;
        Ret=(CreateDirectory(P,&sa)==TRUE);
        return Ret;
    }
    else
	{
        return false;
	}
}

/* 处理系统消息，常用于循环体中 */
void MyBits::ProcessMessages()
{
	MSG message; 
	if(::PeekMessage (&message,NULL,0,0,PM_REMOVE)) 
	{ 
		::TranslateMessage (&message); 
		::DispatchMessage (&message); 
	} 
}

/* 文件处理通用函数 */
bool MyBits::ProcFile(CString pTo,CString pFrom, int ProcMode)
{
	TCHAR to[MAX_PATH], from[MAX_PATH];
	wcscpy_s(from, pFrom + _T("\0\0"));	
	from[wcslen(from) + 1] = 0;
	wcscpy_s(to, pTo + _T("\0\0"));
	to[wcslen(to) + 1] = 0;
	SHFILEOPSTRUCT FileOp = {0};	
	FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR; //不出现确认对话框|需要时直接创建一个文件夹,不需用户确定
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = _T("正在运行");	
	FileOp.pFrom = from;	
	FileOp.pTo = to;
	FileOp.wFunc = ProcMode;
	return( SHFileOperation(&FileOp) == 0 );
}