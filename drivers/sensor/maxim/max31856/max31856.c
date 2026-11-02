/*
 * Copyright (c) 2026 Vincent Surkijn
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#define DT_DRV_COMPAT maxim_max31856

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#warning "MAX31856 driver enabled without any devices"
#endif

#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>

LOG_MODULE_REGISTER(MAX31856, CONFIG_SENSOR_LOG_LEVEL);

struct max31856_config {
	struct spi_dt_spec spi;
};

struct max31856_data {
	uint32_t sample;
};

static int max31856_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	return 0;
}

static int max31856_channel_get(const struct device *dev, enum sensor_channel chan,
				struct sensor_value *val)
{
	return 0;
}

static DEVICE_API(sensor, max31856_api) = {
	.sample_fetch = max31856_sample_fetch,
	.channel_get = max31856_channel_get,
};

static int max31856_init(const struct device *dev)
{
	const struct max31856_config *config = dev->config;

	if (!spi_is_ready_dt(&config->spi)) {
		LOG_ERR_DEVICE_NOT_READY(config->spi.bus);
		return -ENODEV;
	}

	return 0;
}

#define MAX31856_INIT(n)                                                                           \
	static struct max31856_data max31856_data_##n;                                             \
	static const struct max31856_config max31856_config_##n = {                                \
		.spi = SPI_DT_SPEC_INST_GET(n, SPI_OP_MODE_CONTROLLER | SPI_WORD_SET(8U)),         \
	};                                                                                         \
	SENSOR_DEVICE_DT_INST_DEFINE(n, &max31856_init, NULL, &max31856_data_##n,                  \
				     &max31856_config_##n, POST_KERNEL,                            \
				     CONFIG_SENSOR_INIT_PRIORITY, &max31856_api);

DT_INST_FOREACH_STATUS_OKAY(MAX31856_INIT)
