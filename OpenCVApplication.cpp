// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include "Functions.h"

void testOpenImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image", src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName) == 0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName, "bmp");
	while (fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(), src);
		if (waitKey() == 27) //ESC pressed
			break;
	}
}

void testImageOpenAndSave()
{
	Mat src, dst;

	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image

	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}

	// Get the image resolution
	Size src_size = Size(src.cols, src.rows);

	// Display window
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one

	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);

	printf("Press any key to continue ...\n");
	waitKey(0);
}

void testNegativeImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				uchar neg = MAX_PATH - val;
				dst.at<uchar>(i, j) = neg;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("negative image", dst);
		waitKey();
	}
}

void testParcurgereSimplaDiblookStyle()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		int w = src.step; // no dword alignment is done !!!
		Mat dst = src.clone();

		double t = (double)getTickCount(); // Get the current time [s]

		// the fastest approach using the �diblook style�
		uchar* lpSrc = src.data;
		uchar* lpDst = dst.data;
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i * w + j];
				lpDst[i * w + j] = 255 - val;
				/* sau puteti scrie:
				uchar val = lpSrc[i*width + j];
				lpDst[i*width + j] = 255 - val;
				//	w = width pt. imagini cu 8 biti / pixel
				//	w = 3*width pt. imagini cu 24 biti / pixel
				*/
			}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("negative image", dst);
		waitKey();
	}
}

void testColor2Gray()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);

		int height = src.rows;
		int width = src.cols;

		Mat dst = Mat(height, width, CV_8UC1);

		// Asa se acceseaaza pixelii individuali pt. o imagine RGB 24 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec3b v3 = src.at<Vec3b>(i, j);
				uchar b = v3[0];
				uchar g = v3[1];
				uchar r = v3[2];
				dst.at<uchar>(i, j) = (r + g + b) / 3;
			}
		}

		imshow("input image", src);
		imshow("gray image", dst);
		waitKey();
	}
}

void testBGR2HSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;
		int w = src.step; // latimea in octeti a unei linii de imagine

		Mat dstH = Mat(height, width, CV_8UC1);
		Mat dstS = Mat(height, width, CV_8UC1);
		Mat dstV = Mat(height, width, CV_8UC1);

		// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
		uchar* dstDataPtrH = dstH.data;
		uchar* dstDataPtrS = dstS.data;
		uchar* dstDataPtrV = dstV.data;

		Mat hsvImg;
		cvtColor(src, hsvImg, CV_BGR2HSV);
		// definire pointer la matricea (24 biti/pixeli) a imaginii HSV
		uchar* hsvDataPtr = hsvImg.data;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int hi = i * width * 3 + j * 3;
				// sau int hi = i*w + j * 3;	//w = 3*width pt. imagini 24 biti/pixel
				int gi = i * width + j;

				dstDataPtrH[gi] = hsvDataPtr[hi] * 510 / 360;		// H = 0 .. 255
				dstDataPtrS[gi] = hsvDataPtr[hi + 1];			// S = 0 .. 255
				dstDataPtrV[gi] = hsvDataPtr[hi + 2];			// V = 0 .. 255
			}
		}

		imshow("input image", src);
		imshow("H", dstH);
		imshow("S", dstS);
		imshow("V", dstV);
		waitKey();
	}
}

void testResize()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		Mat dst1, dst2;
		//without interpolation
		resizeImg(src, dst1, 320, false);
		//with interpolation
		resizeImg(src, dst2, 320, true);
		imshow("input image", src);
		imshow("resized image (without interpolation)", dst1);
		imshow("resized image (with interpolation)", dst2);
		waitKey();
	}
}

void testCanny()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src, dst, gauss;
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int k = 0.4;
		int pH = 50;
		int pL = k * pH;
		GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
		Canny(gauss, dst, pL, pH, 3);
		imshow("input image", src);
		imshow("canny", dst);
		waitKey();
	}
}

void testVideoSequence()
{
	VideoCapture cap("Videos/rubic.avi"); // off-line video from file
	//VideoCapture cap(0);	// live video from web cam
	if (!cap.isOpened()) {
		printf("Cannot open video capture device.\n");
		waitKey();
		return;
	}

	Mat edges;
	Mat frame;
	char c;

	while (cap.read(frame))
	{
		Mat grayFrame;
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
		Canny(grayFrame, edges, 40, 100, 3);
		imshow("source", frame);
		imshow("gray", grayFrame);
		imshow("edges", edges);
		c = cvWaitKey();  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n");
			break;  //ESC pressed
		};
	}
}


