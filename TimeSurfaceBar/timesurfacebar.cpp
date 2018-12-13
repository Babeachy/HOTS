#include "timesurfacebar.h"

#include <QtWidgets/QComboBox>

#include <math.h>
#include <iostream>

using namespace QtDataVisualization;

TimeSurfaceBar::TimeSurfaceBar(Q3DBars *surface)
    :graph(surface),
     series(new QBar3DSeries)
{
    readData = new readMatSpikeData("D:/dev/Data/Spikes_center_out_data_HOTS.mat",  "spikedata");

    timeSurfaceAxis = new QValue3DAxis();
    timeSurfaceAxis->setTitle("Time Surface");
    timeSurfaceAxis->setTitleVisible(true);
    channelAxis = new QCategory3DAxis();
    channelAxis->setTitle("Channel");
    channelAxis->setTitleVisible(true);
    timeAxis = new QCategory3DAxis();
    timeAxis->setTitle("Time");
    timeAxis->setTitleVisible(true);



    differenceAxis = new QValue3DAxis();
    differenceAxis->setTitle("Difference");
    differenceAxis->setTitleVisible(true);
    iChannelAxis = new QCategory3DAxis();
    iChannelAxis->setTitle("Channel i");
    iChannelAxis->setTitleVisible(true);
    jChannelAxis = new QCategory3DAxis();
    jChannelAxis->setTitle("Channel i");
    jChannelAxis->setTitleVisible(true);





    fillChannelChannelProxy(trialNumber);

    std::cout << "Done gathering data." << std::endl;
}

TimeSurfaceBar::~TimeSurfaceBar()
{
    delete graph;
}

void TimeSurfaceBar::fillTimeSurfaceProxy(int trialNumber)
{
    //make data array
    //using a loop nested in another loop
    //make row for each loop, adding data to that row in the nested loop
    //then add that row to the data array
    //then finally set the array to be dataArray using the reset array feature

    QBarDataArray *dataSet = new QBarDataArray;

    QBarDataRow *dataRow;

    double timeIncrement = (readData->getEndTime(trialNumber)-readData->getStartTime(trialNumber))/numTimes;
    int numSpikes = readData->getNumSpikes(trialNumber);
    double* channels = readData->getChannels(trialNumber);
    double* spikeTimes = readData->getSpikeTimes(trialNumber);

    double timeSurface;
    for (int i = 1; i <= 35; i++)
    {
    //i is the channel number
        dataRow = new QBarDataRow(numTimes+1);
        int spikeTimeIndex = 0;
        double lastSpikeTime = std::numeric_limits<double>::min();
        int rowIndex = 0;
        for (double j = readData->getStartTime(trialNumber); j <= readData->getEndTime(trialNumber); j+= timeIncrement)
        {
            //updating value of last spiketime that occured in channel
            while (true)
            {
                if (spikeTimes[spikeTimeIndex] < j)
                {
                    if (channels[spikeTimeIndex] == i) lastSpikeTime = spikeTimes[spikeTimeIndex];

                    if (spikeTimeIndex < numSpikes - 1) spikeTimeIndex++;
                    else break;

                }
                else break;
            }

            timeSurface = exp((lastSpikeTime-j)/timeConstant);
            std::cout << "adding (" << i << ", " << j << ", " << timeSurface << ")" << std::endl;
            (*dataRow)[rowIndex++].setValue(timeSurface);
        }
        dataSet->append(dataRow);
    }
    //done with loop
    graph->setValueAxis(timeSurfaceAxis);
    graph->setColumnAxis(channelAxis);
    graph->setRowAxis(timeAxis);

    series->setItemLabelFormat(QStringLiteral("Channel: @colLabel, Time: @rowLabel, TimeSurface: @valueLabel"));
    series->setMesh(QAbstract3DSeries::MeshBevelBar);

    series->dataProxy()->resetArray(dataSet);

    graph->addSeries(series);
}

