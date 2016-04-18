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

#include <Wire.h>
#include "mlx90393.h"
#include "Elektor_utils.h"
char *p_msg = "http://www.elektor-labs.com/140555/";


#define HOST_DATA_SIZE  (4)
int host_data[HOST_DATA_SIZE] = { 0, 0, 0, 0 };
int field = 0;
int multiplier = 1;


void print_pretty_table(void)
{
  serial_print_value_padded(mlx90393_t);
  Serial.print(' ');
  serial_print_value_padded(mlx90393_x);
  Serial.print(' ');
  serial_print_value_padded(mlx90393_y);
  Serial.print(' ');
  serial_print_value_padded(mlx90393_z);
  Serial.println();
}

void do_read_eeprom_word(int i)
{
  mlx90393_read_memory_word(i);
  Serial.print(mlx90393_data_buffer[0]);
  Serial.print(mlx90393_data_buffer[1]);
  Serial.print(mlx90393_data_buffer[2]);
  Serial.println();
}
  
void do_command(int *p_host_data)
{
  //Serial.print(p_host_data[0]);
  //Serial.print(',');
  //Serial.print(p_host_data[1]);
  //Serial.print(',');
  //Serial.print(p_host_data[2]);
  //Serial.print(',');
  //Serial.print(p_host_data[3]);
  //Serial.println();
      
  switch (p_host_data[0])
  {
    case 0:
      // Read EEPROM word.
      //Serial.println("Read EEPROM word.");
      do_read_eeprom_word(p_host_data[1]);
      return;

    case 1:
      // Read EEPROM full.
      //Serial.println("Read EEPROM full.");
      for (int i=0; i<63; i++) 
      {
        do_read_eeprom_word(i);
      }
      return;

    case 2:
      // Write register.
      //Serial.println("Write register.");
      mlx90393_write_memory_word(p_host_data[1],p_host_data[2]);
      break;

    case 3:
      //Serial.println("Not implemented.");
      return;

    case 4:
      // Reset.
      //Serial.println("Reset.");
      mlx90393_command(MLX90393_CMD_RT,0,0,0);
      break;

    case 5:
      // Exit.
      //Serial.println("Exit.");
      mlx90393_command(MLX90393_CMD_EX,0,0,0);
      break;

    case 6:
      // Single measurement.
      //Serial.println("Single measurement.");
      mlx90393_command(MLX90393_CMD_SM,p_host_data[1],0,0);
      break;

    case 7:
      // Read measurement.
      //Serial.println("Read measurement.");
      mlx90393_command(MLX90393_CMD_RM,p_host_data[1],0,0);
      Serial.print(mlx90393_data_buffer[0]);
      Serial.print(mlx90393_data_buffer[1]);
      Serial.print(mlx90393_data_buffer[2]);
      Serial.print(mlx90393_data_buffer[3]);
      Serial.print(mlx90393_data_buffer[4]);
      Serial.print(mlx90393_data_buffer[5]);
      Serial.print(mlx90393_data_buffer[6]);
      Serial.print(mlx90393_data_buffer[7]);
      Serial.print(mlx90393_data_buffer[8]);
      Serial.print(mlx90393_data_buffer[9]);
      Serial.print(mlx90393_data_buffer[10]);
      Serial.println();
      return;

    case 8:
      // Burst.
      //Serial.println("Burst.");
      mlx90393_command(MLX90393_CMD_SB,p_host_data[1],0,0);
      break;

    case 9:
      // Wake up on change.
      //Serial.println("Wake up on change.");
      mlx90393_command(MLX90393_CMD_SWOC,p_host_data[1],0,0);
      break;

    case 10:
      // Memory recall.
      //Serial.println("Memory recall.");
      mlx90393_command(MLX90393_CMD_HR,0,0,0);
      break;

    case 11:
      // Memory store.
      //Serial.println("Memory store.");
      mlx90393_command(MLX90393_CMD_HS,0,0,0);
      break;
      
    default:
      //Serial.println("Unhandled.");
      return;
  }
  Serial.print(mlx90393_data_buffer[0]);
  Serial.println();
}
  
void setup(void)
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Elektor 140555 MLX90393 BoB");
  Serial.println(p_msg);

  Wire.begin();
  //mlx90393_read_memory((uint8_t*)mlx90393_memory,0,64);
  //serial_hexdump((uint8_t*)mlx90393_memory,2*64,16);
}

void loop(void)
{
  uint8_t i;
  uint8_t zyxt = MLX90393_T | MLX90393_X | MLX90393_Y | MLX90393_Z;
  //uint8_t zyxt = MLX90393_Z;

  mlx90393_command(MLX90393_CMD_SM,zyxt,0,0);
  // Serial.print("SM: ");
  // serial_print_hex(mlx90393_data_buffer[0]);
  // Serial.println();

  delay(500);

  mlx90393_command(MLX90393_CMD_RM,zyxt,0,0);
  // Serial.print("RM: ");
  // for (i=0; i<9; i++)
  // {
  //   serial_print_hex(mlx90393_data_buffer[i]);
  //   Serial.print(' ');
  // }
  // Serial.println();

  mlx90393_decode(zyxt);
  print_pretty_table();
  
  delay(500);
  
  /*if (Serial.available())
  {
    int ch = Serial.read();
    if (ch==',')
    {
      field += 1; // Next field.
      host_data[field] = 0; // Clear field.
      multiplier = 1; // Reset multiplier.
    }
    else if (ch>='0' && ch<='9' && field<HOST_DATA_SIZE)
    {
      host_data[field] = multiplier*host_data[field] + (ch-'0');
      multiplier *= 10;
    }
    else
    {
      if (field==3) do_command(host_data);
      field = 0; // First field.
      host_data[field] = 0; // Clear field.
      multiplier = 1;
    }
  }*/
    
}

