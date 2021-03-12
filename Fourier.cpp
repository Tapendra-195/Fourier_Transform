/*#############################################################################
#  Program for doing Fourier transform on the data supplied.                  #
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
const float PI = acos(-1);

//Complex number class
class comp{
public:
  float real, img,freq,amp,phase;
  //Complex number constructor.
  comp(float r, float i){
    real =r;
    img=i;
    amp = sqrt(r*r+i*i);//Magnitude of complex number
    phase = atan2(i,r);//Angle of complex number from real axis.
  }

  //product of two complex number.
  comp operator *(comp b){
    comp p = comp(this->real*b.real - this->img*b.img, this->img*b.real+this->real*b.img); 
    return p;
  }

  //Assignment operator for complex number
  void operator =(comp b){
    this->real = b.real;
    this->img = b.img;
    this->freq = b.freq;
    this->amp = b.amp;
    this->phase = b.phase;
  }
  
  //Addition operator for complex number
  comp operator +(comp b){
    comp p = comp(this->real+b.real, this->img+b.img);
    return p;
  }
};

//Discrete Fourier Transform function.
//Returns complex number for complex valued data input.
vector<comp> FourierT(vector<comp>& ft){
  vector<comp> Xk; //Result(output)
  int N = ft.size();
  
  for(int k=0; k<N;k++){
    comp xk(0,0);
    for(int n=0; n<N; n++){
      //using formula for discrete fourier transform.
      float ang = 2.0*PI*k*n/N;
      float Re = cos(ang)/N;
      float Im = -1.0*sin(ang)/N;
      comp p(Re,Im);
      xk= xk+(ft[n]*p);
    }
    
    xk.freq = k; //Storing the frequency information.
    Xk.push_back(xk);
  }
  
  return Xk;
}

int main(int argc, char **argv){
  //Reading the coordinates from the text file
  vector<comp> ft;
  string line;
  ifstream myfile ("coordinates.txt");
  if (myfile.is_open())
    {
      int tenth=0;
      while ( getline (myfile,line) )
	{
	  size_t x_ind= line.find('\t');
	  string x1= line.substr(0,x_ind);
	  string y1= line.substr(x_ind);
	  float x=std::stof(x1)-500;  //shifting the origin to the centre(almost) of the image (500,400).
	  float y=std::stof(y1)-500;
	  if(tenth==0){
	  ft.push_back(comp(x,y));
	  }
	  tenth++;
	  if(tenth==10){ //only keep every tenth data from text file. There is too much data recorded.
	    tenth=0;
	  }
	}
      myfile.close();
    }
    
  
  vector<comp> out = FourierT(ft); //Applying Fourier transform.
  
  //sorting the output according to magnitude of complex number. This step is optional. It makes the result look cleaner. If you do this you lineup circles according to size resulting in cleaner image.
  int swap =1;
  while(swap!=0){
    swap =0;
    for(int i=0; i<out.size();i++){
      comp prev= out[i];
      comp next = out[i+1];
      //Sort in decreasing order of magnitude of complex number.
      if(next.amp>prev.amp){
	out[i]=next;
	out[i+1]=prev;
	swap++;
      }
    }
  }
  
  float t=0;
  vector<float> wave;
  std::vector<Point> vec;
  float N = out.size();
  for(;;){
    Mat m = cv::Mat(cv::Size(1000, 1000), CV_64FC3, Scalar(0,0,0)); //Canvas to draw into.
    //Shifting to the centre(almost) of the image.
    float x = 500;
    float y = 500;
    //This is like Phaser. At a constant time you add all the involved fourier terms and you get your the location.
    for(int i =0; i<out.size();i++){
      float x0=x;
      float y0=y;
      float f = out[i].freq; //Frequency of component
      float a = out[i].amp; //Magnitude of Complex number.
      x += a*cos(2*PI*f*t/N + out[i].phase);
      y += a*sin(2*PI*f*t/N + out[i].phase);
      
      circle (m, Point( x0, y0 ), a, Scalar(255,110,187), 1, 0 );
      cv::line(m, Point(x0,y0), Point(x,y), cv::Scalar(0,255,0),1); 
    }
    
    vec.push_back(Point(x,y));
    std::vector<Point> r_vec(vec.rbegin(), vec.rend()); //Reversing so that if something is deleted it's older one.
    if (r_vec.size()>999){ //delete if the size of vector exceedes 700
      r_vec.pop_back();
      vec.clear();
      vec = vector<Point>(r_vec.rbegin(), r_vec.rend());
    }

    //Now draw the line from points.
    polylines(m,r_vec,false,Scalar(0,245,250),1);
    
    imshow("live",m);
    if (waitKey(5) >= 0)
      break;
    
    t+=1; //change time.
    //clear the canvas after a entire loop ends.
    if(t==out.size()+5){
      t=0;
      vec.clear();
      r_vec.clear();
    }
  }
  
  return 0;
}
