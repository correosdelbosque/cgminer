/*
 * Copyright 2013-2014 Con Kolivas <kernel@kolivas.org>
 * Copyright 2012-2014 Xiangfu <xiangfu@openmobilefree.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  See COPYING for more details.
 */

#ifndef _AVALON2_H_
#define _AVALON2_H_

#include "util.h"
#include "fpgautils.h"

#ifdef USE_AVALON2

#define AVA2_MINER_THREADS	1

#define AVA2_RESET_FAULT_DECISECONDS	10
#define AVA2_IO_SPEED		115200

#define AVA2_DEFAULT_MODULARS	4

#define AVA2_PWM_MAX	0x3FF
#define AVA2_DEFAULT_FAN_PWM	15 /* % */
#define AVA2_DEFAULT_FAN_MIN	10
#define AVA2_DEFAULT_FAN_MAX	85

#define AVALON2_TEMP_OVERHEAT	98
#define AVALON2_DEFAULT_POLLING_DELAY	20 /* ms */

#define AVA2_DEFAULT_VOLTAGE_MIN	6000
#define AVA2_DEFAULT_VOLTAGE_MAX	11000

#define AVA2_DEFAULT_FREQUENCY_MIN	200
#define AVA2_DEFAULT_FREQUENCY_MAX	2000

/* Avalon2 default values */
#define AVA2_DEFAULT_MINERS	10
#define AVA2_DEFAULT_VOLTAGE	10000 /* v * 10000 */
#define AVA2_DEFAULT_FREQUENCY	1500 /* In MHs */

/* Avalon3 default values */
#define AVA2_AVA3_MINERS	5
#define AVA2_AVA3_VOLTAGE	6660 /* 0.666v */
#define AVA2_AVA3_FREQUENCY	450  /* MHz * 11.8 = MHs: 450MHz means ~5.3GHs */

/* Avalon4 default values */
#define AVA2_AVA4_MINERS	10
#define AVA2_AVA4_VOLTAGE	7000 /* 0.666v */
#define AVA2_AVA4_FREQUENCY	200  /* MHz * 11.8 = MHs: 450MHz means ~5.3GHs */
#define AVA2_AVA4_AUCSPEED	1000000
#define AVA2_AVA4_AUCXDELAY  4800

/* Avalon2 protocol package type */
#define AVA2_H1	'A'
#define AVA2_H2	'V'

#define AVA2_P_COINBASE_SIZE	(6 * 1024)
#define AVA2_P_MERKLES_COUNT	20

#define AVA2_P_COUNT	39
#define AVA2_P_DATA_LEN		(AVA2_P_COUNT - 7)

#define AVA2_P_DETECT	10
#define AVA2_P_STATIC	11
#define AVA2_P_JOB_ID	12
#define AVA2_P_COINBASE	13
#define AVA2_P_MERKLES	14
#define AVA2_P_HEADER	15
#define AVA2_P_POLLING	16
#define AVA2_P_TARGET	17
#define AVA2_P_REQUIRE	18
#define AVA2_P_SET	19
#define AVA2_P_TEST	20

#define AVA2_P_ACK		21
#define AVA2_P_NAK		22
#define AVA2_P_NONCE		23
#define AVA2_P_STATUS		24
#define AVA2_P_ACKDETECT	25
#define AVA2_P_TEST_RET		26

/* Only for addressing */
#define AVA2_P_DISCOVER		30
#define AVA2_P_SETDEVID		31

#define AVA2_P_ACKDISCOVER	40
#define AVA2_P_ACKSETDEVID	41

#define AVA2_MODULE_BROADCAST	0

/* Avalon2/3 firmware prefix */
#define AVA2_FW2_PREFIXSTR	"20"
#define AVA2_FW3_PREFIXSTR	"33"
#define AVA2_FW35_PREFIXSTR	"35"
#define AVA2_FW4_PREFIXSTR	"40"

#define AVA2_MM_VERNULL		"NONE"

