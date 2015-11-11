#pragma once
#ifndef CMAPNMASK_CLASS_DEF
#define CMAPNMASK_CLASS_DEF
#include "stdafx.h"
using namespace cv;

class Map
{
public:
	Map();
	Map(Rect Roi);
	void remap(vector<Mat>&src, Rect Roi);
	Mat xmap;
	Mat ymap;
	Mat mask;
	//Mat dst;
	//void Initial(Rect Roi);

};


class CamInfo
{
public:
	vector<Mat> m_imagesbase;
	vector<Mat> m_xmapbase;
	vector<Mat> m_ymapbase;
	vector<Mat> m_maskbase;
	vector<double> m_focal; // Focal length
	vector<double> m_aspect; // Aspect ratio
	vector<double> m_ppx; // Principal point X
	vector<double> m_ppy; // Principal point Y
	vector<Mat> m_r; // Rotation
	vector<Mat> m_t; // Translation
	vector<Mat> m_k;

};

class CmapNmask:public CamInfo
{
public:
	CmapNmask();
	CmapNmask(Rect Roi);
	void initialdst(Size size);
	int Calibrate(CString Path,int Num);
	int MapGenerate(CString Path, int Num);
	Mat m_xmap;
	Mat m_ymap;
	Mat m_mask;
	Mat m_dst;
	vector<Mat> m_images;

};

#endif