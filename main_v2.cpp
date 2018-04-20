#include <stdio.h>
#include <time.h>


// take a picture after 2s, output to file "image.jpg", lower quality
// raspistill -t 2000 -o image.jpg -q 5

// take a picture after 2s, output to file "image.jpg", custom dimensions
// raspistill -t 2000 -o image.jpg -w 640 -h 480

void getImage()
{
    // snap a picture from raspberry
}


bool motion(imageDifference)
{
    // check to see if there's a significant blob on the screen
}


void streamVideo(recordingNumber)
{
    filename = "Video file %i" recordingNumber;
    // record video on raspberry and then save to filename
}


int main(void)
{
    timer_t timer;
    updateTime = 600;    // seconds (10 min)
    width      = 640;
    height     = 480;
    lowPower   = false;
    
    refBuffer  = (float*)malloc(width*height*sizeof(float));
    compBuffer = (float*)malloc(width*height*sizeof(float));
    diffBuffer = (float*)malloc(width*height*sizeof(float));
    
    refImage  = getImage(*refBuffer);
    startTime = timer;
    while(lowPower)
    {
        sleep(1000);
        // if enough time has elapsed since the reference image,
        // it is time to update the reference image.
        if(difftime(timer, startTime) >= updateTime)
        {
            startTime = timer;
            refImage = getImage(*refBuffer);
            sleep(1000);
        }
        compImage = getImage(*compBuffer);
        diffImage = getDifference(refImage, compImage);
        if(motion(diffImage))
        {
            lowPower = false;
            while(!lowPower)
            {
                videoStream();
            }
        }
    }
    
    free(refBuffer);
    free(compBuffer);
    free(diffBuffer);
}