//void TimeSurfaceBar::fillChannelChannelProxy(int trialNumber)
//{

//    double timeIncrement = (readData->getEndTime(trialNumber)-readData->getStartTime(trialNumber))/numTimes;
//    int numSpikes = readData->getNumSpikes(trialNumber);
//    double* channels = readData->getChannels(trialNumber);
//    double* spikeTimes = readData->getSpikeTimes(trialNumber);
//    double iTimeSurface;
//    double jTimeSurface;
//    double squareDifferenceTotal;
//    double difference;
//    QBarDataRow *dataRow;
//    QBarDataArray *dataSet = new QBarDataArray;


//    for (int i = 1; i <= 35; i++)
//    {
//        dataRow = new QBarDataRow(36);
//        for (int j = 1; j <= 35; j++)
//        {
//            int spikeTimeIndex = 0;
//            double iLastSpikeTime = std::numeric_limits<double>::min();
//            double jLastSpikeTime = std::numeric_limits<double>::min();
//            int numNumbers = 0;
//            squareDifferenceTotal = 0;
//            for (double k = readData->getStartTime(trialNumber); k <= readData->getEndTime(trialNumber); k+= timeIncrement)
//            {
//                //updating value of last spiketime that occured in each channel
//                while (true)
//                {
//                    if (spikeTimes[spikeTimeIndex] < j)
//                    {
//                        if (channels[spikeTimeIndex] == i) iLastSpikeTime = spikeTimes[spikeTimeIndex];
//                        if (channels[spikeTimeIndex] == j) jLastSpikeTime = spikeTimes[spikeTimeIndex];

//                        if (spikeTimeIndex < numSpikes - 1) spikeTimeIndex++;
//                        else break;

//                    }
//                    else break;
//                }

//                iTimeSurface = exp((iLastSpikeTime-k)/timeConstant);
//                jTimeSurface = exp((jLastSpikeTime-k)/timeConstant);
//                //std::cout << "iTimeSurface: " << iTimeSurface << "jTimeSurface: " << jTimeSurface << std::endl;
//                //std::cout << "i: " << i << ", j: " << j << ", adding: " << pow(iTimeSurface-jTimeSurface,2) << std::endl;
//                squareDifferenceTotal += pow(iTimeSurface-jTimeSurface, 2);
//                numNumbers++;
//            }
//            difference = pow(squareDifferenceTotal/numNumbers, 0.5);
//            std::cout << "i: " << i << ", j: " << j << ", difference: " << difference << std::endl;
//            differences[i-1][j-1] = difference;
//            (*dataRow)[j-1].setValue(difference);


//        }
//        dataSet->append(dataRow);

//    }

//    graph->setValueAxis(differenceAxis);
//    graph->setColumnAxis(iChannelAxis);
//    graph->setRowAxis(jChannelAxis);

//    series->setMesh(QAbstract3DSeries::MeshBevelBar);

//    series->dataProxy()->resetArray(dataSet);

//    graph->addSeries(series);
//}

void TimeSurfaceBar::fillChannelChannelProxy(int trialNumber)
{
    setDifferences(trialNumber);
    QBarDataRow *dataRow;
    QBarDataArray *dataSet = new QBarDataArray;

    for (int i = 1; i <=35; i++)
    {

        dataRow = new QBarDataRow(36);
        for (int j = 1; j <= 35; j++)
        {
            std::cout << "a" << std::endl;
            (*dataRow)[j-1].setValue(differences[i-1][j-1]);
            std::cout << "b" << std::endl;

        }
        dataSet->append(dataRow);
        std::cout <<"c" << std::endl;
    }

    std::cout <<"d" << std::endl;
    graph->setValueAxis(differenceAxis);
    graph->setColumnAxis(iChannelAxis);
    graph->setRowAxis(jChannelAxis);

    series->setMesh(QAbstract3DSeries::MeshBevelBar);

    std::cout <<"e" << std::endl;
    series->dataProxy()->resetArray(dataSet);
    std::cout <<"f" << std::endl;
    graph->addSeries(series);
    std::cout <<"g" <<std::endl;
}

