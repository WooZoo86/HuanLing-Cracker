
// ProgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HuanLing Cracker.h"
#include "ProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CProgDlg 对话框




CProgDlg::CProgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDERH, sl_H);
	DDX_Control(pDX, IDC_SLIDERS, sl_S);
	DDX_Control(pDX, IDC_SLIDERL, sl_L);
	DDX_Control(pDX, IDC_EDITH, ed_H);
	DDX_Control(pDX, IDC_EDITS, ed_S);
	DDX_Control(pDX, IDC_EDITL, ed_L);
	DDX_Control(pDX, IDC_SPIN1, sp_H);
	DDX_Control(pDX, IDC_SPIN2, sp_S);
	DDX_Control(pDX, IDC_SPIN3, sp_L);
}

BEGIN_MESSAGE_MAP(CProgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CProgDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CProgDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CProgDlg 消息处理程序

BOOL CProgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	buffer = (unsigned char *)calloc(1024*MEM_MB, 1024);
	if(!buffer)
	{
		MessageBox(_T("内存不足！    "), _T("提示信息"), MB_OK);
		exit(0);
	}
	TCHAR path[1024];
	GetModuleFileName(NULL, path, 1024);
	Root = path;
	Root = Root.Left(Root.ReverseFind('\\')); 
	SrcPath = _T("D:\\幻灵游侠2.5\\");
	GetDlgItem(IDC_SRC)->SetWindowText(SrcPath);

	ed_H.SetWindowTextW(_T("60"));
	ed_S.SetWindowTextW(_T("0"));
	ed_L.SetWindowTextW(_T("0"));
	sp_H.SetRange(0, 999);
	sp_S.SetRange(0, 999);
	sp_L.SetRange(0, 999);
	sl_H.SetRange(0, 255);
	sl_H.SetPos(255);
	sl_S.SetRange(0, 255);
	sl_S.SetPos(70);
	sl_L.SetRange(0, 255);
	sl_L.SetPos(65);

	FreeImage_Initialise();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CProgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CProgDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(buffer)
	{
		free(buffer);
	}
	FreeImage_DeInitialise();
	CDialog::OnClose();
}

void CProgDlg::proc_file(CString file_name)
{
	CFile fp, out;
	if(!fp.Open(SrcPath + file_name, CFile::modeRead | CFile::typeBinary, 0))
	{
		MessageBox(_T("无法打开资源文件！   "), _T("提示信息"), MB_OK);
		return;
	}
	GetDlgItem(IDC_PROC)->SetWindowText(_T("分析结构：") + file_name);
	CString dir = _T("\\") + file_name.Left(file_name.GetLength() - 4) + _T("\\");
	Bits.SuperMkDir(Root + dir);
	
	unsigned long count, index_pos;
	fp.Seek(4, SEEK_SET);
	fp.Read(&count, 4);				//文件数目
	fp.Read(&index_pos, 4);			//索引区位置
	fp.Seek(index_pos, SEEK_SET);
	
	index_unit *units = (index_unit *)(buffer + 10*1024*1024);
	unsigned long i;
	for(i=0; i<count; i++)
	{
		fp.Read(units + i, sizeof(index_unit));
	}
	CString temp;
	for(i=0; i<count; i++)
	{
		fp.Seek((units + i)->pos, SEEK_SET);
		fp.Read(buffer, (units + i)->length);
		temp.Format(_T("%u"), (units + i)->id);
		GetDlgItem(IDC_PROC)->SetWindowText(_T("输出文件：") + dir + temp);
		if(*(buffer) + *(buffer + 1) == 0 && (*(buffer + 2)==2 || *(buffer + 2)==0xA))
		{
			temp += _T(".tga");
		}
		else if(*(buffer)==0x42 && *(buffer + 1)==0x4D)
		{
			temp += _T(".bmp");
		}
		else if(*(buffer)==0xFF && *(buffer + 1)==0xD8)
		{
			temp += _T(".jpg");
		}
		else if(*(buffer)==0x4e && *(buffer + 1)==0x44)
		{
			temp += _T(".rle");
		}
		else if(*(buffer)==0x52 && *(buffer + 1)==0x49)
		{
			temp += _T(".wav");
		}
		else
		{
			temp += _T(".ini");
		}

		if(out.Open(Root + dir + temp, CFile::modeCreate | CFile::modeWrite, 0))
		{
			out.Write(buffer, (units + i)->length);
			out.Close();
		}
		Bits.ProcessMessages();
	}

	fp.Close();
}

void CProgDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	proc_file(_T("data.wdf"));
	proc_file(_T("character.wdf"));
	proc_file(_T("pet.wdf"));
	MessageBox(_T("操作已完成！   "), _T("提示信息"), MB_OK);
}

void CProgDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	Bits.SuperMkDir(Root + _T("\\output\\"));
	LoopImage(Root);

	LoopImage(L"character/");
	MessageBox(_T("操作已完成！   "), _T("提示信息"), MB_OK);
}


/* 遍历目录，核心函数！ */
void CProgDlg::LoopImage(CString s)
{
	CFileFind f;
	CString ss;

	int t = f.FindFile(s + _T("\\*.*"));
	while(t)
	{ 
		t = f.FindNextFile();
		if(f.IsDots())
		{
			continue;
		}
		else if(f.IsDirectory())
		{
			LoopImage(f.GetFilePath());
		}

		ss = f.GetFileName();
		if(ss.Right(3) == _T("rle"))
		{
			SetDlgItemText(IDC_PROC, ss);	
			OneFile(f.GetFilePath());		
		}
	}
	f.Close();	
}

double CProgDlg::Hue2RGB(double v1, double v2, double vH)
{
    if (vH < 0) 
    {
		vH = vH + 1.0;
    }
    
    if (vH > 1) 
    {
		vH = vH - 1.0;
    }
    
    if (6.0 * vH < 1.0) 
    {
		return v1 + (v2 - v1) * 6.0 * vH;
    }
    if (2.0 * vH < 1.0) 
    {
		return v2;
    }
    if (3.0 * vH < 2.0) 
    {
		return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
    }
    else
    {
		return (v1);
	}
}

//RLE算法解压缩
static VOID DecryptRLEFile(BYTE* pData, BYTE* pos)
{
	int nTemp = 0;
	int nMultiple1 = 0;
	int nMultiple2 = 0;

	BYTE Res1 = 0;
	BYTE Res2 = 0;
	BYTE Res3 = 0;

	nMultiple1 = 4 * *(BYTE *)(pos + 2);
	if (*(BYTE *)(pos + 1))
	{
		nMultiple2 = nMultiple1 * *(BYTE *)(pos + 1);
		nTemp = (*(BYTE *)pos & 0x1F) * nMultiple2;
		Res3 = (((unsigned int)*(BYTE *)(pos + 2) << 8) - nMultiple2) >> 6;
		Res2 = (((unsigned int)*(BYTE *)(pos + 2) << 13) - nTemp) >> 11;
		Res1 = (nTemp + ((unsigned int)*(BYTE *)(pos + 2) << 13) - 32 * nMultiple2) >> 11;
		switch ((signed int)*(BYTE *)pos >> 5)
		{
		case 0:
			*(BYTE *)pData = *(BYTE *)pData & 0xE0 | ((signed int)Res3 >> 3) & 0x1F;
			*(WORD *)pData = *(WORD *)pData & 0xF81F | 32 * (((signed int)Res1 >> 2) & 0x3F);
			*(BYTE *)(pData + 1) = *(BYTE *)(pData + 1) & 7 | 8 * ((nMultiple1 >> 3) & 0x1F);
			break;
		case 1:
			*(BYTE *)pData = *(BYTE *)pData & 0xE0 | ((signed int)Res3 >> 3) & 0x1F;
			*(WORD *)pData = *(WORD *)pData & 0xF81F | 32 * ((nMultiple1 >> 2) & 0x3F);
			*(BYTE *)(pData + 1) = *(BYTE *)(pData + 1) & 7 | 8 * (((signed int)Res2 >> 3) & 0x1F);
			break;
		case 2:
			*(BYTE *)pData = *(BYTE *)pData & 0xE0 | ((signed int)Res1 >> 3) & 0x1F;
			*(WORD *)pData = *(WORD *)pData & 0xF81F | 32 * ((nMultiple1 >> 2) & 0x3F);
			*(BYTE *)(pData + 1) = *(BYTE *)(pData + 1) & 7 | 8 * (((signed int)Res3 >> 3) & 0x1F);
			break;
		case 3:
			*(BYTE *)pData = *(BYTE *)pData & 0xE0 | (nMultiple1 >> 3) & 0x1F;
			*(WORD *)pData = *(WORD *)pData & 0xF81F | 32 * (((signed int)Res2 >> 2) & 0x3F);
			*(BYTE *)(pData + 1) = *(BYTE *)(pData + 1) & 7 | 8 * (((signed int)Res3 >> 3) & 0x1F);
			break;
		case 4:
			*(BYTE *)pData = *(BYTE *)pData & 0xE0 | (nMultiple1 >> 3) & 0x1F;
			*(WORD *)pData = *(WORD *)pData & 0xF81F | 32 * (((signed int)Res3 >> 2) & 0x3F);
			*(BYTE *)(pData + 1) = *(BYTE *)(pData + 1) & 7 | 8 * (((signed int)Res1 >> 3) & 0x1F);
			break;
		case 5:
			*(BYTE *)pData = *(BYTE *)pData & 0xE0 | ((signed int)Res2 >> 3) & 0x1F;
			*(WORD *)pData = *(WORD *)pData & 0xF81F | 32 * (((signed int)Res3 >> 2) & 0x3F);
			*(BYTE *)(pData + 1) = *(BYTE *)(pData + 1) & 7 | 8 * ((nMultiple1 >> 3) & 0x1F);
			break;
		default:
			*(BYTE *)pData &= 0xE0u;
			*(WORD *)pData &= 0xF81Fu;
			*(BYTE *)(pData + 1) &= 7u;
			break;
		}
	}
	else
	{
		nTemp = 4 * *(BYTE *)(pos + 2);
		*(BYTE *)pData = *(BYTE *)pData & 0xE0 | (nMultiple1 >> 3) & 0x1F;
		*(WORD *)pData = *(WORD *)pData & 0xF81F | 32 * ((nTemp >> 2) & 0x3F);
		*(BYTE *)(pData + 1) = *(BYTE *)(pData + 1) & 7 | 8 * ((nMultiple1 >> 3) & 0x1F);
	}
}

