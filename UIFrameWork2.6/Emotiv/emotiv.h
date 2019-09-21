#ifndef EMOTIV_H
#define EMOTIV_H


#define EMOKIT_INT_ERROR    1


#define E_EMOTIV_DRIVER_ERROR   -1
#define E_EMOTIV_NOT_INITED     -2
#define E_EMOTIV_NOT_OPENED     -3


#define MAX_STR 255

#define EMOTIV_KEYSIZE 16 /* 128 bits == 16 bytes */

#define EMOTIV_CONSUMER 0
#define EMOTIV_RESEARCH 1

/* ID of the feature report we need to identify the device
   as consumer/research */
#define EMOTIV_REPORT_ID    0
#define EMOTIV_REPORT_SIZE  9


#include <stdint.h>
#include "/usr/local/include/hidapi/hidapi.h"
#include "mcrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QObject>


/// Vendor ID for all emotiv devices
const static uint32_t EMOTIV_VID = 0x21a1;
/// Product ID for all emotiv devices
const static uint32_t EMOTIV_PID = 0x0001;

/// Out endpoint for all emotiv devices
const static uint32_t EMOTIV_OUT_ENDPT = 0x02;
/// In endpoint for all emotiv devices
const static uint32_t EMOTIV_IN_ENDPT  = 0x82;

struct Emotiv_Contact_Quality {//values > 4000 are good
    short F3, FC6, P7, T8, F7, F8, T7, P8, AF4, F4, AF3, O2, O1, FC5;
};
struct Emotiv_Raw_Data_Values{
    int F3, FC6, P7, T8, F7, F8, T7, P8, AF4, F4, AF3, O2, O1, FC5; //raw data values
};

struct Emokit_Frame {
    unsigned char counter;                                          //loops from 0 to 128 (129 values)
    struct Emotiv_Raw_Data_Values raw_data;
    struct Emotiv_Contact_Quality contact_quality;
    char gyroX, gyroY;
    unsigned char battery;                                          //percentage of full charge, read on counter=128
};


const unsigned char F3_MASK[14]     = {10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7};
const unsigned char FC6_MASK[14]    = {214, 215, 200, 201, 202, 203, 204, 205, 206, 207, 192, 193, 194, 195};
const unsigned char P7_MASK[14]     = {84, 85, 86, 87, 72, 73, 74, 75, 76, 77, 78, 79, 64, 65};
const unsigned char T8_MASK[14]     = {160, 161, 162, 163, 164, 165, 166, 167, 152, 153, 154, 155, 156, 157};
const unsigned char F7_MASK[14]     = {48, 49, 50, 51, 52, 53, 54, 55, 40, 41, 42, 43, 44, 45};
const unsigned char F8_MASK[14]     = {178, 179, 180, 181, 182, 183, 168, 169, 170, 171, 172, 173, 174, 175};
const unsigned char T7_MASK[14]     = {66, 67, 68, 69, 70, 71, 56, 57, 58, 59, 60, 61, 62, 63};
const unsigned char P8_MASK[14]     = {158, 159, 144, 145, 146, 147, 148, 149, 150, 151, 136, 137, 138, 139};
const unsigned char AF4_MASK[14]    = {196, 197, 198, 199, 184, 185, 186, 187, 188, 189, 190, 191, 176, 177};
const unsigned char F4_MASK[14]     = {216, 217, 218, 219, 220, 221, 222, 223, 208, 209, 210, 211, 212, 213};
const unsigned char AF3_MASK[14]    = {46, 47, 32, 33, 34, 35, 36, 37, 38, 39, 24, 25, 26, 27};
const unsigned char O2_MASK[14]     = {140, 141, 142, 143, 128, 129, 130, 131, 132, 133, 134, 135, 120, 121};
const unsigned char O1_MASK[14]     = {102, 103, 88, 89, 90, 91, 92, 93, 94, 95, 80, 81, 82, 83};
const unsigned char FC5_MASK[14]    = {28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23, 8, 9};
const unsigned char QUALITY_MASK[14]= {99,100,101,102,103,104,105,106,107,108,109,110,111,112};


class Emotiv : public QObject
{
    Q_OBJECT
public:
    explicit Emotiv(QObject *parent = 0);
    ~Emotiv();

private:
    hid_device* _dev;
    wchar_t serial[MAX_STR];                    // USB Dongle serial number
    int _is_open;                               // Is device currently open
    int _is_inited;                             // Is device current initialized
    MCRYPT td;                                  // mcrypt context
    unsigned char key[EMOTIV_KEYSIZE];          // crypt key for device
    unsigned char *block_buffer;                // temporary storage for decrypt
    int blocksize;                              // Size of current block
    unsigned char raw_frame[32];                // Raw encrypted data received from headset
    unsigned char raw_unenc_frame[32];          // Raw unencrypted data received from headset
    unsigned char last_battery;                 //last reported battery value, in percentage of full
    struct Emotiv_Contact_Quality last_quality; //last reported contact quality
public:
    struct Emokit_Frame current_frame;          // Last information received from headset

public:
    int emotiv_get_count();
    int emotiv_identify_device(hid_device *dev) ;
    int emotiv_init_crypto(int dev_type) ;
    void emotiv_get_crypto_key(int dev_type) ;
    int emotiv_open(unsigned int device_index);
    int emotiv_close();
    int emotiv_read_data();
    void emotiv_deinit() ;
    void PrintHex(unsigned char* data, int len);
    void emotiv_delete();
    void emotiv_get_raw_frame(unsigned char buf[32]) ;
    int emotiv_init();


    unsigned char battery_value(unsigned char in);
    int get_level(unsigned char frame[32], const unsigned char bits[14]) ;
    Emotiv_Contact_Quality handle_quality() ;
    int emotiv_get_next_raw() ;
    void emotiv_get_next_frame() ;
};

#endif // EMOTIV_H
