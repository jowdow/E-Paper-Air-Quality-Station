#include "SCD-30.h"


#ifdef SCD30_ADDRESS
static const uint8_t SCD30_I2C_ADDRESS = SCD30_ADDRESS;
#else
static const uint8_t SCD30_I2C_ADDRESS = 0x61;
#endif



#define SCD30_CMD_START_PERIODIC_MEASUREMENT 0x0010
#define SCD30_CMD_STOP_PERIODIC_MEASUREMENT 0x0104
#define SCD30_CMD_READ_MEASUREMENT 0x0300
#define SCD30_CMD_SET_MEASUREMENT_INTERVAL 0x4600
#define SCD30_CMD_GET_DATA_READY 0x0202
#define SCD30_CMD_SET_TEMPERATURE_OFFSET 0x5403
#define SCD30_CMD_SET_ALTITUDE 0x5102
#define SCD30_CMD_SET_FORCED_RECALIBRATION 0x5204
#define SCD30_CMD_AUTO_SELF_CALIBRATION 0x5306
#define SCD30_CMD_READ_SERIAL 0xD033
#define SCD30_SERIAL_NUM_WORDS 16
#define SCD30_WRITE_DELAY_US 20000

#define SCD30_MAX_BUFFER_WORDS 24
#define SCD30_CMD_SINGLE_WORD_BUF_LEN \
    (SENSIRION_COMMAND_SIZE + SENSIRION_WORD_SIZE + CRC8_LEN)

int16_t scd30_start_periodic_measurement(uint16_t ambient_pressure_mbar) {
    if (ambient_pressure_mbar &&
        (ambient_pressure_mbar < 700 || ambient_pressure_mbar > 1400)) {
        /* out of allowable range */
        return STATUS_FAIL;
    }

    return sensirion_i2c_write_cmd_with_args(
        SCD30_I2C_ADDRESS, SCD30_CMD_START_PERIODIC_MEASUREMENT,
        &ambient_pressure_mbar, SENSIRION_NUM_WORDS(ambient_pressure_mbar));
}

int16_t scd30_stop_periodic_measurement() {
    return sensirion_i2c_write_cmd(SCD30_I2C_ADDRESS,
                                   SCD30_CMD_STOP_PERIODIC_MEASUREMENT);
}

int16_t scd30_read_measurement(float* co2_ppm, float* temperature,
                               float* humidity) {
    int16_t error;
    uint8_t data[3][4];

    error =
        sensirion_i2c_write_cmd(SCD30_I2C_ADDRESS, SCD30_CMD_READ_MEASUREMENT);
    if (error != NO_ERROR)
        return error;

    error = sensirion_i2c_read_words_as_bytes(SCD30_I2C_ADDRESS, &data[0][0],
                                              SENSIRION_NUM_WORDS(data));
    if (error != NO_ERROR)
        return error;

    *co2_ppm = sensirion_bytes_to_float(data[0]);
    *temperature = sensirion_bytes_to_float(data[1]);
    *humidity = sensirion_bytes_to_float(data[2]);

    return NO_ERROR;
}

int16_t scd30_set_measurement_interval(uint16_t interval_sec) {
    int16_t error;

    if (interval_sec < 2 || interval_sec > 1800) {
        /* out of allowable range */
        return STATUS_FAIL;
    }

    error = sensirion_i2c_write_cmd_with_args(
        SCD30_I2C_ADDRESS, SCD30_CMD_SET_MEASUREMENT_INTERVAL, &interval_sec,
        SENSIRION_NUM_WORDS(interval_sec));
    sensirion_sleep_usec(SCD30_WRITE_DELAY_US);

    return error;
}

int16_t scd30_get_data_ready(uint16_t* data_ready) {
    return sensirion_i2c_delayed_read_cmd(
        SCD30_I2C_ADDRESS, SCD30_CMD_GET_DATA_READY, 3000, data_ready,
        SENSIRION_NUM_WORDS(*data_ready));
}

