#include "stdafx.h"
#include "CmapNmask.h"
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/opencv_modules.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching/detail/autocalib.hpp"
#include "opencv2/stitching/detail/blenders.hpp"
#include "opencv2/stitching/detail/camera.hpp"
#include "opencv2/stitching/detail/exposure_compensate.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "opencv2/stitching/detail/motion_estimators.hpp"
#include "opencv2/stitching/detail/seam_finders.hpp"
#include "opencv2/stitching/detail/util.hpp"
#include "opencv2/stitching/detail/warpers.hpp"
#include "opencv2/stitching/warpers.hpp"

using namespace std;
using namespace cv;
using namespace cv::detail;

Map::Map(){}
Map::Map(Rect Roi)
{
	xmap.create(Roi.height + 1, Roi.width + 1, CV_32F);
	xmap.setTo(Scalar::all(0));
	ymap.create(Roi.height + 1, Roi.width + 1, CV_32F);
	ymap.setTo(Scalar::all(0));
	mask.create(Roi.height + 1, Roi.width + 1, CV_8U);
	mask.setTo(Scalar::all(255));
	//dst.create(Roi.height + 1, Roi.width + 1, CV_8U);
	//dst.setTo(Scalar::all(255));
}

CmapNmask::CmapNmask(){}
CmapNmask::CmapNmask(Rect Roi)
{
	m_xmap.create(Roi.height + 1, Roi.width + 1, CV_32F);
	m_xmap.setTo(Scalar::all(0));
	m_ymap.create(Roi.height + 1, Roi.width + 1, CV_32F);
	m_ymap.setTo(Scalar::all(0));
	m_mask.create(Roi.height + 1, Roi.width + 1, CV_8U);
	m_mask.setTo(Scalar::all(255));
}

void CmapNmask::initialdst(Size size)
{
	m_dst = Mat::zeros(size, 16);
}

int CmapNmask::Calibrate(CString Path, int Num)
{//haven't done yet
	HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
	MessageBox(hwnd,_T("start"),NULL,NULL);
	// Default command line args
	char img_names[50];
	bool preview = false;
	//bool try_gpu = false;
	bool try_gpu = true;
	double work_megapix = 0.6;
	double seam_megapix = 0.1;
	double compose_megapix = -1;
	float conf_thresh = 1.f;
	string features_type = "surf";
	string ba_cost_func = "ray";
	string ba_refine_mask = "xxxxx";
	bool do_wave_correct = true;
	WaveCorrectKind wave_correct = detail::WAVE_CORRECT_HORIZ;
	bool save_graph = false;
	//bool save_graph = true;
	std::string save_graph_to;
	string warp_type = "spherical";
	//string warp_type = "plane";
	int expos_comp_type = ExposureCompensator::GAIN_BLOCKS;
	//float match_conf = 0.3f;
	float match_conf = 0.6f;
	string seam_find_type = "gc_color";
	int blend_type = Blender::MULTI_BAND;
	//int blend_type = Blender::FEATHER;
	float blend_strength = 5;
	string result_name = "result.jpg";

	double work_scale = 1, seam_scale = 1, compose_scale = 1;
	bool is_work_scale_set = false, is_seam_scale_set = false, is_compose_scale_set = false;
	Ptr<FeaturesFinder> finder;
	if (features_type == "surf")
	{
		finder = new SurfFeaturesFinder();
	}
	else if (features_type == "orb")
	{
		finder = new OrbFeaturesFinder();
	}
	else
	{
		cout << "Unknown 2D features type: '" << features_type << "'.\n";
		return -1;
	}

	char buffer[256];
	Mat image_temp;
	double seam_work_aspect = 1;
	Mat full_img,img;
	vector<Size> full_img_sizes(Num);
	vector<ImageFeatures> features(Num);
	for (int i = 0; i < Num; i++)
	{
		sprintf(buffer, "%S\\snapshot#%d.jpg", Path, i);
		full_img= imread(buffer);
		m_images.push_back(full_img);
		full_img_sizes[i] = full_img.size();
		if (full_img.empty())
		{
			cout << "failed to open image#:" << i << endl;
			return -1;
		}
		if (work_megapix < 0)
		{
			img = full_img;
			work_scale = 1;
			is_work_scale_set = true;
		}
		else
		{
			if (!is_work_scale_set)
			{
				work_scale = min(1.0, sqrt(work_megapix * 1e6 / full_img.size().area()));
				is_work_scale_set = true;
			}
			resize(full_img, img, Size(), work_scale, work_scale);
		}
		if (!is_seam_scale_set)
		{
			seam_scale = min(1.0, sqrt(seam_megapix * 1e6 / full_img.size().area()));
			seam_work_aspect = seam_scale / work_scale;
			is_seam_scale_set = true;
		}

		(*finder)(img, features[i]);
		features[i].img_idx = i;
		cout << "Features in image #" << i << features[i].keypoints.size() << endl;
		resize(full_img, img, Size(), seam_scale, seam_scale);
		m_images.push_back(img);
	}
	vector<MatchesInfo> pairwise_matches;
	BestOf2NearestMatcher matcher(try_gpu, match_conf);
	matcher(features, pairwise_matches);
	matcher.collectGarbage();


	vector<int> indices = leaveBiggestComponent(features, pairwise_matches, conf_thresh);
	vector<Mat> img_subset;
	vector<Size> full_img_sizes_subset;
	for (size_t i = 0; i < indices.size(); ++i)
	{
		img_subset.push_back(m_images[indices[i]]);
		full_img_sizes_subset.push_back(full_img_sizes[indices[i]]);
	}

	m_images = img_subset;

	full_img_sizes = full_img_sizes_subset;

	// Check if we still have enough images
	int num_images_after = static_cast<int>(m_images.size());
	if (num_images_after != Num)
	{
		cout << "auto feature points matching fail, try manual!" << endl;
		return -1;
	}
	MessageBox(hwnd, _T("done"), NULL, NULL);
	////////not finished




	return 0;
	}
	
