#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{

  VideoCapture cap(-1);

  //check if the file was opened properly
  if(!cap.isOpened())
  {
      cout << "Webcam could not be opened succesfully" << endl;
      exit(-1);
  }
  else
  {
      cout << "Webcam is OK! I found it!\n" << endl;
  }

  int w = 960;
  int h = 544;
  cap.set(CV_CAP_PROP_FRAME_WIDTH, w);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, h);

  Mat frame;
  cap >> frame;

  // converts the image to gray scale
  Mat frame_in_gray;
  cvtColor(frame, frame_in_gray, CV_BGR2GRAY);

  // process the Canny algorithm
  cout << "processing image with Canny..." << endl;
  int threshold1 = 0;
  int threshold2 = 28;
  Canny(frame_in_gray, frame_in_gray, threshold1, threshold1);
  
  // saving the images in the files system

  cout << "Saving the images..." << endl;
  imwrite("captured.jpg", frame);
  imwrite("captured_with_edges.jpg", frame_in_gray);

  // release the camera
  cap.release();

  return 0;
}