int16_t scd30_set_temperature_offset(uint16_t temperature_offset) {
    int16_t error;

    error = sensirion_i2c_write_cmd_with_args(
        SCD30_I2C_ADDRESS, SCD30_CMD_SET_TEMPERATURE_OFFSET,
        &temperature_offset, SENSIRION_NUM_WORDS(temperature_offset));
    sensirion_sleep_usec(SCD30_WRITE_DELAY_US);

    return error;
}

int16_t scd30_set_altitude(uint16_t altitude) {
    int16_t error;

    error = sensirion_i2c_write_cmd_with_args(SCD30_I2C_ADDRESS,
                                              SCD30_CMD_SET_ALTITUDE, &altitude,
                                              SENSIRION_NUM_WORDS(altitude));
    sensirion_sleep_usec(SCD30_WRITE_DELAY_US);

    return error;
}

int16_t scd30_get_automatic_self_calibration(uint8_t* asc_enabled) {
    uint16_t word;
    int16_t error;

    error = sensirion_i2c_read_cmd(SCD30_I2C_ADDRESS,
                                   SCD30_CMD_AUTO_SELF_CALIBRATION, &word,
                                   SENSIRION_NUM_WORDS(word));
    if (error != NO_ERROR)
        return error;

    *asc_enabled = (uint8_t)word;

    return NO_ERROR;
}

int16_t scd30_enable_automatic_self_calibration(uint8_t enable_asc) {
    int16_t error;
    uint16_t asc = !!enable_asc;

    error = sensirion_i2c_write_cmd_with_args(SCD30_I2C_ADDRESS,
                                              SCD30_CMD_AUTO_SELF_CALIBRATION,
                                              &asc, SENSIRION_NUM_WORDS(asc));
    sensirion_sleep_usec(SCD30_WRITE_DELAY_US);

    return error;
}

int16_t scd30_set_forced_recalibration(uint16_t co2_ppm) {
    int16_t error;

    error = sensirion_i2c_write_cmd_with_args(
        SCD30_I2C_ADDRESS, SCD30_CMD_SET_FORCED_RECALIBRATION, &co2_ppm,
        SENSIRION_NUM_WORDS(co2_ppm));
    sensirion_sleep_usec(SCD30_WRITE_DELAY_US);

    return error;
}

int16_t scd30_read_serial(char* serial) {
    int16_t error;

    error = sensirion_i2c_write_cmd(SCD30_I2C_ADDRESS, SCD30_CMD_READ_SERIAL);
    if (error)
        return error;

    sensirion_sleep_usec(SCD30_WRITE_DELAY_US);
    error = sensirion_i2c_read_words_as_bytes(
        SCD30_I2C_ADDRESS, (uint8_t*)serial, SCD30_SERIAL_NUM_WORDS);
    serial[2 * SCD30_SERIAL_NUM_WORDS] = '\0';
    return error;
}

const char* scd30_get_driver_version() {
    return SCD_DRV_VERSION_STR;
}

uint8_t scd30_get_configured_address() {
    return SCD30_I2C_ADDRESS;
}

int16_t scd30_probe() {
    uint16_t data_ready;

    /* try to read data-ready state */
    return scd30_get_data_ready(&data_ready);
}


/**
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count) {
    return (int8_t)HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(address << 1),
                                          data, count, 100);
}

/**
 * Execute one write transaction on the I2C bus, sending a given number of
 * bytes. The bytes in the supplied buffer must be sent to the given address. If
 * the slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_write(uint8_t address, const uint8_t* data,
                           uint16_t count) {
    return (int8_t)HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(address << 1),
                                           (uint8_t*)data, count, 100);
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(uint32_t useconds) {
    uint32_t msec = useconds / 1000;
    if (useconds % 1000 > 0) {
        msec++;
    }

    /*
     * Increment by 1 if STM32F1 driver version less than 1.1.1
     * Old firmwares of STM32F1 sleep 1ms shorter than specified in HAL_Delay.
     * This was fixed with firmware 1.6 (driver version 1.1.1), so we have to
     * fix it ourselves for older firmwares
     */
    if (HAL_GetHalVersion() < 0x01010100) {
        msec++;
    }

    HAL_Delay(msec);
}