void TimeSurfaceBar::changeSelectionMode(int selectionMode)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        int flags = comboBox->itemData(selectionMode).toInt();
        graph->setSelectionMode(QAbstract3DGraph::SelectionFlags(flags));
    }


}

void TimeSurfaceBar::numTimesChanged(int newNumTimes)
{
    numTimes = newNumTimes;
}

void TimeSurfaceBar::numTimesReset()
{
    numTimes = DEFAULT_NUM_TIMES;
}

void TimeSurfaceBar::timeConstantChanged(double newConstant)
{
    timeConstant = newConstant;
}

void TimeSurfaceBar::timeConstantReset()
{
    timeConstant = DEFAULT_TIME_CONSTANT;
}

void TimeSurfaceBar::trialNumberChanged(int val)
{
    trialNumber = val;
}

void TimeSurfaceBar::channelNumberChanged(int val)
{
    channelSelected = val;
}

void TimeSurfaceBar::slice()
{
    //
}


void TimeSurfaceBar::render()
{
    fillTimeSurfaceProxy(trialNumber);
}

void TimeSurfaceBar::setDifferences(int trialNumber)
{
    double timeIncrement = (readData->getEndTime(trialNumber)-readData->getStartTime(trialNumber))/numTimes;
    int numSpikes = readData->getNumSpikes(trialNumber);
    double* channels = readData->getChannels(trialNumber);
    double* spikeTimes = readData->getSpikeTimes(trialNumber);
    double iTimeSurface;
    double jTimeSurface;
    double squareDifferenceTotal;
    double difference;


    for (int i = 1; i <= 35; i++)
    {
        for (int j = 1; j <= 35; j++)
        {
            int spikeTimeIndex = 0;
            double iLastSpikeTime = std::numeric_limits<double>::min();
            double jLastSpikeTime = std::numeric_limits<double>::min();
            int numNumbers = 0;
            squareDifferenceTotal = 0;
            for (double k = readData->getStartTime(trialNumber); k <= readData->getEndTime(trialNumber); k+= timeIncrement)
            {
                //updating value of last spiketime that occured in each channel
                while (true)
                {
                    if (spikeTimes[spikeTimeIndex] < j)
                    {
                        if (channels[spikeTimeIndex] == i) iLastSpikeTime = spikeTimes[spikeTimeIndex];
                        if (channels[spikeTimeIndex] == j) jLastSpikeTime = spikeTimes[spikeTimeIndex];

                        if (spikeTimeIndex < numSpikes - 1) spikeTimeIndex++;
                        else break;

                    }
                    else break;
                }

                iTimeSurface = exp((iLastSpikeTime-k)/timeConstant);
                jTimeSurface = exp((jLastSpikeTime-k)/timeConstant);
                //std::cout << "iTimeSurface: " << iTimeSurface << "jTimeSurface: " << jTimeSurface << std::endl;
                //std::cout << "i: " << i << ", j: " << j << ", adding: " << pow(iTimeSurface-jTimeSurface,2) << std::endl;
                squareDifferenceTotal += pow(iTimeSurface-jTimeSurface, 2);
                numNumbers++;
            }
            difference = pow(squareDifferenceTotal/numNumbers, 0.5);
            //std::cout << i-1 << j-1 << std::endl;
            differences[i-1][j-1] = difference;
        }
    }
    std::cout<< "Got to end" << std::endl;
}

//void TimeSurfaceBar::SortChannels()
//{
//    int i = 0;
//    sortedChannels[i] = 1;
//    int channelLookingAt = 1;
//    double minDifference;
//    for (int k = 0; k < 35; k++)
//    {
//        minDifference = std::numeric_limits<double>::max();
//        for (int l = 0; l < 35 ; l++)
//        {
//            //l+1 is the channel being compared to
//            if (differerence[channelLookingAt][l])
//        }
//    }
//}


