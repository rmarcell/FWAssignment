#include "buffer.h"
#include <stddef.h>
#include <math.h>   /* abs */

/* Return the error according to the nominal distance and num of samples */
static int calculateTimingError(timestamp_t ts_start, timestamp_t ts_now, int diffNominal, int n_current_samples)
{
    int err = (ts_now-ts_start) - (n_current_samples*diffNominal);
    return(err);
}

/*  Initialize buffer with timestamp and sample time 
    Error conditions return -1 
*/
int bufInit(buf_t* buf, timestamp_t timestamp, sampletime_t sampletime)
{
    int retval = 0;

    if(NULL != buf)
    {
        buf->endIndex = 0U;
    }

    if(NULL == buf)
    {
        retval = -1;
    }
    else if ((TIMESTAMP_MIN > timestamp) || (TIMESTAMP_MAX < timestamp))
    {
        retval = -1;
    }
    else
    {
        if(1U == bufIsFull(buf))
        {   
            /* Buffer is full */
            retval = -1;
        }
        else
        {
            buf->timestamp = timestamp;
            buf->sampleTime = sampletime;

            buf->endIndex = 0U;

            buf->accumulatedError = 0U;

            retval = 0;
        }
    }
    return retval;
}

/* 
Add new sample to buffer
Possible errors:
    - Buffer is NULL -> return -1
    - Buffer is full -> return -1
    - Sample timestamp is out of range -> invalidate sample
*/
int bufAddSample(buf_t* buf, timestamp_t timestamp, sample_t sample)
{
    int retval = 0;

    if (NULL == buf)
    {
        retval = -1;
    }
    else if(0U != bufIsFull(buf))
    {    
        /* Buffer is full */
        retval = -1;
    }
    else
    {
        /* Timing error handling */
        int tsError = calculateTimingError(buf->timestamp, timestamp, buf->sampleTime, buf->endIndex);

        if(abs(tsError) >= buf->sampleTime)
        {
            /* Timing error is too large: invalidate */
            buf->sample[buf->endIndex] = INVALID_SAMPLE;
            buf->endIndex++; 
        }
        else
        {
            buf->accumulatedError += tsError;
            buf->sample[buf->endIndex] = sample;
            buf->endIndex++;
        }
        retval = 0;           
    }
    return retval;
}


int bufClear(buf_t* buf)
{
    int retval = 0;

    if (NULL == buf)
    {
        retval = -1;
    }
    else
    {
        buf->endIndex = 0;
        retval = 0;
    }
    return retval;
}

int bufIsFull(buf_t* buf)
{
    int retval = 0;
    retval = (buf->endIndex == MAX_N_SAMPLES)?1U:0U; 
    return retval;
}

int bufReInit(buf_t* buf, timestamp_t timestamp, sampletime_t sampletime)
{
    int retval = 0;

    /* Input validity check */
    if(NULL == buf)
    {
        retval = -1;
    }
    else if ((TIMESTAMP_MIN > timestamp) || (TIMESTAMP_MAX < timestamp))
    {
        retval = -1;
    }
    else
    {
        buf->timestamp = timestamp;
        buf->sampleTime = sampletime;

        bufClear(buf);

        retval = 0;
    }
    return retval;

}  
