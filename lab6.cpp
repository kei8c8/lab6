//============================================================================
// Name        : lab6.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
using namespace std;

void usage(); //not related to the object so separate function

class Signal{ //defining object
	private: //declares the private variables
		int length;
		double maximum_value;
		double average;
		double *signal_data;
		void average_calc();
		double act_max(); //finds max found in the signal_data
	public:
		//declares three constructors
		Signal();
		Signal(int);
		Signal(char*);
		~Signal(); //declares destructor
		void Sig_info(); //prints info
		void Save_file(char*); //saves info to new file
		void offset(double);
		void scale(double);
		void statistics();
		void normalize();
		void center();
};

Signal::Signal(){
	//initializes values to zero
	length = 0;
	maximum_value = 0;
	average = 0;
	signal_data = 0;

	ifstream file; //creates pointer to file
	int i=0;
	char* name;
	name = new char[20]; //allocates memory for the file name

	file.open ("Raw_data_01.txt", ios::in); //default is the first file, opens file

	if(!(file.is_open())){ //if not opened
		cout << "\nError opening file.\n" <<endl; //prints error message
		usage(); //shows possible usage
		exit(0); //exit if file couldn't be opened
	}

	file >> length >> maximum_value;  //read in length and max from the file
	signal_data = new double[length]; //allocate memory for the array

	for(i=0; i<length; i++){
		file >> signal_data[i]; //read in the numbers from the file
	}

	file.close(); //closes the file
	delete [] name; //frees allocated memory

}

Signal::Signal(int fileNumber){
	//initializes variables to zero
	length = 0;
	maximum_value = 0;
	average = 0;
	signal_data = 0;

	ifstream file; //creates file object
	int i=0;
	char* name;
	name = new char[20]; //allocated memory for file name

	sprintf(name, "Raw_data_%02d.txt", fileNumber); //prints num to file name
	file.open(name, ios::in); //opens designated file

	if(!(file.is_open())){ //if file doesnt open then print error and show usage
			cout << "\nError opening file.\n" << endl;
			usage();
			exit(0); //exit if file couldn't be opened
	}

	file >> length >> maximum_value; //read in length and max from the file

	signal_data = new double[length]; //allocate memory for the array

	for(i=0; i<length; i++){
		file >> signal_data[i]; //read in the numbers from the file
	}

	file.close(); //closes the file
	delete [] name; //frees allocated memory
}

Signal::Signal(char *fileName){
	//initializes variables to zero
	length = 0;
	maximum_value = 0;
	average = 0;
	signal_data = 0;

	ifstream file; //creates file object
	int i=0;

	file.open(fileName, ios::in); //opens designated file

	if(!(file.is_open())){ //if file doesn't open print error
		cout << "\nError opening file.\n" << endl;
		usage();
		exit(0); //exit if file couldn't be opened
	}

	file >> length >> maximum_value; //read in length and max from the file

	signal_data = new double[length]; //allocate memory for the array

	for(i=0; i<length; i++){
		file >> signal_data[i]; //read in the numbers from the file
	}

	file.close(); //closes the file
}

Signal::~Signal(){
	delete [] signal_data; //frees memory from signal_data
}

