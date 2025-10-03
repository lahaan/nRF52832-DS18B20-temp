#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

const struct device *const ds18b20_dev = DEVICE_DT_GET_ONE(maxim_ds18b20);

/*

	Setup:
	DS18B20  - NRF52DK (52832, same as RAK4600)
	"digital (onewire) pin" (DQ) - p0.11 on nrf52dk 
	VCC - 3.3V (VDD)
	GND - GND
	2.5K pullup resistor between DQ and VCC (2x 5k in parallel)
	100nF MLCC cap between VCC and GND
	more deets: https://github.com/sigurdnev/Nordic-DS18B20?tab=readme-ov-file

*/

void main(void)
{
    if (!device_is_ready(ds18b20_dev)) {
        LOG_ERR("DS18B20 not ready");
        return;
    }

    while (1) {
        if (sensor_sample_fetch(ds18b20_dev) == 0) {
            struct sensor_value temp;
            if (sensor_channel_get(ds18b20_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) == 0) {
                printk("Temperature: %.2f °C\n", sensor_value_to_double(&temp));
            }
        }
        k_sleep(K_SECONDS(1));
    }
}