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
    // 追加一条日志到日志窗口
    void appendMessage(const QString& text);

    // 清空日志
    void clear();

private:
    Ui::LogPanel* ui = nullptr;
};

#endif // LOGPANEL_H
