/*
 * Main.cpp
 *
 *  Created on: 08/03/2016
 *      Author: Diogo Eiras
 */

#include "Main.hpp"

int main(int argc, char ** args) {

	ifstream inputFile(INPUT_FILE, std::ifstream::in);
	ofstream outputFile(OUTPUT_FILE);
	unsigned int ** inputMatrix;
	unsigned int ** outputMatrix;

	PGMData imageHeader = getHeaderDataFromPGM(inputFile);

	inputMatrix = new unsigned int *[imageHeader.height];
	outputMatrix = new unsigned int *[imageHeader.height];
	for (size_t j = 0; j < imageHeader.height; j++) {
		outputMatrix[j] = new unsigned int[imageHeader.width];
		inputMatrix[j] = new unsigned int[imageHeader.width];
	}

	getDataFromPGM(inputFile, imageHeader, inputMatrix);

	smoothOperation(inputMatrix, outputMatrix, imageHeader);

	writeSmoothImage(outputFile, outputMatrix, imageHeader);

	inputFile.close();
	outputFile.close();

	for (size_t line = 0; line < imageHeader.height; line++) {
		delete[] inputMatrix[line];
		delete[] outputMatrix[line];
	}
	delete[] inputMatrix;
	delete[] outputMatrix;

	return 0;
}

void getDataFromPGM(ifstream & inputFile, PGMData imageHeader,
		unsigned int ** inputMatrix) {

	string temp;

	for (size_t line = 0; line < imageHeader.height; line++) {
		for (size_t column = 0; column < imageHeader.width; column++) {
			getline(inputFile, temp, '\n');
			inputMatrix[line][column] = atoi(temp.c_str());
		}
	}

}

PGMData getHeaderDataFromPGM(ifstream & inputFile) {

	PGMData imageHeader;

	string temp;

	getline(inputFile, imageHeader.magicNumber, '\n');
	getline(inputFile, imageHeader.comment, '\n');
	getline(inputFile, temp, ' '); // width value
	imageHeader.width = atoi(temp.c_str());
	getline(inputFile, temp, '\n'); // height value
	imageHeader.height = atoi(temp.c_str());
	getline(inputFile, temp, '\n'); // max scale value
	imageHeader.maxValue = atoi(temp.c_str());

	return imageHeader;

}

void smoothOperation(unsigned int ** inputMatrix, unsigned int ** outputMatrix,
		PGMData const imageHeader) {

	short k[3][3] = { { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } };

	for (size_t j = 0; j < imageHeader.height - 2; j++) {
		for (size_t i = 0; i < imageHeader.width - 2; i++) {
			unsigned int sum = 0;
			for (int row = 0; row < 3; row++)
				for (int column = 0; column < 3; column++)
					sum += inputMatrix[j + row][i + column] * k[row][column];
			sum = sum / 16;
			outputMatrix[j + 1][i + 1] = sum;
		}
	}

}

void writeSmoothImage(ofstream & outputFile, unsigned int ** outputMatrix,
		PGMData const imageHeader) {
	outputFile << imageHeader.magicNumber << endl;
	outputFile << imageHeader.comment << endl;
	outputFile << imageHeader.width << " " << imageHeader.height << endl;
	outputFile << imageHeader.maxValue << endl;
	for (size_t line = 0; line < imageHeader.height; line++) {
		for (size_t column = 0; column < imageHeader.width; column++) {
			outputFile << outputMatrix[line][column] << endl;
		}
	}
}
