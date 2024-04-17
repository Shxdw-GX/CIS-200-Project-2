#pragma once
#include<vector>
#include <iostream>
#include "Job.h"
using namespace std;

class JobQueue {
private:
    vector<Job> normalQueue;
    vector<Job> priorityQueue;
    vector<Job> completedJobs;
    
public:
    JobQueue();
    bool isNormalQueueEmpty() const;
    bool isPriorityQueueEmpty() const;
    void insertNormalQueue(const Job& job);
    void insertPriorityQueue(const Job& job);
    void insertCompletedJob(const Job& job);
    void insertFrontNormalQueue(const Job& job);
    Job getFrontNormalQueue();
    Job getFrontPriorityQueue();
    int getTotalQueueSize() const;
    int getCurrentNormalQueueSize() const;
    int getCurrentPriorityQueueSize() const;
    int getCompletedJobsCount() const;
    void incrementQueueWaitTimes();
    int getJobsArrivedCount(char jobType) const;
    int getJobsCompletedCount(char jobType) const;
    int getTotalQueueTime() const;
    Job removeJobPQ();
    Job removeJobNorm();
    int getNumberAJobsCompleted();
    int getNumberBJobsCompleted();
    int getNumberCJobsCompleted();
    int getNumberDJobsCompleted();
};
