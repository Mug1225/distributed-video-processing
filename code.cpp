#include <opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include <bits/stdc++.h>
#include<time.h>
#include<map>
#include<string>
#include<vector>
#include<omp.h>
#include<math.h>
#include<mpi.h>
#include<cstdio>


using namespace std;
using namespace cv;


map<int, string> int_to_dna;
map<string, string> dna_xor;
map<string, int> dna_to_int;


int* keygen(float x,float r,int size)
{
   float temp1;
   float key[size];
   int *index=new int[size];
   int temp;
   for(int i=0;i<size;i++)
   {
       x=r*x*(1-x);
       key[i]=x;
       index[i]=i;
   }
   for(int i=0;i<size;i++)
   {
       for(int j=0;j<size;j++)
       {
           if(key[i]>key[j])
           {
               temp1=key[i];
               key[i]=key[j];
               key[j]=temp1;


               temp=index[i];
               index[i]=index[j];
               index[j]=temp;
           }
       }
   }
   return index;
}


vector<int> factor(int m)
{
   vector<int> key1;
   for(int i=2;i<m+1;i++)
   {
       if(m%i==0)
           key1.push_back(i);
   }
   return key1;
}


Mat shuffle(int m,int n,Mat img,int x1,int y1,int height,int width,int t)
{
   height+=10-(height%10);
   width+=10-(width%10);
   vector<int> key1=factor(height);
   int x=(int)((float)key1.size()*t/10);
   //cout<<x;
   x=key1.at(x);
   Mat img1=img.clone();
   int *key=keygen(0.01,3.95,x);
   //cout<<"\t "<<x1+height<<" "<<x1<<" "<<height<<" "<<y1+width<<endl;
//#pragma omp parallel for shared(img,img1)
   for(int i=x1;i<x1+height;i+=x)
   {
       for(int k=0;k<x;k++)
       {
           for(int j=y1;j<y1+width;j++)
           {
               //cout<<i+k<<","<<j;
               //cout<<" "<<i+key[k]<<","<<j<<endl;
               img1.at<Vec3b>(i+k,j)=img.at<Vec3b>(i+key[k],j);
           }
       }
   }


   key1=factor(width);
   x=(int)((float)key1.size()*t/10);
   x=key1.at(x);


   key=keygen(0.01,3.95,x);
   for(int i=y1;i<y1+width;i+=x)
   {
       for(int k=0;k<x;k++)
       {
           for(int j=x1;j<x1+height;j++)
           {
               //cout<<i+k<<","<<j;
               //cout<<" "<<i+key[k]<<","<<j<<endl;
               img1.at<Vec3b>(j,i+k)=img.at<Vec3b>(j,i+key[k]);
           }
       }
   }
   img.release();
   return img1;
}


string* dnaxor(string a)
{
   string* pass=new string[a.length()*4];
   int temp,dictval,pos;
   for(int i=0;i<a.length();i++)
   {
       temp=(int)a.at(i);
       pos=4*i;
       dictval=temp%4;
       pass[pos+0]=int_to_dna.at(dictval);
       temp=temp>>2;
       dictval=temp%4;
       pass[pos+1]=int_to_dna.at(dictval);
       temp=temp>>2;
       dictval=temp%4;
       pass[pos+2]=int_to_dna.at(dictval);
       temp=temp>>2;
       dictval=temp%4;
       pass[pos+3]=int_to_dna.at(dictval);
   }
   return pass;
}


char dna(uchar a,string s1,string s2,string s3, string s4)
{




   string out[4];
   char temp=a;
   temp=a%4;
   out[0]=int_to_dna.at(temp);
   a=a>>2;
   temp=a%4;
   out[1]=int_to_dna.at(temp);
   a=a>>2;
   temp=a%4;
   out[2]=int_to_dna.at(temp);
   a=a>>2;
   temp=a%4;
   out[3]=int_to_dna.at(temp);
   char val=0;


   //cout<<out[0]+s1<<" "<<out[1]+s2<<" "<<out[2]+s2<<" "<<out[3]+s4<<endl;
   val+=dna_to_int.at(dna_xor.at(out[0]+s1));
   val=val<<2;
   val+=dna_to_int.at(dna_xor.at(out[1]+s2));
   val=val<<2;
   val+=dna_to_int.at(dna_xor.at(out[2]+s3));
   val=val<<2;
   val+=dna_to_int.at(dna_xor.at(out[3]+s4));
   return val;
}




