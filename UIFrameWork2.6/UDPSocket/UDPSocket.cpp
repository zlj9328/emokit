#include "UDPSocket/UDPSocket.h"

int SerialNumber = 0;

/**
 * @brief UDPHelper::UDPHelper
 * @param IP
 * @param Port
 */
UDPHelper::UDPHelper(std::string IP,int Port,QObject *parent) :
    QObject(parent)
{
    m_IP = IP;
    m_Port = Port;
    InitUDPSocket();
}

/**
 * @brief UDPHelper::~UDPHelper
 */
UDPHelper::~UDPHelper()
{
    close(socketfd);
}

/**
 * @brief UDPHelper::InitUDPSocket
 */
void UDPHelper::InitUDPSocket()
{
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketfd < 0)
    {
        qDebug("create socket error!!\n");
    }

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(m_IP.data());
    server_addr.sin_port = htons(m_Port);
}

/**
 * @brief UDPHelper::Unbind
 */
void UDPHelper::Unbind()
{
    close(socketfd);
}

/**
 * @brief UDPHelper::UDPSend
 * @param data
 * @param length
 * @return
 */
int UDPHelper::UDPSend(const char *data, int length)
{
    return sendto(socketfd,data,length,0,(sockaddr *)&server_addr,sizeof(server_addr));
}

/**
 * @brief UDPHelper::UDPRecv
 * @param data
 * @param length
 * @return
 */
int  UDPHelper::UDPRecv(char *data, int length)
{
    //...
}

/**
 * @brief UDPHelper::SendChannelPkt
 * @param channelName1
 * @param channel1
 * @param channelName12
 * @param channel2
 */
void UDPHelper::SendChannelPkt(const char * channelName1, int channel1, const char * channelName12, int channel2)
{
    PktChannelData pktChannelData;

    pktChannelData.header.nPktType = PKT_CHANNEL;
    pktChannelData.keyNumber.nKey = SerialNumber;

    strcpy(pktChannelData.channelType1, channelName1);
    pktChannelData.channelData1 = channel1;
    strcpy(pktChannelData.channelType2, channelName12);
    pktChannelData.channelData2 = channel2;

    UDPSend((char *)&pktChannelData,sizeof(pktChannelData));
}

/**
 * @brief UDPHelper::SendAckPkt
 */
void UDPHelper::SendAckPkt()
{
    PktAck pktAck;
    pktAck.header.nPktType = PKT_ACK;
    pktAck.keyNumber.nKey = SerialNumber;
    UDPSend((char *)&pktAck, sizeof(pktAck));
}

/**
 * @brief UDPHelper::SendLoginPkt
 */
void UDPHelper::SendLoginPkt()
{
    PktLogin pktLogin;
    pktLogin.header.nPktType = PKT_LOGIN;
    pktLogin.keyNumber.nKey = SerialNumber;
    UDPSend((char *)&pktLogin, sizeof(pktLogin));
}

/**
 * @brief UDPHelper::SendLogOutPkt
 */
void UDPHelper::SendLogOutPkt()
{
    PktLogOut pktLogOut;
    pktLogOut.header.nPktType = PKT_LOGOUT;
    pktLogOut.keyNumber.nKey = SerialNumber;
    UDPSend((char *)&pktLogOut, sizeof(pktLogOut));
}

/**
 * @brief UDPHelper::SendHeartRatePkt
 * @param nBMP
 * @param nSignal
 * @param nIBI
 */
void UDPHelper::SendHeartRatePkt(int nBMP, int nSignal, int nIBI)
{
    PktHeartRate pktHeartRate;
    pktHeartRate.header.nPktType = PKT_HEART_RATE;
    pktHeartRate.keyNumber.nKey = SerialNumber;

    pktHeartRate.BMP = nBMP;
    pktHeartRate.Signal = nSignal;
    pktHeartRate.IBI = nIBI;

    UDPSend((char *)&pktHeartRate, sizeof(pktHeartRate));

}
