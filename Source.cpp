#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<iomanip>
using namespace std;   //so far Ive created the data file, organized it, implemented two quques using vectors, and read the data from the file into the queues

struct Customer {
	char type;
	int arrivalTime;
	int processTime;
};

class PriorityQueue {
private:
	vector<Customer> lowQueue;
	vector<Customer> highQueue;

public:

	void addLowQueue(Customer& customer) {
		lowQueue.push_back(customer);
	}

	void addHighQueue(Customer& customer) {
		highQueue.push_back(customer);
	}

	void displayLowQueue() {
		cout << "Low Queue contents: " << endl;
		for (const auto&  customer : lowQueue) {
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

};

int main() {

	PriorityQueue Queue;
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

			if (type == 'A' || type == 'B' || type == 'C') {
				Queue.addLowQueue(customer);  // Add to low priority queue
			}
			
			else if (type == 'D') {
				//Customer customer = { type, arrivalTime, processTime };
				Queue.addHighQueue(customer); // Add to high priority queue
			}

			else {
				cout << "Error: Invalid type found: " << type << endl;
			}
		}

		else {
			cout << "Error: Unable to read line: " << line << endl;
		}
	}

	Queue.displayLowQueue();    //used these to make sure each queue had correct data in it (I think this good; this has A, B, and C)
	Queue.displayHighQueue();   //used these to make sure each queue had correct data in it (I think this good; this only has D)




	sortedFile.close();    //make sure to close the file and clean up after yourslef :)

	return 0;
}