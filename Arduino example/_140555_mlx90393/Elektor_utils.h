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

#include "stdint.h"


// Similar to printf("%02x",val);
void serial_print_hex(uint8_t val);

// Produce a readable hexdump of a buffer.
void serial_hexdump(uint8_t *p_data, uint8_t data_size);

// Produce a readable hexdump.
void serial_hexdump(uint8_t *p_data, uint16_t data_size, uint8_t bytes_per_line);

// Good for tables.
void serial_print_value_padded(uint16_t val);

// Generic I2C <start><write><repeated start><read><stop>.
// One buffer is used for TX and RX so you better make it
// large enough for both operations.
uint8_t i2c_write_read(uint8_t i2c_address, uint8_t *p_data, uint8_t tx_size, uint8_t rx_size);

// Assemble an uint16_t from <MSB,LSB>. 
uint16_t assemble_16(uint8_t *p_data);

// Assemble an uint32_t from <MSB,LSB>. 
uint32_t assemble_32(uint8_t *p_data);
