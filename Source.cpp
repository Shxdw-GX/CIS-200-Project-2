#include<iostream>
#include<vector>    
#include<fstream>  
#include<iomanip>  
#include<sstream>  
#include "Job.h"  
#include "Queue.h"
#include "Processor.h"
using namespace std;

const int NUMOFPROCESSORS = 2;
const int TIMETORUN = 50;

int main() {
    int totalTimeIdle = 0;
    int totalTimeProcessing = 0;
    int totalIdleTime = 0;

    vector<Job> data;
    Job jobs;


    ofstream logFile1("logFile.txt", ios::trunc); // Truncate the log file to reset it for every run

    ifstream sortedFile("sorted.txt");  //opening our data so we can read from it
    if (!sortedFile) {
        cout << "Oopsie, couldn't open sorted file to read" << endl;
    }
    else {
        cout << "Woo-hoo, sorted file was open!" << endl;
    }


    char type;
    int arrivalTime;
    int processingTime;
    int numAjobs = 0;
    int numBjobs = 0;
    int numCjobs = 0;
    int numDjobs = 0;
    int overallNumJobs = 0;
    int completedJobs = 0;
    int maxJobsInQueue = 0;
    while (!(sortedFile.eof())) {    //reading data 
        sortedFile >> jobs.type;
        sortedFile >> jobs.arrivalTime;
        sortedFile >> jobs.processingTime;
        jobs.timeInQueue = 0;
        switch (jobs.type) {
        case 'A':
            overallNumJobs++;
            jobs.overallJobNumber = overallNumJobs;
            break;

        case 'B':
            overallNumJobs++;
            jobs.overallJobNumber = overallNumJobs;
            break;

        case 'C':
            overallNumJobs++;
            jobs.overallJobNumber = overallNumJobs;
            break;

        case 'D':
            overallNumJobs++;
            jobs.overallJobNumber = overallNumJobs;
            break;
        default:
            break;

        }
        data.push_back(jobs);



    }

  


    unsigned int time;
    JobQueue queue;
    vector<Processor>processors(NUMOFPROCESSORS);  //setting the processors

    for (int i = 0; i < NUMOFPROCESSORS; i++) {
        processors.at(i).setProcessorNumber(i + 1);   //setting the processor numbers
    }

    vector<int>queueSizeAtTime(TIMETORUN + 2);   //to get the queue size
    queueSizeAtTime.at(0) = 0;
    queueSizeAtTime.at(TIMETORUN + 1) = 0;

    vector<int>averageTimeJobInQueue(TIMETORUN + 2);    //to get the average time in job
    averageTimeJobInQueue.at(0) = 0;
    averageTimeJobInQueue.at(TIMETORUN + 1) = 0;

    bool multipleEvents = false;  //tick marks for logging purposes

    cout << "# of processors: " << NUMOFPROCESSORS << endl;

    for (time = 1; time <= TIMETORUN; time++) {

        queue.incrementQueueWaitTimes();
        averageTimeJobInQueue.at(time) = queue.getTotalQueueTime();

        multipleEvents = false;



        while ((data.at(0).arrivalTime == time) && (!data.empty())) {    //setting info
            jobs = data.at(0);
            data.erase(data.begin());


            switch (jobs.type) {
            case 'A':
                numAjobs++;
                jobs.jobTypeNumber = numAjobs;
                break;

            case 'B':
                numBjobs++;
                jobs.jobTypeNumber = numBjobs;
                break;

            case 'C':
                numCjobs++;
                jobs.jobTypeNumber = numCjobs;
                break;

            case 'D':
                numDjobs++;
                jobs.jobTypeNumber = numDjobs;
                break;
            default:
                break;
            }




            if (jobs.type == 'D') {   //if job is high priority

                multipleEvents = true;
                queue.insertPriorityQueue(jobs);

                logFile1 << "Time " << time << ": ";
                if (multipleEvents) {
                    logFile1 << "-";
                }
                else {
                    logFile1 << " ";
                }
                

                logFile1 << "Arrival: Overall Job: " << jobs.overallJobNumber << ", " << "Job " << jobs.type << ":" << jobs.jobTypeNumber << ", Processing Time: " << jobs.processingTime << ";";
                logFile1 << endl;

            }

            else {   //if job is not high prriority

                multipleEvents = true;
                queue.insertNormalQueue(jobs);

                logFile1 << "Time " << time << ": ";
                if (multipleEvents) {
                    logFile1 << "-";
                }
                else {
                    logFile1 << " ";
                }
             
                logFile1 << "Arrival: Overall Job: " << jobs.overallJobNumber << ", " << "Job " << jobs.type << ":" << jobs.jobTypeNumber << ", Processing Time: " << jobs.processingTime << ";";
                logFile1 << endl;


            }
        }


        logFile1 << '\n';
        

        for (int i = 0; i < NUMOFPROCESSORS; i++) {

            if (processors.at(i).hasJob() && processors.at(i).isHighPriority()) {    //1) Busy with high priority.
                processors.at(i).makeBusy();
                processors.at(i).reduceProcessingTime();
                processors.at(i).incrementRunTime();
                totalTimeProcessing++;

                if (processors.at(i).getCurrentJob().processingTime == 0) {
                    completedJobs++;
                   
                    logFile1 << "Complete Processing Job : " << completedJobs << ", Job " << processors.at(i).peekCurrentJob().type << " : " << processors.at(i).peekCurrentJob().jobTypeNumber;
                    logFile1 << "\n";
                    queue.insertCompletedJob(processors.at(i).getCurrentJob());
                    processors.at(i).prepareForNewJob();

                }


            }
            else if (processors.at(i).hasJob() && !processors.at(i).isHighPriority() && queue.isPriorityQueueEmpty()) {    //2) Busy with normal but there is NO  high Job in queue.
                processors.at(i).makeBusy();
                processors.at(i).reduceProcessingTime();
                processors.at(i).incrementRunTime();
                totalTimeProcessing++;

                if (processors.at(i).getCurrentJob().processingTime == 0) {
                    completedJobs++;
                    logFile1 << "Time " << time << ": ";
                    if (multipleEvents) {
                        logFile1 << "-";
                    }
                    else {
                        logFile1 << " ";
                    }
                    logFile1 << "Complete Processing Job : " << completedJobs << ", Job " << processors.at(i).peekCurrentJob().type << " : " << processors.at(i).peekCurrentJob().jobTypeNumber;
                    logFile1 << "\n";
                    queue.insertCompletedJob(processors.at(i).getCurrentJob());
                    processors.at(i).prepareForNewJob();

                }
            }

            else if (processors.at(i).hasJob() && !processors.at(i).isHighPriority() && !queue.isPriorityQueueEmpty()) {   //3) Busy with normal but there is a high in queue
                if (processors.at(i).getCurrentJob().processingTime == 0) {
                    completedJobs++;
                    logFile1 << "Time " << time << ": ";
                    if (multipleEvents) {
                        logFile1 << "-";
                    }
                    else {
                        logFile1 << " ";
                    }
                    logFile1 << "Complete Processing Job : " << completedJobs << ", Job " << processors.at(i).peekCurrentJob().type << " : " << processors.at(i).peekCurrentJob().jobTypeNumber;
                    logFile1 << "\n";
                    queue.insertCompletedJob(processors.at(i).getCurrentJob());
                    processors.at(i).prepareForNewJob();

                }
                else
                {
                    queue.insertFrontNormalQueue(processors.at(i).peekCurrentJob());  
                    processors.at(i).removeCurrentJob();

                    processors.at(i).insertJob(queue.removeJobPQ());
                    multipleEvents = true;
                }
                if (processors.at(i).getIdleTime() == -1) {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
                }
                else {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
                }
           


                logFile1 << "Time " << time << ": ";
                if (multipleEvents) {
                    logFile1 << "-";
                }
                else {
                    logFile1 << " ";
                }

                logFile1 << " Begin Processing Job: ";
                logFile1 << jobs.overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << ":" << processors.at(i).peekCurrentJob().jobTypeNumber;
                logFile1 << " in CPU " << processors.at(i).getProcessorNumber();
                logFile1 << endl;
            }


            else if ((!processors.at(i).hasJob() && !queue.isPriorityQueueEmpty())) {   // (this checks if there is no job and there is a high priority in queue)

                multipleEvents = true;
              


                logFile1 << "Time " << time << ": ";
                if (multipleEvents) {
                    logFile1 << "-";
                }
                else {
                    logFile1 << " ";
                }


                logFile1 << " Begin Processing Job: ";
                if (processors.at(i).getIdleTime() == -1) {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
                }
                else {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
                }
                processors.at(i).insertJob(queue.removeJobPQ());
                processors.at(i).makeBusy();
                logFile1 << jobs.overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << ":" << processors.at(i).peekCurrentJob().jobTypeNumber;
                logFile1 << " in CPU " << processors.at(i).getProcessorNumber();
                logFile1 << endl;


            }

            else if ((!processors.at(i).hasJob() && !queue.isNormalQueueEmpty())) {    //if there is no job and there is a normal job in queue

                multipleEvents = true;
              


                logFile1 << "Time " << time << ": ";
                if (multipleEvents) {
                    logFile1 << "-";
                }
                else {
                    logFile1 << " ";
                }

                logFile1 << " Begin Processing Job: ";
                if (processors.at(i).getIdleTime() == -1) {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
                }
                else {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
                }
                processors.at(i).insertJob(queue.removeJobNorm());
                processors.at(i).makeBusy();
                logFile1 << jobs.overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << ":" << processors.at(i).peekCurrentJob().jobTypeNumber;
                logFile1 << " in CPU " << processors.at(i).getProcessorNumber();
                logFile1 << endl;
            }

            


        }//this is the end of the big loop

        queueSizeAtTime.at(time) = queue.getTotalQueueSize();





        logFile1 << "Time " << time << ": ";
        if (multipleEvents) {
            logFile1 << "-";
        }
        else {
            logFile1 << " ";
        }

        logFile1 << "Queue: ";
        if (queue.getTotalQueueSize() == 0) {
            logFile1 << "Empty; ";
        }
        else {
            logFile1 << queue.getTotalQueueSize() << " Job(s); ";

        }
        if (maxJobsInQueue < queue.getTotalQueueSize()) {   //to get the max jobs in queue
            maxJobsInQueue = queue.getTotalQueueSize();
        }
        else {
            maxJobsInQueue = maxJobsInQueue;
        }

        for (int i = 0; i < NUMOFPROCESSORS; i++) {
            if ((!processors.at(i).hasJob() && queue.isNormalQueueEmpty()) && (!processors.at(i).hasJob() && queue.isPriorityQueueEmpty())) {  //if there is no job in processor and no job in queues
                processors.at(i).incrementIdleTime();
                totalIdleTime++;
                logFile1 << "CPU " << processors.at(i).getProcessorNumber() << " Idle Time:" << processors.at(i).getIdleTime() << " ";
            }
            else {
                logFile1 << "CPU " << processors.at(i).getProcessorNumber() << " Run Time:" << processors.at(i).getRunningTime() << "; ";
            }
           
        }
        logFile1 << '\n';

    }  //this is the end of the time loop


    double averageQueueSize = 0.0;      //to calculate the average queue size
    for (int i = 0; i < queueSizeAtTime.size(); i++) {
        averageQueueSize += queueSizeAtTime.at(i);
    }
    averageQueueSize = averageQueueSize / TIMETORUN;

    int totalTimeInQueue = 0.0;
    int averageTimeInQueue = 0.0;   
    totalTimeInQueue = averageTimeJobInQueue.at(TIMETORUN - 1);
    averageTimeInQueue = totalTimeInQueue / queue.getTotalQueueSize()/*TIMETORUN*/;

    cout << '\n';
    cout << "Number of processor(s) being used: " << NUMOFPROCESSORS;
    cout << '\n';
    cout << "Current queue size: " << queue.getTotalQueueSize();
    cout << '\n';
    cout << "Average queue size: " << averageQueueSize;
    cout << '\n';
    cout << "Maximum jobs in queue: " << maxJobsInQueue;
    cout << '\n';
    cout << "Total time jobs are in queue: " << totalTimeInQueue << " time units";
    cout << '\n';
    cout << "Average time jobs are in queue: " << averageTimeInQueue << " time units";
    cout << '\n';
    cout << "Total number of A jobs arrived: " << numAjobs;
    cout << '\n';
    cout << "Total number of A jobs completed: " << queue.getNumberAJobsCompleted();
    cout << '\n';
    cout << "Total number of B jobs arrived: " << numBjobs;
    cout << '\n';
    cout << "Total number of B jobs completed: " << queue.getNumberBJobsCompleted();
    cout << '\n';
    cout << "Total number of C jobs arrived: " << numCjobs;
    cout << '\n';
    cout << "Total number of C jobs completed: " << queue.getNumberCJobsCompleted();
    cout << '\n';
    cout << "Total number of D jobs arrived: " << numDjobs;
    cout << '\n';
    cout << "Total number of D jobs completed: " << queue.getNumberDJobsCompleted();
    cout << '\n';
    cout << "total jobs completed: " << queue.getCompletedJobsCount();
    cout << '\n';
    cout << "Total time CPU(s) were processing: " << totalTimeProcessing << " time units";
    cout << '\n';
    cout << "Total time CPU(s) were idle: " << totalIdleTime << " time units";       

    logFile1.close();
    return 0;
}
