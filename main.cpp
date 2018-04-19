#include <stdio.h>
#include <time.h>

__global__
void imageDifferential(int* picA, int* picB, int* out)
{
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    out[id] = picA[id] - picB[id]
}

// take a picture after 2s, output to file "image.jpg", lower quality
// raspistill -t 2000 -o image.jpg -q 5

// take a picture after 2s, output to file "image.jpg", custom dimensions
// raspistill -t 2000 -o image.jpg -w 640 -h 480

int main(void)
{
    float *picA, *picB, *output, *d_picA, *d_picB, *d_output;
    width = 640;
    height = 480; 
    picA = (float*)malloc(N*sizeof(float));
    picB = (float*)malloc(N*sizeof(float));
    output = (float*)malloc(N*sizeof(float));

    cudaMalloc(&d_picA, N*sizeof(float));
    cudaMalloc(&d_picB, N*sizeof(float));
    cudaMalloc(&d_output, N*sizeof(float));
   
    lowPower = True;
    time_t timer;
    setHost(picA, picB, output);
    setDevice();
    refImage = getImage();
    startTime = timer;
    updateTime = 600;    // seconds
    while(lowPower)
    {
        // is it time to update reference?
        if(difftime(timer, startTime) >= updateTime)
        {
            refImage = getImage();
            startTime = timer;
            sleep(1000);
        }
        // take picture
        newImage = getImage();
        // compare picture
        cudaMemcpy(d_picA, picA, N*sizeof(float), cudaMemcpyToDevice);
        cudaMemcpy(d_picB, picB, N*sizeof(float), cudaMemcpyToDevice);
        imageDifferential<<<width, height>>>(picA, picB, output, d_picA, d_picB, d_output);
       
        // determine if there's motion
        if(motion(imageDifference))
        {
            // if yes, go into high pwr mode
            lowPower = False;
        }
        while(!lowPower)
        {
            streamVideo();
            // check for motion again every few seconds
            if(!motion(imageDifference))
            {
                lowPower = True;
            }
        }
        // if no, do over again
        sleep(1000);
    }