int main(int argc, char *argv[]) {
	//initialize variables
	int x=1;
	int y=1;
	int flag=0;
	int num=0;
	int flag1=0;
	int flag3=0;
	int choice=0;
	double off2=0;
	double sca2=0;
	Signal *sig=NULL; //pointer of type Signal

	//scans through entire command line looking for -n
	while(y < argc && flag == 0){
		if((argv[y][0] == '-') && (argv[y][1] == 'n')){ //compares to -n
			num = 0;

			if((y+1) >= argc){ // if no number is entered after -n
				cout << "\nError. Not enough information." << endl; //print error
				usage(); //shows usage
			}

			num = atoi(argv[y+1]); // converts character to integer

			if(num<1 || num>14){ //checks if a number option
				usage();
			}
			else{ //if in range
				Signal sig1(num); //calls constructor with int parameter
				sig = &sig1; //assigns address to Signal pointer
				flag = 1; //file is found and opened

			}

			y++; //increments past y+1 because it's already found and used
		}
		else if((argv[y][0] == '-') && (argv[y][1] == 'f')){ //compares to -f
			char *nam=0;
			int len=0;

			if((y+1) >= argc){ // if no number is entered after -n
				cout << "\nError. Not enough information." << endl; //print error
				usage(); //shows usage
			}

			len = strlen(argv[y+1]); //finds the length of the given string
			//allocates enough memory for the string and the .txt added to it
			nam = new char[len+5];

			sprintf(nam, "%s.txt", argv[y+1]); //prints to the filename

			Signal sig1(nam); //calls constructor with char* parameter
			sig = &sig1; //assigns address to Signal pointer
			flag = 1; //file is found and opened
			delete[] nam; //free allocated memory

			y++; //increments past y+1 because it's found
		}
		y++; //increments through the argv elements
	}

	if(flag != 1){ //-n/-f are not found, so use default
		Signal sig1; //default constructor
		sig = &sig1; //assigns address to Signal pointer
	}

	while(x < argc){ //looks for other command line arguments

		if((argv[x][0] == '-') && (argv[x][1] == 'n')){
			x++; //if found, skip because file has already been opened before
		}
		else if((argv[x][0] == '-') && (argv[x][1] == 'f')){
			x++; //if found, skip because file has already been opened
		}
		else if((argv[x][0] == '-') && (argv[x][1] == 'o')){
			flag1=1; //-o found
			//initializes variables
			int num1;
			double off=0;

			if((x+1) >= argc){ //checks if there is an argument entered after the -o
				cout << "\nError. Not enough information.\n" << endl;
				usage(); //shows valid command-line arguments
			}

			num1 = sscanf(argv[x+1], "%lf", &off); //checks if offset contains characters

			if(num1==0){  //if yes, print error
				cout << "\nError.\n" << endl;
				usage();
			}

			sig->offset(off); //calls offset method through the Signal pointer

			x++; //already found the next element
		}
		else if((argv[x][0] == '-') && (argv[x][1] == 's')){
			flag1 = 1; //-s found
			//initializes variables
			int num2;
			double sca=0;

			if((x+1) >= argc){ //prints usage if there is no argument after -s
				cout << "\nError. Not enough information.\n" << endl;
				usage();
			}

			num2 = sscanf(argv[x+1], "%lf", &sca); //checks if number contains characters

			if(num2==0){ //if yes, print error
				cout << "\nError.\n" << endl;
				usage();
			}

			sig->scale(sca); //calls scale method through Signal pointer

			x++; //increments past next element
		}
		else if((argv[x][0] == '-') && (argv[x][1] == 'S')){
			flag1 = 1; //-S found

			sig->statistics(); //calls statistics method
		}
		else if((argv[x][0] == '-') && (argv[x][1] == 'C')){
			flag1 = 1; //-C found

			sig->center(); //calling center method
		}
		else if((argv[x][0] == '-') && (argv[x][1] == 'N')){
			flag1 = 1; //-N found

			sig->normalize(); //calls normalize method
		}

		x++; //increments to next command-line argument
	}

	if(flag1 != 1){ //if no command-line arguments found for methods
			do{
				cout << "Options: \n" << "1) Offset\n" << "2) Scale\n"
					 << "3) Center\n" << "4) Normalize\n" << "5) Statistics\n"
					 << endl;

				cout << "Please enter the number of your choice above: ";
				cin >> choice; //user selects option

				if(choice<1 || choice>5){
					flag3 = 0; //keep looping
				}
				else{
					flag3 = 1; //breaks loop
				}


			}while(flag3 != 1); //keep asking for valid option


		switch(choice){
			case 1:
				cout << "\nPlease enter the offset number: ";
				cin >> off2; //gets offset from user input
				cout << endl;
				sig->offset(off2); //calls offset method
				break;
			case 2:
				cout << "\nPlease enter the scale number: ";
				cin >> sca2; //gets scale from user
				cout << endl;
				sig->scale(sca2); //calls scale method
				break;
			case 3:
				sig->center(); //calling center method
				break;
			case 4:
				sig->normalize(); //calls normalize method
				break;
			case 5:
				sig->statistics(); //calls statistics method
				break;
		}//end of switch
	}//end of flag != 1

	char filenam[] = "NewFile.txt"; //file name to create in Save_file

	sig->Sig_info(); //prints length, max, and average
	sig->Save_file(filenam); //creates a new file

	return 0;
}

