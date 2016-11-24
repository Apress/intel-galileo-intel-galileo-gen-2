#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eye_cascade_name = "haarcascade_eye.xml";

void faceDetect(Mat img);
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

using namespace cv;
using namespace std;

int main(int argc, const char *argv[])
{

  if( !face_cascade.load( face_cascade_name ) )
  { 
    cout << face_cascade_name << " not found!! aborting..." << endl;
    exit(-1); 
  };

  if( !eyes_cascade.load( eye_cascade_name ) )
  { 
    cout << eye_cascade_name << " not found!! aborting..." << endl;
    exit(-1); 
  };

  // 0 is the ID of the built-in laptop camera, change if you want to use other camera
  VideoCapture cap(-1);

  //check if the file was opened properly
  if(!cap.isOpened())
  {
      cout << "Capture could not be opened succesfully" << endl;
      return -1;
  }
  else
  {
      cout << "camera is ok\n" << endl;
  }

  int w = 432;
  int h = 240;
  cap.set(CV_CAP_PROP_FRAME_WIDTH, w);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, h);

  Mat frame;
  cap >> frame;

  cout << "processing the image...." << endl;

  faceDetect(frame);
  imwrite("face_and_eyes.jpg", frame);

  // release the camera
  cap.release();
      
  cout << "done!" << endl;
  return 0;
}

void faceDetect(Mat img)
{
  std::vector<Rect> faces;
  std::vector<Rect> eyes;
  bool two_eyes = false;
  bool any_eye_detected = false;

  //detecting faces
  face_cascade.detectMultiScale( img, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  if (faces.size() == 0) 
  {
       cout << "Try again.. I did not dectected any faces..." << endl;
       return;
  }

  // it is possible to face more than one human face in the image
  for( size_t i = 0; i < faces.size(); i++ )
  {

     // rectangle in the face
     rectangle( img, faces[i], Scalar( 255, 100, 0 ), 4, 8, 0 );
     
     Mat frame_gray;
     cvtColor( img, frame_gray, CV_BGR2GRAY );

     // croping only the face in region defined by faces[i]
     std::vector<Rect> eyes;
     Mat faceROI = frame_gray( faces[i] );

     // In each face, detect eyes
     eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
     
     for( size_t j = 0; j < eyes.size(); j++ )
      {
         Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
         int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
         circle( img, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );

      }
     
    }


}
