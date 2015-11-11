#include "stdafx.h"
#include "MFCApplication1_Multi_BetaDlg.h"

int signal=0;


int dst=0;
int ThreadLock;

int Num_Cams = 5;
CmapNmask globalmap;
vector<Mat>Images;
vector<CvCapture*> Capture;

Point disp_origin = Point(0, 0);
int disp_width = 640;
int disp_height = 480;
Rect Roi_disp = Rect(disp_origin.x, disp_origin.y, disp_width, disp_height);


CCriticalSection CCritical;
CEvent g_event[THREADNUM];
CString FolderPathName;


CvVideoWriter* video = cvCreateVideoWriter("d:/sample/result.avi", CV_FOURCC('D', 'I', 'V', 'X'), 25, cvSize(disp_width, disp_height));
int FrameCounter = 0;
int rotation = BST_UNCHECKED;

IplImage *mapx, *mapy;


//UINT ThreadProc(LPVOID pParam)  //////signed int map
//{
//	HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
//	LPTHREADDATA pData = (LPTHREADDATA)pParam;
//
//	Mat dst_temp = Mat::zeros(Roi_disp.size(), 16);
//	//Vec3b NullPixel = Vec3b(255, 255, 255);
//	int x_, y_;
//	Point StartPoint;
//	Point EndPoint;
//
//
//
//	int x_s, x_l, y_s, y_l;
//	Vec3b* src_tl;
//	Vec3b* dst_;
//	int* xmap_, *ymap_;
//	int* mask_;
//	int CamIdx;
//	int y_offset = pData->Roi->tl().y - pData->Roi_dst->tl().y;
//	while (TRUE)
//		//int i = 1;
//		//while (i>0)
//	{
//
//		StartPoint = pData->Roi->tl();
//		EndPoint = pData->Roi->br();
//
//		WaitForSingleObject(g_event[pData->ID], INFINITE);
//		g_event[pData->ID].ResetEvent();
//
//
//		for (int y = StartPoint.y; y < EndPoint.y; y++)
//		{
//			//Vec3b* dst_ = pData->Obj->m_dst.ptr<Vec3b>(y - StartPoint.y);
//			dst_ = dst_temp.ptr<Vec3b>(y - StartPoint.y + y_offset);
//			xmap_ = pData->Obj->m_xmap.ptr<int>(y);
//			ymap_ = pData->Obj->m_ymap.ptr<int>(y);
//			mask_ = pData->Obj->m_mask.ptr<int>(y);
//
//			for (int x = StartPoint.x; x < EndPoint.x; x++)
//			{
//				CamIdx = mask_[x];
//				if (CamIdx == 255)
//				{
//					//dst_[x] = NullPixel;
//					dst_[x - StartPoint.x] = Vec3b(0, 0, 0);
//				}
//				else
//				{
//					x_ = xmap_[x];
//					y_ = ymap_[x];
//
//					x_s = x_;
//					//x_l = (int)ceil(x_);
//					y_s = y_;
//					//y_l = (int)ceil(y_);
//					//float n = y_ - y_s, m = x_ - x_s;
//					if (x_s < 0 || y_s < 0 || x_s>(*pData->Images)[CamIdx].cols - 2 || y_s>(*pData->Images)[CamIdx].rows - 2)
//					{
//						//cout << "out of range" << endl;
//						dst_[x - StartPoint.x] = Vec3b(255, 255, 255);
//					}
//					else
//					{
//
//						src_tl = (*pData->Images)[CamIdx].ptr<Vec3b>(y_s, x_s);
//						dst_[x - StartPoint.x] = *src_tl;
//					}
//				}
//			}
//		}
//
//		CCritical.Lock();
//		pData->Obj->m_dst = pData->Obj->m_dst | dst_temp;
//		CCritical.Unlock();
//		::PostMessage(hwnd, WM_PULSE, NULL, NULL);
//	}
//	dst_temp.release();
//	return 0;
//};


UINT ThreadProc(LPVOID pParam)  //////float map
{
	HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
	LPTHREADDATA pData = (LPTHREADDATA)pParam;

	Mat dst_temp = Mat::zeros(Roi_disp.size(), 16);
	//Vec3b NullPixel = Vec3b(255, 255, 255);
	float x_, y_;
	Point StartPoint;
	Point EndPoint;
	
	

	int x_s, x_l, y_s, y_l;
	Vec3b* src_tl;
	Vec3b* dst_;
	float* xmap_, *ymap_;
	unsigned char* mask_;
	int CamIdx;
	int y_offset = pData->Roi->tl().y-pData->Roi_dst->tl().y;
	while (TRUE)
	{

		StartPoint = pData->Roi->tl();
		EndPoint = pData->Roi->br();

		WaitForSingleObject(g_event[pData->ID],INFINITE);
		g_event[pData->ID].ResetEvent();


			for (int y = StartPoint.y; y < EndPoint.y; y++)
			{
				//Vec3b* dst_ = pData->Obj->m_dst.ptr<Vec3b>(y - StartPoint.y);
				dst_ = dst_temp.ptr<Vec3b>(y - StartPoint.y+y_offset);
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
							dst_[x - StartPoint.x] = Vec3b(255, 255, 255);
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
			CCritical.Unlock();
			::PostMessage(hwnd, WM_PULSE, NULL, NULL);
	}
	dst_temp.release();
	return 0;
};