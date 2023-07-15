#ifndef BUFFER_H
#define BUFFER_H

/* TEST */
#ifndef UNITTEST
    #define UNITTEST
#endif

typedef unsigned long timestamp_t;
typedef short sampletime_t;
typedef char sample_t;

#define FLASH_SIZE      50400U
#if !defined(UNITTEST)
    #define MAX_N_SAMPLES   ((FLASH_SIZE-sizeof(timestamp_t)-sizeof(sampletime_t))/sizeof(sample_t))
#else
    #define MAX_N_SAMPLES   10U     /* TEST */
#endif

#define TIMESTAMP_MIN  0x386D4380   /* 2000.01.01. 00:00:00 */
#define TIMESTAMP_MAX  0xF6678A7F   /* 2100.12.31. 23:59:59 */

#define INVALID_SAMPLE 255U

typedef struct buf_s{
    timestamp_t timestamp;
    sampletime_t sampleTime;
    sample_t sample[MAX_N_SAMPLES];

    unsigned int accumulatedError;

    unsigned int endIndex;
}buf_t;

int bufInit(buf_t* buf, timestamp_t timestamp, sampletime_t sampletime);
int bufAddSample(buf_t* buf, timestamp_t timestamp, sample_t sample);
int bufClear(buf_t* buf);
int bufIsFull(buf_t* buf);
int bufReInit(buf_t* buf, timestamp_t timestamp, sampletime_t sampletime);

#endif /* BUFFER_H */
