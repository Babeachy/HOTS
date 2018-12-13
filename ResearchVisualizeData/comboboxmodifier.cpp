#include "comboboxmodifier.h"
#include "datamodifier.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollBar>
#include <string>

#include <iostream>

ComboBoxModifier::ComboBoxModifier(QGroupBox *newBox, DataModifier *newModifier)
{
    box = newBox;
    modifier = newModifier;
    //Set up things in memory and just display them when called
    //Box 1

    trialInfoWidget = new QWidget();
    trialSelect = new QScrollBar(Qt::Horizontal);
    trialSelect->setMinimum(1);
    trialSelect->setMaximum(modifier->getNumTrials());
    trialLabel = new QLabel(QString::number(1));
    labelLabel = new QLabel(QString::number(modifier->getTrialLabel(0)));
    trialInfo = new QHBoxLayout(trialInfoWidget);
    trialInfo->addWidget(new QLabel(QStringLiteral("Trial: ")));
    trialInfo->addWidget(trialLabel);
    trialInfo->addWidget(new QLabel(QStringLiteral("Label: ")));
    trialInfo->addWidget(labelLabel);
    trialInfo->addWidget(trialSelect);

    channelInfoWidget = new QWidget();
    channelSelect = new QScrollBar(Qt::Horizontal);
    channelSelect->setMinimum(1);
    channelSelect->setMaximum(totalChannels);
    channelLabel = new QLabel(QString::number(1));
    channelInfo = new QHBoxLayout(channelInfoWidget);
    channelInfo->addWidget(new QLabel(QStringLiteral("Channel: ")));
    channelInfo->addWidget(channelLabel);
    channelInfo->addWidget(channelSelect);



    boxLayout = new QVBoxLayout();
    graphDescriptionLabel = new QLabel();
    boxLayout->addWidget(graphDescriptionLabel);
    boxLayout->addWidget(trialInfoWidget);
    boxLayout->addWidget(channelInfoWidget);

    box->setLayout(boxLayout);
    setBox1();

    std::cout << "got to this point" << std::endl;
    connect(trialSelect, SIGNAL(valueChanged(int)), this, SLOT(updateTrialLabel()));
    connect(channelSelect, SIGNAL(valueChanged(int)), this, SLOT(updateChannelLabel()));
    std::cout << "got to this point later" << std::endl;
}

void ComboBoxModifier::setBox1()
{
    currentBox = 1;
    box->setTitle(QStringLiteral("Graph 1"));
    graphDescriptionLabel->setText(QStringLiteral("Graphs the xyz positions at which spikes occur on the specified channels."));
    boxLayout->addWidget(channelInfoWidget);

}

void ComboBoxModifier::setBox2()
{
    currentBox = 2;
    box->setTitle(QStringLiteral("Graph 2"));
    graphDescriptionLabel->setText(QStringLiteral("Graphs xyz positions for all channels."));
    boxLayout->removeWidget(channelInfoWidget);
}

void ComboBoxModifier::setBox3()
{
    currentBox = 3;
    box->setTitle(QStringLiteral("Graph 3"));
    graphDescriptionLabel->setText(QStringLiteral("Graphs xy positions over time axis for all channels."));
    boxLayout->removeWidget(channelInfoWidget);
}

void ComboBoxModifier::setBox4()
{
    currentBox = 4;
    box->setTitle(QStringLiteral("Graph 4"));
    graphDescriptionLabel->setText(QStringLiteral("Graphs the xy positions over time at which spikes occur on the specified channels."));
    boxLayout->addWidget(channelInfoWidget);

}


void ComboBoxModifier::render()
{
    //send information from combobox to datamodifier to change chart information
    if (currentBox == 1)
    {
        modifier->setGraph1(trialSelect->value()-1, channelSelect->value());
    }
    else if (currentBox == 2)
    {
        modifier->setGraph2(trialSelect->value()-1);
    }
    else if (currentBox == 3)
    {
        modifier->setGraph3(trialSelect->value()-1);
    }
    else if (currentBox == 4)
    {
        modifier->setGraph4(trialSelect->value()-1, channelSelect->value());
    }
}


void ComboBoxModifier::updateTrialLabel()
{
    trialLabel->setText(QString::number(trialSelect->value()));
    labelLabel->setText(QString::number(modifier->getTrialLabel(trialSelect->value()-1)));

    if (autoRender) render();
}

void ComboBoxModifier::updateChannelLabel()
{
    channelLabel->setText(QString::number(channelSelect->value()));

    if (autoRender) render();
}

void ComboBoxModifier::updateAutoRender()
{
    if (autoRender) autoRender = false;
    else autoRender = true;
}

void ComboBoxModifier::updateComboBox(int x)
{
    if (x == 0)
    {
        setBox1();
    }
    else if (x == 1)
    {
        setBox2();
    }
    else if (x == 2)
    {
        setBox3();
    }
    else if (x == 3)
    {
        setBox4();
    }

    if (autoRender) render();
}
