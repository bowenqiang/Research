#include "stdafx.h"
#include "MFCApplication1_Multi_AlphaDlg.h"

int signal = 0;
CSemaphore g_Semaphore(1, 1);
int threadnum = 2;
int dst = 0;

int Num_Cams = 6;
CmapNmask globalmap;
vector<Mat>Images;
vector<CvCapture*> Capture;

Point disp_origin = Point(0, 0);
int disp_width = 640;
int disp_height = 480;
Rect Roi_disp = Rect(disp_origin.x, disp_origin.y, disp_width, disp_height);



CCriticalSection CCritical;
CEvent g_event[2];
CEvent event_1, event_2;
HANDLE event[2];

UINT ThreadProc(LPVOID pParam)
{
	HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
	LPTHREADDATA pData = (LPTHREADDATA)pParam;

	Mat dst_temp = Mat::zeros(pData->Roi_dst->size(), 16);
	//Vec3b NullPixel = Vec3b(255, 255, 255);
	float x_, y_;
	Point StartPoint;
	Point EndPoint;
	StartPoint = pData->Roi->tl();
	EndPoint = pData->Roi->br();
	int x_s, x_l, y_s, y_l;
	Vec3b* src_tl;
	Vec3b* dst_;
	float* xmap_;
	float* ymap_;
	unsigned char* mask_;
	int CamIdx;
	int i = 1;
	while (TRUE)
	{

		//MessageBox(NULL, _T("checkpoint1"),NULL , MB_OK);
		//WaitForSingleObject(g_event[pData->ID],INFINITE);
		g_Semaphore.Lock();
		//MessageBox(NULL, _T("checkpoint2"), NULL, MB_OK);

		for (int y = StartPoint.y; y < EndPoint.y; y++)
		{
			//Vec3b* dst_ = pData->Obj->m_dst.ptr<Vec3b>(y - StartPoint.y);
			dst_ = dst_temp.ptr<Vec3b>(y - StartPoint.y);
			xmap_ = pData->Obj->m_xmap.ptr<float>(y);
			ymap_ = pData->Obj->m_ymap.ptr<float>(y);
			mask_ = pData->Obj->m_mask.ptr<unsigned char>(y);

			for (int x = StartPoint.x; x < EndPoint.x; x++)
			{
				CamIdx = (int)mask_[x];
				if (CamIdx == 255)
				{
					//dst_[x] = NullPixel;
					dst_[x - StartPoint.x] = Vec3b(0, 0, 0);
				}
				else
				{
					x_ = xmap_[x];
					y_ = ymap_[x];

					x_s = (int)floor(x_);
					x_l = (int)ceil(x_);
					y_s = (int)floor(y_);
					y_l = (int)ceil(y_);
					//float n = y_ - y_s, m = x_ - x_s;
					if (x_s < 0 || y_s < 0 || x_l>(*pData->Images)[CamIdx].cols - 1 || y_l>(*pData->Images)[CamIdx].rows - 1)
					{
						//cout << "out of range" << endl;
						dst_[x - StartPoint.x] = Vec3b(0, 0, 0);
					}
					else
					{

						src_tl = (*pData->Images)[CamIdx].ptr<Vec3b>(y_s, x_s);
						dst_[x - StartPoint.x] = *src_tl;
					}
				}
			}
		}

		CCritical.Lock();
		pData->Obj->m_dst = pData->Obj->m_dst | dst_temp;
		//signal++;
		::PostMessage(hwnd, WM_LOOP, NULL, NULL);
		CCritical.Unlock();
		//MessageBox(NULL, _T("threadend"), NULL, NULL);
		//Sleep(10);

		//i--;



	}

	dst_temp.release();
	return 0;
};

/*
UINT ThreadProc(LPVOID pParam)
{
LPTHREADDATA pData = (LPTHREADDATA)pParam;

while (1)
{
MessageBox(NULL, _T("thread"), NULL, MB_OK);
WaitForSingleObject(g_event[pData->ID], INFINITE);
MessageBox(NULL, _T("checkpoint"), NULL, MB_OK);
int i = 0;
while (i < 10)
{
i++;
}
CCritical.Lock();
*pData->Obj = *pData->Obj + i;
signal = signal + 1;
CCritical.Unlock();
//CWnd* hwnd = AfxGetMainWnd();
//::PostMessage(pData->hWindows,WM_LOOP,NULL,NULL);
}
return 0;
}
*/