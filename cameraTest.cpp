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
    testOptions("-vf -hf -t 1000 -q 5 -n", timer, &one, &two);
    testOptions("-vf -hf -t 1000 -q 10 -n", timer, &one, &two);
    testOptions("-vf -hf -t 1000 -q 20 -n", timer, &one, &two);
    testOptions("-vf -hf -t 1000 -q 30 -n", timer, &one, &two);
    testOptions("-vf -hf -t 1000 -q 40 -n", timer, &one, &two);
    }
}

