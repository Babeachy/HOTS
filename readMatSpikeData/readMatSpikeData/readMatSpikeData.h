#pragma once
#include <mat.h>
#include <matrix.h>
#include <vector>
#include <array>

class readMatSpikeData {
public:
	MATFile * mfPtr;
	mxArray * dataPtr;
	readMatSpikeData(const char* matFileName, const char* structureName = "spikedata");

	int getNumSpikes(int trialNumber);
	double getStartTime(int trialNumber);
	double getEndTime(int trialNumber);
	double* getSpikeTimes(int trialNumber);
	double* getChannels(int trialNumber);
	int getNumPos(int trialNumber);
	double* getPosTimeStamps(int trialNumber);
	std::vector<std::array<double, 3>> getPositions(int trialNumber);
	//get Vx, Vy, Vz
	int getTotalTrials();
	int getTrialLabel(int trialNumber);


//	~readMatSpikeData();
};