#ifndef COMBOBOXMODIFIER_H
#define COMBOBOXMODIFIER_H

#include "datamodifier.h"
#include <QObject>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

class ComboBoxModifier : public QObject
{
    Q_OBJECT
public:
    explicit ComboBoxModifier(QGroupBox *newBox, DataModifier *newModifier);
    //~ComboBoxModifier();

    void setBox1();
    void setBox2();
    void setBox3();
    void setBox4();
    void render();

public Q_SLOTS:
//    void changeComboBox(int number);
    void updateTrialLabel();
    void updateChannelLabel();

    void updateAutoRender();
    void updateComboBox(int x);

private:
    QGroupBox *box;
    QVBoxLayout *boxLayout;
    QScrollBar *channelSelect;
    QScrollBar *trialSelect;
    int totalChannels = 35;
    int currentBox;
    DataModifier *modifier;
    QHBoxLayout *trialInfo;
    QHBoxLayout *channelInfo;
    QLabel *trialLabel;
    QLabel *channelLabel;
    QLabel *labelLabel;
    QLabel *graphDescriptionLabel;
    QWidget *trialInfoWidget;
    QWidget *channelInfoWidget;

    bool autoRender = true;




};

#endif // COMBOBOXMODIFIER_H
