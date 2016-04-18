About the MLX90393 library

The MLX90393 library supports the chip in I2C mode but SPI mode is easy enough to implement. Only one call (to i2c_write_read) has to be replaced by an SPI equivalent.

An effort has been made to make the library as user-friendly as possible and functions have been provided to control every possible parameter of the chip.

The library is written in C and can be used in C++ projects too.

A simple Arduino example is included showing how to use the driver.

Related Elektor projects:
- 140555 MLX90393 software-defined 3-axis magnetometer, https://www.elektormagazine.com/140555
