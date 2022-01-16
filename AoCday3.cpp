//algorithm: with 12 numbers per line, and a lot of lines, we should do this line by line.
//no dynamic multidimensional array is necessary, but several other arrays are.
//Necessities: array to hold input, 2D array to hold #s of 1s and 0s, two arrays to hold final gamma rate and epsilon rate - could do 2D, but for readability, declare two 1D
//then, convert to decimal (int) with double dabble formula - starting from index 0 and total 0, double the total and add the digit, then move up 1 digit and repeat
//lastly. multiply the two decimals together to receive the power consumption

#include <iostream>
#include <fstream>
//declare global constants
const int FILE_SIZE = 1000, LINE_SIZE = 12;
//declare functions
void readLine(int binaryLine[], const int lineSize, std::ifstream& inStream);
//Precondition: the array is of size lineSize, as is each line of numbers in the input file
//Postcondition: fills the array with each individual number on the line; discards the ending newline character
void positionCounter(int binaryLine[], int commonPosition[][2], const int size);
//Precondition: binaryLine[] and commonPosition[] are both arrays of the same size, represented by argument size
//Postcondition: checks through each index of binaryLine; increases commonPosition[i][0] by 1 if binaryLine[i] == 0, else increases commonPosition[i][1] by 1
int binaryToDecimal(int binaryLine[], const int size);
//Precondition: binaryLine[size] is an array containing binary values only
//Postcondition: using the double dabble method, returns the decimal value of the binary line
int** reportSifter(int** diagnosticReport, int index, int& size, bool isOxygen);
//sifts through diagnosticReport of size [size] and searches through each entry of diagnosticReport[][index], finding the most common value.
//isOxygen determines what happens with the most common value and what happens during ties.
//isOxygen == true: Returns a new array and its size (stored in param size) containing only the entries with the most common value in that index, and entries containing 1 if there is a tie. Deletes the old array.
//isOxygen == false: Returns a new array and its size (stored in param size) containing only the entries with the least common value in that index, and entries containing 0 if there is a tie. Deletes the old array.
void lineSifter(int** oldArray, const int oldSize, int** newArray, const int newSize, int parseValue, int index);
//sifts through lines to find parseValue at oldArray[][index]; if the value is found, adds the line to newArray
int** new2DArray(const int sizeFirst, const int sizeSecond);
//returns a new array of array[sizeFirst][sizeSecond]


//main
int main()
{
	using namespace std;
	//Part 1
	//initialize variables
	int inputLine[12], zeroOne[12][2], gammaRate[12], epsilonRate[12];
	int gammaDecimal = 0, epsilonDecimal = 0;
	ifstream inStream("input3.txt");
	if (inStream.fail())
		exit(1);
	//fill zeroOne with value
	for (int i = 0; i < LINE_SIZE; i++)
	{
		zeroOne[i][0] = 0;
		zeroOne[i][1] = 0;
	}
	//begin program
	while (!(inStream.eof()))
	{
		readLine(inputLine, LINE_SIZE, inStream);
		positionCounter(inputLine, zeroOne, LINE_SIZE);
	}
	inStream.close();
	for (int i = 0; i < LINE_SIZE; i++)
	{
		if (zeroOne[i][0] > zeroOne[i][1])
		{
			gammaRate[i] = 0;
			epsilonRate[i] = 1;
		}
		else
		{
			gammaRate[i] = 1;
			epsilonRate[i] = 0;
		}
	}
	inStream.close();
	gammaDecimal = binaryToDecimal(gammaRate, LINE_SIZE);
	epsilonDecimal = binaryToDecimal(epsilonRate, LINE_SIZE);
	cout << "Power consumption: " << gammaDecimal * epsilonDecimal << endl;
	//Part 2
	//initialize variables
	int oxygenDecimal, co2Decimal, reportSize = FILE_SIZE;
	int** diagnosticReport;
	inStream.open("input3.txt");
	if (inStream.fail())
		exit(1);
	//initialize report
	diagnosticReport = new int* [FILE_SIZE];
	for (int i = 0; i < FILE_SIZE; i++)
		diagnosticReport[i] = new int[LINE_SIZE];
	//Find oxygen rating
	for (int currentLine = 0; currentLine < FILE_SIZE && (!(inStream.eof())); currentLine++)
	{
		readLine(diagnosticReport[currentLine], LINE_SIZE, inStream);
	}
	inStream.close();
	for (int index = 0; index < LINE_SIZE && reportSize > 1; index++)
	{
		diagnosticReport = reportSifter(diagnosticReport, index, reportSize, true);
	}
	oxygenDecimal = binaryToDecimal(diagnosticReport[0], LINE_SIZE);
	//Set up conditions for next run
	diagnosticReport = new int* [FILE_SIZE];
	for (int i = 0; i < FILE_SIZE; i++)
		diagnosticReport[i] = new int[LINE_SIZE];
	reportSize = FILE_SIZE;
	inStream.open("input3.txt");
	if (inStream.fail())
		exit(1);
	//Find C02 rating
	for (int currentLine = 0; currentLine < FILE_SIZE && (!(inStream.eof())); currentLine++)
	{
		readLine(diagnosticReport[currentLine], LINE_SIZE, inStream);
	}
	inStream.close();
	for (int index = 0; index < LINE_SIZE && reportSize > 1; index++)
	{
		diagnosticReport = reportSifter(diagnosticReport, index, reportSize, false);
	}
	co2Decimal = binaryToDecimal(diagnosticReport[0], LINE_SIZE);
	cout << "Life support rating: " << oxygenDecimal * co2Decimal << endl;
	//delete dynamic array
	for (int i = 0; i < reportSize; i++)
		delete[] diagnosticReport[i];
	delete[] diagnosticReport;
	return 0;
}







