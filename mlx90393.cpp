/*
  Elektor project 140555 MLX90393 BoB
  Copyright (c) 2015 Elektor.  All rights reserved.
  Author: CPV, 25/02/2015

  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this software; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "mlx90393.h"


// Up to 9 bytes may be returned.
uint8_t mlx90393_data_buffer[10];
uint8_t mlx90393_status = 0;
uint16_t mlx90393_t = 0;
uint16_t mlx90393_x = 0;
uint16_t mlx90393_y = 0;
uint16_t mlx90393_z = 0;
uint8_t mlx90393_memory[64*2]; // 64 16-bit words.


uint8_t mlx90393_command(uint8_t cmd, uint8_t zyxt, uint8_t address, uint16_t data)
{
  uint8_t rx_size = 1;
  uint8_t tx_size = 1;
  cmd &= 0xf0;
  zyxt &= 0x0f;
  address &= 0x3f;

  switch (cmd)
  {
    case MLX90393_CMD_SB:
    case MLX90393_CMD_SWOC:
    case MLX90393_CMD_SM:
      cmd |= zyxt;
      break;

    case MLX90393_CMD_RM:
      cmd |= zyxt;
      if ((zyxt&MLX90393_T)!=0) rx_size += 2;
      if ((zyxt&MLX90393_X)!=0) rx_size += 2;
      if ((zyxt&MLX90393_Y)!=0) rx_size += 2;
      if ((zyxt&MLX90393_Z)!=0) rx_size += 2;
      break;

    case MLX90393_CMD_RR:
      mlx90393_data_buffer[1] = address << 2;
      rx_size += 2;
      tx_size = 2;
      break;

    case MLX90393_CMD_WR:
      mlx90393_data_buffer[1] = (data&0xff00) >> 8;
      mlx90393_data_buffer[2] = data & 0x00ff;
      mlx90393_data_buffer[3] = address << 2;
      tx_size = 4;
      break;

    case MLX90393_CMD_NOP:
    case MLX90393_CMD_EX:
    case MLX90393_CMD_RT:
    case MLX90393_CMD_HR:
    case MLX90393_CMD_HS:
      break;
  }

  mlx90393_data_buffer[0] = cmd;
  return i2c_write_read(MLX90393_I2C_ADDRESS,(uint8_t*)mlx90393_data_buffer,tx_size,rx_size);
}


void mlx90393_decode(uint8_t zyxt)
{
  uint8_t *p = (uint8_t *)mlx90393_data_buffer;
  mlx90393_status = *p;
  p += 1;
  if ((zyxt&MLX90393_T)!=0)
  {
    mlx90393_t = assemble_16(p);
    p += 2;
  }
  if ((zyxt&MLX90393_X)!=0)
  {
    mlx90393_x = assemble_16(p);
    p += 2;
  }
  if ((zyxt&MLX90393_Y)!=0)
  {
    mlx90393_y = assemble_16(p);
    p += 2;
  }
  if ((zyxt&MLX90393_Z)!=0)
  {
    mlx90393_z = assemble_16(p);
    p += 2;
  }
}


uint16_t mlx90393_read_memory_word(uint8_t address)
{
  mlx90393_command(MLX90393_CMD_RR,0,address,0);
  // mlx90393_data_buffer[0] contains status code.
  return assemble_16((uint8_t*)&mlx90393_data_buffer[1]);
}


void mlx90393_write_memory_word(uint8_t address, uint8_t data)
{
  mlx90393_command(MLX90393_CMD_WR,0,address,data);
}


void mlx90393_read_memory(uint8_t *p_dst, uint8_t address, uint8_t size)
{
  uint16_t val;
  while (size!=0)
  {
     val = mlx90393_read_memory_word(address);
     *p_dst = (val>>8)&0xff;
     p_dst += 1;
     *p_dst = val&0xff;
     p_dst += 1;
     address += 1;
     size -= 1;
  }
}