void CProgDlg::OneFile(CString DestFile)
{
	CFile fp;
	if(!fp.Open(DestFile, CFile::modeRead | CFile::typeBinary, NULL))
	{
		MessageBox(_T("无法打开文件！    "), _T("提示信息"), MB_OK);
		return;
	}
	ULONGLONG len = Bits.FileSize(DestFile);
	if(len > 10*1024*1024)
	{
		MessageBox(_T("文件体积太大！    "), _T("提示信息"), MB_OK);
		fp.Close();
		return;
	}
	unsigned long width, height, data_length;
	fp.Seek(4, SEEK_SET);
	fp.Read(&width, 4);
	fp.Read(&height, 4);
	fp.Seek(4, SEEK_CUR);
	fp.Read(&data_length, 4);
	fp.Seek(4, SEEK_CUR);
	unsigned char *col = buffer + (MEM_MB - 1) * 1024 * 1024;		//颜色表
	unsigned long *index = (unsigned long *)(col + 1024*6);			//行索引，最多9K行	
	unsigned char *image = col + 1024 * 40;							//原始图像数据，最大约为960KB

	fp.Read(col, 0x300);
	BYTE *buf = (BYTE *)malloc(0x200);
	if (!buf)
	{

		return;
	}
	memset(buf, 0, 0x200);

	DWORD count = 0;
	WORD *pos = (WORD *)buf;

	BYTE Data[2] = { 0 };
	do
	{
		DecryptRLEFile(Data, (PBYTE)(col + 3 * count));
		*pos = *(WORD *)Data;
		++count;
		++pos;
	} while (count < 0x100);
	//memset(col,0,0x300);
	//memcpy(col,buf,0x200);

	free(buf);

	fp.Read(index, height * 4);
	fp.Seek(len-data_length, SEEK_SET);
	fp.Read(image, data_length);
	
	FIBITMAP *bitmap = FreeImage_Allocate(width, height, 32);
	unsigned long x, i, p =0;
	unsigned short num1, num2, num3;
	signed long m;
	RGBQUAD rgb;	
    double var_1, var_2;	
	//unsigned short r, g, b; 
	double H, S, L;
	double base_h = sl_H.GetPos() * 1.0;
	double base_s = sl_S.GetPos() * 1.0;
	double base_l = sl_L.GetPos() * 1.0;
	double ex_h = sp_H.GetPos() * 0.001;
	double ex_s = sp_S.GetPos() * 0.001;
	double ex_l = sp_L.GetPos() * 0.001;

	if(bitmap) // 位图创建成功!
	{
		for(i=0; i<height; i++)
		{
			p = *(index + i);
			x = 0;
			while(x < width)
			{
				num1 = *(image + p);
				num2 = *(image + p + 1);
				num3 = *(image + p + 2);
				if(num1>0x7F)		//相同颜色的行程
				{
					
					rgb.rgbBlue = *(col + num2*3 + 0) << 2;
					rgb.rgbGreen = *(col + num2*3 + 1) << 2;
					rgb.rgbRed = *(col + num2*3 + 2) << 2;
					
					//rgb.rgbBlue = rgb.rgbGreen = rgb.rgbRed = 0;					
					rgb.rgbReserved = num3;
					m = num1 - 0x7F;
					for(; m>0; m--)
					{
						FreeImage_SetPixelColor(bitmap, x, i, &rgb);
						x++;
					}
					p += 3;
				}
				else				//单个颜色
				{
					m = num1 + 1;
					p += 1;
					for(; m>0; m--)
					{
						num1 = *(image + p);
						num2 = *(image + p + 1);
						
						
						//H = *(col + num1*3 + 0) / base_h + ex_h;
						//S = *(col + num1*3 + 1) / base_s + ex_s;
						//L = *(col + num1*3 + 2) / base_l + ex_l;
						//
						//if (*(col + num1*3 + 1) == 0)        //HSL values = 0 ÷ 1
						//{
						//	rgb.rgbRed = L * 255.0;                   //RGB results = 0 ÷ 255
						//	rgb.rgbGreen = L * 255.0;
						//	rgb.rgbBlue = L * 255.0;
						//}
						//else
						//{
						//	if (L < 0.5) var_2 = L * (1 + S);
						//	else         var_2 = (L + S) - (S * L);

						//	var_1 = 2.0 * L - var_2;

						//	rgb.rgbRed = 255.0 * Hue2RGB(var_1, var_2, H + (1.0 / 3.0));
						//	rgb.rgbGreen = 255.0 * Hue2RGB(var_1, var_2, H);
						//	rgb.rgbBlue = 255.0 * Hue2RGB(var_1, var_2, H - (1.0 / 3.0));
						//}
						
						rgb.rgbRed = (*(col + num1*3 + 1) << 2 );
						rgb.rgbGreen = (*(col + num1*3 + 2) << 2);
						rgb.rgbBlue = (*(col + num1*3 + 0) << 4);
						
						rgb.rgbReserved = num2;									
					
						FreeImage_SetPixelColor(bitmap, x, i, &rgb);
						x++;
						p += 2;
					}					
				}				
			}
		}
		CString temp = DestFile.Right(DestFile.GetLength() - DestFile.ReverseFind('\\'));
		temp = temp.Left(temp.GetLength() - 3);
		//FreeImage_SaveU(FIF_PNG, bitmap, Root + _T("\\output\\") + temp + _T(".png"), 0);
		//FreeImage_SaveU(FIF_PNG, bitmap, _T("data\\output\\") + temp + _T(".png"), 0);
		if (!FreeImage_SaveU(FIF_PNG, bitmap, _T("./output") + temp + _T("png"), 0))
		{
			DWORD dwErr = GetLastError();

			return;
		}
		

		FreeImage_Unload(bitmap);
	}	
	/*
	bitmap = FreeImage_Allocate(640, 640, 32);
	p = 0;
	for(i=0; i<640; i+=40)
	{
		for(x=0; x<640; x+=40, p++)
		{
			rgb.rgbRed = *(col + p*3 + 1) << 2;
			rgb.rgbGreen = *(col + p*3 + 2) << 2;
			rgb.rgbBlue = *(col + p*3 + 0) << 4;
			rgb.rgbReserved = 255;
			for(m=0; m<1600; m++)
			{
				FreeImage_SetPixelColor(bitmap, x+m%40, i+m/40, &rgb);
			}
		}
	}

	FreeImage_SaveU(FIF_PNG, bitmap, Root + _T("\\output\\pal.png"), 0);
	FreeImage_Unload(bitmap);
	*/
	fp.Close();
}