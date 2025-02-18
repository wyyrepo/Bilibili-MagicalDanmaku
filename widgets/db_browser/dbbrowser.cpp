#include <QTextBlock>
#include "dbbrowser.h"
#include "ui_dbbrowser.h"
#include "fileutil.h"

DBBrowser::DBBrowser(SqlService *service, QSettings* settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DBBrowser),
    service(service),
    settings(settings)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    setAttribute(Qt::WA_DeleteOnClose, true);
    ui->splitter->restoreState(settings->value("sql/view_splitter_state").toByteArray());
    ui->splitter->restoreGeometry(settings->value("sql/view_splitter_geometry").toByteArray());

    QString codes = settings->value("sql/codes").toString();
    if (codes.isEmpty())
    {
        // 设置一些示例
        codes = readTextFile(":/documents/sql");
    }
    ui->queryEdit->setPlainText(codes);
}

DBBrowser::~DBBrowser()
{
    // 保存代码
    QString full = ui->queryEdit->toPlainText();
    settings->setValue("sql/codes", full);

    // 保存状态
    settings->setValue("sql/view_splitter_state", ui->splitter->saveState());
    settings->setValue("sql/view_splitter_geometry", ui->splitter->saveGeometry());
    delete ui;
}

void DBBrowser::showQueryResult(QString sql)
{
    sql = sql.trimmed();
    if (sql.trimmed().isEmpty())
        return ;
    qInfo() << "SQL查询：" << sql;

    if (model)
        delete model;
    model = new QSqlQueryModel(this);
    QSqlQuery query(sql);
    model->setQuery(query);
    ui->resultTable->setModel(model);
    ui->resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    if (query.lastError().type() != QSqlError::NoError)
    {
        qWarning() << query.lastError().text();
        QMessageBox::critical(this, "查询出错", query.lastError().text());
    }
}

void DBBrowser::on_execButton_clicked()
{
    QString full = ui->queryEdit->toPlainText();
    settings->setValue("sql/codes", full);

    QString code;
    QTextCursor cursor = ui->queryEdit->textCursor();
    if (cursor.hasSelection()) // 选中代码
    {
        code = cursor.selectedText();
    }
    else // 当前行
    {
        int pos = cursor.position();
        int left = full.lastIndexOf("\n", pos - 1) + 1;
        int right = full.indexOf("\n", pos);
        if (right == -1)
            right = full.length();
        code = full.mid(left, right - left);
    }
    emit signalProcessVariant(code);

    showQueryResult(code);
}

void DBBrowser::on_resultTable_customContextMenuRequested(const QPoint &pos)
{

}
