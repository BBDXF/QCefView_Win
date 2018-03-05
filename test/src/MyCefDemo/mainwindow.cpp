#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // add navi
    m_pEditUrl = new QLineEdit("http://www.hao123.com",this);
//    m_pEditUrl->setBaseSize(120,24);
    ui->mainToolBar->addWidget(m_pEditUrl);

    m_pBtnGo = new QPushButton("Go",this);
    ui->mainToolBar->addWidget(m_pBtnGo);
    connect(m_pBtnGo, SIGNAL(clicked(bool)), this, SLOT(slot_btn_go()));
    // cef view
    QDir dir = QCoreApplication::applicationDirPath();
    QString uri = QDir::toNativeSeparators(dir.filePath("web\\QCefViewTestPage.html"));
    m_pEditUrl->setText(uri);
    m_pCefView = new MyCefView(uri,this);
    ui->centralLayout->addWidget(m_pCefView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_btn_go()
{
    QString strUrl = m_pEditUrl->text();
    QUrl urlCheck(strUrl);
    if(urlCheck.isValid()){
        m_pCefView->navigateToUrl(strUrl);
    }else{
        qDebug()<<strUrl<<"is unvalid!";
    }
}

void MainWindow::on_actionTest_triggered()
{
    qsrand(::GetTickCount());
    QColor color(qrand());

    QCefEvent event("colorChangedEvent");
    event.setStringProperty("color", color.name());
    m_pCefView->broadcastEvent("colorChange", event);
}
