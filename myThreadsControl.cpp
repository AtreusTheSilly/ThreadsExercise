#include "mythreadscontrol.h"

MyThreadsControl::MyThreadsControl(int threadNumber, int intervalMs, QObject *parent)
    : QThread(parent),
      _threadNumber(threadNumber),
      _interval(intervalMs),
      _timer(nullptr),
      _fullStopRequested(false)
{
    qDebug() << "Thread [" << _threadNumber << "] has been created with interval of " << _interval << " ms";
}


MyThreadsControl::~MyThreadsControl()
{
    _fullStopRequested = true;
    if (isRunning())
    {
        // отправка сигнала для остановки таймера в потоке
        emit stopTimerRequest();

        // ожидание завершения потока
        quit();
        wait();
    }
    qDebug() << "Thread [" << _threadNumber << "] has been destroyed";
}


void MyThreadsControl::run()
{
    // создание таймера
    _timer = new QTimer();
    _timer->setInterval(_interval);

    // соединение сигнала таймера со слотом
    connect(_timer,
            &QTimer::timeout,
            this,
            &MyThreadsControl::onTimerTimeout);

    // соединение сигнала запроса остановки таймера со слотом
    connect(this,
            &MyThreadsControl::stopTimerRequest,
            this,
            &MyThreadsControl::onStopTimerRequest,
            Qt::QueuedConnection);

    // запуск таймера
    _timer->start();

    qDebug() << "Thread [" << _threadNumber << "] has been started with timer interval of" << _interval << "ms";

    exec();

    // очистка ресурсов при выходе из цикла 'exec()'
    if (_timer)
    {
        if (_timer->isActive())
            _timer->stop();
        delete _timer;
        _timer = nullptr;
    }

    qDebug() << "Thread [" << _threadNumber << "] has finished";
}


void MyThreadsControl::stopTimer()
{
    _fullStopRequested = true;

    // отправка сигнала для остановки таймера
    emit stopTimerRequest();

    quit();
}


void MyThreadsControl::onStopTimerRequest()
{
    if (_timer && _timer->isActive())
    {
        _timer->stop();
        qDebug() << "Timer has been stopped in thread [" << _threadNumber << "]";
    }
}


void MyThreadsControl::onTimerTimeout()
{
    if (!_fullStopRequested)
    {
        QTime currentTime = QTime::currentTime();

        emit sendTimeInfo(_threadNumber, currentTime);
    }
}




