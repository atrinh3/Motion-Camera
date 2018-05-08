// camera test
// take several pictures of varying quality and sizes to see which
// works the best for motion detection.
void getImage(*options, name)
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
        cmd = malloc((count + 5) * sizeof(char **));
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
        cmd[count + 2] = "-o ";
        cmd[count + 3] = name;
        cmd[count + 4] = (char *)0;
        execv("/usr/bin/raspistill", cmd);
    }    
}


void getDifference(*reference, *compare)
{
    if (DEBUG)
    {
        printf("Subtracting images.\n");
    }
    // use the memory space for compare to store difference picture.
    cvAbsDiff(compare, reference, compare);
}


void testOptions(options, timer, *picA, *picB)
{
    getImage(options, options + "_v1.jpg");
    sleep(1000);
    picA = cvLoadImage(options + "_v1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    getImage(options, options + "_v2v.jpg");
    sleep(1000);
    picB = cvLoadImage(options + "_v2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    starTime = timer;
    getDifference(&picA, &picB);
    testTime = difftime(timer, startTime);
    printf("With options being: " + options);
    printf(", the time to do image subtraction was %f.\n", testTime);
}


int main(void)
{
    timer_t timer;
    int interval = 1;      // seconds
    
    IplImage* one, two;
    
    
    startTime = timer;
    options = "-vf -hf -t 1000 -q 5 -n";
    getImage(options, "q5 v1");
    sleep(interval * 1000);
    one = cvLoadImage("q5 v1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    getImage(options, "q5 v2");
    sleep(interval * 1000);
    two = cvLoadImage("q5 v2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    testTime = difftime(timer, startTime);
    printf("With q = 5, it takes %f seconds to do image subtraction.\n", testTime);
    
    startTime = timer;
    options = "-vf -hf -t 1000 -q 10 -n";
    getImage(options, "q10 v1");
    sleep(interval * 1000);
    one = cvLoadImage("q10 v1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    getImage(options, "q10 v2");
    sleep(interval * 1000);
    two = cvLoadImage("q10 v2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    testTime = difftime(timer, startTime);
    printf("With q = 10, it takes %f seconds to do image subtraction.\n");
    
    startTime = timer;
    options = "-vf -hf -t 1000 -q 20 -n";
    getImage(options, "q20 v1");
    sleep(interval * 1000);
    one = cvLoadImage("q20 v1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    getImage(options, "q20 v2");
    sleep(interval * 1000);
    two = cvLoadImage("q20 v2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    testTime = difftime(timer, startTime);
    printf("With q = 20, it takes %f seconds to do image subtraction.\n", testTime);
    
    startTime = timer;
    options = "-vf -hf -t 1000 -q 30 -n";
    getImage(options, "q30 v1");
    sleep(interval * 1000);
    one = cvLoadImage("q30 v1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    getImage(options, "q30 v2");
    sleep(interval * 1000);
    two = cvLoadImage("q30 v2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    testTime = difftime(timer, startTime);
    
    startTime = timer;
    options = "-vf -hf -t 1000 -q 40 -n";
    getImage(options, "q40 v1");
    sleep(interval * 1000);
    one = cvLoadImage("q40 v1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    getImage(options, "q40 v2");
    sleep(interval * 1000);
    two = cvLoadImage("q40 v2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    testTime = difftime(timer, startTime);
    
    
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




