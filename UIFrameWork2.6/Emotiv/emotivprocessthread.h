#ifndef EMOTIVPROCESSTHREAD_H
#define EMOTIVPROCESSTHREAD_H

#include <QThread>
#include "Emotiv/emotiv.h"
#include "Emotiv/processemotivdata.h"

class EmotivProcessThread : public QThread
{
    Q_OBJECT
public:
    EmotivProcessThread();
    ~EmotivProcessThread();

    void run();

private:
    bool thread_state;
public:
    Emotiv *emotiv;
    ProcessEmotivData *procEmotivData;

signals:
    void BatterySignal(int value);
    void EmotivStateSignal(int value);
};

#endif // EMOTIVPROCESSTHREAD_H
