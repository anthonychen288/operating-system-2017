#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "mpi.h"

#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const int MAXBYTES = 8 * 1024 * 1024;
uchar buffer[MAXBYTES];

void matsnd(const Mat& m,int dest){
    int rows  = m.rows;
    int cols  = m.cols;
    int type  = m.type();
    int channels = m.channels();
    memcpy(&buffer[0 * sizeof(int)],(uchar*)&rows,sizeof(int));
    memcpy(&buffer[1 * sizeof(int)],(uchar*)&cols,sizeof(int));
    memcpy(&buffer[2 * sizeof(int)],(uchar*)&type,sizeof(int));

    int bytespersample=1; // change if using shorts or floats
    int bytes=m.rows*m.cols*channels*bytespersample;
	
    memcpy(&buffer[3*sizeof(int)],m.data,bytes);
    MPI_Send(&buffer,bytes+3*sizeof(int),MPI_UNSIGNED_CHAR,dest,0,MPI_COMM_WORLD);
}

Mat matrcv(int src){
    MPI_Status status;
    int count,rows,cols,type,channels;

    MPI_Recv(&buffer,sizeof(buffer),MPI_UNSIGNED_CHAR,src,0,MPI_COMM_WORLD,&status);
    MPI_Get_count(&status,MPI_UNSIGNED_CHAR,&count);
    memcpy((uchar*)&rows,&buffer[0 * sizeof(int)], sizeof(int));
    memcpy((uchar*)&cols,&buffer[1 * sizeof(int)], sizeof(int));
    memcpy((uchar*)&type,&buffer[2 * sizeof(int)], sizeof(int));
	
    // Make the mat
    Mat received= Mat(rows,cols,type,(uchar*)&buffer[3*sizeof(int)]);
    return received;
}

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }
	
	int rank, size, start, end, interval;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    int height = image.rows;
    int width = image.cols;
	
	interval = height / size;
	start = rank*interval;
	if (rank == size - 1){
		end = height;
	}else{
		end = (rank + 1)*interval - 1 ;
	}
	interval = end-start;
	Mat image2(interval, image.cols, image.type());
	
	for(int i=0; i < interval;i++){
		for(int j=0;j<width;j++){
			int val = image.at<uchar>(start+i, j)*4;
			if((start+i)==0 && j==0)// upper left corner
			{	
				image2.at<uchar>(i, j) = (val + image.at<uchar>(start+i+1, j) * 2 + image.at<uchar>(start+i, j+1) * 2 
					+ image.at<uchar>(start+i+1, j+1)) / 9;
			}else if((start+i)==(height-1) && j==(width-1))// down right corner
			{	
				image2.at<uchar>(i, j) = (val + image.at<uchar>(start+i-1, j) * 2 + image.at<uchar>(start+i, j-1) * 2
					+ image.at<uchar>(start+i-1, j-1)) / 9;
			}else if((start+i)==0 && j==(height-1))// upper right corner
			{	
				image2.at<uchar>(i, j) = (val + image.at<uchar>(start+i, j-1) * 2 + image.at<uchar>(start+i+1, j) * 2
					+ image.at<uchar>(start+i+1, j-1)) / 9;
			}else if((start+i)==(width-1) && j==0)// down left corner
			{	
				image2.at<uchar>(i, j) = (val + image.at<uchar>(start+i-1, j) * 2 + image.at<uchar>(start+i, j+1) * 2
					+ image.at<uchar>(start+i-1, j+1)) / 9;
			}else if((start+i)==0 && j!=0){	// left margin
				image2.at<uchar>(i, j) = (val + (image.at<uchar>(start+i-1, j) + image.at<uchar>(start+i, j+1) + image.at<uchar>(start+i+1, j)) * 2 
						+ image.at<uchar>(start+i-1, j+1) + image.at<uchar>(start+i+1, j+1)) / 12;
			}else if((start+i)!=0 && j==0){	// upper margin
				image2.at<uchar>(i, j) = (val + (image.at<uchar>(start+i, j+1) + image.at<uchar>(start+i+1, j) + image.at<uchar>(start+i, j-1)) * 2 
						+ image.at<uchar>(start+i+1, j-1) + image.at<uchar>(start+i+1, j+1)) / 12;
			}else if((start+i)==width-1 && j !=0){	// right margin
				image2.at<uchar>(i, j) = (val + (image.at<uchar>(start+i+1, j) + image.at<uchar>(start+i-1, j) + image.at<uchar>(start+i, j-1)) * 2 
						+ image.at<uchar>(start+i-1, j-1) + image.at<uchar>(start+i-1, j+1)) / 12;
			}else if(j==height-1 && (start+i)!=0){	// down margin
				image2.at<uchar>(i, j) = (val + (image.at<uchar>(start+i, j-1) + image.at<uchar>(start+i-1, j) + image.at<uchar>(start+i, j+1)) * 2 + 
						image.at<uchar>(start+i-1, j-1) + image.at<uchar>(start+i-1, j+1)) / 12;
			}else{
				image2.at<uchar>(i, j) = (val + (image.at<uchar>(start+i+1, j) + image.at<uchar>(start+i, j+1) + image.at<uchar>(start+i-1, j) + image.at<uchar>(start+i, j-1))*2+
						image.at<uchar>(start+i+1, j+1)+image.at<uchar>(start+i+1, j-1)+image.at<uchar>(start+i-1, j+1)+image.at<uchar>(start+i-1, j-1))/16;
			}
		}
    }
	// GaussianBlur(image, image2, Size(3, 3), 0, 0);
	
	if (rank == 0)
	{
		for(int i=rank+1;i<size;i++)
		{
			Mat receive=matrcv(i);
			image2.push_back(receive);
		}
		/*imshow("gray_blur", image2);
		// Wait for a keystroke in the window
		waitKey(0);*/
		imwrite("gray_blur.jpg",image2);
	}else{
		matsnd(image2, 0);
	}
	
	MPI_Finalize();
	
    return 0;
}