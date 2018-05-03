#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bcm2835.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define GRAYSCALE_THRESHOLD 50
#define DISTANCE_THRESHOLD 100
#define AREA_THRESHOLD 10000
#define WIDTH 640
#define HEIGHT 480
#define DEBUG true

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
void streamVideo(recordDuration, *options)
{
    if((pid = fork()) = 0)
    {
        char **cmd;
        cmd = "raspivid -o h264.h264 -t %i", recordDuration * 1000;
        execv("/usr/bin/raspivid", cmd);
    }
    //cmd = "MP4Box -add h264.h264 VideoStream.mp4"
    //cmd = "rm h264.h264"
}


// take a picture after 2s, output to file "image.jpg", lower quality
// raspistill -t 2000 -o image.jpg -q 5

// take a picture after 2s, output to file "image.jpg", custom dimensions
// raspistill -t 2000 -o image.jpg -w 640 -h 480

void getImage(*options)
{
    if ((pid = fork()) == 0)
    {
        char **cmd;
        
        // count tokens in the passed non-default options string.
        int count = 0;
        char *copy;
        copy = strdup(options);
        if (strtok(copy, " \t") != NULL)
            count = 1;
            while (strtok(NULL, "\t") != NULL)
            {
                count++;
            }
        cmd = malloc((count + 4) * sizeof(char **));
        free(copy);
        
        if (count)
        {
            int i;
            copy = strdup(options);
            cmd[1] = strtok(copy, " \t");
            for (i = 2; i <= count; i++)
            {
                cmd[i] = strtok(NULL, " \t");
            }
        }
        // add default options
        cmd[0] = "raspistill";
        cmd[count + 1] = "-n";
        cmd[count + 2] = "-o temp.jpg";
        cmd[count + 3] = (char *)0;
        execv("/usr/bin/raspistill", cmd);
    }    
}


/*
This function will create the differential image that will be used to scan
for motion.  In order to conserve storage space, the output will be saved 
in place of the compare image.  This can be done because the compare image
will be refreshed anyways soon after this operation is performed.
*/
void getDifference(*reference, *compare)
{
    if (DEBUG)
    {
        printf("Subtracting images.\n");
    }
    // use the memory space for compare to store difference picture.
    cvAbsDiff(compare, reference, compare);
}


void filterBlobs(*blobVector)
{
    for (int i = 0; i < blobVector.size(); i++)
    {
        currentBlob = blobVector[i];
        if (currentBlob.getArea() < AREA_THRESHOLD)
        {
            blobVector.erase(blobVector.begin()+i);
            i--;
        }
    }
}

/*
Determines whether or not there's motion detected.  Uses the Blob class to
group detected changes in the pictures together.  Changes will be determined
by iterating through every pixel in the differential image to check if it
is over a set threshold.  Blobs that are very small will be filtered out,
leaving larger objects like cars, people, animals behind. If by the end of 
this function, the blob vector is empty, then there were no significant
changes detected in the differential image.
*/
bool motion(*imageDifference)
{
    std::vector<Blob> blobs;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            current = imageDifference.at<uchar>(i,j);
            if (current > GRAYSCALE_THRESHOLD)
            {
                bool found = false;
                for (int k = 0; k < blobs.size(); k++)
                {
                    b = blobs[k];
                    if (b.isNear(i, j, DISTANCE_THRESHOLD))
                    {
                        b.addPoint(i, j);
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    blobs.push_back(Blob(i, j));
                }
            }
        }
    }
    filterBlobs(&blobs);
    return (!blobs.empty);
}


int main(void)
{
    /*
    Initiate variables and parameters.
    Timer information will be used to determine when to obtain a new reference
    picture or a new compare image.
    The remaining parameters dictate the characteristics of the image obtained
    using the raspistill cmd.
    */
    timer_t timer;
    updateTime = 600;    // seconds (10 min)
    senseTime  = 2;      // seconds
    width      = WIDTH;
    height     = HEIGHT;
    lowPower   = false;
    options = "-vf -hf -t 1000 -q 5 -n";
    if (DEBUG)
    {
        updateTime = 30;
        options = "-vf -hf -t 1000 -q 5";
    }
    
    
    IplImage* reference, compare;
    
    getImage(options);
    sleep(1000);
    reference = cvLoadImage("temp.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(DEBUG)
    {
        printf("Getting initial reference background picture.\n")
    }
    // may have to adjust reference if something happens to pass through the
    // frame. Maybe take multiple pics and get avg so that there isn't a ref 
    // pic with movement already in it.
    startTime = timer;
    while(lowPower)
    {
        sleep(senseTime * 1000);
        // if enough time has elapsed since the reference image,
        // it is time to update the reference image.
        if(difftime(timer, startTime) >= updateTime)
        {
            // restart the reference timer and get a new reference image.
            startTime = timer;
            getImage(options);
            sleep(1000);
            reference = cvLoadImage("temp.jpg", CV_LOAD_IMAGE_GRAYSCALE);
            if(DEBUG)
            {
                printf("Resetting reference background picture.\n");
            }
            sleep(senseTime * 1000);
        }
        getImage(options);
        sleep(1000);
        compare = cvLoadImage("temp.jpg", CV_LOAD_IMAGE_GRAYSCALE);
        if(DEBUG)
        {
            printf("Getting updated picture to check for movement.\n");
            elapsedTime = difftime(timer, startTime);
            printf("Elapsed time from ref picture = " + elapsedTime + " s.\n");
        }
        getDifference(&reference, &compare);
        if(motion(&compare))
        {
            lowPower = false;
            while(!lowPower)
            {
                videoStream(10);
                sleep(10 * 1000);
                lowPower = true;
            }
        }
    }
}
