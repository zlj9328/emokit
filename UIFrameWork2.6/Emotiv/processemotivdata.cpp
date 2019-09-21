#include "processemotivdata.h"

ProcessEmotivData::ProcessEmotivData(QObject *parent) :
    QObject(parent)
{
    m_ProData.channelcase = 1;
    memset(m_ProData.Data,0,PROCESSMAXLENGTH*PROCSSCHANNAL*sizeof(float));
    m_ProData.channum = 14; //default
    m_ProData.datalength = FFTW_DATA_SIZE;
    m_fftLen = FFTW_DATA_SIZE;

    ch_pre_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * m_fftLen);
    ch_pre_out= (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * m_fftLen);
    ch_next_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * m_fftLen);
    ch_next_out= (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * m_fftLen);

    FF_Dat_num = 0;
    Average = 0;
    flag = 0;
}

ProcessEmotivData::~ProcessEmotivData()
{
    fftw_destroy_plan(p);
    fftw_free(ch_pre_in);
    fftw_free(ch_pre_out);
    fftw_free(ch_next_in);
    fftw_free(ch_next_out);

}

float ProcessEmotivData::Calculate_Amplitude( int len, float real, float imag )
{
    float result = 0.0;
    result = (real / len) * (real / len) + (imag / len )* (imag / len);
    result = sqrt(result);
    return result;
}

void ProcessEmotivData::Smooth( int length, float chdata[] )
{
    int i;
    for(i = 0; i < length - 2; i++)
        chdata[i] = (chdata[i] + chdata[i + 1] + chdata[i + 2]) / 3;
}

int ProcessEmotivData::UseClassifer()
{
    return Classify(destion1,destion2);
}

float ProcessEmotivData::AverageMax( float ch_pre[], float ch_next[] )
{
    int i;
    float sum_pre = 0.0;
    float sum_next = 0.0;
    int Min = 3;
    int Max = 12;
    for(i = Min; i <= Max; i++)
    {
        sum_pre += 2 * ch_pre[i];
        sum_next += 2 * ch_next[i];
    }
    return ((sum_next - sum_pre) / (Max - Min + 1));
}


int ProcessEmotivData::Classify( float ch_pre_M[],float ch_next_M[] )
{
    float classify_result=0.0;
    classify_result=AverageMax(ch_pre_M, ch_next_M);
    if (classify_result<0)
        return 0;
    else return 1;
}

void ProcessEmotivData::FFTW_Transform(const struct Emokit_Frame *ef)
{
    int i = 0;
    m_ProData.Data[0][FF_Dat_num] = ef->raw_data.F3;
    m_ProData.Data[1][FF_Dat_num] = ef->raw_data.FC6;
    m_ProData.Data[2][FF_Dat_num] = ef->raw_data.P7;
    m_ProData.Data[3][FF_Dat_num] = ef->raw_data.T8;
    m_ProData.Data[4][FF_Dat_num] = ef->raw_data.F7;
    m_ProData.Data[5][FF_Dat_num] = ef->raw_data.F8;
    m_ProData.Data[6][FF_Dat_num] = ef->raw_data.T7;
    m_ProData.Data[7][FF_Dat_num] = ef->raw_data.P8;
    m_ProData.Data[8][FF_Dat_num] = ef->raw_data.AF4;
    m_ProData.Data[9][FF_Dat_num] = ef->raw_data.F4;
    m_ProData.Data[10][FF_Dat_num] = ef->raw_data.AF3;
    m_ProData.Data[11][FF_Dat_num] = ef->raw_data.O2;
    m_ProData.Data[12][FF_Dat_num] = ef->raw_data.O1;
    m_ProData.Data[13][FF_Dat_num] = ef->raw_data.FC5;

    Average = (m_ProData.Data[0][FF_Dat_num]+m_ProData.Data[1][FF_Dat_num]+m_ProData.Data[2][FF_Dat_num]
                                +m_ProData.Data[3][FF_Dat_num]+m_ProData.Data[4][FF_Dat_num]+m_ProData.Data[5][FF_Dat_num]+m_ProData.Data[6][FF_Dat_num]
                                +m_ProData.Data[7][FF_Dat_num]+m_ProData.Data[8][FF_Dat_num]+m_ProData.Data[9][FF_Dat_num]+m_ProData.Data[10][FF_Dat_num]
                                +m_ProData.Data[11][FF_Dat_num]+m_ProData.Data[12][FF_Dat_num]+m_ProData.Data[13][FF_Dat_num])/14;
    ch_pre_in[FF_Dat_num][0] = m_ProData.Data[0][FF_Dat_num]-Average;
    ch_next_in[FF_Dat_num][0] = m_ProData.Data[9][FF_Dat_num]-Average;
    ch_pre_in[FF_Dat_num][1] = 0.0;
    ch_next_in[FF_Dat_num][1] = 0.0;

    if(FF_Dat_num > 63 && flag == 0)
    {
        ChannelAvgDat[0][FF_Dat_num-64] = m_ProData.Data[0][FF_Dat_num]-Average;
        ChannelAvgDat[1][FF_Dat_num-64] = m_ProData.Data[9][FF_Dat_num]-Average;
    }
    FF_Dat_num ++;

    if(FF_Dat_num >= FFTW_DATA_SIZE)
    {

        if(flag == 1)
        {
            for(i=0;i<64;i++)
            {
                ch_pre_in[i][0] = ChannelAvgDat[0][i];
                ch_next_in[i][0] = ChannelAvgDat[1][i];
            }
            for(i=0;i<64;i++)
            {
                ChannelAvgDat[0][i] = ch_pre_in[i+64][0];
                ChannelAvgDat[1][i] = ch_next_in[i+64][0];
            }

         }
         flag = 1;

         FF_Dat_num = 64;
         p = fftw_plan_dft_1d(m_fftLen ,ch_pre_in,ch_pre_out, FFTW_FORWARD, FFTW_ESTIMATE);
         fftw_execute(p); /* repeat as needed*/
         p = fftw_plan_dft_1d(m_fftLen ,ch_next_in,ch_next_out, FFTW_FORWARD, FFTW_ESTIMATE);
         fftw_execute(p);
    }

    //calculate the amplitude;
    for(i = 0; i < m_fftLen; i++)
    {
        destion1[i] = Calculate_Amplitude(m_fftLen,ch_pre_out[i][0],ch_pre_out[i][1]);
        destion2[i] = Calculate_Amplitude(m_fftLen,ch_next_out[i][0],ch_next_out[i][1]);
    }
    Smooth(m_fftLen,destion1);
    Smooth(m_fftLen,destion2);
}
