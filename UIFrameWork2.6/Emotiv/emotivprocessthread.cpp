#include "emotivprocessthread.h"

EmotivProcessThread::EmotivProcessThread()
{
    thread_state = false;
    /*Emotiv Init*/
    emotiv = new Emotiv(this);
    procEmotivData = new ProcessEmotivData(this);
}

EmotivProcessThread::~EmotivProcessThread()
{
    delete emotiv;
    delete procEmotivData;
}

void EmotivProcessThread::run()
{
    while(thread_state)
    {
        emotiv->emotiv_read_data();
        emotiv->emotiv_get_next_frame();
        procEmotivData->FFTW_Transform(&emotiv->current_frame);
        usleep(3000);
    }
}
