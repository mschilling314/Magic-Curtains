#include "i2c.h"


// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

void i2c_init(const nrf_twi_mngr_t* i2c){
    i2c_manager = i2c;
}

void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data){
    uint8_t datum[] = {reg_addr, data};
    nrf_twi_mngr_transfer_t const write_transfer[] = {
            NRF_TWI_MNGR_WRITE(i2c_addr, datum, 2, 0)
    };
    nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
}


uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
    uint8_t rx_buf = 0;
    nrf_twi_mngr_transfer_t const read_transfer[] = {
            //TODO: implement me
            NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
            NRF_TWI_MNGR_READ(i2c_addr,&rx_buf,1,0)
    };
    nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);

    return rx_buf;
}