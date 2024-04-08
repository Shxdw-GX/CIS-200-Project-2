//
//  queue.cpp
//  200project2
//
//  Created by shreyas babel on 08/04/24.
//

#include "queue.hpp"

using namespace std;


JobQueue::JobQueue() {
    
}

bool JobQueue::isNormalQueueEmpty() const {
    return normalQueue.empty();
}

bool JobQueue::isPriorityQueueEmpty() const {
    return priorityQueue.empty();
}

void JobQueue::insertNormalQueue(const Job& job) {
    normalQueue.push(job);
}

void JobQueue::insertPriorityQueue(const Job& job) {
    priorityQueue.push(job);
}

void JobQueue::insertCompletedJob(const Job& job) {
    completedJobs.push_back(job);
}

void JobQueue::insertFrontNormalQueue(const Job& job) {
    queue<Job> tempQueue;
    tempQueue.push(job);

    // Move all existing jobs to temporary queue
    while (!normalQueue.empty()) {
        tempQueue.push(normalQueue.front());
        normalQueue.pop();
    }

    // Move new job to front of normal queue
    while (!tempQueue.empty()) {
        normalQueue.push(tempQueue.front());
        tempQueue.pop();
    }
}

Job JobQueue::getFrontNormalQueue() {
    if (!normalQueue.empty()) {
        Job frontJob = normalQueue.front();
        normalQueue.pop();
        return frontJob;
    } else {
        // Return empty job if queue is empty
        return Job{};
    }
}

Job JobQueue::getFrontPriorityQueue() {
    if (!priorityQueue.empty()) {
        Job frontJob = priorityQueue.front();
        priorityQueue.pop();
        return frontJob;
    } else {
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
    queue<Job> tempQueue = normalQueue;

    while (!tempQueue.empty()) {
        Job& job = tempQueue.front();
        job.timeInQueue++;
        tempQueue.pop();
    }

    tempQueue = priorityQueue;

    while (!tempQueue.empty()) {
        Job& job = tempQueue.front();
        job.timeInQueue++;
        tempQueue.pop();
    }
}

int JobQueue::getJobsArrivedCount(char jobType) const {
    int count = 0;

    queue<Job> tempQueue = normalQueue;

    while (!tempQueue.empty()) {
        if (tempQueue.front().type == jobType) {
            count++;
        }
        tempQueue.pop();
    }

    tempQueue = priorityQueue;

    while (!tempQueue.empty()) {
        if (tempQueue.front().type == jobType) {
            count++;
        }
        tempQueue.pop();
    }

    return count;
}

int JobQueue::getJobsCompletedCount(char jobType) const {
    int count = 0;

    for (const auto& job : completedJobs) {
        if (job.type == jobType) {
            count++;
        }
    }

    return count;
}

int JobQueue::getTotalQueueTime() const {
    int totalTime = 0;

    queue<Job> tempQueue = normalQueue;

    while (!tempQueue.empty()) {
        totalTime += tempQueue.front().timeInQueue;
        tempQueue.pop();
    }

    tempQueue = priorityQueue;

    while (!tempQueue.empty()) {
        totalTime += tempQueue.front().timeInQueue;
        tempQueue.pop();
    }

    return totalTime;
}
