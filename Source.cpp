#include<iostream>
#include<vector>
#include<fstream>
#include<iomanip>
#include "Job.h"
#include "Queue.h"
#include "Processor.h"



int main() {













	return 0;
}

















#if 0

#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<iomanip>
using namespace std;   //so far Ive created the data file, organized it, implemented two quques using vectors, and read the data from the file into the queues

int NUM_OF_PROCESSORS = 2;    //use this to change how many processors you want to use

struct Customer {
	char type;
	int arrivalTime;
	int processTime;
};


class Processor {
private:
	int idleTime;
	int runTime;
	bool isRunning;
	Customer jobType;
	int processorNumber;
	int remainingProcessTime;

public:

	Processor(int number) : idleTime(0), runTime(0), isRunning(false), processorNumber(number), remainingProcessTime(0) {}

	int getIdleTime() { return idleTime; }  //gets the idle time

	int getRunTime() { return runTime; }    // get run time

	int getProcessorNumber() { return processorNumber; }  //gets the processor number

	bool getIsRunning() { return isRunning; }    //returns if processor is running

	Customer getJobType() { return jobType; }   //returns job type processor is working on

	int getReaminingProcessingTime() { return remainingProcessTime; }

	void peekJob() {  //need to implement this 


	}

	void assignJob(Customer customer) {  //gives a job to a processor
		jobType = customer;
		remainingProcessTime = customer.processTime; // Set remaining process time
		runTime = 0;
		isRunning = true;
	}

	void processJob() {
		if (remainingProcessTime > 0) { // If a job has processing time left
			remainingProcessTime--; // Decrease remaining processing time
			runTime++;              //how long the processor has been running
		}
		else {
			++idleTime; // This means the processor is empty
			isRunning = false;
		}
	}

	void stopJob() {   //this shoul be used when doing the interrupted queue
		isRunning = false;
	}


};


class PriorityQueue {
private:
	vector<Customer> fileQueue;
	vector<Customer> lowQueue;
	vector<Customer> highQueue;
	vector<Customer> interruptedQueue;
	vector<Processor> processors;
	int time;
public:

	PriorityQueue(int numProcessors) : time(0) {
		for (int i = 0; i < numProcessors; ++i) {
			processors.push_back(Processor(i + 1));
		}
	}

	void addLowQueue(Customer& customer) {
		lowQueue.push_back(customer);
	}

	void addHighQueue(Customer& customer) {
		highQueue.push_back(customer);
	}

	void addFileQueue(Customer& customer) {
		fileQueue.push_back(customer);
	}



	void displayLowQueue() {
		cout << "Low Queue contents: " << endl;
		for (const auto& customer : lowQueue) {
			cout << "Type: " << customer.type << ", Arrival Time: " << customer.arrivalTime
				<< ", Process Time: " << customer.processTime << endl;
		}
		cout << endl;
	}

	void displayHighQueue() {
		cout << "High Queue contents:" << endl;
		for (const auto& customer : highQueue) {
			cout << "Type: " << customer.type << ", Arrival Time: " << customer.arrivalTime
				<< ", Process Time: " << customer.processTime << endl;
		}
		cout << endl;
	}

	void displayFileQueue() {
		cout << "File Queue contents:" << endl;
		for (const auto& customer : fileQueue) {
			cout << "Type: " << customer.type << ", Arrival Time: " << customer.arrivalTime
				<< ", Process Time: " << customer.processTime << endl;
		}
		cout << endl;
	}

	void jobTypeNumber() {   //gets the job type number (Ex: if the first job A is being worked on, it is A:1, when the second A job is being
		//worked on, the it would be A:2)

	}

	void processJobs() {
		/*while (!fileQueue.empty() || !lowQueue.empty() || !highQueue.empty() || !interruptedQueue.empty()) {*/  //if there are still jobs in any of the queues, we will process the job
		for (int time = 0; time < 10000; time++) {
			for (auto& Processor : processors) {
				Processor.processJob();
			}
			processFileQueue();
			processLowQueue();
			processHighQueue();
			processInterruptedQueue();
			displayMetrics();
			/*}*/
		}
	}

	/*}*/



	void processFileQueue() {
		for (auto it = fileQueue.begin(); it != fileQueue.end();) {
			if (it->arrivalTime == time) {
				if (it->type == 'A' || it->type == 'B' || it->type == 'C') {
					lowQueue.push_back(*it);
				}
				else if (it->type == 'D') {
					highQueue.push_back(*it);
				}
				it = fileQueue.erase(it); // Remove the processed job from the file queue
			}
			else {
				++it;
			}
		}
	}

