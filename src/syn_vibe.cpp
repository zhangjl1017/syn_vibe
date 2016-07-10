#include "vibeBackgroundOwn.h"
#include "bs_post_processing.h"
#include "cv.h"
#include "highgui.h"
#include"synopsis.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
//    const char * videoFilePath="/home/zmx/output_file_zz.avi";
//    string file;
//    file = "/home/zmx/output_file_zz.avi";
//    const char * synvideoFilePath="/home/zmx/syn.avi";
    const char * videoFilePath="/home/xuefengmiao/Videos/test.mp4";
	//const char * videoFilePath="/home/xuefengmiao/test/test/people.avi";
    //const char * videoFilePath="/home/xuefengmiao/test/test/12.avi";
    string file;
    file = "/home/xuefengmiao/test/test/output_file_zz.avi";
    const char * synvideoFilePath="/home/xuefengmiao/test/test/syn_test.avi";
	vibeModel* vbM = NULL;
	unsigned char*  image_data = NULL; // image data in current image
    unsigned char*  output_data = NULL; // image data after background subtraction
	vector<cv::Rect> bs_bbox; // contain rectangle regions after post processing

//	if (argc == 1)
//	{
//		cout << "input video path:"<< endl;
//		cin >> file;
//	}
//	else

	CvCapture* video = cvCaptureFromFile(videoFilePath);
    int fps = (int)cvGetCaptureProperty(video, CV_CAP_PROP_FPS);
    IplImage *frame = cvQueryFrame(video);
    int width = frame->width;
	int height = frame->height;
	int nChannels = frame->nChannels;
	//cluster for rectangle
	int cluster_dist_threshold = 15;
	int rect_threshold = 10;

	output_data = (unsigned char *) malloc (width * height * nChannels);
    IplImage *foreground = cvCreateImage(cvGetSize(frame), frame->depth, 1);
	foreground->imageData = (char *) output_data;
	cvNamedWindow("FG",1);
	cvNamedWindow("ORI",1);
    IplImage *background = cvCreateImage(cvGetSize(frame), frame->depth, 3);
    //background->imageData=( char *)frame->imageData;
    //将没有移动物体的帧作为背景；这里为了后面实验，选取第一帧作为背景，这里第一帧恰没有移动物体;
   // cvShowImage("bg_samples",frame);//背景
    int frame_no=0;
    const int frame_num_used=(int) cvGetCaptureProperty(video, CV_CAP_PROP_FRAME_COUNT);
    //cout<<"frame_num_used="<<frame_num_used<<endl;
    while (frame_no<frame_num_used)
	{
        if(frame_no==0)
        {

            cvCopy(frame,background);
        }
        image_data = (unsigned char *) frame->imageData;
		if (!vbM)
			vbM = VibeModelInit(width, height, nChannels, image_data);
        //background->imageData=(char *) frame->imageData-(char*)foreground->imageData;///////////////////
       //
		VibeModelUpdate(vbM, image_data, output_data);
        ///////////////////////
       // cvShowImage("bg_samples_2",background);//背景
		//open and close operation
        cvMorphologyEx(foreground, foreground, 0, 0, CV_MOP_CLOSE, 1);
        cvMorphologyEx(foreground, foreground, 0, 0, CV_MOP_OPEN, 1);

        // cluster to get large region of foreground pixel
        bs_bbox.clear();

        find_rect_region(output_data, width, height, 0,
            cluster_dist_threshold, rect_threshold, bs_bbox);
      //  cout << bs_bbox.size() << endl;
		 //draw bs_bbox in the foreground image
		cv::Rect r;
        for (int i = 0; i < bs_bbox.size(); i++) {
            r = bs_bbox[i];
            //r = bs_region_bbox[i];
            cvRectangle(frame, r.tl(), r.br(), cv::Scalar(255,0,0), 2); // draw bs rectangle in whole img
            cvRectangle(foreground, r.tl(), r.br(), cv::Scalar(255), 2); // draw bs rectangle in foreground img
        }
		cvShowImage("ORI", frame);
		cvShowImage("FG", foreground);
        cvShowImage("bg_samples",background);
        if(cvWaitKey( 15) == 27) break;

        frame = cvQueryFrame(video);
        frame_no++;
    }	//充当背景建模中背景

     list< list<tube *> >  database;
     IplImage* bgImgimg1 = cvCreateImage(cvSize(background->width, background->height), IPL_DEPTH_8U, 1);//创建目标图像
     cvCvtColor(background,bgImgimg1,CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
     buildTrackDB(videoFilePath,bgImgimg1,database,30,900,3,0.2,0.5,false);
     cout<<"buildTrackDB is finished -----"<<endl;
     mergeDB(database,synvideoFilePath,fps,cvSize(background->width, background->height),background);
     cout<<"mergeDB is finished -----"<<endl;
     freeDB(database);
	cvReleaseCapture(&video);
	cvDestroyWindow("FG");
	cvDestroyWindow("ORI");
    //system("pause");
	return 0;
}