void testSnap()
{
	VideoCapture cap(0); // open the deafult camera (i.e. the built in web cam)
	if (!cap.isOpened()) // openenig the video device failed
	{
		printf("Cannot open video capture device.\n");
		return;
	}

	Mat frame;
	char numberStr[256];
	char fileName[256];

	// video resolution
	Size capS = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	// Display window
	const char* WIN_SRC = "Src"; //window for the source frame
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Snapped"; //window for showing the snapped frame
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, capS.width + 10, 0);

	char c;
	int frameNum = -1;
	int frameCount = 0;

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		if (frame.empty())
		{
			printf("End of the video file\n");
			break;
		}

		++frameNum;

		imshow(WIN_SRC, frame);

		c = cvWaitKey(10);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished");
			break;  //ESC pressed
		}
		if (c == 115) { //'s' pressed - snapp the image to a file
			frameCount++;
			fileName[0] = NULL;
			sprintf(numberStr, "%d", frameCount);
			strcat(fileName, "Images/A");
			strcat(fileName, numberStr);
			strcat(fileName, ".bmp");
			bool bSuccess = imwrite(fileName, frame);
			if (!bSuccess)
			{
				printf("Error writing the snapped image\n");
			}
			else
				imshow(WIN_DST, frame);
		}
	}

}

void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
			x, y,
			(int)(*src).at<Vec3b>(y, x)[2],
			(int)(*src).at<Vec3b>(y, x)[1],
			(int)(*src).at<Vec3b>(y, x)[0]);
	}
}

void testMouseClick()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", MyCallBackFunc, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}


void imreconstruct(Mat reference, Mat marker) {
	Mat dst;
	/*Inputs are two images : ImReference and ImMarker, with marker <= reference
		Intermediate image : ImRec
		Output image : ImResult
		Copy ImMarker into ImRec
		copy ImRec into ImResult
		ImDilated = Dilation(ImResult)
		ImRec = Minimum(ImDilated, ImReference)
		If ImRec != ImResult then return to step 5.*/
}

Mat openingByReconstruction(Mat src) {

	Mat dst;
	Mat element = getStructuringElement(MORPH_CROSS, Size(5, 5));
	erode(src, dst, element, Point(-1, -1), 2);
	dilate(src, dst, element, Point(-1, -1), 2);

	return dst;
}

Mat closingByReconstruction(Mat src) {

	Mat dst;
	Mat element = getStructuringElement(MORPH_CROSS, Size(5, 5));
	dilate(src, dst, element, Point(-1, -1), 2);
	erode(src, dst, element, Point(-1, -1), 2);

	return dst;
}

void non_maxima_suppression(const cv::Mat& image, cv::Mat& mask, bool remove_plateaus) {
	// find pixels that are equal to the local neighborhood not maximum (including 'plateaus')
	cv::dilate(image, mask, cv::Mat());
	cv::compare(image, mask, mask, cv::CMP_GE);

	// optionally filter out pixels that are equal to the local minimum ('plateaus')
	if (remove_plateaus) {
		cv::Mat non_plateau_mask;
		cv::erode(image, non_plateau_mask, cv::Mat());
		cv::compare(image, non_plateau_mask, non_plateau_mask, cv::CMP_GT);
		cv::bitwise_and(mask, non_plateau_mask, mask);
	}

}

class WatershedSegmenter {
private:
	cv::Mat markers;
public:
	void setMarkers(cv::Mat& markerImage)
	{
		markerImage.convertTo(markers, CV_32S);
	}

	cv::Mat process(cv::Mat& image)
	{
		cv::watershed(image, markers);
		markers.convertTo(markers, CV_8U);
		return markers;
	}
};

void watershedAlgo(Mat& src, Mat& dst) {
	cv::Mat markers(src.size(), CV_8U, cv::Scalar(-1));
	//Rect(topleftcornerX, topleftcornerY, width, height);
	//top rectangle
	markers(Rect(0, 0, src.cols, 5)) = Scalar::all(1);
	//bottom rectangle
	markers(Rect(0, src.rows - 5, src.cols, 5)) = Scalar::all(1);
	//left rectangle
	markers(Rect(0, 0, 5, src.rows)) = Scalar::all(1);
	//right rectangle
	markers(Rect(src.cols - 5, 0, 5, src.rows)) = Scalar::all(1);
	//centre rectangle
	int centreW = src.cols / 4;
	int centreH = src.rows / 4;
	markers(Rect((src.cols / 2) - (centreW / 2), (src.rows / 2) - (centreH / 2), centreW, centreH)) = Scalar::all(2);
	markers.convertTo(markers, CV_BGR2GRAY);
	imshow("markers", markers);

	//Create watershed segmentation object
	WatershedSegmenter segmenter;
	segmenter.setMarkers(markers);
	cv::Mat wshedMask = segmenter.process(src);
	cv::Mat mask;
	convertScaleAbs(wshedMask, mask, 1, 0);
	double thresh = threshold(mask, mask, 1, 255, THRESH_BINARY);
	bitwise_and(src, src, dst, mask);

	dst.convertTo(dst, CV_8U);

}

