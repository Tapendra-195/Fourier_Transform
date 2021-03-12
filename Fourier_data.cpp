/*#############################################################################
#  Function to get the (x,y) location of points in image and write them in    #
#  a file as tab separated list of x tab y position. This then can be feed    #
#  into the code that will perform Fourier transform in the data to get the   #
#  frequency information form spatial information. The code will then use     #
#  obtained frequency and amplitude to draw the spatial information that was  #
#  passed.                                                                    #
#                                                                             #
##############################################################################*/

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/features2d.hpp>
#include <fstream>

using namespace cv;
using namespace std;

bool record = false; // 
float ix=-1,iy = -1; //variable to keep mouse position.
Mat img;
vector<Point>lne;
vector<int> prv_sz;
bool close_file=false;
ofstream myfile ("coordinates.txt");
int count1=0; //counts number of points added and stores so that we can undo using double right click.

//this function will be called every time you move your mouse over the image
// the coordinates will be in x and y variables
static void onMouse(int event,int x,int y,int,void*)
{
  Mat img2;
  img2 = img.clone();

  circle(img2,Point(x,y),3,Scalar(0,255,40),-1);
  polylines(img2,lne,false,Scalar(0,255,205),3);
  imshow("Image",img2);
    
  //Start recording coordinate after left mouse is clicked.
  if (event == EVENT_LBUTTONDOWN){
    record = true;
    ix=x; iy = y;
  }
  //Record the mouse position after the left mouse button is clicked and before left mouse button is released.
  else if (event == EVENT_MOUSEMOVE && record){
    lne.push_back(Point(x,y));
    count1++; //Counting number of position recorded so that we can use undo feature on right click.
  }
  if(event==EVENT_LBUTTONUP){
    prv_sz.push_back(count1);
    count1=0;
    record=false;
  }
  //undo feature on right click
  if(event == EVENT_RBUTTONDOWN){
    if(lne.size()!=0){
      for(int i=0;i<prv_sz[prv_sz.size()-1];i++){
	lne.pop_back();
      }
      prv_sz.pop_back();
    }
  }
  //save data and exit on double left click
  if(event == EVENT_LBUTTONDBLCLK){
    close_file=true;
    if(myfile.is_open()){
      for(int i=0; i<lne.size();i++){
	myfile<<lne[i].x<<'\t'<<lne[i].y<<std::endl;
      }
      myfile.close();
      destroyAllWindows();

      }
  }
}

int main(int argc, char **argv){
  if(argc==1){
    std::cerr<<"Please use following format and try again"<<std::endl;
    std::cerr<<"./Fourier_data [image path]"<<std::endl;
    return 1;
  }
  Mat a = imread(argv[1]); //Input image;
  img =a.clone();
  namedWindow("Image");
  setMouseCallback("Image",onMouse); //Callback function on mouse click.
  waitKey();
  
  return 0;
}
