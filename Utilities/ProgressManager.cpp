#include "ProgressManager.h"

ProgressManager::ProgressManager(int totalNeeded) {
    this->totalNeeded = totalNeeded;
    this->current = 0;
    this->percAtLastOutput = 0.0;
}

void ProgressManager::startTimer() {
    time(&start);
    time(&end);
    int h, m, s;
    timeSinceStart(&h, &m, &s);
    fprintf(stderr,"Progress:\n");
    fprintf(stderr,"0%%\tTime elapsed: %d hours, %d minutes, and %d seconds\n", h, m, s);
}

void ProgressManager::advance() {
    current++;
    
    double currPerc = (double)current/totalNeeded * 100;
    if (currPerc - percAtLastOutput >= 1.0) {
        percAtLastOutput = currPerc;
        int h, m, s;
        timeSinceStart(&h, &m, &s);
        fprintf(stderr,"%.0lf%%\tTime elapsed: %d hours, %d minutes, and %d seconds\n", currPerc, h, m, s);
    }
    
    if (current == totalNeeded) {
        fprintf(stderr,"\nDone!\n");
        int h, m, s;
        timeSinceStart(&h, &m, &s);
        fprintf(stderr, "Time to complete: %d hours, %d minutes, and %d seconds\n", h, m, s);
    }
}

void ProgressManager::reset() {
    
}

void ProgressManager::timeSinceStart(int *hours, int *min, int *sec) {
    time(&end);
    double secondsSinceStart = difftime(end, start);
    *hours = secondsSinceStart/3600;
    *min = (secondsSinceStart - 3600*(*hours)) / 60;
    *sec = secondsSinceStart - 3600*(*hours) - 60*(*min);
}
