#ifndef TIMESURFACEBAR_H
#define TIMESURFACEBAR_H

#include <QtDataVisualization/Q3DBars>
#include <QtDataVisualization/QBar3DSeries>
#include <QtDataVisualization/QBarDataProxy>
#include <QtDataVisualization/QCategory3DAxis>
#include <QtDataVisualization/QValue3DAxis>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>

#include <readMatSpikeData.h>

using namespace QtDataVisualization;

class TimeSurfaceBar:public QObject
{
    Q_OBJECT
public:
    explicit TimeSurfaceBar(Q3DBars *surface);
    ~TimeSurfaceBar();

public Q_SLOTS:
    void changeSelectionMode(int mode);
    void numTimesChanged(int numTimes);
    void numTimesReset();
    void timeConstantChanged(double timeConstant);
    void timeConstantReset();
    void trialNumberChanged(int val);
    void channelNumberChanged(int channelVal);
    void slice();



    void render();

private:
    Q3DBars *graph;
    QBarDataProxy *dataProxy;
    QBar3DSeries *series;

    QValue3DAxis *timeSurfaceAxis;
    QCategory3DAxis *channelAxis;
    QCategory3DAxis *timeAxis;

    QValue3DAxis *differenceAxis;
    QCategory3DAxis *iChannelAxis;
    QCategory3DAxis *jChannelAxis;

    void fillTimeSurfaceProxy(int trialNumber);
    void fillChannelChannelProxy(int trialNumber);

    readMatSpikeData *readData;

    const double DEFAULT_TIME_CONSTANT = 0.1;
    const int DEFAULT_TRIAL_NUMBER = 1;
    const int DEFAULT_NUM_TIMES = 100;

    double timeConstant = DEFAULT_TIME_CONSTANT;
    int trialNumber = DEFAULT_TRIAL_NUMBER;
    int numTimes = DEFAULT_NUM_TIMES;

    int channelSelected = 1;

    double differences[36][36];

    double sortedChannels[36];

    void setDifferences(int trialNumber);
};

#endif // TIMESURFACEBAR_H
