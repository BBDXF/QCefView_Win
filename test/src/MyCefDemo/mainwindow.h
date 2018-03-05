#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include "mycefview.h"
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slot_btn_go();
private slots:
    void on_actionTest_triggered();

private:
    Ui::MainWindow *ui;
    MyCefView* m_pCefView;
    QLineEdit* m_pEditUrl;
    QPushButton* m_pBtnGo;

};

#endif // MAINWINDOW_H
