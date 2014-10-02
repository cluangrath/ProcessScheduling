#include <cstdio>
#include <deque>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

void findLowestInQueue(deque<int> inpQueue, int returnValue[]);

struct Process {
public:
	int start_time;
	int cpuValues[150];
	int ssdValues[150];
	int inpValues[150];
	int numOfInpElements; //Denote the number of inp, ssd, and cpu that we are actually storing in the array.
	int numOfSsdElements;
	int numOfCpuElements;
	int inputPosition;
	int ssdPosition;
	int cpuPosition;
	std::deque<char> instructionSequence;

	/*
	 Deque Instructions:
	 - c means cpu, s ssd, i inp
	 - Because we are storing all cpu data in one array, we have no concept of the
	 order that the cpu data will be called. i.e, how do we know it doesn't go CPU-SSD-CPU-SSD
	 */

};

int main() {
	Process *currentProcess = new Process();

	ifstream myFile("input.txt");
	string str;

	std::deque<int> processIndices;
	Process processArray[150];

	int pArrayLoc = -1; //Counter details the index in the array that the current process is at.

	//initial process setup
	while (std::getline(myFile, str)) {
		//cout << "String is: " << str;
		if (str.find("NEW") != std::string::npos) {
			//cout << "In NEW\n";
			//If it finds the word "NEW" within the string, create a new process
			//and set the *currentProcess variable to it.

			//TODO: Check to see if currentProcess needs to be pushed into Deque<Process>. Deque<Process> has not been created yet.

			if (pArrayLoc > -1) {
				processArray[pArrayLoc] = *currentProcess;
			}
			pArrayLoc = pArrayLoc + 1;
			Process *newProcess = new Process();

			//find the integer value in the string
			int spacePosition = str.find(' '); //find the space

			//cout << "Space position: " << spacePosition;

			std::string stringStartTime = str.substr(spacePosition); //substring from the space to the end of the string

			//cout << "Integer value found: " << stringStartTime << '\n';
			int startTime = atoi(stringStartTime.c_str()); //parse the numerical string into an int

			//cout << "Integer parsed value is: " << startTime << '\n';

			newProcess->start_time = startTime; //set the newProcess' start_time to startTime

			*currentProcess = *newProcess; //keep a reference to this object in a more 'global' scope where it stays preserved
			//cout << currentProcess->start_time;

			currentProcess->numOfInpElements = 0; //Initialize all of our struct's counters
			currentProcess->numOfSsdElements = 0;
			currentProcess->numOfCpuElements = 0;
			currentProcess->inputPosition = 0;
			currentProcess->cpuPosition = 0;
			currentProcess->ssdPosition = 0;
			currentProcess->instructionSequence.empty();
			//parray[counter] = *currentProcess;
			processIndices.push_back(pArrayLoc);
		}

		if (str.find("INP") != std::string::npos) {

			//cout << "In INP\n";
			//the line from the file contains 'INP'
			int spacePosition = str.find(' '); //find the space
			std::string stringInpTime = str.substr(spacePosition); //substring from the space to the end of the string
			int inpTime = atoi(stringInpTime.c_str()); //parse the numerical string into an int

			currentProcess->inpValues[currentProcess->numOfInpElements] =
					inpTime; //Add the time needed to our cpu queue
			currentProcess->instructionSequence.push_back('i'); //Add the inp to the order
			cout << currentProcess->instructionSequence.size() << '\n';
			currentProcess->numOfInpElements = currentProcess->numOfInpElements + 1; //Increment the inp counter

			//handle the inpTime here
		}

		if (str.find("SSD") != std::string::npos) {
			//cout << "In SSD\n";
			//the line from the file contains 'SSD'
			int spacePosition = str.find(' '); //find the space
			std::string stringSsdTime = str.substr(spacePosition); //substring from the space to the end of the string
			int ssdTime = atoi(stringSsdTime.c_str()); //parse the numerical string into an int

			currentProcess->ssdValues[currentProcess->numOfSsdElements] =
					ssdTime; //Add time
			currentProcess->instructionSequence.push_back('s'); //Add to order
			cout << currentProcess->instructionSequence.size() << '\n';
			currentProcess->numOfSsdElements = currentProcess->numOfSsdElements + 1; //Increment

			//handle the ssdTime here
		}

		if (str.find("CPU") != std::string::npos) {

			//cout << "In CPU\n";
			//the line from the file contains 'CPU'
			int spacePosition = str.find(' '); //find the space
			std::string stringCpuTime = str.substr(spacePosition); //substring from the space to the end of the string
			int cpuTime = atoi(stringCpuTime.c_str()); //parse the numerical string into an int

			currentProcess->cpuValues[currentProcess->numOfCpuElements] =
					cpuTime; //Time in array
			currentProcess->instructionSequence.push_back('c'); //Type of operation to end of queue
			cout << currentProcess->instructionSequence.size() << '\n';
			currentProcess->numOfCpuElements = currentProcess->numOfCpuElements + 1; //Increment the location of the cpu counter

			//handle the cpuTime here
		}
		//cout << "Finished handling one instruction\n";
		//cout << "End of line" << '\n';
	}

	//cout << "Finished going through my input file!\n";

	//cout << "Manually adding last process to processArray\n";
	processArray[pArrayLoc] = *currentProcess; //add the last currentProcess in manually since it doesn't fall into the loop mechanism

	//cout << "About to create cpuQueue\n";
	std::deque<int> cpuQueue;

	//cout << "About to create ssdQueue\n";
	std::deque<int> ssdQueue;

	//cout << "About to create inpQueue\n";
	std::deque<int> inpQueue;


	int returnValues [2] = { }; //use this object with findLowestInQueue. index of the lowest value in inpQueue will be in position 0, value in position 1

	int globalClock = 0; //instantiate the globalClock @ 0

	//CPU 1 Properties
	int finishTime1 = 0; //globalClock + processArray[pArrayLoc].cpuVals[cpuPos]
	int processArrayLoc1 = 0;
	bool isBusy1 = false;

	//CPU 2 Properties
	int finishTime2 = 0; //globalClock + processArray[pArrayLoc].cpuVals[cpuPos]
	int processArrayLoc2 = 0;
	bool isBusy2 = false;

	cout << "About to do !processIndices.empty() loop\n";
	while (!processIndices.empty()) {
		int front = processIndices.front(); //reference the front of the processIndices queue
		Process *curProc = &processArray[front]; //reference the current process using the front index value
		processIndices.pop_front(); //pop the first process index value off the queue
		cout << "The number of instructions for this process are: " << curProc->instructionSequence.size() << "\n"; //write out the number of instructions associated with this current process


		//Chad to Ryan: I put this here, but I'm not sure that this is where we want to do this. I think we need to write a function that will
			//find us the next 'occuring' event
		globalClock = curProc->start_time; //change the globalClock to this currentProcess' start time

		//TODO: Loop through the curProc's instructionSequence. List of 'events' we need to check for in order to update the globalClock:
		//SSD event ending
		//INP event ending
		//NEW process being 'created' <- Only if it's the 'nearest' time, otherwise we overlap SSD/INP/CPU times
		//CPU event ending
		while (!curProc->instructionSequence.empty()) {
			char currentInstruction = curProc->instructionSequence.front(); //grab the character for this instruction
			curProc->instructionSequence.pop_front(); //remove the current character so we can move forward after we're done processing this one...

			cout << "Current instruction is: " << currentInstruction << "\n";

			if (currentInstruction == 'c' || currentInstruction == 'C') {
				cpuQueue.push_back(front);
				//do some logic here to handle if it needs to be 'put' on CPU 1 or 2
				if (!isBusy1)
				{
					//if CPU 1 isn't busy, let it take this CPU time. handle CPU 1's properties here!

				}
				else if (!isBusy2)
				{
					//if CPU 1 is busy, check CPU 2 to see if it's doing work. handle CPU 2's properties here!
				}
			} else if (currentInstruction == 'i' || currentInstruction == 'I') {
				inpQueue.push_back(front);
			} else if (currentInstruction == 's' || currentInstruction == 'S') {
				ssdQueue.push_back(front);
			}
		}
	}
}//end of main()