	void processLowQueue() {
		for (auto& customer : lowQueue) {
			for (auto& Processor : processors) {
				if (!Processor.getIsRunning()) {
					Processor.assignJob(customer);
					break;
				}
			}
		}

	}

	void processHighQueue() {
		for (auto& customer : highQueue) {
			for (auto& Processor : processors) {
				if (!Processor.getIsRunning()) {
					Processor.assignJob(customer);
					break;
				}
			}
		}
	}

	void processInterruptedQueue() {
		for (auto& customer : interruptedQueue) {
			for (auto& processor : processors) {
				if (!processor.getIsRunning()) {
					processor.assignJob(customer);
					interruptedQueue.erase(interruptedQueue.begin());
					break;
				}
			}
		}
		interruptedQueue.clear();		// Clear the interruptedQueue after processing all interrupted jobs
	}

	void displayMetrics() {
		/*for (int t = 1; t <= time; ++t) {*/
		cout << "Time " << time << ": ";

		// Display queue status
		cout << "Queue: ";
		if (fileQueue.empty() && lowQueue.empty() && highQueue.empty() && interruptedQueue.empty()) {
			cout << "Empty; ";
		}
		else {
			cout << "1 Job; "; // Assuming all queues have the same status
		}

		// Display CPU status for each processor
		for (auto& Processor : processors) {
			cout << "CPU " << Processor.getProcessorNumber() << " ";
			if (Processor.getIsRunning()) {
				cout << "Run Time: " << Processor.getRunTime() << "; ";
			}
			{
				cout << "Idle Time: " << Processor.getIdleTime() << "; ";
			}
		}
		cout << endl;
	}
	/*}*/


	//void processJobs() {

	//	for (auto& Processor : processors) {
	//		Processor.processJob();
	//	}
	//	if (!highQueue.empty()) {  //this means there is a high priority job in the queue
	//		for (auto& Processor : processors) {
	//			if (Processor.getIsRunning() && Processor.getJobType().type != 'D') {
	//				Processor.stopJob();
	//				interruptedQueue.push_back(Processor.getJobType());
	//			}
	//		}
	//	}

	//	for (auto& Processor : processors) {
	//		if (!highQueue.empty() && !Processor.getIsRunning()) {
	//			Processor.assignJob(highQueue.front());
	//			highQueue.erase(highQueue.begin());
	//		}
	//	}

	//	for (auto& Processor : processors) {
	//		if (interruptedQueue.empty() || Processor.getIsRunning()) {
	//			continue; // Skip if the processor is running or no jobs in the waiting queue
	//		}
	//		Processor.assignJob(interruptedQueue.front());
	//		interruptedQueue.erase(interruptedQueue.begin());
	//	}

	//	for (auto& Processor : processors) {
	//		if (lowQueue.empty() && Processor.getIsRunning()) {
	//			continue;
	//		}
	//			Processor.assignJob(lowQueue.front());
	//			lowQueue.erase(lowQueue.begin());
	//		}
	//	}


};

int main() {

	PriorityQueue Queue(NUM_OF_PROCESSORS);
	ifstream sortedFile("sorted.txt");  //opening our data so we can read from it
	if (!sortedFile) {
		cout << "Oopsie, couldn't open sorted file to read" << endl;
	}
	else {
		cout << "Woo-hoo, sorted file was open!" << endl;
	}

	string line;
	while (getline(sortedFile, line)) {
		stringstream ss(line);
		char type;
		int arrivalTime;
		int processTime;

		if (ss >> type >> arrivalTime >> processTime) {
			Customer customer = { type, arrivalTime, processTime };
			if (type == 'A' || type == 'B' || type == 'C' || type == 'D') {
				Queue.addFileQueue(customer);    //adds info to the file queue
			}

			//if (type == 'A' || type == 'B' || type == 'C') {     //will use this somewhere else
			//	Queue.addLowQueue(customer);  // Add to low priority queue
			//}
			//
			//else if (type == 'D') {
			//	Queue.addHighQueue(customer); // Add to high priority queue
			//}

			else {
				cout << "Error: Invalid type found: " << type << endl;
			}
		}

		else {
			cout << "Error: Unable to read line: " << line << endl;
		}
	}

	Queue.processJobs();
	//Queue.displayLowQueue();    //used these to make sure each queue had correct data in it (I think this good; this has A, B, and C)
	//Queue.displayHighQueue();   //used these to make sure each queue had correct data in it (I think this good; this only has D)
	//Queue.displayFileQueue();     //contains all jobs



	sortedFile.close();    //make sure to close the file and clean up after yourslef :)

	return 0;
}


#endif 0