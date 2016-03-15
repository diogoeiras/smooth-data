/*
 * Main.hpp
 *
 *  Created on: 08/03/2016
 *      Author: Diogo Eiras
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#define INPUT_FILE "input.pgm"
#define OUTPUT_FILE "output.pgm"
#define OUTPUT_FILE_2 "output2.pgm"
#define BUFFER_SIZE 1024

using namespace std;

// To be used on color images
struct pixel_data {
	unsigned int red, green, blue;
};
typedef pixel_data pixel;

struct pgm_data {
	string magicNumber, comment;
	unsigned int width, height, maxValue;
};
typedef pgm_data PGMData;

void getDataFromPGM(ifstream & inputfile, PGMData imageHeader,
		unsigned int ** inputMatrix);
PGMData getHeaderDataFromPGM(ifstream & inputfile);
void smoothOperation(unsigned int ** inputMatrix, unsigned int ** outputMatrix,
		PGMData const imageHeader);
void writeSmoothImage(ofstream & outputFile, unsigned int ** outputMatrix,
		PGMData const imageHeader);
void smoothOperation2(unsigned int ** inputMatrix, unsigned int ** outputMatrix,
		PGMData const imageHeader, unsigned int * bufferA,
		unsigned int * bufferB, unsigned int * bufferC);

#endif /* MAIN_HPP_ */