//define functions
void readLine(int binaryLine[], const int lineSize, std::ifstream& inStream)
{
	using namespace std;
	char next;
	for (int i = 0; i < lineSize; i++)
	{
		inStream.get(next);
		binaryLine[i] = static_cast<int>(next) - 48; //in ascii decimal, 48 = 0 and 49 = 1
	}
	inStream.get(next);
	return;
}

void positionCounter(int binaryLine[], int commonPosition[][2], const int size)
{
	for (int i = 0; i < size; i++)
	{
		if (binaryLine[i]) //if 1
			commonPosition[i][1]++;
		else //if 0
			commonPosition[i][0]++;
	}
	return;
}

int binaryToDecimal(int binaryLine[], const int size)
{
	int total = 0;
	for (int i = 0; i < size; i++)
	{
		total *= 2;
		total += binaryLine[i];
	}
	return total;
}

int** reportSifter(int** diagnosticReport, int index, int& size, bool isOxygen)
{
	const int oldSize = size;
	int** newReport;
	int ones = 0, zeroes = 0;
	bool searchOnes;
	//find common value
	for (int i = 0; i < oldSize; i++)
	{
		if (diagnosticReport[i][index])
			ones++;
		else //if 0
			zeroes++;
	}
	//determine how to use common value
	if (isOxygen)
	{
		if (ones >= zeroes)
		{
			size = ones;
			searchOnes = true;
		}
		else // if (ones < zeroes)
		{
			size = zeroes;
			searchOnes = false;
		}
	}
	else //if not isOxygen
	{
		if (ones >= zeroes)
		{
			size = zeroes;
			searchOnes = false;
		}
		else //if (zeroes > ones)
		{
			size = ones;
			searchOnes = true;
		}
	}
	//create new array and add parsed lines to it
	newReport = new2DArray(size, LINE_SIZE);
	if (searchOnes) //if searching for ones
		lineSifter(diagnosticReport, oldSize, newReport, size, 1, index);
	else //if searching for zeroes
		lineSifter(diagnosticReport, oldSize, newReport, size, 0, index);
	//delete old array
	for (int i = 0; i < oldSize; i++)
		delete[] diagnosticReport[i];
	delete[] diagnosticReport;
	return newReport;
}

void lineSifter(int** oldArray, const int oldSize, int** newArray, const int newSize, int parseValue, int index)
{
	int newIndex = 0;
	for (int i = 0; i < oldSize && newIndex < newSize; i++)
	{
		if (oldArray[i][index] == parseValue)
		{
			for (int j = 0; j < LINE_SIZE; j++)
				newArray[newIndex][j] = oldArray[i][j];
			newIndex++;
		}
	}
	return;
}

int** new2DArray(const int sizeFirst, const int sizeSecond)
{
	int** newArray;
	newArray = new int* [sizeFirst];
	for (int i = 0; i < sizeFirst; i++)
		newArray[i] = new int[sizeSecond];
	return newArray;
}