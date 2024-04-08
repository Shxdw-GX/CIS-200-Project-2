//
//  queue.hpp
//  200project2
//
//  Created by shreyas babel on 08/04/24.
//

#ifndef queue_hpp
#define queue_hpp


#include <queue>
#include <vector>
#include "job.hpp"

using namespace std;

class JobQueue {
private:
    queue<Job> normalQueue;
    queue<Job> priorityQueue;
    vector<Job> completedJobs;
    vector<Job> data;

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
};

#endif /* queue_hpp */