#define AVA2_ID_AVA2		3255
#define AVA2_ID_AVA3		3233
#define AVA2_ID_AVA4		3222
#define AVA2_ID_AVAX		3200

#define AVA2_IIC_RESET		0xa0
#define AVA2_IIC_INIT		0xa1
#define AVA2_IIC_DEINIT		0xa2
#define AVA2_IIC_WRITE		0xa3
#define AVA2_IIC_READ		0xa4
#define AVA2_IIC_XFER		0xa5
#define AVA2_IIC_INFO		0xa6

#define AVA2_DNA_LEN		8

enum avalon2_fan_fixed {
	FAN_FIXED,
	FAN_AUTO,
};

struct avalon2_pkg {
	uint8_t head[2];
	uint8_t type;
	uint8_t idx;
	uint8_t cnt;
	uint8_t data[32];
	uint8_t crc[2];
};
#define avalon2_ret avalon2_pkg

struct avalon2_info {
	cglock_t update_lock;

	struct timeval last_stratum;
	struct pool pool0;
	struct pool pool1;
	struct pool pool2;
	int pool_no;

	int modulars[AVA2_DEFAULT_MODULARS];
	char mm_version[AVA2_DEFAULT_MODULARS][16];
	char mm_dna[AVA2_DEFAULT_MODULARS][AVA2_DNA_LEN];
	int dev_type[AVA2_DEFAULT_MODULARS];
	bool enable[AVA2_DEFAULT_MODULARS];

	int set_frequency;
	int set_voltage;

	int get_voltage[AVA2_DEFAULT_MODULARS];
	int get_frequency[AVA2_DEFAULT_MODULARS];
	int power_good[AVA2_DEFAULT_MODULARS];

	int fan_pwm;
	int fan_pct;
	int temp_max;
	int auc_temp;

	int fan[2 * AVA2_DEFAULT_MODULARS];
	int temp[2 * AVA2_DEFAULT_MODULARS];

	int local_works[AVA2_DEFAULT_MODULARS];
	int hw_works[AVA2_DEFAULT_MODULARS];

	int local_work[AVA2_DEFAULT_MODULARS];
	int hw_work[AVA2_DEFAULT_MODULARS];
	int matching_work[AVA2_DEFAULT_MINERS * AVA2_DEFAULT_MODULARS];
	int chipmatching_work[AVA2_DEFAULT_MINERS * AVA2_DEFAULT_MODULARS][4];

	int led_red[AVA2_DEFAULT_MODULARS];
};

struct avalon2_iic_info {
	uint8_t iic_op;
	union {
		uint32_t aucParam[2];
		uint8_t slave_addr;
	} iic_param;
};

struct avalon2_discover_info {
	char mm_dna[AVA2_DNA_LEN];
	int modular_id;
	char mm_version[16];
};

#define AVA2_WRITE_SIZE (sizeof(struct avalon2_pkg))
#define AVA2_READ_SIZE AVA2_WRITE_SIZE
#define AVA2_IIC_P_SIZE	64

#define AVA2_GETS_OK 0
#define AVA2_GETS_TIMEOUT -1
#define AVA2_GETS_RESTART -2
#define AVA2_GETS_ERROR -3
#define AVA2_GETS_EXTRA_DATA -4

#define AVA2_SEND_OK 0
#define AVA2_SEND_ERROR -1

#define avalon2_open(devpath, baud, purge)  serial_open(devpath, baud, AVA2_RESET_FAULT_DECISECONDS, purge)
#define avalon2_close(fd) close(fd)

extern char *set_avalon2_fan(char *arg);
extern char *set_avalon2_freq(char *arg);
extern char *set_avalon2_voltage(char *arg);
extern char *set_avalon2_fixed_speed(enum avalon2_fan_fixed *f);
extern enum avalon2_fan_fixed opt_avalon2_fan_fixed;
extern int opt_avalon2_overheat;
extern int opt_avalon2_polling_delay;
extern int opt_avalon2_aucspeed;
extern int opt_avalon2_aucxdelay;
#endif /* USE_AVALON2 */
#endif	/* _AVALON2_H_ */
