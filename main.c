#include "main.h"

/* Simple print function only for testing */
static void buf_print(buf_t* buf)
{
    if(NULL != buf)
    {

        char time[20];

        time_t t;
        struct tm* tm;
        
        t = buf->timestamp;
        tm = localtime(&t);

        strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", tm);

        printf("--------------------\n");
        printf("Buffer: \n");
        printf("Timestamp:\t %s\n", time);
        printf("Sampletime:\t %d\n", buf->sampleTime);
        printf("Endindex:\t %d\n", buf->endIndex);
        printf("Accumulated Error:\t %d\n", buf->accumulatedError);

        for(int i = 0; i < buf->endIndex; i++)
        {
            t = buf->timestamp+(buf->sampleTime)*i;
            tm = localtime(&t);
            
            strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", tm);

            printf("Sample %d:\t %d\t (%s)\n", i, buf->sample[i], time);
        }
        printf("--------------------\n\n");
    }
}


/* Reset variables, clear buffer */
static void testReset(buf_t* buf)
{
    /* Clear buffer */
    bufClear(buf);

    printf("=== Shall be empty ===\n");
    buf_print(buf);
}


int main(void)
{ 
    buf_t testbuffer;
    sample_t samplePeriod = 60U;

    bufInit(&testbuffer, TIMESTAMP_MIN, samplePeriod);

    /* =================== Storing elements ==================== */

    testReset(&testbuffer);

    for(int i=0; i<10; i++)
    {
        bufAddSample(&testbuffer, TIMESTAMP_MIN+samplePeriod*i, i);
    }
    printf("=== Shall contain elements ===\n");
    buf_print(&testbuffer);


    /* =================== Increasing time error ==================== */

    testReset(&testbuffer);

    printf("=== Shall contain elements with increasing error ===\n");
    for(int i=0; i<10; i++)
    {
        bufAddSample(&testbuffer, TIMESTAMP_MIN+samplePeriod*i+i*10, i);
    }
    buf_print(&testbuffer);


    /* =================== Time error leads to invalidation ==================== */

    testReset(&testbuffer);

    bufAddSample(&testbuffer, TIMESTAMP_MIN, 55U);                                      /* Valid sample, stored */
    bufAddSample(&testbuffer, TIMESTAMP_MIN+(1.0f*samplePeriod)-samplePeriod, 55U);     /* Too early */
    bufAddSample(&testbuffer, TIMESTAMP_MIN+(2.0f*samplePeriod)+samplePeriod, 55U);     /* Too late */
    bufAddSample(&testbuffer, TIMESTAMP_MIN+(3.0f*samplePeriod), 55U);                  /* Valid sample, stored */
    
    printf("=== Shall be: valid, invalid, invalid, valid ===\n");
    buf_print(&testbuffer);

    testReset(&testbuffer);


    /* =================== Buffer full -> not storing ==================== */
    
    bufInit(&testbuffer, TIMESTAMP_MIN, samplePeriod);
    
    int retval = 0;
    for(int i=0; i<MAX_N_SAMPLES; i++)
    {
        retval = bufAddSample(&testbuffer, TIMESTAMP_MIN+samplePeriod*i, i);
        printf("retval shall be 0: %d\n", retval);        
    }
    for(int i=MAX_N_SAMPLES; i<MAX_N_SAMPLES+4U; i++)
    {
        retval = bufAddSample(&testbuffer, TIMESTAMP_MIN+samplePeriod*i, i);
        printf("retval shall be (-1): %d\n", retval);        
    }
    buf_print(&testbuffer);

    return 0;
}
