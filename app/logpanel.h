#ifndef LOGPANEL_H
#define LOGPANEL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LogPanel; }
QT_END_NAMESPACE

class QString;

class LogPanel : public QWidget
{
    Q_OBJECT
public:
    explicit LogPanel(QWidget *parent = nullptr);
    ~LogPanel() override;

public slots:
    // Append a log entry to the log window
    void appendMessage(const QString& text);

    // Clear logs
    void clear();

private:
    std::unique_ptr<Ui::LogPanel> ui;
};

#endif // LOGPANEL_H
