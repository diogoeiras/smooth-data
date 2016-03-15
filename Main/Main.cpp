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
	ofstream outputFile2(OUTPUT_FILE_2);
	unsigned int ** inputMatrix;
	unsigned int ** outputMatrix;
	unsigned int ** outputMatrixLocal;
	unsigned int bufferA[BUFFER_SIZE];
	unsigned int bufferB[BUFFER_SIZE];
	unsigned int bufferC[BUFFER_SIZE];

	PGMData imageHeader = getHeaderDataFromPGM(inputFile);

	inputMatrix = new unsigned int *[imageHeader.height];
	outputMatrix = new unsigned int *[imageHeader.height];
	outputMatrixLocal = new unsigned int *[imageHeader.height];
	for (unsigned int j = 0; j < imageHeader.height; j++) {
		outputMatrix[j] = new unsigned int[imageHeader.width];
		inputMatrix[j] = new unsigned int[imageHeader.width];
		outputMatrixLocal[j] = new unsigned int[imageHeader.width];
	}

	getDataFromPGM(inputFile, imageHeader, inputMatrix);

	smoothOperation(inputMatrix, outputMatrix, imageHeader);
	smoothOperation2(inputMatrix,outputMatrixLocal,imageHeader,bufferA,bufferB,bufferC);

	writeSmoothImage(outputFile, outputMatrix, imageHeader);
	writeSmoothImage(outputFile2, outputMatrixLocal, imageHeader);

	inputFile.close();
	outputFile.close();
	outputFile2.close();

	for (size_t line = 0; line < imageHeader.height; line++) {
		delete[] inputMatrix[line];
		delete[] outputMatrix[line];
		delete[] outputMatrixLocal[line];
	}
	delete[] inputMatrix;
	delete[] outputMatrix;
	delete[] outputMatrixLocal;

	return 0;
}

void getDataFromPGM(ifstream & inputFile, PGMData imageHeader,
		unsigned int ** inputMatrix) {

	string temp;

	for (unsigned int line = 0; line < imageHeader.height; line++) {
		for (unsigned int column = 0; column < imageHeader.width; column++) {
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

	for (unsigned int j = 0; j < imageHeader.height - 2; j++) {
		for (unsigned int i = 0; i < imageHeader.width - 2; i++) {
			unsigned int sum = 0;
			for (int row = 0; row < 3; row++)
				for (int column = 0; column < 3; column++)
					sum += inputMatrix[j + row][i + column] * k[row][column];
			sum = sum / 16;
			outputMatrix[j + 1][i + 1] = sum;
		}
	}

}

void smoothOperation2(unsigned int ** inputMatrix, unsigned int ** outputMatrix,
		PGMData const imageHeader, unsigned int * bufferA,
		unsigned int * bufferB, unsigned int * bufferC) {

	short k[3][3] = { { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } };

	bufferA = inputMatrix[0];
	bufferB = inputMatrix[1];

	for (unsigned int j = 0; j < imageHeader.height - 2; j++) {

		bufferC = inputMatrix[j+2];
		for (unsigned int i = 0; i < imageHeader.width - 2; i++) {
			unsigned int sum = 0;
			sum += bufferA[i]*k[0][0] + bufferA[i+1]*k[0][1] + bufferA[i+2]*k[0][2];
			sum += bufferB[i]*k[1][0] + bufferB[i+1]*k[1][1] + bufferB[i+2]*k[1][2];
			sum += bufferC[i]*k[2][0] + bufferC[i+1]*k[2][1] + bufferC[i+2]*k[2][2];
			sum = sum / 16;
			outputMatrix[j+1][i+1] = sum;
		}
		bufferA = bufferB;
		bufferB = bufferC;
	}
}

void writeSmoothImage(ofstream & outputFile, unsigned int ** outputMatrix,
		PGMData const imageHeader) {
	outputFile << imageHeader.magicNumber << endl;
	outputFile << imageHeader.comment << endl;
	outputFile << imageHeader.width << " " << imageHeader.height << endl;
	outputFile << imageHeader.maxValue << endl;
	for (unsigned int line = 0; line < imageHeader.height; line++) {
		for (unsigned int column = 0; column < imageHeader.width; column++) {
			outputFile << outputMatrix[line][column] << endl;
		}
	}
}
