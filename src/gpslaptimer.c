#include <gps.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "debug.h"

#define HOST "localhost"
#define GPSD_PORT "2947"
#define GPS_WAIT_TIME 1000000 // In microseconds



void timespec_diff(const struct timespec *start, const struct timespec *stop,
                   struct timespec *result)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }

    return;
}

int main() {

    int rc;
    struct timespec start_time = {0};
    struct timespec finish_time = {0};
    struct timespec total_time = {0};

    struct gps_data_t gps_data;
    if ((rc = gps_open(HOST, GPSD_PORT, &gps_data)) == -1) {
        printf("code: %d, reason: %s\n", rc, gps_errstr(rc));
        return EXIT_FAILURE;
    }

    (void)gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);

    bool gps_has_data = false;
    bool run = true;
    float time_waiting_on_data = 0.0;
    while (run) {

        clock_gettime(CLOCK_MONOTONIC, &start_time);

        gps_has_data = gps_waiting(&gps_data, GPS_WAIT_TIME);
        if (!gps_has_data) {
            // We have timed out with no data, go check again
            time_waiting_on_data += (GPS_WAIT_TIME / 1000000.0);
            printf("No data received in %d seconds...\n", time_waiting_on_data);
            continue;
        }
        time_waiting_on_data = 0.0;

        rc = gps_read(&gps_data);
        if(rc < 0) {
            printf("code: %d, reason: %s\n", rc, gps_errstr(rc));
        }
        else if(rc == 0) {
            printf("No data read...\n");
        }
        else {
            DEBUG_PRINT("Received %d bytes from gpsd.\n", rc);
            /* Display data from the GPS receiver. */
            DEBUG_PRINT("GPS Status: %d\n", gps_data.status);

            if(gps_data.status &= (STATUS_DGPS_FIX | STATUS_FIX)) {
                printf("Latitude: %f\n", gps_data.fix.latitude);
                clock_gettime(CLOCK_MONOTONIC, &finish_time);
                (void)timespec_diff(&start_time, &finish_time, &total_time);
                printf("Time to get GPS data: %lld:%llu\n", (long long)total_time.tv_sec, (unsigned long long)total_time.tv_nsec);
            }
        }
    }

    /* When you are done... */
    gps_stream(&gps_data, WATCH_DISABLE, NULL);
    gps_close (&gps_data);

    return EXIT_SUCCESS;   //sleep(1);
}

