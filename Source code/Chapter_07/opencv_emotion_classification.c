/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 *
 *  Manoel Ramon - 06/15/2014 
 *  manoel.ramon@gmail.com
 *                 code changed from original facerec_fisherface.cpp
 *                 added:
 *                 - adaption to emotions detection instead gender
 *                 - picture took from the default video device
 *                 - added face and eyes recognition
 *                 - crop images based in human anatomy 
 *                 - prediction based in face recognized 
 *
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eye_cascade_name = "haarcascade_eye.xml";

Mat faceDetect(Mat img);
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

using namespace cv;
using namespace std;

enum EmotionState_t {
  SMILE     =0,   // 0
  SURPRISED,      // 1
  SERIOUS,        // 2
};

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

int main(int argc, const char *argv[])
{
  
  EmotionState_t emotion;
  
  // Check for valid command line arguments, print usage
  // if no arguments were given.
  if (argc < 2) {
    cout << "usage: " << argv[0] << " <csv.ext> <output_folder> " << endl;
    exit(1);
  }
    
  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
  if( !eyes_cascade.load( eye_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
  
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
      cout << "camera is ok.. Stay 2 ft away from your camera\n" << endl;
  }
  
  int w = 432;
  int h = 240;
  cap.set(CV_CAP_PROP_FRAME_WIDTH, w);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, h);
  
  Mat frame;
  cap >> frame;
  
  cout << "processing the image...." << endl;
  
  Mat testSample = faceDetect(frame);
  
  // Get the path to your CSV.
  string fn_csv = string(argv[1]);
  // These vectors hold the images and corresponding labels.
  vector<Mat> images;
  vector<int> labels;
  // Read in the data. This can fail if no valid
  // input filename is given.
  try 
  {
    read_csv(fn_csv, images, labels);
  } catch (cv::Exception& e) {
    cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
    // nothing more we can do
    exit(1);
  }
  // Quit if there are not enough images for this demo.
  if(images.size() <= 1) 
  {
    string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
    CV_Error(CV_StsError, error_message);
  }
  // Get the height from the first image. We'll need this
  // later in code to reshape the images to their original
  // size:
  int height = images[0].rows;
  
  // The following lines create an Fisherfaces model for
  // face recognition and train it with the images and
  // labels read from the given CSV file.
  // If you just want to keep 10 Fisherfaces, then call
  // the factory method like this:
  //
  //      cv::createFisherFaceRecognizer(10);
  //
  // However it is not useful to discard Fisherfaces! Please
  // always try to use _all_ available Fisherfaces for
  // classification.
  //
  // If you want to create a FaceRecognizer with a
  // confidence threshold (e.g. 123.0) and use _all_
  // Fisherfaces, then call it with:
  //
  //      cv::createFisherFaceRecognizer(0, 123.0);
  //
  Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
  model->train(images, labels);
  
  // The following line predicts the label of a given
  // test image:
  int predictedLabel = model->predict(testSample);
  
  // To get the confidence of a prediction call the model with:
  //
  //      int predictedLabel = -1;
  //      double confidence = 0.0;
  //      model->predict(testSample, predictedLabel, confidence);
  //
  string result_message = format("Predicted class = %d", predictedLabel);
  cout << result_message << endl;
  
  // giving the result
  switch (predictedLabel)
  {
    case SMILE:
      cout << "You are happy!" << endl;
      break;
    case SURPRISED:
      cout << "You are surprised!" << endl;
      break;
    case SERIOUS:
      cout << "You are serious!" << endl;
      break;
  }
  
  return 0;
    
  cap.release();
  
  return 0;
}

Mat faceDetect(Mat img)
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
       exit(-1);  // abort everything
  }

  Point p1 = Point(0,0);
  for( size_t i = 0; i < faces.size(); i++ )
  {

    // we cannot draw in the image !!! otherwise will mess with the prediction
    // rectangle( img, faces[i], Scalar( 255, 100, 0 ), 4, 8, 0 );

     
     Mat frame_gray;
     cvtColor( img, frame_gray, CV_BGR2GRAY );

     // croping only the face in region defined by faces[i]
     std::vector<Rect> eyes;
     Mat faceROI = frame_gray( faces[i] );

     //In each face, detect eyes
     eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 3, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
     
      for( size_t j = 0; j < eyes.size(); j++ )
      {
         Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
         // we cannot draw in the image !!! otherwise will mess with the prediction
         // int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
         // circle( img, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );

         if (j==0) 
	   {
              p1 = center; 
	      any_eye_detected = true;
	   }
         else
         {
	      two_eyes = true;
	 }
      }
     
    }

  cout << "SOME DEBUG" << endl;
  cout << "-------------------------" << endl;
  cout << "faces detected:" << faces.size() << endl;
  cout << "x: " << faces[0].x << endl; 
  cout << "y: " << faces[0].y << endl; 
  cout << "w: " << faces[0].width << endl;
  cout << "h: " << faces[0].height << endl << endl;

  Mat imageInRectangle;
  imageInRectangle =  img(faces[0]);
  Size recFaceSize = imageInRectangle.size();

  cout << recFaceSize << endl;

  // for debug
  imwrite("imageInRectangle.jpg", imageInRectangle);
  

  double crop_x_prop = 0.1;
  double crop_y_prop = 0.15;

  int rec_w = 0;
  int rec_h = faces[0].height * 0.64;


  // checking the (x,y) for cropped rectangle
  // based in human anatomy
  int px = 0;
  int py = 2 * 0.125 * faces[0].height;

  Mat cropImage;


  cout << "faces[0].x:" << faces[0].x << endl;
  p1.x = p1.x - faces[0].x;
  cout << "p1.x:" << p1.x << endl;
  if (any_eye_detected)
  {
      if (two_eyes)
      {
	  cout << "two eyes detected" << endl;
	  // we have detected two eyes
          // we have p1 and p2
          // left eye
          px = p1.x /  1.35;

      }
      else
      {
	  // only one eye was found.. need to check if the 
          // left or right eye
          // we have only p1
          if (p1.x > recFaceSize.width/2)
          {
              // right eye
            cout << "only right eye detected" << endl;
	    px = p1.x / 1.75;  

          }
          else
          {
              // left eye
            cout << "only left eye detected" << endl;
            px = p1.x /  1.35;
          }
      }

  }
  else
  {
   
      // no eyes detected but we have a face
      px = 25;
      py = 25;
      rec_w = recFaceSize.width-50;
      rec_h = recFaceSize.height-30;

  }

  rec_w = (faces[0].width - px) * 0.75;
  cout << "px   :" << px << endl;
  cout << "py   :" << py << endl;
  cout << "rec_w:" << rec_w << endl;
  cout << "rec_h:" << rec_h << endl;

  cropImage = imageInRectangle(Rect(px, py, rec_w, rec_h));

  Size dstImgSize(70,70); // same image size of db
  Mat finalSizeImg;
  resize(cropImage, finalSizeImg, dstImgSize);

  // for debug
  imwrite("onlyface.jpg", finalSizeImg);
  
  cvtColor( finalSizeImg, finalSizeImg, CV_BGR2GRAY );

  return finalSizeImg;

}
