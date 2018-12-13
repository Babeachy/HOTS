#include "mainwindow.h"
#include <QApplication>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>

#include "datamodifier.h"
#include "comboboxmodifier.h"

using namespace QtDataVisualization;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q3DScatter *graph = new Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(graph);

    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    widget->setWindowTitle(QStringLiteral("A Cosine Wave"));

    QComboBox *themeList = new QComboBox(widget);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));
    themeList->setCurrentIndex(6);

    QPushButton *labelButton = new QPushButton(widget);
    labelButton->setText(QStringLiteral("Change label style"));

    QCheckBox *smoothCheckBox = new QCheckBox(widget);
    smoothCheckBox->setText(QStringLiteral("Smooth dots"));
    smoothCheckBox->setChecked(true);

    QComboBox *itemStyleList = new QComboBox(widget);
    itemStyleList->addItem(QStringLiteral("Sphere"), int(QAbstract3DSeries::MeshSphere));
    itemStyleList->addItem(QStringLiteral("Cube"), int(QAbstract3DSeries::MeshCube));
    itemStyleList->addItem(QStringLiteral("Minimal"), int(QAbstract3DSeries::MeshMinimal));
    itemStyleList->addItem(QStringLiteral("Point"), int(QAbstract3DSeries::MeshPoint));
    itemStyleList->setCurrentIndex(0);

    QPushButton *cameraButton = new QPushButton(widget);
    cameraButton->setText(QStringLiteral("Change camera preset"));

    QPushButton *itemCountButton = new QPushButton(widget);
    itemCountButton->setText(QStringLiteral("Toggle item count"));

    QCheckBox *backgroundCheckBox = new QCheckBox(widget);
    backgroundCheckBox->setText(QStringLiteral("Show background"));
    backgroundCheckBox->setChecked(true);

    QCheckBox *gridCheckBox = new QCheckBox(widget);
    gridCheckBox->setText(QStringLiteral("Show grid"));
    gridCheckBox->setChecked(true);

    QComboBox *shadowQuality = new QComboBox(widget);
    shadowQuality->addItem(QStringLiteral("None"));
    shadowQuality->addItem(QStringLiteral("Low"));
    shadowQuality->addItem(QStringLiteral("Medium"));
    shadowQuality->addItem(QStringLiteral("High"));
    shadowQuality->addItem(QStringLiteral("Low Soft"));
    shadowQuality->addItem(QStringLiteral("Medium Soft"));
    shadowQuality->addItem(QStringLiteral("High Soft"));
    shadowQuality->setCurrentIndex(4);

    QFontComboBox *fontList = new QFontComboBox(widget);
    fontList->setCurrentFont(QFont("Arial"));

    //My Code
    QComboBox *graphType = new QComboBox(widget);
    graphType->addItem(QStringLiteral("1"));
    graphType->addItem(QStringLiteral("2"));
    graphType->addItem(QStringLiteral("3"));
    graphType->addItem(QStringLiteral("4"));
    graphType->setCurrentIndex(0);

    QGroupBox *graphInfo = new QGroupBox(QStringLiteral("Graph 1"), widget);

    QWidget *renderWidget = new QWidget();
    QHBoxLayout *renderSection = new QHBoxLayout(renderWidget);

    QCheckBox *autoRenderCheckBox = new QCheckBox();
    autoRenderCheckBox->setText(QStringLiteral("Auto-render"));
    autoRenderCheckBox->setChecked(true);
    QPushButton *renderButton = new QPushButton(widget);
    renderButton->setText(QStringLiteral("Render"));

    renderSection->addWidget(autoRenderCheckBox);
    renderSection->addWidget(renderButton);

    QMenuBar *menuBar = new QMenuBar();

    vLayout->addWidget(menuBar);
    vLayout->addWidget(labelButton, 0, Qt::AlignTop);
    vLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    vLayout->addWidget(itemCountButton, 0, Qt::AlignTop);
    vLayout->addWidget(backgroundCheckBox);
    vLayout->addWidget(gridCheckBox);
    vLayout->addWidget(smoothCheckBox, 0, Qt::AlignTop);
    vLayout->addWidget(new QLabel(QStringLiteral("Change dot style")));
    vLayout->addWidget(itemStyleList);
    vLayout->addWidget(new QLabel(QStringLiteral("Change theme")));
    vLayout->addWidget(themeList);
    vLayout->addWidget(new QLabel(QStringLiteral("Adjust shadow quality")));
    vLayout->addWidget(shadowQuality);
    vLayout->addWidget(new QLabel(QStringLiteral("Change font")));
    vLayout->addWidget(fontList, 1, Qt::AlignTop);
    vLayout->addWidget(graphType, 1, Qt::AlignTop);
    vLayout->addWidget(graphInfo, 1, Qt::AlignTop);
    vLayout->addWidget(renderWidget, 1, Qt::AlignTop);

//    QLineEdit *debug = new QLineEdit();
//    vLayout->addWidget(debug, 0, Qt::AlignTop);

    DataModifier *modifier = new DataModifier(graph);
    ComboBoxModifier *comboBoxModifier = new ComboBoxModifier(graphInfo, modifier);

    QObject::connect(graphType, SIGNAL(currentIndexChanged(int)), comboBoxModifier, SLOT(updateComboBox(int)));

    QObject::connect(autoRenderCheckBox, &QCheckBox::stateChanged, comboBoxModifier, &ComboBoxModifier::updateAutoRender);

    QObject::connect(renderButton, &QPushButton::clicked, comboBoxModifier, &ComboBoxModifier::render);

    QObject::connect(cameraButton, &QPushButton::clicked, modifier,
                     &DataModifier::changePresetCamera);
    QObject::connect(labelButton, &QPushButton::clicked, modifier,
                     &DataModifier::changeLabelStyle);

    QObject::connect(backgroundCheckBox, &QCheckBox::stateChanged, modifier,
                     &DataModifier::setBackgroundEnabled);
    QObject::connect(gridCheckBox, &QCheckBox::stateChanged, modifier,
                     &DataModifier::setGridEnabled);
    QObject::connect(smoothCheckBox, &QCheckBox::stateChanged, modifier,
                     &DataModifier::setSmoothDots);

    QObject::connect(modifier, &DataModifier::backgroundEnabledChanged,
                     backgroundCheckBox, &QCheckBox::setChecked);
    QObject::connect(modifier, &DataModifier::gridEnabledChanged,
                     gridCheckBox, &QCheckBox::setChecked);
    QObject::connect(itemStyleList, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeStyle(int)));

    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeTheme(int)));

    QObject::connect(shadowQuality, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeShadowQuality(int)));

  //  QObject::connect(graphType, SIGNAL(currentIndexChanged(int)), this, SLOT(changeGroupBox(int)));

    QObject::connect(modifier, &DataModifier::shadowQualityChanged, shadowQuality,
                     &QComboBox::setCurrentIndex);
    QObject::connect(graph, &Q3DScatter::shadowQualityChanged, modifier,
                     &DataModifier::shadowQualityUpdatedByVisual);



    QObject::connect(modifier, &DataModifier::fontChanged, fontList,
                     &QFontComboBox::setCurrentFont);



    widget->show();
    return app.exec();
}
