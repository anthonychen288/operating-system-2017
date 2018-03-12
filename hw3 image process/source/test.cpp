#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    Mat image2 = image.clone();
    
    int height = image.rows;
    int width = image.cols;

    for(int i=0; i < height;i++){
		for(int j=0;j<width;j++){
			image2.at<uchar>(i,j) = 255 - image2.at<uchar>(i,j);
		}
    }
    
    imwrite("gray_negative.jpg",image2);

    //waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
