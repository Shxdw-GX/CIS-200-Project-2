//
//  processor.cpp
//  200project2
//
//  Created by shreyas babel on 08/04/24.
//

#include "processor.hpp"


Processor::Processor(int num) {
    processorNumber = num;
    runningStatus = false;
    isBusy = false;
    idleTime = 0;
    runningTime = 0;
}

bool Processor::hasJob() const {
    return isBusy;
}

bool Processor::isHighPriority() const {
    // Check if current job is high priority (e.g., 'D')
    return currentJob.type == 'D';
}

Job Processor::getCurrentJob() const {
    return currentJob;
}

void Processor::prepareForNewJob() {
    isBusy = false;
    currentJob = {}; // Reset current job
}

void Processor::insertJob(const Job& job) {
    currentJob = job;
    isBusy = true;
}

int Processor::getProcessorNumber() const {
    return processorNumber;
}

int Processor::getIdleTime() const {
    return idleTime;
}

int Processor::getRunningTime() const {
    return runningTime;
}

void Processor::reduceProcessingTime() {
    if (isBusy) {
        currentJob.processingTime--;
        runningTime++;
    }
}

void Processor::incrementIdleTime() {
    if (!isBusy) {
        idleTime++;
    }
}

void Processor::setProcessorNumber(int num) {
    processorNumber = num;
}

bool Processor::isRunning() const {
    return runningStatus;
}
