#ifndef DATAMODIFIER_H
#define DATAMODIFIER_H

#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtGui/QFont>

#include <readMatSpikeData.h>

using namespace QtDataVisualization;



class DataModifier : public QObject
{
    Q_OBJECT
public:
    explicit DataModifier(QtDataVisualization::Q3DScatter *scatter);
 //   ~DataModifier();

    void setGraph1(int trialNumber, int channelNumber);
    void setGraph2(int trialNumber);
    void setGraph3(int trialNumber);
    void setGraph4(int trialNumber, int channelNumber);

    void changeStyle();
    void changePresetCamera();
    void changeLabelStyle();
    void changeFontSize(int fontsize);
    void setBackgroundEnabled(int enabled);
    void setGridEnabled(int enabled);
    void setSmoothDots(int smooth);
    void start();

    int getNumTrials();
    int getTrialLabel(int trialNumber);

public Q_SLOTS:
    void changeStyle(int style);
    void changeTheme(int theme);
    void changeShadowQuality(int quality);
    void shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality shadowQuality);

Q_SIGNALS:
    void triggerDebug();
    void backgroundEnabledChanged(bool enabled);
    void gridEnabledChanged(bool enabled);
    void shadowQualityChanged(int quality);
    void fontChanged(QFont font);

private:
    QVector3D randVector();
    Q3DScatter *m_graph;
    int m_fontSize;
    QAbstract3DSeries::Mesh m_style;
    bool m_smooth;
    int m_itemCount;
    float m_curveDivider;


    int trialNumber = 0;
    int channelNumber = 1;
    readMatSpikeData *readData;

    bool useZAxis = true;
};

#endif // DATAMODIFIER_H
