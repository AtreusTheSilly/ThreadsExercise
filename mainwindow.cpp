#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#define MAIN_TIMER_MS    120000
#define FIRST_THREAD_TIMER_MS    10000
#define SECOND_THREAD_TIMER_MS    15000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _thread1(nullptr),
      _thread2(nullptr),
      _timer(new QTimer(this)),
      _threadsRunning(false)
{
    ui->setupUi(this);

    // соединение нажатия на кнопку со слотом
    connect(ui->pushButton,
            &QPushButton::clicked,
            this,
            &MainWindow::on_pushButton_clicked);

   // настройка таймера основного потока
    _timer->setInterval(MAIN_TIMER_MS);
    _timer->setSingleShot(true);
    connect(_timer,
            &QTimer::timeout,
            this,
            &MainWindow::onTimeExpired);
}


MainWindow::~MainWindow()
{
    // закрытие потоков
    cleanupThreads();

    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(_threadsRunning)
    {
        return;
    }

    ui->pushButton->setEnabled(false);

    // ------ Поток 1 ------
    _thread1 = new MyThreadsControl(1, FIRST_THREAD_TIMER_MS, this);
    connect(_thread1,
            &MyThreadsControl::sendTimeInfo,
            this,
            &MainWindow::onReceiveTimeInfo);

    // ------ Поток 2 ------
    _thread2 = new MyThreadsControl(2, SECOND_THREAD_TIMER_MS, this);
    connect(_thread2,
            &MyThreadsControl::sendTimeInfo,
            this,
            &MainWindow::onReceiveTimeInfo);

    // запуск потоков и таймера
    _thread1->start();
    _thread2->start();

    _timer->start();

    _threadsRunning = true;

    qDebug() << "Threads are running";
}


void MainWindow::onReceiveTimeInfo(int threadNumber, QTime currentTime)
{
    qDebug() << "Thread [" << threadNumber << "], \t Time: " << currentTime.toString();
}


void MainWindow::onTimeExpired()
{
    qDebug() << "Timer expired, stopping threads...";

    // закрытие потоков
    cleanupThreads();

    ui->pushButton->setEnabled(true);
}


void MainWindow::cleanupThreads()
{
    if (_thread1)
    {
        _thread1->stopTimer();
        _thread1->wait();
        delete _thread1;
        _thread1 = nullptr;
    }

    if (_thread2)
    {
        _thread2->stopTimer();
        _thread2->wait();
        delete _thread2;
        _thread2 = nullptr;
    }

    _threadsRunning = false;

    qDebug() << "Threads has been stopped";
}
