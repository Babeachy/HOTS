#include "datamodifier.h"
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtCore/qmath.h>
#include <QtCore/qrandom.h>
#include <QtWidgets/QComboBox>

#include <vector>
#include <array>

#include <readMatSpikeData.h>
#include <iostream>

using namespace QtDataVisualization;

//const int numberOfItems = 3600;
//const float curveDivider = 3.0f;
//const int lowerNumberOfItems = 900;
//const float lowerCurveDivider = 0.75f;

DataModifier::DataModifier(Q3DScatter *scatter)
    : m_graph(scatter),
      m_fontSize(40.0f),
      m_style(QAbstract3DSeries::MeshSphere),
      m_smooth(true)
    // m_itemCount(lowerNumberOfItems),
    // m_curveDivider(lowerCurveDivider)
{
    m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    m_graph->addSeries(series);

    readData = new readMatSpikeData("D:/dev/Data/Spikes_center_out_data_HOTS.mat",  "spikedata");
    //addData();
    //Initialize default graph here instead
    //setGraph1(0, 1);
    setGraph1(0, 1);
}


void DataModifier::setGraph1(int trialNum, int channelNum){
    //currently the default graph
    //selects a single channel
    //plots x, y, and z data at times when spikes occur
    //if spike occurs before position is set, it is ignored
    //if spike occurs, a data point is plotted at the last position triggered

    // Configure the axes according to the data
    m_graph->axisX()->setTitle("X");

    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitle("Y");

    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitle("Z");
    m_graph->axisZ()->setTitleVisible(true);

    int numSpikesInChannel = 0;
    int numSpikes = readData->getNumSpikes(trialNum);
    double *channels = readData->getChannels(trialNum);
    double *spikeTimes = readData->getSpikeTimes(trialNum);

    for (int i = 0; i < numSpikes; i++){
        if (channels[i] == channelNum){
            numSpikesInChannel++;
        }
    }


    QScatterDataArray *dataArray = new QScatterDataArray;

    if (numSpikesInChannel == 0)
    {
        m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
        return;
    }

    dataArray->resize(numSpikesInChannel);
    QScatterDataItem *ptrToDataArray = &dataArray->first();


    double *posTimeStamps = readData->getPosTimeStamps(trialNum);
    std::vector<std::array<double, 3>> pos = readData->getPositions(trialNum);
    int numPos = readData->getNumPos(trialNum);


    bool nextTimeFlag;
    for (int i = 0, k = 0; i < numSpikes; i++){
        //i is the index of the spike
        //k is the current index for posTimeStamps
        if (channelNum <= channels[i]+0.3 && channelNum >= channels[i]-0.3){
            //corresponding spiketime is spikeTimes[i]
            nextTimeFlag = false;
            while (!nextTimeFlag){

                //sets currentTimeStamp to be the latest that occurs before the spikeTime
                //checking out of bounds
                if (k+1 < numPos){
                    //check if spikeTime is greater than nextTimeStamp


                    if (spikeTimes[i] >= posTimeStamps[k+1]){
                        k++;
                    }
                    else nextTimeFlag = true;
                }
                else nextTimeFlag = true;
            }
            ptrToDataArray->setPosition(QVector3D(pos[k][0], pos[k][1], pos[k][2]));
            ptrToDataArray++;
        }
    }

    m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
}

void DataModifier::setGraph2(int trialNum)
{
    m_graph->axisX()->setTitle("X");
    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitle("Y");
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitle("Z");
    m_graph->axisZ()->setTitleVisible(true);

    int numPos = readData->getNumPos(trialNum);

    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(numPos);
    QScatterDataItem *ptrToDataArray = &dataArray->first();

    std::vector<std::array<double, 3>> pos = readData->getPositions(trialNum);

    for (int i = 0; i < numPos; i++)
    {
        ptrToDataArray->setPosition(QVector3D(pos[i][0], pos[i][1], pos[i][2]));
        ptrToDataArray++;
    }

    m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);

}

void DataModifier::setGraph3(int trialNum)
{
    m_graph->axisX()->setTitle("X");
    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitle("Y");
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitle("t");
    m_graph->axisZ()->setTitleVisible(true);

    int numPos = readData->getNumPos(trialNum);

    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(numPos);
    QScatterDataItem *ptrToDataArray = &dataArray->first();

    double *posTimeStamps = readData->getPosTimeStamps(trialNum);
    std::vector<std::array<double, 3>> pos = readData->getPositions(trialNum);

    for (int i = 0; i < numPos; i++)
    {
        ptrToDataArray->setPosition(QVector3D(pos[i][0], pos[i][1], posTimeStamps[i]));
        ptrToDataArray++;
    }

    m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
}