void findLowestInQueue(deque<int> inpQueue, int returnValue[])
{
	cout << "Inside of findLowestInQueue\n";

	//copy inpQueue to a different queue so we don't destroy the referenced inpQueue
	//cout << "About to copy inpQueue. Num of elements in inpQueue: " << inpQueue.size() << "\n";
	int i = 0;
	deque<int> copyOfInpQueue;

	while (i < inpQueue.size())
	{
		copyOfInpQueue.push_back(inpQueue[i]);
		i = i + 1;
	}

	//cout << "Copied inpQueue. num of elements: " << copyOfInpQueue.size() << "\n";

	int lowestValueIndex = 0;
	int lowestValue = inpQueue.front();

	copyOfInpQueue.pop_front(); //remove the first reference so it checks the second one inside of the loop

	int counter = 1;

	while (!copyOfInpQueue.empty())
	{
		//cout << "Inside of !inpQueue.empty() loop\n";
		int tempIndexValue = counter; //keep reference to the current index since we pop the front off...
		int tempValue = copyOfInpQueue.front();

		if (tempValue < lowestValue) {
			lowestValue = tempValue;
			lowestValueIndex = tempIndexValue;
		}
		copyOfInpQueue.pop_front(); //remove the value we were just dealing with
	}

	returnValue[0] = lowestValueIndex;
	returnValue[1] = lowestValue;
}//end findLowestInQueue

