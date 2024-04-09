#include "Queue.h"
//#include "Job.h"

JobQueue::JobQueue() {

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
    normalQueue.erase(normalQueue.begin());
    //vector<Job> tempQueue;
    //tempQueue.push_back(job);

    //// Move all existing jobs to temporary queue
    // while (!normalQueue.empty()) {
    //   tempQueue.push_back(normalQueue.front());
    //    normalQueue.pop_back();
    //}

    //// Move new job to front of normal queue
    //while (!tempQueue.empty()) {
    //    normalQueue.push_back(tempQueue.front());
    //    tempQueue.pop_back();
    //}
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
        priorityQueue.front();  //ask why he is popping???
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

    /*vector<Job> tempQueue = normalQueue;

    while (!tempQueue.empty()) {
        Job& job = tempQueue.front();
        job.timeInQueue++;
        tempQueue.pop_back();
    }

    tempQueue = priorityQueue;

    while (!tempQueue.empty()) {
        Job& job = tempQueue.front();
        job.timeInQueue++;
        tempQueue.pop_back();
    }*/
}

int JobQueue::getJobsArrivedCount(char jobType) const {   //idk about this check it
    int count = 0;

    vector<Job> tempQueue = normalQueue;

    while (!tempQueue.empty()) {
        if (tempQueue.front().type == jobType) {
            count++;
        }
        tempQueue.pop_back();
    }

    tempQueue = priorityQueue;

    while (!tempQueue.empty()) {
        if (tempQueue.front().type == jobType) {
            count++;
        }
        tempQueue.pop_back();
    }

    return count;
}

int JobQueue::getJobsCompletedCount(char jobType) const {
    int count = 0;

    for (const auto& job : completedJobs) {
        /*if (job.type == jobType) {
            count++;
        }*/
        count++;
    }

    return count;
}

int JobQueue::getTotalQueueTime() const {    //idk about this check it
    int totalTime = 0;

    vector<Job> tempQueue = normalQueue;

    while (!tempQueue.empty()) {
        totalTime += tempQueue.front().timeInQueue;
        tempQueue.pop_back();
    }

    tempQueue = priorityQueue;

    while (!tempQueue.empty()) {
        totalTime += tempQueue.front().timeInQueue;
        tempQueue.pop_back();
    }

    return totalTime;
}

//void JobQueue::insertDataQueue(const Job& job)
//{
//    data.push_back(job);
//}










