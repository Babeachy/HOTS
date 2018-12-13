#include "timesurfacebar.h"
#include <QApplication>

#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QScrollBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Q3DBars *graph = new Q3DBars();
    QWidget *container = QWidget::createWindowContainer(graph);



    if (!graph->hasContext()){
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);//displays the graph
    hLayout->addLayout(vLayout);//displays the VBOXlayout that has all of our buttons on it
    vLayout->setAlignment(Qt::AlignTop);

    widget->setWindowTitle("Time Surface");



    TimeSurfaceBar *modifier = new TimeSurfaceBar(graph);

    QComboBox *selectionModeList = new QComboBox(widget);
    selectionModeList->addItem(QStringLiteral("None"),
                               int(QAbstract3DGraph::SelectionNone));
    selectionModeList->addItem(QStringLiteral("Bar"),
                               int(QAbstract3DGraph::SelectionItem));
    selectionModeList->addItem(QStringLiteral("Row"),
                               int(QAbstract3DGraph::SelectionRow));
    selectionModeList->addItem(QStringLiteral("Bar and Row"),
                               int(QAbstract3DGraph::SelectionItemAndRow));
    selectionModeList->addItem(QStringLiteral("Column"),
                               int(QAbstract3DGraph::SelectionColumn));
    selectionModeList->addItem(QStringLiteral("Bar and Column"),
                               int(QAbstract3DGraph::SelectionItemAndColumn));
    selectionModeList->addItem(QStringLiteral("Row and Column"),
                               int(QAbstract3DGraph::SelectionRowAndColumn));
    selectionModeList->addItem(QStringLiteral("Bar, Row and Column"),
                               int(QAbstract3DGraph::SelectionItemRowAndColumn));
    selectionModeList->addItem(QStringLiteral("Slice into Row"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionRow));
    selectionModeList->addItem(QStringLiteral("Slice into Row and Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndRow));
    selectionModeList->addItem(QStringLiteral("Slice into Column"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionColumn));
    selectionModeList->addItem(QStringLiteral("Slice into Column and Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndColumn));
    selectionModeList->addItem(QStringLiteral("Multi: Bar, Row, Col"),
                               int(QAbstract3DGraph::SelectionItemRowAndColumn
                                   | QAbstract3DGraph::SelectionMultiSeries));
    selectionModeList->addItem(QStringLiteral("Multi, Slice: Row, Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndRow
                                   | QAbstract3DGraph::SelectionMultiSeries));
    selectionModeList->addItem(QStringLiteral("Multi, Slice: Col, Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndColumn
                                   | QAbstract3DGraph::SelectionMultiSeries));
    selectionModeList->setCurrentIndex(1);

    //increase amount of times(decrease interval)
    QHBoxLayout *numTimesLayout = new QHBoxLayout();
    QSpinBox *numTimesBox = new QSpinBox();
    numTimesBox->setRange(1, 10000000000);
    numTimesBox->setValue(100);
    QPushButton *numTimesButton = new QPushButton(QStringLiteral("Reset"), nullptr);
    numTimesLayout->addWidget(numTimesBox, 1);
    numTimesLayout->addWidget(numTimesButton);

    //change time constant
    QHBoxLayout *timeConstantLayout = new QHBoxLayout();
    QDoubleSpinBox *timeConstantBox = new QDoubleSpinBox();
    numTimesBox->setValue(1);
    QPushButton *timeConstantButton = new QPushButton(QStringLiteral("Reset"), nullptr);
    numTimesLayout->addWidget(timeConstantBox, 1);
    numTimesLayout->addWidget(timeConstantButton);

    QHBoxLayout *trialNumberLayout = new QHBoxLayout();
    QScrollBar *trialNumberScroll = new QScrollBar(Qt::Orientation::Horizontal, nullptr);
    trialNumberScroll->setRange(1, 303);
    trialNumberScroll->setValue(1);
    QLabel *trialNumberLabel = new QLabel(QString::number(1));
    trialNumberLayout->addWidget(trialNumberScroll, 1);
    trialNumberLayout->addWidget(trialNumberLabel);

    //For selecting a channel, and a button to set the selection mode
    QHBoxLayout *channelSelectLayout = new QHBoxLayout();
    QScrollBar *channelSelectScroll = new QScrollBar(Qt::Orientation::Horizontal, nullptr);
    trialNumberScroll->setRange(1, 35);
    trialNumberScroll->setValue(1);
    QLabel *channelNumberLabel = new QLabel(QString::number(1));
    QPushButton *channelSelectButton = new QPushButton(QStringLiteral("Slice"));
    channelSelectLayout->addWidget(channelSelectScroll, 1);
    channelSelectLayout->addWidget(channelNumberLabel);
    channelSelectLayout->addWidget(channelSelectButton);


    QPushButton *renderButton = new QPushButton(QStringLiteral("Render"));


    vLayout->addWidget(new QLabel(QStringLiteral("Selection Mode")));
    vLayout->addWidget(selectionModeList);
    vLayout->addWidget(new QLabel(QStringLiteral("Number of times")));
    vLayout->addLayout(numTimesLayout);
    vLayout->addWidget(new QLabel(QStringLiteral("Time Constant")));
    vLayout->addLayout(timeConstantLayout);
    vLayout->addWidget(new QLabel("Trial Number"));
    vLayout->addLayout(trialNumberLayout);
    vLayout->addWidget(new QLabel("Slice at Channel"));
    vLayout->addLayout(channelSelectLayout);
    vLayout->addWidget(renderButton);

    QObject::connect(selectionModeList, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeSelectionMode(int)));

    QObject::connect(numTimesBox, SIGNAL(valueChanged(int)), modifier, SLOT(numTimesChanged(int)));
    QObject::connect(numTimesButton, SIGNAL(released()), modifier, SLOT(numTimesReset()));

    QObject::connect(timeConstantBox, SIGNAL(valueChanged(double)), modifier, SLOT(timeConstantChanged(double)));
    QObject::connect(timeConstantButton, SIGNAL(released()), modifier, SLOT(timeConstantReset()));

    QObject::connect(trialNumberScroll, SIGNAL(valueChanged(int)), modifier, SLOT(trialNumberChanged(int)));
    QObject::connect(trialNumberScroll, SIGNAL(valueChanged(int)), trialNumberLabel, SLOT(setNum(int)));


    QObject::connect(channelSelectScroll, SIGNAL(valueChanged(int)), modifier, SLOT(channelNumberChanged(int)));
    QObject::connect(channelSelectScroll, SIGNAL(valueChanged(int)), channelNumberLabel, SLOT(setNum(int)));
    QObject::connect(channelSelectButton, SIGNAL(released()), modifier, SLOT(slice()));

    QObject::connect(renderButton, SIGNAL(released()), modifier, SLOT(render()));

    widget->show();

    return a.exec();
}