void DataModifier::setGraph4(int trialNum, int channelNum){
    //currently the default graph
    //selects a single channel
    //plots x, y, and z data at times when spikes occur
    //if spike occurs before position is set, it is ignored
    //if spike occurs, a data point is plotted at the last position triggered

    // Configure the axes according to the data
    m_graph->axisX()->setTitle("X");

    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitle("Y");

    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitle("t");
    m_graph->axisZ()->setTitleVisible(true);

    int numSpikesInChannel = 0;
    int numSpikes = readData->getNumSpikes(trialNum);
    double *channels = readData->getChannels(trialNum);
    double *spikeTimes = readData->getSpikeTimes(trialNum);

    for (int i = 0; i < numSpikes; i++){
        if (channels[i] == channelNum){
            numSpikesInChannel++;
        }
    }




    QScatterDataArray *dataArray = new QScatterDataArray;

    if (numSpikesInChannel == 0)
    {
        m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
        return;
    }

    dataArray->resize(numSpikesInChannel);
    QScatterDataItem *ptrToDataArray = &dataArray->first();


    double *posTimeStamps = readData->getPosTimeStamps(trialNum);
    std::vector<std::array<double, 3>> pos = readData->getPositions(trialNum);
    int numPos = readData->getNumPos(trialNum);


    bool nextTimeFlag;
    for (int i = 0, k = 0; i < numSpikes; i++){
        //k is the current index for posTimeStamps
        if (channelNum <= channels[i]+0.3 && channelNum >= channels[i]-0.3){
            //corresponding spiketime is spikeTimes[i]
            nextTimeFlag = false;
            while (!nextTimeFlag){

                //sets currentTimeStamp to be the latest that occurs before the spikeTime
                //checking out of bounds
                if (k+1 < numPos){
                    //check if spikeTime is greater than nextTimeStamp


                    if (spikeTimes[i] >= posTimeStamps[k+1]){
                        k++;
                    }
                    else nextTimeFlag = true;
                }
                else nextTimeFlag = true;
            }
            ptrToDataArray->setPosition(QVector3D(pos[k][0], pos[k][1], posTimeStamps[k]));
            ptrToDataArray++;
        }
    }

    m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
}
















void DataModifier::changeStyle(int style)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        m_style = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        if (m_graph->seriesList().size())
            m_graph->seriesList().at(0)->setMesh(m_style);
    }
}

void DataModifier::setSmoothDots(int smooth)
{
    m_smooth = bool(smooth);
    QScatter3DSeries *series = m_graph->seriesList().at(0);
    series->setMeshSmooth(m_smooth);
}

void DataModifier::changeTheme(int theme)
{
    Q3DTheme *currentTheme = m_graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
}

void DataModifier::changePresetCamera()
{
    static int preset = Q3DCamera::CameraPresetFrontLow;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
}


void DataModifier::changeLabelStyle()
{
    m_graph->activeTheme()->setLabelBackgroundEnabled(!m_graph->activeTheme()->isLabelBackgroundEnabled());
}


void DataModifier::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
{
    int quality = int(sq);
    emit shadowQualityChanged(quality); // connected to a checkbox in main.cpp
}

void DataModifier::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    m_graph->setShadowQuality(sq);
}

void DataModifier::setBackgroundEnabled(int enabled)
{
    m_graph->activeTheme()->setBackgroundEnabled((bool)enabled);
}

void DataModifier::setGridEnabled(int enabled)
{
    m_graph->activeTheme()->setGridEnabled((bool)enabled);
}



QVector3D DataModifier::randVector()
{
    return QVector3D(
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f -
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f,
                (float)(QRandomGenerator::global()->bounded(100)) / 100.0f -
                (float)(QRandomGenerator::global()->bounded(100)) / 100.0f,
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f -
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f);
}

int DataModifier::getNumTrials()
{
    return readData->getTotalTrials();
}

int DataModifier::getTrialLabel(int trialNumber)
{
    return readData->getTrialLabel(trialNumber);
}

