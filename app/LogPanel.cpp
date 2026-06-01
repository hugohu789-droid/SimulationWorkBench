#include "logpanel.h"
#include "ui_LogPanel.h"

#include <QPlainTextEdit>
#include <QTextCursor>

LogPanel::LogPanel(QWidget* parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::LogPanel>())
{
    ui->setupUi(this);
}

LogPanel::~LogPanel() = default;

void LogPanel::appendMessage(const QString& text)
{
    if (text.isEmpty()) {
        return;
    }

    ui->logTextEdit->appendPlainText(text);

    // Automatically scrolling to the end aligns with the usage habits of the "log panel".
    QTextCursor cursor = ui->logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->logTextEdit->setTextCursor(cursor);
}

void LogPanel::clear()
{
    ui->logTextEdit->clear();
}
