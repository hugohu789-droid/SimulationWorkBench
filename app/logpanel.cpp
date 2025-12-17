#include "logpanel.h"
#include "ui_LogPanel.h"

#include <QPlainTextEdit>
#include <QTextCursor>

LogPanel::LogPanel(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LogPanel)
{
    ui->setupUi(this);
}

LogPanel::~LogPanel()
{
    delete ui;
}

void LogPanel::appendMessage(const QString& text)
{
    if (text.isEmpty()) {
        return;
    }

    ui->logTextEdit->appendPlainText(text);

    // 自动滚动到末尾，符合“日志面板”的使用习惯
    QTextCursor cursor = ui->logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->logTextEdit->setTextCursor(cursor);
}

void LogPanel::clear()
{
    ui->logTextEdit->clear();
}
