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
	int overallNumJobs = 1;
	while (sortedFile >> type >> arrivalTime >> processingTime) {

		jobs.type = type;
		jobs.arrivalTime = arrivalTime;
		jobs.processingTime = processingTime;

	
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

	bool multipleEvents = false;  //tick marks for logging purposes

	cout << "# of processors: " << NUMOFPROCESSORS << endl;

	for (time = 1; time <= TIMETORUN; time++) {

		queue.incrementQueueWaitTimes();

		multipleEvents = false;
		
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

		for (int i = 0; i < NUMOFPROCESSORS; i++) {
			if (processors[i].hasJob()) {
				logFile1 << "CPU " << processors.at(i).getProcessorNumber() << " Run Time:" << processors.at(i).getRunningTime() << "; ";
			}
			else {
				logFile1 << "CPU " << processors.at(i).getProcessorNumber() << " Idle Time:" << processors.at(i).getIdleTime() << "; ";
			}

		}
		logFile1 << endl;

		logFile1 << "Time " << time << ": " << "Arrival: Overall Job: " << overallNumJobs << ", " << "Job " << jobs.type << ":" << jobs.jobTypeNumber << ", Processing Time: " << jobs.processingTime << ";" << endl;


		while ((data.at(0).arrivalTime == time)) {
			jobs.arrivalTime = data.at(0).arrivalTime;
			jobs.processingTime = data.at(0).processingTime;
			jobs.type = data.at(0).type;
			data.erase(data.begin());

			if (jobs.type == 'D') {
				numDjobs++;
				queue.insertPriorityQueue(jobs);

				logFile1 << "Time " << time << ": ";
				if (multipleEvents) {
					logFile1 << "-";
				}
				else {
					logFile1 << " ";
				}

				logFile1  << "Arrival: Overall Job: " << overallNumJobs << ", " << "Job " << jobs.type << ":" << numDjobs << ", Processing Time: " << jobs.processingTime << ";" << endl;

				multipleEvents = true;
				overallNumJobs++;
			}
			else if (jobs.type == 'C') {
				numCjobs++;
				queue.insertNormalQueue(jobs);

				logFile1 << "Time " << time << ": ";
				if (multipleEvents) {
					logFile1 << "-";
				}
				else {
					logFile1 << " ";
				}

				logFile1 <<  "Arrival: Overall Job: " << overallNumJobs << ", " << "Job " << jobs.type << ":" << numCjobs << ", Processing Time: " << jobs.processingTime << ";" << endl;

				multipleEvents = true;
				overallNumJobs++;
			}
			else if (jobs.type == 'B') {
				numBjobs++;
				queue.insertNormalQueue(jobs);

				logFile1 << "Time " << time << ": ";
				if (multipleEvents) {
					logFile1 << "-";
				}
				else {
					logFile1 << " ";
				}

				logFile1 << "Arrival: Overall Job: " << overallNumJobs << ", " << "Job " << jobs.type << ":" << numBjobs << ", Processing Time: " << jobs.processingTime << ";" << endl;

				multipleEvents = true;
				overallNumJobs++;
			}
			else if (jobs.type == 'A') {
				numAjobs++;
				queue.insertNormalQueue(jobs);

				logFile1 << "Time " << time << ": ";
				if (multipleEvents) {
					logFile1 << "-";
				}
				else {
					logFile1 << " ";
				}

				logFile1 << "Arrival: Overall Job: " << overallNumJobs << ", " << "Job " << jobs.type << ":" << numAjobs << ", Processing Time: " << jobs.processingTime << ";" << endl;

				multipleEvents = true;
				overallNumJobs++;
			}
		}
	}


	for (int i = 0; i < NUMOFPROCESSORS; i++) {
		if (!processors.at(i).hasJob()) {
			if (!queue.isPriorityQueueEmpty()) {
				logFile1 << "Time " << time << ": ";
				if (multipleEvents) {
					logFile1 << "-";
				}
				else {
					logFile1 << " ";
				}
				logFile1 << "- Begin Processing Job:";
				if (processors.at(i).getIdleTime() == -1) {
					totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
				}
				else {
					totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
				}
				processors.at(i).insertJob(queue.removeJobPQ());
				logFile1 << processors.at(i).peekCurrentJob().overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << ":" << processors.at(i).peekCurrentJob().jobTypeNumber << " in CPU " << processors.at(i).getProcessorNumber() << endl;
				multipleEvents = true;
			}
			else {
				if (!queue.isNormalQueueEmpty()) {
					logFile1 << "Time " << time << ": ";
					if (multipleEvents) {
						logFile1 << "-";
					}
					else {
						logFile1 << " ";
					}
					logFile1 << "- Begin Processing Job:";
					if (processors.at(i).getIdleTime() == -1) {
						totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
					}
					else {
						totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
					}
					processors.at(i).insertJob(queue.removeJobNorm());
					logFile1 << processors.at(i).peekCurrentJob().overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << ":" << processors.at(i).peekCurrentJob().jobTypeNumber << " in CPU " << processors.at(i).getProcessorNumber() << endl;
					multipleEvents = true;
				}
				else {
					if (processors.at(i).getIdleTime() == -1) {
						totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
					}
					else {
						totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
					}
					processors.at(i).incrementIdleTime();
				}
			}

		}
		else {
			if (!processors.at(i).isHighPriority()) {
				if (!queue.isPriorityQueueEmpty()) {

					queue.insertNormalQueue(processors.at(i).removeCurrentJob());
					processors.at(i).insertJob(queue.removeJobPQ());
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
					logFile1 << "- Begin Processing Job:";
					processors.at(i).insertJob(queue.removeJobPQ());
					logFile1 << processors.at(i).peekCurrentJob().overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << ":" << processors.at(i).peekCurrentJob().jobTypeNumber << " in CPU " << processors.at(i).getProcessorNumber() << endl;
					multipleEvents = true;
				}
			}
		}
	}

	for (int i = 0; i < NUMOFPROCESSORS; i++) {
				if (processors.at(i).hasJob()) {
					processors.at(i).reduceProcessingTime();
					if (processors.at(i).getCurrentJob().processingTime == 0) {
						logFile1 << "Time " << time << ": ";
						if (multipleEvents) {
							logFile1 << "-";
						}
						else {
							logFile1 << " ";
						}
						logFile1 << "Complete Processing Job : " << processors.at(i).peekCurrentJob().overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << " : " << processors.at(i).peekCurrentJob().jobTypeNumber << endl;
						logFile1 << "\m";
						queue.insertCompletedJob(processors.at(i).getCurrentJob());
						processors.at(i).prepareForNewJob();

						
					}
					if (!processors.at(i).hasJob()) {
						if (!queue.isPriorityQueueEmpty()) {
							logFile1 << "Time " << time << ": ";
							if (multipleEvents) {
								logFile1 << "-";
							}
							else {
								logFile1 << " ";
							}
							logFile1 << "- Begin Processing Job:";
							if (processors.at(i).getIdleTime() == -1) {
								totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
							}
							else {
								totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
							}
							processors.at(i).insertJob(queue.removeJobPQ());
							logFile1 << processors.at(i).peekCurrentJob().overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << ":" << processors.at(i).peekCurrentJob().jobTypeNumber << " in CPU " << processors.at(i).getProcessorNumber() << endl;
							multipleEvents = true;
						}
						else {
							if (!queue.isNormalQueueEmpty()) {
								logFile1 << "Time " << time << ": ";
								if (multipleEvents) {
									logFile1 << "-";
								}
								else {
									logFile1 << " ";
								}
								logFile1 << "- Begin Processing Job:";
								if (processors.at(i).getIdleTime() == -1) {
									totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
								}
								else {
									totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
								}
								processors.at(i).insertJob(queue.removeJobNorm());
								logFile1 << processors.at(i).peekCurrentJob().overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << ":" << processors.at(i).peekCurrentJob().jobTypeNumber << " in CPU " << processors.at(i).getProcessorNumber() << endl;
								multipleEvents = true;
							}
							else {
								if (processors.at(i).getIdleTime() == -1) {
									totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime() + 1;
								}
								else {
									totalTimeIdle = totalTimeIdle + processors.at(i).getIdleTime();
								}
								processors.at(i).incrementIdleTime();
							}
						}

					/*for (int i = 0; i < NUMOFPROCESSORS; i++) {
						if (!processors.at(i).hasJob()) {
							processors.at(i).incrementIdleTime();
						}

					}*/
				}
					else {
						if (!processors.at(i).isHighPriority()) {
							if (!queue.isPriorityQueueEmpty()) {

								queue.insertNormalQueue(processors.at(i).removeCurrentJob());
								processors.at(i).insertJob(queue.removeJobPQ());
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
								logFile1 << "- Begin Processing Job:";
								processors.at(i).insertJob(queue.removeJobPQ());
								logFile1 << processors.at(i).peekCurrentJob().overallJobNumber << ", Job " << processors.at(i).peekCurrentJob().type << ":" << processors.at(i).peekCurrentJob().jobTypeNumber << " in CPU " << processors.at(i).getProcessorNumber() << endl;
								multipleEvents = true;
							}
						}
					}
			}
		}


	
	logFile1.close();
	return 0;
}