void Signal::Sig_info(){ //prints length, max, and average
	cout << "\nLength: " << length << "\nCurrent maximum value: "
		 << maximum_value << "\nCurrent average: " << average << endl;
	return;
}

void Signal::Save_file(char *newFilename){
	int x=0;

	ofstream file; //creates file to write to
	file.open(newFilename); //creates new file

	if(!(file.is_open())){ //if not opened
		cout << "\nCannot open file." << endl;
		usage();
		exit(0); //exits if file isn't opened
	}

	file << length << " " << (double) maximum_value << "\n"; //prints the length and offset/scale

	for(x=0;x<length;x++){
		file << (double) signal_data[x] << "\n"; //prints signal_data to new file
	}

	file.close(); //closes file
	return;
}

void Signal::offset(double off){
	int x=0;

	for(x=0;x<length;x++){
		signal_data[x] = (double) (signal_data[x] + off); //adds offset to array of numbers
	}

	maximum_value = (double) act_max(); //finds the max in signal_data
	average_calc(); //calls average so it can be printed in Sig_info

	return;
}

void Signal::scale(double sca){
	int x=0;

	for(x=0;x<length;x++){
		signal_data[x] = (double) (signal_data[x] * sca); //creates the array of scaled numbers
	}

	maximum_value = (double) act_max(); //finds the max of singal_data
	average_calc(); //calls average so it can be printed in Sig_info

	return;
}

void usage(void){ //prints proper format of command-line arguments
	cout << "\nUsage:"
	     << "\n\t-n <Valid file number>           \tSelects the file to open."
		 << "\n\t-n                               \tOpens a default file."
		 << "\n\t-f <Valid file name>             \tOpens the specified file."
	     << "\n\t-o <Offset value>                \tOffsets the file numbers by the given value."
	     << "\n\t-s <scale factor value>          \tScales the file numbers by the given value."
	     << "\n\t-S                               \tShows statistics."
	     << "\n\t-C                               \tCenters the signal."
	     << "\n\t-N                               \tNormalizes the signal.\n"
	     << "\nExamples:\n   ./lab6 -n 2 - s 6.7\n   ./lab4 -C -n 4\n\n" << endl;
	exit(0); //quits program
}

double Signal::act_max(void){
	int x=0;
	double actual_max=0;

	actual_max = (double) signal_data[0]; //makes initial max the first number in the array

	for(x=0;x<length;x++){
		if((double) signal_data[x]>actual_max){ //compares each number to the max
			actual_max = (double) signal_data[x]; //if larger, then make it the max
		}
	}

	return actual_max;
}

void Signal::average_calc(void){
	int i=0;
	double sum=0;

	for(i=0;i<length;i++){
		sum += (double) signal_data[i]; //adds all the numbers in the array together
	}

	average = (double) sum / length; //divides sum by the amount of numbers in the array

	return;
}

void Signal::statistics(void){
	maximum_value = (double) act_max(); //finds the maximum
	average_calc(); //determines the average of the array
}

void Signal::normalize(){
	double new_max;

	new_max = ((double) 1 / (double) maximum_value); //scale it by dividing by the max
	scale(new_max); //scales existing array

	maximum_value = (double) act_max(); //finds the max of signal_data
	average_calc(); //calls average so it can be printed in Sig_info

	return;
}

void Signal::center(){
	average_calc(); //finds the average of the numbers
	average *= (-1); //so it can be subtracted in the offset
	offset(average); //assigns to array in offset function

	maximum_value = (double) act_max(); //finds the max of the signal_data
	average_calc(); //finds average to print in Sig_info

	return;
}
