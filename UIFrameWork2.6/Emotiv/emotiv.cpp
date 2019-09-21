#include "emotiv.h"

Emotiv::Emotiv(QObject *parent) :
    QObject(parent)
{
    _dev = NULL;
    block_buffer = NULL;
    _is_open = 0;
    _is_inited = 0;
    hid_init();
    _is_inited = 1;

}

Emotiv::~Emotiv()
{
    emotiv_close();
}

/**
 * @brief Emotiv::emotiv_get_count
 * @return
 */
int Emotiv::emotiv_get_count()
{
    int count = 0;
    struct hid_device_info* devices;
    struct hid_device_info* device_cur;
    if (!_is_inited)
    {
        return E_EMOTIV_NOT_INITED;
    }
    devices = hid_enumerate(EMOTIV_VID, EMOTIV_PID);

    device_cur = devices;
    while(device_cur) {
        ++count;
        device_cur = device_cur->next;
    }

    hid_free_enumeration(devices);
    return count;
}

/**
 * @brief Emotiv::emotiv_identify_device
 * @param dev
 * @return
 */
int Emotiv::emotiv_identify_device(hid_device *dev)
{
    /* currently we check to see if the feature report matches the consumer
       model and if not we assume it's a research model.*/
    int nbytes, i, dev_type = EMOTIV_CONSUMER;
    unsigned char buf[EMOTIV_REPORT_SIZE];
    char report_consumer[] = {0x00, 0xa0, 0xff, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x00};
    buf[0] = EMOTIV_REPORT_ID;
    nbytes = hid_get_feature_report(dev, buf, sizeof(buf));
    if (nbytes != EMOTIV_REPORT_SIZE)
    {
        return -1;
    }
    for (i=0; i < nbytes; ++i) {
        if (buf[i] != report_consumer[i]) {
            dev_type = EMOTIV_RESEARCH;
            break;
        }
    }
    return dev_type;
}

/**
 * @brief Emotiv::emotiv_init_crypto
 * @param dev_type
 * @return
 */
int Emotiv::emotiv_init_crypto(int dev_type)
{
    emotiv_get_crypto_key(dev_type);

    //libmcrypt initialization
    td = mcrypt_module_open(MCRYPT_RIJNDAEL_128, NULL, MCRYPT_ECB, NULL);
    blocksize = mcrypt_enc_get_block_size(td); //should return a 16bits blocksize

    block_buffer = (unsigned char *)malloc(blocksize);

    mcrypt_generic_init(td, key, EMOTIV_KEYSIZE, NULL);
    return 0;
}

/**
 * @brief Emotiv::emotiv_get_crypto_key
 * @param dev_type
 */
void Emotiv::emotiv_get_crypto_key(int dev_type)
{
    unsigned char type = (unsigned char) dev_type;
    unsigned int l = 16;
    type &= 0xF;
    type = (type == 0);

    key[0] = (uint8_t)serial[l-1];
    key[1] = '\0';
    key[2] = (uint8_t)serial[l-2];
    if(type) {
        key[3] = 'H';
        key[4] = (uint8_t)serial[l-1];
        key[5] = '\0';
        key[6] = (uint8_t)serial[l-2];
        key[7] = 'T';
        key[8] = (uint8_t)serial[l-3];
        key[9] = '\x10';
        key[10] = (uint8_t)serial[l-4];
        key[11] = 'B';
    }
    else {
        key[3] = 'T';
        key[4] = (uint8_t)serial[l-3];
        key[5] = '\x10';
        key[6] = (uint8_t)serial[l-4];
        key[7] = 'B';
        key[8] = (uint8_t)serial[l-1];
        key[9] = '\0';
        key[10] = (uint8_t)serial[l-2];
        key[11] = 'H';
    }
    key[12] = (uint8_t)serial[l-3];
    key[13] = '\0';
    key[14] = (uint8_t)serial[l-4];
    key[15] = 'P';
}

/**
 * @brief Emotiv::emotiv_open
 * @param device_index
 * @return
 */
int Emotiv::emotiv_open(unsigned int device_index)
{
    int dev_type;
    int count = 0;
    struct hid_device_info* devices;
    struct hid_device_info* device_cur;

    if (!_is_inited)
    {
        return E_EMOTIV_NOT_INITED;
    }
    devices = hid_enumerate(EMOTIV_VID, EMOTIV_PID);
    device_cur = devices;
    while(device_cur) {
        if(count == device_index)
        {
            _dev = hid_open_path(device_cur->path);
            break;
        }
        ++count;
        device_cur = device_cur->next;
    }
    hid_free_enumeration(devices);
    if(!_dev) {
        return E_EMOTIV_NOT_OPENED;
    }
    _is_open = 1;
    //dev_type = emokit_identify_device(s->_dev);
    dev_type = EMOTIV_RESEARCH;
    hid_get_serial_number_string(_dev, serial, MAX_STR);
    emotiv_init_crypto(dev_type);

    return 0;
}

