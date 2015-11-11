#include "stdafx.h"
#include "highgui.h"
#include "CmapNmask.h"

#define DEMO

#define THREADNUM 4

//#define RECORD

extern int signal;
extern int dst;
extern int ThreadLock;

extern int Num_Cams;
extern CmapNmask globalmap;
extern vector<Mat>Images;
extern vector<CvCapture*> Capture;

extern Point disp_origin;
extern int disp_width;
extern int disp_height;
extern Rect Roi_disp;



extern UINT ThreadProc(LPVOID pParam);

extern CCriticalSection CCritical;
extern CEvent g_event[THREADNUM];
extern CString FolderPathName;

extern CvVideoWriter* video;
extern int FrameCounter;

typedef struct _ThreadData
{
	CmapNmask* Obj;
	vector<Mat>* Images;
	Rect *Roi;
	Rect* Roi_dst;
	int ID;
}*LPTHREADDATA, THREADDATA;

extern int rotation;

extern IplImage *mapx, *mapy;