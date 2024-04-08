//
//  processor.hpp
//  200project2
//
//  Created by shreyas babel on 08/04/24.
//

#ifndef processor_hpp
#define processor_hpp

#include "job.hpp"

class Processor {
private:
    int processorNumber;
    bool runningStatus;
    bool isBusy;
    int idleTime;
    int runningTime;
    Job currentJob;

public:
    Processor(int num);

    bool hasJob() const;
    bool isHighPriority() const;
    Job getCurrentJob() const;
    void prepareForNewJob();
    void insertJob(const Job& job);
    int getProcessorNumber() const;
    int getIdleTime() const;
    int getRunningTime() const;
    void reduceProcessingTime();
    void incrementIdleTime();
    void setProcessorNumber(int num);
    bool isRunning() const;
};


#endif /* processor_hpp */
