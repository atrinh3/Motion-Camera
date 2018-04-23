#include <stdio.h>
#include <time.h>


// take a picture after 2s, output to file "image.jpg", lower quality
// raspistill -t 2000 -o image.jpg -q 5

// take a picture after 2s, output to file "image.jpg", custom dimensions
// raspistill -t 2000 -o image.jpg -w 640 -h 480

void getImage(imageLocation)
{
    // snap a picture from raspberry
    cmd = "raspistill -o " + imageLocation + ".jpg -w 640 -h 480";
    *imageLocation = takepicture;
}

void getDifference(referenceLocation, compareLocation)
{
    ref = *reference;
    comp = *compare;
}


bool motion(&imageDifference)
{
    // check to see if there's a significant blob on the screen
    return false;
    return true;
}


// install MP4Box w/:
// sudo apt-get install -y gpac

// # Capture 30 seconds of raw video at 640x480 and 150kB/s bit rate into a pivideo.h264 file:
// raspivid -t 30000 -w 640 -h 480 -fps 25 -b 1200000 -p 0,0,640,480 -o pivideo.h264 

// # Wrap the raw video with an MP4 container: 
// MP4Box -add pivideo.h264 pivideo.mp4

// # Remove the source raw file, leaving the remaining pivideo.mp4 file to play
// rm pivideo.h264

// take a 10s video using .h264 format
// raspivid -o video.h264 -t 10000
void streamVideo(recordDuration)
{
    cmd = "raspivid -o h264.h264 -t %i", recordDuration * 1000;
    
    cmd = "MP4Box -add h264.h264 VideoStream.mp4"
    
    cmd = "rm h264.h264"
}


int main(void)
{
    timer_t timer;
    updateTime = 600;    // seconds (10 min)
    senseTime  = 2;      // seconds
    width      = 640;
    height     = 480;
    lowPower   = false;
    
    refBuffer  = (int*)malloc(width*height*sizeof(int));
    compareBuffer = (int*)malloc(width*height*sizeof(int));
    diffBuffer = (int*)malloc(width*height*sizeof(int));
    
    getImage(&refBuffer);
    startTime = timer;
    while(lowPower)
    {
        sleep(senseTime * 1000);
        // if enough time has elapsed since the reference image,
        // it is time to update the reference image.
        if(difftime(timer, startTime) >= updateTime)
        {
            startTime = timer;
            getImage(&refBuffer);
            sleep(senseTime * 1000);
        }
        getImage(&compareBuffer);
        diffImage = getDifference(&refBuffer, &compareBuffer);
        if(motion(&diffImage))
        {
            lowPower = false;
            while(!lowPower)
            {
                videoStream(10);
            }
        }
    }
    
    free(refBuffer);
    free(compareBuffer);
    free(diffBuffer);
}
