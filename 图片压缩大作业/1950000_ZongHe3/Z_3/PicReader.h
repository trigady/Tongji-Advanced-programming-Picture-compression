/******************************************************************
* ��ע�⣡                                                         
* ��ͷ�ļ���Ϊ���װ��WinAPI��WIC�ײ㺯�������������ͼƬ��ȡ��������  
* ���װ�������ⲿ�⣬����������һ����Լ��������������ϸ�Ķ����¹涨  
*     �������������⣬�뼰ʱ��ϵ��ʦ�����̣�                          
*     readPic()����Ϊ���װ��WinAPI�еķ��������Խ�ͼƬ��ȡΪRGBA��   
* bitmap���ݣ����������������ͨ���޸ĺ������ߵ���WinAPIʵ�����ѹ��
******************************************************************/
#ifndef PIC_READER_H
#define PIC_READER_H

#include <stdio.h>
#include <windows.h>
#include <wincodec.h>
#include <commdlg.h>
#include <graphics.h>

template <typename T>
inline void SafeRelease(T *&p) {
	if (nullptr != p) {
		p->Release();
		p = nullptr;
	}
}

class PicReader {
public:
	PicReader();
	~PicReader();

	void readPic(LPCSTR);
	void getData(BYTE *&,UINT &, UINT &);
	void showPic(const BYTE*, const UINT, const UINT);
private:
	void init();
	bool checkHR(HRESULT);
	void quitWithError(LPCSTR);

	HWND                    hWnd;
	HANDLE                  hFile;
	IWICImagingFactory     *m_pIWICFactory;
	IWICFormatConverter    *m_pConvertedSourceBitmap;
};
#endif