/**
 * @brief Emotiv::emotiv_close
 * @return
 */
int Emotiv::emotiv_close()
{
    if(!_is_open)
    {
        return E_EMOTIV_NOT_OPENED;
    }
    hid_close(_dev);
    _is_open = 0;
    emotiv_delete();
    return 0;

}

/**
 * @brief Emotiv::emotiv_read_data
 * @return
 */
int Emotiv::emotiv_read_data()
{
    return hid_read(_dev,raw_frame, 32);
}

/**
 * @brief Emotiv::emotiv_deinit
 */
void Emotiv::emotiv_deinit()
{
    mcrypt_generic_deinit (td);
    mcrypt_module_close(td);
}

/**
 * @brief Emotiv::PrintHex
 * @param data
 * @param len
 */
void Emotiv::PrintHex(unsigned char* data, int len)
{
    int i;

    for(i=0; i<len; i++)
    {
        printf("%02x",data[i]);
    }
}

/**
 * @brief Emotiv::emotiv_delete
 */
void Emotiv::emotiv_delete()
{
    emotiv_deinit();
    if(block_buffer != NULL)
    {
        free(block_buffer);
    }
//    free(dev);
}

/**
 * @brief Emotiv::emotiv_get_raw_frame
 * @param buf
 */
void Emotiv::emotiv_get_raw_frame(unsigned char buf[32])
{
    memcpy(buf, raw_unenc_frame, 32);
}

/**
 * @brief Emotiv::emotiv_init
 * @return
 */\
int Emotiv::emotiv_init()
{
    int emotiv_count;
    emotiv_count = emotiv_get_count();
    qDebug("CANNOT CONNECT: %d",emotiv_count);
    int r = emotiv_open(0); //-1;
    if(r!=0)
    {
        emotiv_close();
        return EMOKIT_INT_ERROR;
    }

    return 0;
}




/**
 * @brief Emotiv::battery_value
 * @param in
 * @return
 */
unsigned char Emotiv::battery_value(unsigned char in)
{

    if (in>=248) return 100;
    else {
        switch(in) {
            case 247:return 99; break;
            case 246:return 97; break;
            case 245:return 93; break;
            case 244:return 89; break;
            case 243:return 85; break;
            case 242:return 82; break;
            case 241:return 77; break;
            case 240:return 72; break;
            case 239:return 66; break;
            case 238:return 62; break;
            case 237:return 55; break;
            case 236:return 46; break;
            case 235:return 32; break;
            case 234:return 20; break;
            case 233:return 12; break;
            case 232:return 6; break;
            case 231:return 4 ; break;
            case 230:return 3; break;
            case 229:return 2; break;
            case 228:
            case 227:
            case 226:
                return 1;
                break;
            default:
                return 0;
        }
    }
}

/**
 * @brief Emotiv::get_level
 * @param frame
 * @param bits
 * @return
 */
int Emotiv::get_level(unsigned char frame[32], const unsigned char bits[14])
{
    signed char i;
    char b,o;
    int level = 0;

    for (i = 13; i >= 0; --i) {
        level <<= 1;
        b = bits[i]/8+1;
        o = bits[i] % 8 ;

        level |= (frame[b] >> o) & 1;
    }
    return level;
}

/**
 * @brief Emotiv::handle_quality
 * @return
 */
