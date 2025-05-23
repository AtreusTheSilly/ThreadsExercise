#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myThreadsControl.h"

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QtDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// запускает таймер и потоки, выполняется при нажатии на UI-кнопку
    void on_pushButton_clicked();
    /// выводит данные о номере потока и времени, выполняется при их получении
    void onReceiveTimeInfo(int threadNumber, QTime currentTime);
    /// закрывает потоки, выполняется при срабатывании основного таймера
    void onTimeExpired();
    /// очищает ресурсы, закрывает потоки
    void cleanupThreads();

private:
    Ui::MainWindow *ui;
    MyThreadsControl *_thread1;
    MyThreadsControl *_thread2;
    QTimer *_timer;
    /// флаг, запущены ли потоки
    bool _threadsRunning;
};
#endif // MAINWINDOW_H
