#ifndef ProgressManager_h
#define ProgressManager_h

#include <stdio.h>
#include <time.h>

class ProgressManager {
    time_t start;
    time_t end;
    int totalNeeded;
    int current;
    double percAtLastOutput;
    
    void timeSinceStart(int *hours, int *min, int *sec);
    
public:
    ProgressManager(int totalNeeded);
    void startTimer();
    void advance();
    void reset();
};

#endif /* ProgressManager_h */