Emotiv_Contact_Quality Emotiv::handle_quality()
{
    int current_contact_quality = get_level(raw_unenc_frame,QUALITY_MASK);
    switch(raw_unenc_frame[0])
    {
            case 0:
                last_quality.F3=current_contact_quality;
                break;
            case 1:
                last_quality.FC5=current_contact_quality;
                break;
            case 2:
                last_quality.AF3=current_contact_quality;
                break;
            case 3:
                last_quality.F7=current_contact_quality;
                break;
            case 4:
                last_quality.T7=current_contact_quality;
                break;
            case 5:
                last_quality.P7=current_contact_quality;
                break;
            case 6:
                last_quality.O1=current_contact_quality;
                break;
            case 7:
                last_quality.O2=current_contact_quality;
                break;
            case 8:
                last_quality.P8=current_contact_quality;
                break;
            case 9:
                last_quality.T8=current_contact_quality;
                break;
            case 10:
                last_quality.F8=current_contact_quality;
                break;
            case 11:
                last_quality.AF4=current_contact_quality;
                break;
            case 12:
                last_quality.FC6=current_contact_quality;
                break;
            case 13:
                last_quality.F4=current_contact_quality;
                break;
            case 14:
                last_quality.F8=current_contact_quality;
                break;
            case 15:
                last_quality.AF4=current_contact_quality;
                break;
            case 64:
                last_quality.F3=current_contact_quality;
                break;
            case 65:
                last_quality.FC5=current_contact_quality;
                break;
            case 66:
                last_quality.AF3=current_contact_quality;
                break;
            case 67:
                last_quality.F7=current_contact_quality;
                break;
            case 68:
                last_quality.T7=current_contact_quality;
                break;
            case 69:
                last_quality.P7=current_contact_quality;
                break;
            case 70:
                last_quality.O1=current_contact_quality;
                break;
            case 71:
                last_quality.O2=current_contact_quality;
                break;
            case 72:
                last_quality.P8=current_contact_quality;
                break;
            case 73:
                last_quality.T8=current_contact_quality;
                break;
            case 74:
                last_quality.F8=current_contact_quality;
                break;
            case 75:
                last_quality.AF4=current_contact_quality;
                break;
            case 76:
                last_quality.FC6=current_contact_quality;
                break;
            case 77:
                last_quality.F4=current_contact_quality;
                break;
            case 78:
                last_quality.F8=current_contact_quality;
                break;
            case 79:
                last_quality.AF4=current_contact_quality;
                break;
            case 80:
                last_quality.FC6=current_contact_quality;
                break;
            default:
                break;
    }
    return (last_quality);
}

/**
 * @brief Emotiv::emotiv_get_next_raw
 * @return
 */
int Emotiv::emotiv_get_next_raw()
{
    //Two blocks of 16 bytes must be read.

    if (memcpy (block_buffer, raw_frame, blocksize)) {
        mdecrypt_generic (td, block_buffer, blocksize);
        memcpy(raw_unenc_frame, block_buffer, blocksize);
    }
    else {
        return -1;
    }

    if (memcpy(block_buffer, raw_frame + blocksize, blocksize)) {
        mdecrypt_generic (td, block_buffer, blocksize);
        memcpy(raw_unenc_frame + blocksize, block_buffer, blocksize);
    }
    else {
        return -1;
    }

    return 0;
}

/**
 * @brief Emotiv::emotiv_get_next_frame
 */
void Emotiv::emotiv_get_next_frame()
{
    memset(raw_unenc_frame, 0, 32);

    if (emotiv_get_next_raw()<0) {
        current_frame.counter=0;
    }

    memset(&current_frame.contact_quality,0,sizeof(struct Emotiv_Contact_Quality));
    if (raw_unenc_frame[0] & 128) {
        current_frame.counter = 128;
        current_frame.battery = battery_value( raw_unenc_frame[0] );
        last_battery=current_frame.battery;
    } else {
        current_frame.counter = raw_unenc_frame[0];
        current_frame.battery = last_battery;
    }
    current_frame.raw_data.F3 = get_level(raw_unenc_frame, F3_MASK);
    current_frame.raw_data.FC6 = get_level(raw_unenc_frame, FC6_MASK);
    current_frame.raw_data.P7 = get_level(raw_unenc_frame, P7_MASK);
    current_frame.raw_data.T8 = get_level(raw_unenc_frame, T8_MASK);
    current_frame.raw_data.F7 = get_level(raw_unenc_frame, F7_MASK);
    current_frame.raw_data.F8 = get_level(raw_unenc_frame, F8_MASK);
    current_frame.raw_data.T7 = get_level(raw_unenc_frame, T7_MASK);
    current_frame.raw_data.P8 = get_level(raw_unenc_frame, P8_MASK);
    current_frame.raw_data.AF4 = get_level(raw_unenc_frame, AF4_MASK);
    current_frame.raw_data.F4 = get_level(raw_unenc_frame, F4_MASK);
    current_frame.raw_data.AF3 = get_level(raw_unenc_frame, AF3_MASK);
    current_frame.raw_data.O2 = get_level(raw_unenc_frame, O2_MASK);
    current_frame.raw_data.O1 = get_level(raw_unenc_frame, O1_MASK);
    current_frame.raw_data.FC5 = get_level(raw_unenc_frame, FC5_MASK);

    current_frame.gyroX = raw_unenc_frame[29] - 102;
    current_frame.gyroY = raw_unenc_frame[30] - 104;

    current_frame.contact_quality=handle_quality();

}

