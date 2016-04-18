/*
 * Utility functions for Arduino.
 *
 * Things that you often need, so why not keep them together in one file?
 *
 * Author: Clemens Valens
 * Date: 2015/02/25
 * 
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Arduino.h>
#include <Wire.h>
#include "Elektor_utils.h"


// Similar to printf("%02x",val);
void serial_print_hex(uint8_t val)
{
  static char to_hex[16] = 
  { 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' 
  };
  Serial.print(to_hex[(val>>4)&0x0f]);
  Serial.print(to_hex[val&0x0f]);
}


// Produce a readable hexdump of a buffer.
void serial_hexdump_buffer(uint8_t *p_data, uint8_t data_size)
{
  int i;
  for (i=0; i<data_size; i++)
  {
    if (i>0) Serial.print(" ");
    serial_print_hex(p_data[i]);
  }
  Serial.print(" - ");
  for (i=0; i<data_size; i++)
  {
    if (p_data[i]>31 && p_data[i]<127) Serial.print((char)p_data[i]);
    else Serial.print('.');
  }
  Serial.println();
}

// Produce a readable hexdump.
void serial_hexdump(uint8_t *p_data, uint16_t data_size, uint8_t bytes_per_line)
{
  uint16_t s;
  while (data_size!=0)
  {
    s = data_size<bytes_per_line? data_size : bytes_per_line;
    serial_hexdump_buffer(p_data,s);
    p_data += bytes_per_line;
    data_size -= s;
  }
}

// Good for tables.
void serial_print_value_padded(uint16_t val)
{
  if (val<10) Serial.print(' ');
  if (val<100) Serial.print(' ');
  if (val<1000) Serial.print(' ');
  if (val<10000) Serial.print(' ');
  Serial.print(val);
}


// Generic I2C <start><write><repeated start><read><stop>.
// One buffer is used for TX and RX so you better make it
// large enough for both operations.
uint8_t i2c_write_read(uint8_t i2c_address, uint8_t *p_data, uint8_t tx_size, uint8_t rx_size)
{
  int i;
  Wire.beginTransmission(i2c_address);
  for (i=0; i<tx_size; i++)
  {
    Wire.write(p_data[i]);
  }
  Wire.endTransmission(false);
  Wire.requestFrom(i2c_address,rx_size,(uint8_t)true);
  i = 0;
  while (Wire.available()!=0 && i<rx_size)
  {
    p_data[i] = Wire.read();
    i += 1;
  }
  return i;
}


uint16_t assemble_16(uint8_t *p_data)
{
  uint16_t result = p_data[0];
  result = (result<<8) + p_data[1];
  return result;
}


uint32_t assemble_32(uint8_t *p_data)
{
  int i;
  uint32_t result = p_data[0];
  for (i=1; i<4; i++)
  {
    result = (result<<8) + p_data[i];
  }
  return result;
}
