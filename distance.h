
#include <M5Stack.h>

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define ToF_I2C_addr                                0x29
#define PaHub_I2C_ADDRESS                           0x70

ClosedCube::Wired::TCA9548A tca9548a;

byte gbuf[16];

uint16_t bswap(byte b[]) {
    // Big Endian unsigned short to little endian unsigned short
    uint16_t val = ((b[0] << 8) & b[1]);
    return val;
}

uint16_t makeuint16(int lsb, int msb) {
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}

void write_byte_data(byte data) {
    Wire.beginTransmission(ToF_I2C_addr);
    Wire.write(data);
    Wire.endTransmission();
}

void write_byte_data_at(byte reg, byte data) {
    // write data word at ToF_I2C_addr and register
    Wire.beginTransmission(ToF_I2C_addr);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

void write_word_data_at(byte reg, uint16_t data) {
    // write data word at ToF_I2C_addr and register
    byte b0 = (data & 0xFF);
    byte b1 = ((data >> 8) && 0xFF);

    Wire.beginTransmission(ToF_I2C_addr);
    Wire.write(reg);
    Wire.write(b0);
    Wire.write(b1);
    Wire.endTransmission();
}

byte read_byte_data() {
    Wire.requestFrom(ToF_I2C_addr, 1);
    while (Wire.available() < 1) delay(1);
    byte b = Wire.read();
    return b;
}

byte read_byte_data_at(byte reg) {
    //write_byte_data((byte)0x00);
    write_byte_data(reg);
    Wire.requestFrom(ToF_I2C_addr, 1);
    while (Wire.available() < 1) delay(1);
    byte b = Wire.read();
    return b;
}

uint16_t read_word_data_at(byte reg) {
    write_byte_data(reg);
    Wire.requestFrom(ToF_I2C_addr, 2);
    while (Wire.available() < 2) delay(1);
    gbuf[0] = Wire.read();
    gbuf[1] = Wire.read();
    return bswap(gbuf);
}

void read_block_data_at(byte reg, int sz) {
    int i = 0;
    write_byte_data(reg);
    Wire.requestFrom(ToF_I2C_addr, sz);
    for (i = 0; i < sz; i++) {
        while (Wire.available() < 1) delay(10);
        gbuf[i] = Wire.read();
    }
}

uint16_t VL53L0X_decode_vcsel_period(short vcsel_period_reg) {
    // Converts the encoded VCSEL period register value into the real
    // period in PLL clocks
    uint16_t vcsel_period_pclks = (vcsel_period_reg + 1) << 1;
    return vcsel_period_pclks;
}
    
uint16_t getdistance(int i) {
  tca9548a.selectChannel (i); 
//  M5.Lcd.setCursor(0, 200, 4);
//  M5.Lcd.println("b : ");
//  M5.Lcd.println(i);

  write_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);
  delay(10);
  byte val = 0;
  int cnt  = 0;
  while (cnt < 100) {  // 1 second waiting time max
    delay(2);
    val = read_byte_data_at(VL53L0X_REG_RESULT_RANGE_STATUS);
    if (val & 0x01) 
      break;
    cnt++;
  }
/*  
 if (val & 0x01)
    Serial.println("ready");
  else
    Serial.println("not ready");
*/
  read_block_data_at(0x14, 12);
//  uint16_t acnt = makeuint16(gbuf[7], gbuf[6]);
//  uint16_t scnt = makeuint16(gbuf[9], gbuf[8]);
  uint16_t dist = makeuint16(gbuf[11], gbuf[10]);
//  byte DeviceRangeStatusInternal = ((gbuf[0] & 0x78) >> 3);

  if (dist == 20) dist = 2000;
  return dist;
}
