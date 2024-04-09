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
const int TIMETORUN = 500;

int main() {
	int totalTimeIdle = 0;
	int totalTimeProcessing = 0;

	vector<Job> data;
	Job jobs;

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
	while (sortedFile >> type >> arrivalTime >> processingTime) {

		jobs.type = type;
		jobs.arrivalTime = arrivalTime;
		jobs.processingTime = processingTime;

		switch (jobs.type) {
		case 'A':
			overallNumJobs++;
			numAjobs++;
			jobs.jobTypeNumber = numAjobs;
			jobs.overallJobNumber = overallNumJobs;
			break;
		case 'B':
			overallNumJobs++;
			numBjobs++;
			jobs.jobTypeNumber = numBjobs;
			jobs.overallJobNumber = overallNumJobs;
			break;
		case 'C':
			overallNumJobs++;
			numCjobs++;
			jobs.jobTypeNumber = numCjobs;
			jobs.overallJobNumber = overallNumJobs;
			break;
		case 'D':
			overallNumJobs++;
			numDjobs++;
			jobs.jobTypeNumber = numDjobs;
			jobs.overallJobNumber = overallNumJobs;
			break;
		default:
			break;

		}

		data.push_back(jobs);

	}

	cout << "File Queue contents:" << endl;   //just to check if file contents got copied into data vector correctly
	for (const auto& jobs : data) {
		cout << "Type: " << jobs.type << ", Arrival Time: " << jobs.arrivalTime
			<< ", Process Time: " << jobs.processingTime << endl;
	}
	cout << endl;


	unsigned int time;
	JobQueue queue;
	vector<Processor>processors(NUMOFPROCESSORS);

	for (int i = 0; i < NUMOFPROCESSORS; i++) {
		processors.at(i).setProcessorNumber(i + 1);   //setting the processor numbers
	}

	vector<int>queueSizeAtTime(TIMETORUN + 2);
	queueSizeAtTime.at(0) = 0;
	queueSizeAtTime.at(TIMETORUN + 1) = 0;
	fstream logFile1("logFile.txt", ios::app);

	bool multipleEvents = false;  //tick marks for logging purposes

	cout << "# of processors: " << NUMOFPROCESSORS << endl;

	for (time = 1; time <= 10; time++) {

		multipleEvents = false;
		/*while (!data.empty()) {
			for (const auto& jobs : data) {
				if (jobs.arrivalTime == time) {
					if (jobs.type == 'A' || jobs.type == 'B' || jobs.type == 'C') {
						queue.insertNormalQueue(jobs);
					}
					else if (jobs.type == 'D') {
						queue.insertPriorityQueue(jobs);
					}
					queue.incrementQueueWaitTimes();

				}
			}
		}*/
		while (data.at(0).arrivalTime == time) {
			jobs = data.at(0);
			data.erase(data.begin());
			if (jobs.arrivalTime == 'D') {
				queue.insertPriorityQueue(jobs);
				logFile1 << "Time " << time << ": ";
				if (multipleEvents) {
					logFile1 << "-";
				}
				else {
					logFile1 << " ";
				}

				logFile1 << "Arrival: Overall Job: " << jobs.overallJobNumber << ", " << "Job " << type << ":" << overallNumJobs << ", Processing Time: " << jobs.processingTime << ";";
				logFile1 << "/n";
				multipleEvents = true;
			}
			else {
				queue.insertNormalQueue(jobs);
				logFile1 << "Time " << time << ": ";
				if (multipleEvents) {
					logFile1 << "-";
				}
				else {
					logFile1 << " ";
				}

				logFile1 << "Arrival: Overall Job: " << jobs.overallJobNumber << ", " << "Job " << type << ":" << overallNumJobs << ", Processing Time: " << jobs.processingTime << ";";
				logFile1 << "/n";
				multipleEvents = true;
			}







		}
	}
	logFile1.close();
	return 0;
}
