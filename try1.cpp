#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include<time.h>
#include<map>
#include<string>
#include<vector>
#include<omp.h>
#include<math.h>
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

Mat shuffle(int m,int n,Mat img,int t)
{
	vector<int> key1=factor(m);
	int x=(int)((float)key1.size()*t/30);
	//cout<<x;
	x=key1.at(x);
	//cout<<" "<<x<<"x"<<endl;
	//cout<<2<<endl;
	Mat img1=img.clone();
	int *key=keygen(0.01,3.95,x);
	//cout<<m<<endl;
	//cout<<img1.at<Vec3b>(m,n)<<endl;
	for(int i=0;i<m;i+=x)
	{
		for(int k=0;k<x;k++)
		{
			for(int j=0;j<n;j++)
			{
				//cout<<i+k<<","<<j;
				//cout<<" "<<i+key[k]<<","<<j<<endl;
				img1.at<Vec3b>(i+k,j)=img.at<Vec3b>(i+key[k],j);
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


Mat dna_encode(Mat img,string pass[],int len)
{
	vector<Mat> channels;
	split(img,channels);
	Mat red=channels[2];
	Mat green=channels[1];
	Mat blue=channels[0];
	char a;
	int m=img.size().height;
	int n=img.size().width;
	int pos,i,j,randval;
	omp_set_num_threads(10);

#pragma omp parallel for private(i,j) shared(red,green,blue)
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
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
	string password=argv[2];

    VideoCapture vid_capture("video.mp4");
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
		cout<<f_count<<endl;
        VideoWriter output("output.mp4", VideoWriter::fourcc('X', 'V', 'I', 'D'),fps, frame_size);
		Mat img,frame;
		string *pass=dnaxor(password);
        while (vid_capture.read(frame))
        {
			i++;
			cout<<(float)i/f_count*100<<endl;

			frame=shuffle(frame_height,frame_width,frame,6);


			frame=dna_encode(frame,pass,len);
			output.write(frame);
			frame.release();

		}
    vid_capture.release();
    output.release();
    }
	return 0;
}