void calcHistograma(Mat img, int* hist) {
	//int hist[256];
	int height = img.rows;
	int width = img.cols;

	for (int i = 0; i < 255; i++) {
		hist[i] = 0;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int index = img.at<uchar>(i, j);
			hist[index] = hist[index] + 1;
		}
	}
	//showHistogram("Histograma1", hist, img.cols, img.rows);
}

void adaptive_thresholding(Mat mag, Mat dst) {
	int* hist = (int*)calloc(sizeof(int), 256);
	calcHistograma(mag, hist);

	float p = 0.1;
	float k = 0.4;

	float nr_non_edge_pixels = (1 - p) * ((mag.rows - 2) * (mag.cols - 2) - hist[0]);
	float threshold_high = 0;

	int sum_hist = hist[1];
	for (int i = 2; i < 256; i++) {
		if (sum_hist >= nr_non_edge_pixels) {
			threshold_high = i;
			break;
		}
		sum_hist += hist[i];
	}

	float threshold_low = k * threshold_high;

	uchar NO_EDGE = 0, WEAK_EDGE = 128, STRONG_EDGE = 255;

	for (int i = 0; i < mag.rows; i++) {
		for (int j = 0; j < mag.cols; j++) {
			if (mag.at<uchar>(i, j) < threshold_low) {
				dst.at<uchar>(i, j) = NO_EDGE;
			}
			if (mag.at<uchar>(i, j) >= threshold_low && mag.at<uchar>(i, j) <= threshold_high) {
				dst.at<uchar>(i, j) = WEAK_EDGE;
			}
			if (mag.at<uchar>(i, j) > threshold_high) {
				dst.at<uchar>(i, j) = STRONG_EDGE;
			}
		}
	}
}

void brainHemorrhageDetection() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src, gray, resize, blur, sobelxy, canny, morph_open, morph_close, dst, dst2;
		src = imread(fname);

		#pragma region Pre-processing
		// convert to grayscale
		cvtColor(src, gray, CV_BGR2GRAY);
		// resize cu interpolare
		resizeImg(src, resize, 256, true);
		// blur pt detectare mai buna a marginilor (reduce zgomote)
		GaussianBlur(resize, blur, Size(3, 3), 0);
		// sobel edge detection
		Sobel(blur, sobelxy, CV_64F, 1, 1, 5);
		Canny(blur, canny, 170, 200, 3, false);
		#pragma endregion

#pragma region Morphological Operations
		//reconstruct- implementez echivalent la imreconstruct din matlab
		// opening by reconstruction
		morph_open = openingByReconstruction(blur);
		// closing by reconstruction
		morph_close = closingByReconstruction(morph_open);
		// complement image
		Mat comlpemented_image;
		bitwise_not(morph_close, comlpemented_image);
		// calculate regional maxima
		non_maxima_suppression(morph_open, canny, false);

		// superimpose the image

#pragma endregion

#pragma region Segmentation

		watershedAlgo(morph_open, dst);
		Mat r2, r;
		//bitwise_not(dst, r2);
		cv::Mat img_bw;
		//cvtColor(dst, dst, CV_BGR2GRAY);
		//cv::adaptiveThreshold(dst, img_bw, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 13, 0);
		threshold(dst, img_bw, 200.0, 255.0, THRESH_BINARY);
		watershedAlgo(img_bw, r);

#pragma endregion

#pragma region Results
		imshow("input image", src);
		imshow("result", dst);
		imshow("result2", r);
		
#pragma endregion

		waitKey();
		destroyAllWindows();
	}
}

void tresh() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src, dst;
		src = imread(fname);
		cvtColor(src, src, CV_BGR2GRAY);
		cv::adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 13, 0);
		imshow("result", dst);
	}
}
int main()
{
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Image negative - diblook style\n");
		printf(" 4 - BGR->HSV\n");
		printf(" 5 - Resize image\n");
		printf(" 6 - Canny edge detection\n");
		printf(" 7 - Edges in a video sequence\n");
		printf(" 8 - Snap frame from live video\n");
		printf(" 9 - Mouse callback demo\n");
		printf(" 10 - Proiect");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d", &op);
		switch (op)
		{
		case 1:
			testOpenImage();
			break;
		case 2:
			testOpenImagesFld();
			break;
		case 3:
			testParcurgereSimplaDiblookStyle(); //diblook style
			break;
		case 4:
			//testColor2Gray();
			testBGR2HSV();
			break;
		case 5:
			testResize();
			break;
		case 6:
			testCanny();
			break;
		case 7:
			testVideoSequence();
			break;
		case 8:
			testSnap();
			break;
		case 9:
			testMouseClick();
			break;
		case 10:
			brainHemorrhageDetection();
			break;
		}
	} while (op != 0);
	return 0;
}