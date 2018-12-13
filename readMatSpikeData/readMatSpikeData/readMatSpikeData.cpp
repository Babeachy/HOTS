#include "readMatSpikeData.h"
#include <mat.h>
#include <matrix.h>
#include <vector>
#include <array>

readMatSpikeData::readMatSpikeData(const char * matFileName, const char * structureName) {
	mfPtr = matOpen(matFileName, "r");
	dataPtr = matGetVariable(mfPtr, structureName);
}

int readMatSpikeData::getNumSpikes(int trialNumber) {
	return (int)*mxGetPr(mxGetField(dataPtr, trialNumber, "numSpikes"));
}

double readMatSpikeData::getStartTime(int trialNumber) {
	return *mxGetPr(mxGetField(dataPtr, trialNumber, "startTime"));
}

double readMatSpikeData::getEndTime(int trialNumber) {
	return *mxGetPr(mxGetField(dataPtr, trialNumber, "endTime"));
}

double* readMatSpikeData::getSpikeTimes(int trialNumber) {
	return mxGetPr(mxGetField(dataPtr, trialNumber, "spikeTimes"));
}

double* readMatSpikeData::getChannels(int trialNumber) {
	return mxGetPr(mxGetField(dataPtr, trialNumber, "channel"));
}

int readMatSpikeData::getNumPos(int trialNumber) {
	return (int)mxGetN(mxGetField(dataPtr, trialNumber, "posTimeStamps"));
}

std::vector<std::array<double, 3>> readMatSpikeData::getPositions(int trialNumber) {
	mxArray *temp = mxGetField(dataPtr, trialNumber, "positions");
	double *realTemp = mxGetPr(temp);
	std::vector<std::array<double, 3>> pos;
	for (int i = 0; i < (int)mxGetN(temp)*3; i += 3) {
		pos.push_back({ realTemp[i], realTemp[i + 1], realTemp[i + 2] });
	}

	return pos;
}

double* readMatSpikeData::getPosTimeStamps(int trialNumber) {
	return mxGetPr(mxGetField(dataPtr, trialNumber, "posTimeStamps"));
}

int readMatSpikeData::getTotalTrials()
{
	return (int)mxGetNumberOfElements(dataPtr);
}

int readMatSpikeData::getTrialLabel(int trialNumber) {
	return (int)*mxGetPr(mxGetField(dataPtr, trialNumber, "label"));
}