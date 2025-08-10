#include "WaveformWidget.h"
#include <QFileDialog>


WaveformWidget::WaveformWidget(const QString &title, QWidget *parent)
    : QDockWidget{title, parent},
    m_waveformEnabled(false),
    m_sampleChannels(0),
    m_sampleInterval(8)
{
    initDockWidget();
}

void WaveformWidget::initDockWidget()
{
    QWidget *mainWidget = new QWidget();

    btnGrpWaveChEn = new QButtonGroup(mainWidget);
    QVBoxLayout *chEnLayout = new QVBoxLayout();

    QLabel *intervalPrompt = new QLabel(tr("Sample Interval"), mainWidget);
    intervalPrompt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    comboSampleInterval = new QComboBox(mainWidget);
    comboSampleInterval->clear();
    comboSampleInterval->addItem("/1", 1);
    comboSampleInterval->addItem("/2", 2);
    comboSampleInterval->addItem("/4", 4);
    comboSampleInterval->addItem("/8", 8);
    comboSampleInterval->addItem("/16", 16);
    comboSampleInterval->addItem("/32", 32);
    comboSampleInterval->addItem("/64", 64);
    comboSampleInterval->setCurrentIndex(3);
    chEnLayout->addWidget(intervalPrompt);
    chEnLayout->addWidget(comboSampleInterval);


    for (int ch = CHANNEL_START; ch < CHANNEL_NUM; ch++) {
        QCheckBox *cBoxCh = new QCheckBox(tr("Channel ") + QString::number(ch), mainWidget);
        btnGrpWaveChEn->addButton(cBoxCh, ch);
        chEnLayout->addWidget(cBoxCh);
    }
    btnGrpWaveChEn->setExclusive(false);

    waveView = new WaveformView(this);
    QHBoxLayout *waveLayout = new QHBoxLayout();
    waveLayout->addLayout(chEnLayout);
    waveLayout->addWidget(waveView);


    btnRunWave = new QPushButton(tr("Run"), mainWidget);
    btnAutoScale = new QPushButton(tr("Auto Scale"), mainWidget);
    btnDragSelectOrMove = new QPushButton(tr("Drag Move"), mainWidget);
    btnAddThresholdLine = new QPushButton(tr("Threshold Line"), mainWidget);
    btnAddThresholdLine->setCheckable(true);
    spinThreshold = new QSpinBox(mainWidget);
    spinThreshold->setMaximum(100000);
    spinThreshold->setMinimum(-100000);
    spinThreshold->setValue(0);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnRunWave);
    btnLayout->addWidget(btnAutoScale);
    btnLayout->addWidget(btnDragSelectOrMove);
    btnLayout->addWidget(btnAddThresholdLine);
    btnLayout->addWidget(spinThreshold);


    btnSaveData = new QPushButton(tr("Save Data"), mainWidget);
    btnGrabImage = new QPushButton(tr("Grab Image"), mainWidget);
    editSavePath = new QLineEdit(m_waveformSavePath, mainWidget);
    btnChangeAxis = new QPushButton(tr("Voltage Mode"), mainWidget);
    btnSetDataPath = new QPushButton(tr("Set Save Path"), mainWidget);

    QHBoxLayout *btnLayout2 = new QHBoxLayout();
    btnLayout2->addWidget(btnChangeAxis);
    btnLayout2->addWidget(btnSaveData);
    btnLayout2->addWidget(btnGrabImage);
    btnLayout2->addWidget(btnSetDataPath);
    btnLayout2->addWidget(editSavePath);


    QVBoxLayout *layout = new QVBoxLayout(mainWidget);
    layout->addLayout(btnLayout);
    layout->addLayout(btnLayout2);
    layout->addLayout(waveLayout);

    mainWidget->setLayout(layout);
    setWidget(mainWidget);

    connect(btnGrpWaveChEn, &QButtonGroup::idToggled, this, [this](int id, bool checked){
        if (checked) {
            this->waveView->enableChannel(static_cast<SAMPLE_CHANNEL>(id));
            m_sampleChannels |= (0x01 << id);
        } else {
            this->waveView->disableChannel(static_cast<SAMPLE_CHANNEL>(id));
            m_sampleChannels &= (~(0x01 << id));
        }

        emit waveformStateChanged(m_waveformEnabled, m_sampleChannels, m_sampleInterval);
    });

    connect(comboSampleInterval, &QComboBox::currentIndexChanged, this, [this](int index){
        m_sampleInterval = comboSampleInterval->currentData().toInt();
        emit waveformStateChanged(m_waveformEnabled, m_sampleChannels, m_sampleInterval);
    });

    connect(btnRunWave, &QPushButton::clicked, this, [this](){
        if (btnRunWave->text() == tr("Run")) {
            m_waveformEnabled = true;
            waveView->startGraph();
            btnRunWave->setText(tr("Stop"));
        } else {
            m_waveformEnabled = false;
            waveView->pauseGraph();
            btnRunWave->setText(tr("Run"));
        }
        emit waveformStateChanged(m_waveformEnabled, m_sampleChannels, m_sampleInterval);
    });
    connect(btnAutoScale, &QPushButton::clicked, this, [this](){
        waveView->resetRange();
    });
    connect(btnDragSelectOrMove, &QPushButton::clicked, this, [this](){
        if (btnDragSelectOrMove->text() == tr("Drag Move")) {
            waveView->changeDragFunc(WaveformView::DRAG_FUNC_SELC);
            btnDragSelectOrMove->setText(tr("Drag Select"));
        } else {
            waveView->changeDragFunc(WaveformView::DRAG_FUNC_MOVE);
            btnDragSelectOrMove->setText(tr("Drag Move"));
        }
    });

    connect(btnAddThresholdLine, &QPushButton::clicked, this, &WaveformWidget::onAddThresholdLine);
    connect(spinThreshold, &QSpinBox::valueChanged, this, [this](int val){
        waveView->setThresholdLineValue(val);
    });


    connect(btnChangeAxis, &QPushButton::clicked, this, [this](){
        if (btnChangeAxis->text() == tr("Voltage Mode")) {
            waveView->setDisplayMode(true);
            btnChangeAxis->setText(tr("AD Mode"));
        } else {
            waveView->setDisplayMode(false);
            btnChangeAxis->setText(tr("Voltage Mode"));
        }
    });

    connect(btnSaveData, &QPushButton::clicked, this, [this](){
        QString fileName = this->m_waveformSavePath + QString("/waveform_origin_%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss"));
        qDebug() << "Save waveform data file: " << fileName;
        waveView->saveWaveformData(fileName);
    });

    connect(btnGrabImage, &QPushButton::clicked, this, [this](){
        QString fileName = this->m_waveformSavePath + QString("/waveform_origin_%1.png").arg(QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss"));
        qDebug() << "Save waveform image: " << fileName;
        waveView->snapAPicture(fileName);
    });

    connect(btnSetDataPath, &QPushButton::clicked, this, [this]() {
        QString savePath = QFileDialog::getExistingDirectory(this, tr("Select waveform origin data save path"), "D:\\", QFileDialog::ShowDirsOnly);
        if (!savePath.isEmpty()) {
            m_waveformSavePath = savePath;
            editSavePath->setText(m_waveformSavePath);
        }
    });


}

void WaveformWidget::onAddThresholdLine(bool checked)
{
    waveView->enableThresholdLIne(checked);
    if (checked) {
        spinThreshold->setEnabled(true);
        waveView->setThresholdLineValue(spinThreshold->value());
    } else {
        spinThreshold->setEnabled(false);
    }
}

void WaveformWidget::onReceivedWaveform(const QList<int> &data)
{
    waveView->addSeriesData(data);
}