Mat dna_encode(Mat img,string pass[],int len,int x1,int y1,int height, int width)
{
   vector<Mat> channels;
   split(img,channels);
   Mat red=channels[2];
   Mat green=channels[1];
   Mat blue=channels[0];
   char a;
   //int m=img.size().height;
   //int n=img.size().width;
   int pos,i,j,randval;
   height+=10-(height%10);
   width+=10-(width%10);


#pragma omp parallel for private(i,j) shared(red,green,blue)
   for(i=x1;i<x1+height;i++)
   {
       for(j=y1;j<y1+width;j++)
       {
       try{
           pos=(4*(i+j))%len;
           randval=rand()%3;
           switch(randval)
           {
               case 0:
                   a=red.at<uchar>(i,j);
                   a=dna(a,pass[pos],pass[pos+1],pass[pos+2],pass[pos+3]);
                   red.at<uchar>(i,j)=(uchar)a;
                   break;
               case 1:
                   a=green.at<uchar>(i,j);
                   a=dna(a,pass[pos+0],pass[pos+1],pass[pos+2],pass[pos+3]);
                   green.at<uchar>(i,j)=(uchar)a;
                   break;
               case 2:
                   a=blue.at<uchar>(i,j);
                   a=dna(a,pass[pos+0],pass[pos+1],pass[pos+2],pass[pos+3]);
                   blue.at<uchar>(i,j)=(uchar)a;
                   break;
           }
           //cout<<"         "<<i<<" "<<j<<" "<<(int)red.at<uchar>(i,j)<<" "<<(int)blue.at<uchar>(i,j)<<" "<<(int)green.at<uchar>(i,j)<<" "<<endl;
       }
       catch(const std::out_of_range& e)
       {
           cout<<"error at "<<i<<" "<<j<<" "<<(int)red.at<uchar>(i,j)<<" "<<(int)blue.at<uchar>(i,j)<<" "<<(int)green.at<uchar>(i,j);
           //cout<<" "<<pass[pos]<<" "<<pass[pos+1]<<" "<<pass[pos+2]<<" "<<pass[pos+3]<<" "<<a<<endl;
           //dna2(a,pass[pos],pass[pos+1],pass[pos+2],pass[pos+3]);


           break;
       }
       }
   }


   vector<Mat> channels1;
   channels1.push_back(blue);
   channels1.push_back(green);
   channels1.push_back(red);
   Mat combinedImage;
   merge(channels1, combinedImage);
   return combinedImage;
}




