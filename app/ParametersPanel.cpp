#include "ParametersPanel.h"
#include "ui_ParametersPanel.h"

#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>

ParametersPanel::ParametersPanel(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ParametersPanel)
{
    ui->setupUi(this);

    // 连接 Run 按钮
    connect(ui->runButton, &QPushButton::clicked,
            this, &ParametersPanel::onRunButtonClicked);
}

ParametersPanel::~ParametersPanel()
{
    delete ui;
}

SimulationConfig ParametersPanel::currentConfig() const
{
    SimulationConfig cfg;

    cfg.nx = ui->nxSpinBox->value();
    cfg.ny = ui->nySpinBox->value();
    cfg.timeSteps = ui->timeStepsSpinBox->value();
    cfg.diffusionCoeff = ui->diffusionSpinBox->value();

    return cfg;
}

void ParametersPanel::onRunButtonClicked()
{
    emit runRequested(currentConfig());
}
