#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendEvent_clicked()
{
    ui->dump->appendPlainText(QString("Open event: %1").arg(ui->eventName->text()));
    HANDLE hEvent = OpenEventA(
                EVENT_MODIFY_STATE,
                FALSE,
                ui->eventName->text().toLatin1()
                );
    if (hEvent == NULL)
    {
        DWORD lastError = GetLastError();
        char* errorText = NULL;
        FormatMessageA(
           // use system message tables to retrieve error text
           FORMAT_MESSAGE_FROM_SYSTEM
           // allocate buffer on local heap for error text
           |FORMAT_MESSAGE_ALLOCATE_BUFFER
           // Important! will fail otherwise, since we're not
           // (and CANNOT) pass insertion parameters
           |FORMAT_MESSAGE_IGNORE_INSERTS,
           NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
           lastError,
           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
           (char*)&errorText,  // output
           0, // minimum size for output buffer
           NULL);   // arguments - see note

        if ( NULL != errorText )
        {
           ui->dump->appendPlainText(QString("Cannot open Windows event: %1").arg(QString::fromLatin1(errorText)));
           // release memory allocated by FormatMessage()
           LocalFree(errorText);
           errorText = NULL;
        }
    }
    else
    {
        SetEvent(hEvent);
        ui->dump->appendPlainText("Event set");
        CloseHandle(hEvent);
    }
}