int main(int argc, char** argv)
{
   int_to_dna[0]="A";
   int_to_dna[1]="T";
   int_to_dna[2]="G";
   int_to_dna[3]="C";


   dna_xor["AA"]=dna_xor["TT"]=dna_xor["GG"]=dna_xor["CC"]="A";
   dna_xor["AG"]=dna_xor["GA"]=dna_xor["TC"]=dna_xor["CT"]="G";
   dna_xor["AC"]=dna_xor["CA"]=dna_xor["GT"]=dna_xor["TG"]="C";
   dna_xor["AT"]=dna_xor["TA"]=dna_xor["GC"]=dna_xor["CG"]="T";


   dna_to_int["A"]=0;
   dna_to_int["T"]=1;
   dna_to_int["G"]=2;
   dna_to_int["C"]=3;


   int i=0;
   string vidname=argv[1];
   int Factor=stoi(argv[2]);
   string password=argv[3];


   VideoCapture vid_capture(vidname);
   if(!vid_capture.isOpened())
   {
       cout<<"Eroor opening the file check and try again";
   }
   else
   {
       int fps=vid_capture.get(5);
       int f_count=vid_capture.get(7);
       int frame_width = static_cast<int>(vid_capture.get(3));
       int frame_height = static_cast<int>(vid_capture.get(4));
       int len=password.length()*4;
       Size frame_size(frame_width, frame_height);
       cout<<f_count<<" "<<frame_width<<" "<<frame_height<<endl;
       string *pass=dnaxor(password);
       vid_capture.release();


   omp_set_num_threads(10);
       int node;


       MPI_Init(&argc,&argv);
       MPI_Comm_rank(MPI_COMM_WORLD, &node);
       if(node==3)
       {
           String text="";
           Mat video_stream;
           VideoCapture vid_capture(vidname);
           string trained_classifier_location = "/home/mugesh/opencv-4.x/data/haarcascades/haarcascade_frontalface_alt.xml";
           CascadeClassifier faceDetector;
           faceDetector.load(trained_classifier_location);
           vector<Rect>faces;
           int k=0;
           int x[5];


           while (vid_capture.read(video_stream)){// reading frames from camera and loading them in 'video_stream' Matrix//
               k++;
               cout<<"face "<<k<<endl;
               x[0]=k;
               x[1]=x[2]=x[3]=x[4]=0;
               //cv::cvtColor(video_stream, video_stream, cv::COLOR_BGR2GRAY);
               faceDetector.detectMultiScale(video_stream, faces, 1.1, 4, CASCADE_SCALE_IMAGE, Size(30, 30));//Detecting the faces in 'image_with_humanfaces' matrix//
               for (int i = 0; i < faces.size(); i++){ //for locating the face
                   Mat faceROI = video_stream(faces[i]);//Storing face in the matrix//
                   x[2] = faces[i].x;//Getting the initial row value of face rectangle's starting point//
                   x[1] = faces[i].y;//Getting the initial column value of face rectangle's starting point//
                   x[3] = faces[i].height;//Calculating the height of the rectangle//
                   x[4] = faces[i].width;//Calculating the width of the rectangle//


                   //rectangle(video_stream, Point(x[0], x[1]), Point(x[0]+x[4], x[1]+x[3]), Scalar(255, 0, 255), 2, 8, 0);
               }
               text="temp/fdetect"+to_string(k)+".jpg";
               imwrite(text,video_stream);
               MPI_Send( x,5,MPI_INT,2,1,MPI_COMM_WORLD);
               video_stream.release();
           }
           x[0]=-5;
           MPI_Send( x,5,MPI_INT,2,1,MPI_COMM_WORLD);
           vid_capture.release();


       }
       if(node==2)
       {
           Mat frame;
           String text="";
           char* fname;
           int x[5];
           while (true)
           {


               MPI_Recv(x,5,MPI_INT,3,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
               if(x[0]==-5)
               {
                   break;
               }
               //cout<<x[0]<<" "<<x[1]<<" "<<x[2]<<" "<<x[3]<<" "<<x[4]<<endl;
               cout<<"shuffle  "<<x[0]<<endl;
               text="temp/fdetect"+to_string(x[0])+".jpg";
               fname=&text[0];
               frame=imread(text);
               if(!(x[1]==0&&x[3]==0))
               frame=shuffle(frame_height,frame_width,frame,x[1],x[2],x[3],x[4],Factor);
               remove(fname);
               text="temp/shuffle"+to_string(x[0])+".jpg";
               imwrite(text,frame);
               i=x[0];
               MPI_Send( x,5,MPI_INT,1,0,MPI_COMM_WORLD);
               frame.release();
           }
           //MPI_Send( &i,1,MPI_INT,1,0,MPI_COMM_WORLD);
           x[0]=-5;
           i=x[0];
           MPI_Send( x,5,MPI_INT,1,0,MPI_COMM_WORLD);
           vid_capture.release();
           //output.release();
           cout<<"done\n";
       }
       if(node==1)
       {
           //VideoCapture vid_capture2("output.mp4");
           int find=vidname.find('.');
           string outpu=vidname.substr(0,find)+"out.mp4";
           VideoWriter output(outpu, VideoWriter::fourcc('m', 'p', '4', 'v'),fps, frame_size);
           MPI_Status status;
           int size,j,x[5];
           string text;
           Mat frame1;
           char* fname;
           while(true)
           {


               MPI_Recv(x,5,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
               text ="temp/shuffle"+to_string(x[0])+".jpg";
               fname=&text[0];
               if(x[0]==-5)
               {
                   output.release();
                   break;


               }
               frame1=imread(text);
               cout<<(float)x[0]/f_count*100<<endl;
               if(x[1]!=0&&x[2]!=0)
                   frame1=dna_encode(frame1,pass,len,x[1],x[2],x[4],x[3]);
                   output.write(frame1);
                   frame1.release();
                   remove(fname);
           }
       }
        MPI_Finalize();
   }
   return 0;
}
