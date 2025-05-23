#ifndef MYTHREADSCONTROL_H
#define MYTHREADSCONTROL_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QThread>

/// класс для запуска таймеров внутри потоков
class MyThreadsControl : public QThread
{
    Q_OBJECT

public:
    MyThreadsControl(int threadNumber, int intervalMs, QObject *parent = nullptr);
    ~MyThreadsControl();

    /// остановка таймера
    void stopTimer();

signals:
    /// сигнал отправки данных о номере потока и текущем времени
    void sendTimeInfo(int threadNumber, QTime currentTime);
    /// сигнал для остановки таймера
    void stopTimerRequest();

protected:
    void run() override;

private slots:
    /// выполняется при срабатывании таймера
    void onTimerTimeout();
    /// выполняется при остановке таймера
    void onStopTimerRequest();

private:
    /// номер потока
    int _threadNumber;
    /// временной промежуток (в мс)
    int _interval;
    /// таймер
    QTimer *_timer;
    /// флаг запроса остановки
    bool _fullStopRequested;
};

#endif // MYTHREADSCONTROL_H
