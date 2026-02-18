#ifndef PARAMETERSPANEL_H
#define PARAMETERSPANEL_H

#include <QWidget>

#include "../core/simulationconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ParametersPanel; }
QT_END_NAMESPACE

class ParametersPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ParametersPanel(QWidget *parent = nullptr);
    ~ParametersPanel() override;

    SimulationConfig currentConfig() const;

signals:
    void runRequested(const SimulationConfig& config);

private slots:
    void onRunButtonClicked();

private:
    std::unique_ptr<Ui::ParametersPanel> ui;
};

#endif // PARAMETERSPANEL_H