int CmapNmask::MapGenerate(CString Path, int Num)
{
	//calculate the mapping table based on the camera information.
	char buffer[128];

	Mat intrinsic_mat, distortion_mat;
	sprintf(buffer, "%S\\out_camera_data.yml", FolderPathName);
	FileStorage fs1(buffer, FileStorage::READ);
	fs1["camera_matrix"] >> intrinsic_mat;
	fs1["distortion_coefficients"] >> distortion_mat;
	CvMat intrinsic = intrinsic_mat;
	CvMat distortion = distortion_mat;
	mapx = cvCreateImage(Size(1280, 720), IPL_DEPTH_32F, 1);
	mapy = cvCreateImage(Size(1280, 720), IPL_DEPTH_32F, 1);
	cvInitUndistortMap(&intrinsic, &distortion, mapx, mapy);
	Mat mapx_mat=mapx;
	Mat mapy_mat = mapy;
	//////////////////////////////////////////
	Mat k_temp,r_temp,image_temp;
	for (int i = 0; i < Num; i++)
	{
		sprintf(buffer, "%S\\%d.jpg", Path, i);
		image_temp = imread(buffer);
		m_imagesbase.push_back(image_temp);
		sprintf(buffer, "%S\\Cam_info#%d.xml", Path, i);
		FileStorage fs(buffer, FileStorage::READ);
		fs["K"] >> k_temp;
		fs["R"] >> r_temp;
		m_k.push_back(k_temp);
		m_r.push_back(r_temp);
		k_temp.release();
		r_temp.release();
	}


	vector<double> focals;
	for (int i = 0; i < Num; i++)
	{
		focals.push_back(m_k[i].at<double>(0, 0));
	}
	sort(focals.begin(), focals.end());
	float FL;
	if (focals.size() % 2 == 1)
		FL = static_cast<float>(focals[focals.size() / 2]);
	else
		FL = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;
	focals.swap(vector<double>()); //release focals

	Ptr<WarperCreator> Warper_creater = new cv::SphericalWarper();
	Ptr<RotationWarper> Waper = Warper_creater->create(FL);
	vector<Size> Sizes(Num);
	vector<Point> Corners(Num);
	///// calculate maps for each cams
	for (int i = 0; i < Num; i++)
	{
		Mat K,R,xmap_temp,ymap_temp;		
		m_k[i].convertTo(K, CV_32F);
		m_r[i].convertTo(R, CV_32F);
		Rect ROI = Waper->buildMaps(m_imagesbase[i].size(), K, R, xmap_temp, ymap_temp);
		m_xmapbase.push_back(xmap_temp);
		m_ymapbase.push_back(ymap_temp);
		Corners[i] = ROI.tl();
		Sizes[i] = ROI.size();
	}

	/////////calculate mask for each cams
	Mat dilated_mask, seam_mask, mask;
	vector<Mat>img_warped(Num), img_warped_f(Num);//mask_warped(Num)
	//Ptr<ExposureCompensator> compensator = ExposureCompensator::createDefault(ExposureCompensator::GAIN_BLOCKS);
	Mat mask_temp;
	for (int i = 0; i < Num; i++)
	{

		mask.create(m_imagesbase[i].size(), CV_8U);
		remap(mask, mask_temp, m_xmapbase[i], m_ymapbase[i], INTER_NEAREST, BORDER_CONSTANT);
		//mask_warped[i] = mask_temp;
		m_maskbase.push_back(mask_temp);
		sprintf(buffer, "%S\\Cam_mask#%d.jpg", Path, i);
		imwrite(buffer, m_maskbase[i]);
		remap(m_imagesbase[i], img_warped[i], m_xmapbase[i], m_ymapbase[i], INTER_NEAREST, BORDER_REFLECT);
		img_warped[i].convertTo(img_warped_f[i], CV_32F);

		sprintf(buffer, "%S\\img_warped#%d.jpg", Path, i);
		imwrite(buffer, img_warped[i]);
		mask_temp.release();
	}
	

	Ptr<SeamFinder> seam_finder = new detail::GraphCutSeamFinder(GraphCutSeamFinderBase::COST_COLOR);
	if (seam_finder.empty())
	{
		cout << "can't create the seam finder" << endl;
		return 1;
	}
	//seam_finder->find(img_warped_f, Corners, mask_warped);
	seam_finder->find(img_warped_f, Corners, m_maskbase);

	for (int i = 0; i < Num_Cams; i++)
	{

		dilate(m_maskbase[i], dilated_mask, Mat());
		resize(dilated_mask, seam_mask, m_maskbase[i].size());
		m_maskbase[i] = seam_mask&m_maskbase[i];
		dilated_mask.release();
		seam_mask.release();
		//output map and mask
		sprintf(buffer, "%S\\Cam_Map#%d.xml", Path, i);
		FileStorage fs1(buffer, FileStorage::WRITE);
		fs1 << "mask" << m_maskbase[i];
		fs1.release();
		sprintf(buffer, "%S\\Cam_mask#%d.jpg", Path, i);
		imwrite(buffer, m_maskbase[i]);
		sprintf(buffer, "%S\\img_warped#%d.jpg", Path, i);
		imwrite(buffer, img_warped[i]);
	}
	Rect ROI_BG = resultRoi(Corners, Sizes);
	static Map BackGroundMap(ROI_BG);




	for (int i = 0; i < Num_Cams; i++)
	{
		int dx = Corners[i].x - ROI_BG.x;
		int dy = Corners[i].y - ROI_BG.y;
		//cout << dx << " " << dy << endl;
		for (int y = 0; y < m_xmapbase[i].rows; y++)
		{
			for (int x = 0; x < m_xmapbase[i].cols; x++)
			{

				if (m_maskbase[i].at<unsigned char>(y, x))
				{
					//BackGroundMap.xmap.at<float>(dy + y, dx + x) = m_xmapbase[i].at<float>(y, x);
					//BackGroundMap.ymap.at<float>(dy + y, dx + x) = m_ymapbase[i].at<float>(y, x);
					BackGroundMap.xmap.at<float>(dy + y, dx + x) = mapx_mat.at<float>(m_ymapbase[i].at<float>(y, x),m_xmapbase[i].at<float>(y, x));
					BackGroundMap.ymap.at<float>(dy + y, dx + x) = mapy_mat.at<float>(m_ymapbase[i].at<float>(y, x), m_xmapbase[i].at<float>(y, x));
					BackGroundMap.mask.at<unsigned char>(dy + y, dx + x) = (unsigned char)i;
				}
			}
		}
	}
	//output background maps
	sprintf(buffer, "%S\\BG_Map.xml", Path);
	FileStorage fs2(buffer, FileStorage::WRITE);
	fs2 << "xmap" << BackGroundMap.xmap;
	fs2 << "ymap" << BackGroundMap.ymap;
	fs2 << "mask" << BackGroundMap.mask;
	fs2.release();
	sprintf(buffer, "%S\\BG_mask.jpg", Path);
	imwrite(buffer, BackGroundMap.mask);
	HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
	MessageBox(hwnd, _T("Map Generate successfully"), NULL, NULL);



	return 0;
}