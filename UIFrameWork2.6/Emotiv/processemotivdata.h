#ifndef PROCESSEMOTIVDATA_H
#define PROCESSEMOTIVDATA_H


#define FFTW_DATA_SIZE  128
/*
implement the algorithm
*/
#define PROCSSCHANNAL 14
#define PROCESSMAXLENGTH 128


#include <QObject>
#include <QtMath>
#include "/usr/local/fftw3/include/fftw3.h"
#include "Emotiv/emotiv.h"

typedef struct EEGData
{
    int channelcase;          //c3/c4 or cp3/cp4
    int channum;              //16/8
    float Data[PROCSSCHANNAL][PROCESSMAXLENGTH];
    int datalength;
}ProcessingData;


class ProcessEmotivData : public QObject
{
    Q_OBJECT
public:
    explicit ProcessEmotivData(QObject *parent = 0);
    ~ProcessEmotivData();

public:
    float Calculate_Amplitude(int len, float real, float imag);
    void Smooth(int length, float chdata[]);
    float AverageMax(float ch_pre[], float ch_next[]);
    int Classify(float ch_pre_M[],float ch_next_M[]);
    int UseClassifer();

    void FFTW_Transform(const struct Emokit_Frame *ef);

public:
    float destion1[PROCESSMAXLENGTH];
    float destion2[PROCESSMAXLENGTH];
    //float Average[PROCESSMAXLENGTH];
    int m_DataBlockSize;
    int m_fftLen;
    float ChannelAvgDat[2][64];

    ProcessingData m_ProData;

    fftw_complex *ch_pre_in, *ch_pre_out;
    fftw_complex *ch_next_in, *ch_next_out;
    fftw_plan p;

    int FF_Dat_num;
    float Average;
    int flag;

};

#endif // PROCESSEMOTIVDATA_H
