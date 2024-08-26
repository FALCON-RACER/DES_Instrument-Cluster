#include "ina219.h"
#include <QDebug>
#include "i2cexception.h"

INA219::INA219(const char *filename, int address) : _address(address)  {

    //const char *filename = "/dev/i2c-1";
    //int addr = 0x41; // INA219 I2C address

    // I2C 버스 열기
    if ((_file = open(filename, O_RDWR)) < 0)
        throw I2CException("Failed to open the i2c bus");

    // I2C 슬레이브 주소 설정
    if (ioctl(_file, I2C_SLAVE, address) < 0)
        throw I2CException("Failed to acquire bus access and/or talk to slave");

    setCalibration();
    setConfiguration();
}


uint16_t INA219::readRegister(uint8_t registerNumber) {

    uint16_t readValue;
    char buf[2] = {0};
    buf[0] = registerNumber;


    // 레지스터 주소를 장치에 쓰기
    if (write(_file, buf, 1) != 1) {
        std::cerr << "Failed to write to the i2c bus" << std::endl;
        return 0;
    }


    // 장치에서 2바이트 데이터 읽기
    if (read(_file, buf, 2) != 2) {
        std::cerr << "Failed to read from the i2c bus" << std::endl;
        return 0;
    }
    readValue = (buf[0] << 8) + buf[1];
    return readValue;
}


void INA219::writeRegister(uint8_t registerNumber, uint16_t value)
{
    uint8_t buf[3] = {0};
    buf[0] = registerNumber;
    buf[1] = value >> 8;
    buf[2] = value & 0xff;

    if(write(_file, buf, 3) != 3){
        close(_file);
        throw I2CException("register write err");
    }

}


void INA219::setCalibration() {

    writeRegister(0x05, CALIBRATION_VALUE);
}

void INA219::setConfiguration() {

    u_int16_t configuration = 0;
    configuration |= MODE_SHUNT_AND_BUS_CONTINUOUS;
    configuration |= SHUNT_ADC_12BIT_128SAMPLES;
    configuration |= BUS_ADC_12BIT_128SAMPLES;
    configuration |= PGA_GAIN_8;
    configuration |= BUS_VOLTAGE_RANGE_32V;

    writeRegister(0x00, configuration);
}

float INA219::getBusVoltage() {

    uint16_t busVoltageRaw = readRegister(0x02);
    float busVoltage = ((busVoltageRaw>>3) * 4.0) / 1000.0;

    return busVoltage;
}


float INA219::getShuntVoltage() {

    int shuntVoltageRaw = readRegister(0x01);
    float shuntVoltage = shuntVoltageRaw * 0.01 / 1000; // mv -> V

    return shuntVoltage;
}

float INA219::getBatteryVoltage() {

    float busVoltage = getBusVoltage();
    float shuntVoltage = getShuntVoltage();

    qDebug() << "Bus Voltage:" << busVoltage << "V";
    qDebug() << "Shunt Voltage:" << shuntVoltage << "V";


    return busVoltage + shuntVoltage;
}


