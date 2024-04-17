#include "Queue.h"

JobQueue::JobQueue() {

}
Job JobQueue::removeJobPQ() {
    Job temp;
    if (!priorityQueue.empty()) {
        temp = priorityQueue.at(0);
        priorityQueue.erase(priorityQueue.begin());
    }
    return temp;

}
Job JobQueue::removeJobNorm() {
    Job temp;
    temp = normalQueue.at(0);
    normalQueue.erase(normalQueue.begin());
    return temp;
}
int JobQueue::getNumberAJobsCompleted()
{
    int completedA = 0;
    for (int i = 0; i < completedJobs.size(); i++) {
        if (completedJobs.at(i).type == 'A')
            completedA++;
    }
    return completedA;
}
int JobQueue::getNumberBJobsCompleted()
{
    int completedB = 0;
    for (int i = 0; i < completedJobs.size(); i++) {
        if (completedJobs.at(i).type == 'B')
            completedB++;
    }
    return completedB;
}
int JobQueue::getNumberCJobsCompleted()
{
    int completedC = 0;
    for (int i = 0; i < completedJobs.size(); i++) {
        if (completedJobs.at(i).type == 'C')
            completedC++;
    }
    return completedC;
}
int JobQueue::getNumberDJobsCompleted()
{
    int completedD = 0;
    for (int i = 0; i < completedJobs.size(); i++) {
        if (completedJobs.at(i).type == 'D')
            completedD++;
    }
    return completedD;
}
bool JobQueue::isNormalQueueEmpty() const {
    return normalQueue.empty();
}

bool JobQueue::isPriorityQueueEmpty() const {
    return priorityQueue.empty();
}

void JobQueue::insertNormalQueue(const Job& job) {
    normalQueue.push_back(job);
}

void JobQueue::insertPriorityQueue(const Job& job) {
    priorityQueue.push_back(job);
}

void JobQueue::insertCompletedJob(const Job& job) {
    completedJobs.push_back(job);
}

void JobQueue::insertFrontNormalQueue(const Job& job) {

    normalQueue.insert(normalQueue.begin(), job);

}

Job JobQueue::getFrontNormalQueue() {
    if (!normalQueue.empty()) {
        Job frontJob = normalQueue.front();
        //normalQueue.pop_back();  //ask why he is popping?????
        return frontJob;
    }
    else {
        // Return empty job if queue is empty
        return Job{};
    }
}

Job JobQueue::getFrontPriorityQueue() {
    if (!priorityQueue.empty()) {
        Job frontJob = priorityQueue.front();
        priorityQueue.front();
        return frontJob;
    }
    else {
        // Return empty job if queue is empty
        return Job{};
    }
}

int JobQueue::getTotalQueueSize() const {
    return normalQueue.size() + priorityQueue.size();
}

int JobQueue::getCurrentNormalQueueSize() const {
    return normalQueue.size();
}

int JobQueue::getCurrentPriorityQueueSize() const {
    return priorityQueue.size();
}

int JobQueue::getCompletedJobsCount() const {
    return completedJobs.size();
}

void JobQueue::incrementQueueWaitTimes() {
    for (auto& job : normalQueue) {  //incrementing the time in queue of normal queue
        job.timeInQueue++;
    }

    for (auto& job : priorityQueue) {  //incrementig the time in the queue of priority queue
        job.timeInQueue++;
    }
}



int JobQueue::getJobsArrivedCount(char jobType) const {
    int count = 0;

    // Iterate over normalQueue
    for (const auto& job : normalQueue) {
        if (job.type == jobType) {
            count++;
        }
    }

    // Iterate over priorityQueue
    for (const auto& job : priorityQueue) {
        if (job.type == jobType) {
            count++;
        }
    }

    return count;
}

int JobQueue::getJobsCompletedCount(char jobType) const {
    int count = 0;

    for (const auto& job : completedJobs) {
        
        count++;
    }

    return count;
}


int JobQueue::getTotalQueueTime() const {
    int totalTime = 0;

    // Iterate over normalQueue
    for (const auto& job : normalQueue) {
        totalTime += job.timeInQueue;
    }

    // Iterate over priorityQueue
    for (const auto& job : priorityQueue) {
        totalTime += job.timeInQueue;
    }

    return totalTime;
}








