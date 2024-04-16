#include<iostream>
#include<vector>
#include<fstream>
#include<iomanip>    //fix the processingtime logic, its finishing processing before its actually done (ex, it should take 11 seconds but does it faster)
#include<sstream>   //not all my completed lines show up in the right place and some dont show up at all
#include "job.hpp"    //fix the job number in line 14
#include "queue.hpp"
#include "processor.hpp"
using namespace std;

const int NUMOFPROCESSORS = 3;
const int TIMETORUN = 20;

int main() {
    int totalTimeIdle = 0;
    int totalTimeProcessing = 0;

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
    while (sortedFile >> type >> arrivalTime >> processingTime) {

        jobs.type = type;
        jobs.arrivalTime = arrivalTime;
        jobs.processingTime = processingTime;

    
        data.push_back(jobs);

    }

    unsigned int time;
    JobQueue queue;
    vector<Processor>processors(NUMOFPROCESSORS);

    for (int i = 0; i < NUMOFPROCESSORS; i++) {
        processors.at(i).setProcessorNumber(i + 1);   //setting the processor numbers
    }

    vector<int>queueSizeAtTime(TIMETORUN + 2);
    queueSizeAtTime.at(0) = 0;
    queueSizeAtTime.at(TIMETORUN + 1) = 0;

    bool multipleEvents = false;  //tick marks for logging purposes

    cout << "# of processors: " << NUMOFPROCESSORS << endl;

    for (time = 1; time <= TIMETORUN; time++) {

        queue.incrementQueueWaitTimes();

        multipleEvents = false;

        while ((data.at(0).arrivalTime == time) && (!data.empty())) {
            jobs = data.at(0);   //idk about this
            jobs.arrivalTime = data.at(0).arrivalTime;
            jobs.processingTime = data.at(0).processingTime;
            jobs.type = data.at(0).type;
            data.erase(data.begin());
            cout << '\n';

            if (jobs.type == 'D') {
                numDjobs++;
                overallNumJobs++;
                multipleEvents = true;
                queue.insertPriorityQueue(jobs);

                cout << "Time " << time << ": " << " Arrival: Overall Job: " << overallNumJobs << ", " << "Job " << jobs.type << ":" << numDjobs << ", Processing Time: " << jobs.processingTime << ";";

            }
            else if (jobs.type == 'C') {
                numCjobs++;
                overallNumJobs++;
                multipleEvents = true;
                queue.insertNormalQueue(jobs);

                cout << "Time " << time << ": " << " Arrival: Overall Job: " << overallNumJobs << ", " << "Job " << jobs.type << ":" << numCjobs << ", Processing Time: " << jobs.processingTime << ";";

            }
            else if (jobs.type == 'B') {
                numBjobs++;
                overallNumJobs++;
                multipleEvents = true;

                queue.insertNormalQueue(jobs);

                cout << "Time " << time << ": " << " Arrival: Overall Job: " << overallNumJobs << ", " << "Job " << jobs.type << ":" << numBjobs << ", Processing Time: " << jobs.processingTime << ";";

            }
            else if (jobs.type == 'A') {
                numAjobs++;
                overallNumJobs++;
                multipleEvents = true;
                queue.insertNormalQueue(jobs);
            
                cout << "Time " << time << ": " << " Arrival: Overall Job: " << overallNumJobs << ", " << "Job " << jobs.type << ":" << numAjobs << ", Processing Time: " << jobs.processingTime << ";";

            }
    
        }


        cout << '\n';

        for (int i = 0; i < NUMOFPROCESSORS; i++) {

            if (processors.at(i).hasJob() && processors.at(i).isHighPriority()) { //1) Busy with high priority.
                cout << "Time " << time << ": ";
                if (multipleEvents) {
                    cout << "-";
                }
                else {
                    cout << " ";
                }

                cout << "Queue: ";
                if (queue.getTotalQueueSize() == 0) {
                    cout << "Empty; ";
                }
                else {
                    cout << queue.getTotalQueueSize() << " Job(s); ";

                }
                processors.at(i).makeBusy();
                processors.at(i).reduceProcessingTime();
                processors.at(i).incrementRunTime();
                cout << "CPU " << processors.at(i).getProcessorNumber() << " Run Time:" << processors.at(i).getRunningTime() << "; " << endl ;
                //logFile1 << '\n';

                if (processors.at(i).getCurrentJob().processingTime == 0) {
                    completedJobs++;
                    cout << "Complete Processing Job : " << completedJobs << ", Job " << processors.at(i).peekCurrentJob().type << " : ";
                    if (processors.at(i).peekCurrentJob().type == 'A') {
                        cout << numAjobs;
                    }
                    else if (processors.at(i).peekCurrentJob().type == 'B') {
                        cout << numBjobs;
                    }
                    else if (processors.at(i).peekCurrentJob().type == 'C') {
                        cout << numCjobs;
                    }
                    else {
                        cout << numDjobs;
                    }
                    //cout << "\n";
                    queue.insertCompletedJob(processors.at(i).getCurrentJob());
                    processors.at(i).prepareForNewJob();

                }


            }
            else if (processors.at(i).hasJob() && !processors.at(i).isHighPriority() && queue.isPriorityQueueEmpty()) {    //2) Busy with normal but there is NO  high Job in queue.
                cout << "Time " << time << ": ";
                if (multipleEvents) {
                    cout << "-";
                }
                else {
                    cout << " ";
                }

                cout << "Queue: ";
                if (queue.getTotalQueueSize() == 0) {
                    cout << "Empty; ";
                }
                else {
                    cout << queue.getTotalQueueSize() << " Job(s); ";

                }
                processors.at(i).makeBusy();
                processors.at(i).reduceProcessingTime();
                processors.at(i).incrementRunTime();
                cout << "CPU " << processors.at(i).getProcessorNumber() << " Run Time:" << processors.at(i).getRunningTime() << "; " << endl ;
                //logFile1 << '\n';

                if (processors.at(i).getCurrentJob().processingTime == 0) {
                    completedJobs++;
                    cout << "Time " << time << ": ";
                    if (multipleEvents) {
                        cout << "-";
                    }
                    else {
                        cout << " ";
                    }
                    cout << "Complete Processing Job : " << completedJobs << ", Job " << processors.at(i).peekCurrentJob().type << " : ";
                    if (processors.at(i).peekCurrentJob().type == 'A') {
                        cout << numAjobs;
                    }
                    else if (processors.at(i).peekCurrentJob().type == 'B') {
                        cout << numBjobs;
                    }
                    else if (processors.at(i).peekCurrentJob().type == 'C') {
                        cout << numCjobs;
                    }
                    else {
                        cout << numDjobs;
                    }
                    //cout << "\n";
                    queue.insertCompletedJob(processors.at(i).getCurrentJob());
                    processors.at(i).prepareForNewJob();

                }


            }

            else if (processors.at(i).hasJob() && !processors.at(i).isHighPriority() && !queue.isPriorityQueueEmpty()) {           //3) Busy with normal but there is a high in queue
                
                cout << "Time " << time << ":";

                //idk check if I need to add anyhting else here
                queue.insertFrontNormalQueue(processors.at(i).peekCurrentJob());   //come back to this line i think this is where the interrupted things happen and you need to make sure the updated procesing time is correct when it gets interrupted
                processors.at(i).removeCurrentJob();

                processors.at(i).insertJob(queue.removeJobPQ());
                multipleEvents = true;

                if (processors.at(i).getIdleTime() == -1) {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
                }
                else {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
                }
                cout << " -Begin Processing Job: ";
                //processors.at(i).insertJob(queue.removeJobPQ());     //idk why it was being removed twice
                cout << overallNumJobs << ", Job " << processors.at(i).peekCurrentJob().type << ":";
                if (processors.at(i).peekCurrentJob().type == 'A') {
                    cout << numAjobs;
                }
                else if (processors.at(i).peekCurrentJob().type == 'B') {
                    cout << numBjobs;
                }
                else if (processors.at(i).peekCurrentJob().type == 'C') {
                    cout << numCjobs;
                }
                else {
                    cout << numDjobs;
                }
                cout << " in CPU " << processors.at(i).getProcessorNumber(); // endl;
            }


            else if ((!processors.at(i).hasJob() && !queue.isPriorityQueueEmpty())) {   // (this checks if there is no job and there is a high priority in queue)
                cout << "Time " << time << ":";

                multipleEvents = true;
                cout << " -Begin Processing Job: ";
                if (processors.at(i).getIdleTime() == -1) {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
                }
                else {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
                }
                processors.at(i).insertJob(queue.removeJobPQ());
                processors.at(i).makeBusy();
                cout << overallNumJobs << ", Job " << processors.at(i).peekCurrentJob().type << ":";
                if (processors.at(i).peekCurrentJob().type == 'A') {
                    cout << numAjobs;
                }
                else if (processors.at(i).peekCurrentJob().type == 'B') {
                    cout << numBjobs;
                }
                else if (processors.at(i).peekCurrentJob().type == 'C') {
                    cout << numCjobs;
                }
                else {
                    cout << numDjobs;
                }
                cout << " in CPU " << processors.at(i).getProcessorNumber();
                cout << '\n';


            }

            else if ((!processors.at(i).hasJob() && !queue.isNormalQueueEmpty())) {    //if there is no job and there is a normal job in queue
                cout << "Time " << time << ":";
                multipleEvents = true;
                cout << " -Begin Processing Job: ";
                if (processors.at(i).getIdleTime() == -1) {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
                }
                else {
                    totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
                }
                processors.at(i).insertJob(queue.removeJobNorm());
                processors.at(i).makeBusy();
                cout << overallNumJobs << ", Job " << processors.at(i).peekCurrentJob().type << ":";
                if (processors.at(i).peekCurrentJob().type == 'A') {
                    cout << numAjobs;
                }
                else if (processors.at(i).peekCurrentJob().type == 'B') {
                    cout << numBjobs;
                }
                else if (processors.at(i).peekCurrentJob().type == 'C') {
                    cout << numCjobs;
                }
                else {
                    cout << numDjobs;
                }
                cout << " in CPU " << processors.at(i).getProcessorNumber();
                cout << '\n';



            }

            else if ((!processors.at(i).hasJob() && queue.isNormalQueueEmpty()) && (!processors.at(i).hasJob() && queue.isPriorityQueueEmpty())) {
                processors.at(i).incrementIdleTime();
                cout << "Time " << time << ": ";
                if (multipleEvents) {
                    cout << "-";
                }
                else {
                    cout << " ";
                }
                cout << "Queue: ";
                if (queue.getTotalQueueSize() == 0) {
                    cout << "Empty; ";
                }
                else {
                    cout << queue.getTotalQueueSize() << " Job(s); ";

                }
                cout << "CPU " << processors.at(i).getProcessorNumber() << " Idle Time:" << processors.at(i).getIdleTime() << " ";
                
            }
        
        }//this is the end of the big loop

    }
    return 0;
}


