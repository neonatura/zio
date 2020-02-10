/* Copyright 2018 Neo Natura */

#include "zio.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#define ZIO_OBJ_SELF 1

static uint32_t _zio_obj_index;

static zobj_t *_zio_object_table;

static uint64_t _zio_self_id(void)
{
	struct ifreq buffer;
	uint8_t *raw;
	uint64_t crc;
	uint64_t run;
	int err;
	int i;
	int s;

	memset(&buffer, 0, sizeof(buffer));
	strcpy(buffer.ifr_name, "eth0");

	s = socket(PF_INET, SOCK_DGRAM, 0);

	err = ioctl(s, SIOCGIFHWADDR, &buffer);
	if (err < 0) {
		memset(&buffer, 0, sizeof(buffer));
		strcpy(buffer.ifr_name, "enp2s0");
		err = ioctl(s, SIOCGIFHWADDR, &buffer);
	}

	close(s);

	run = 0;
	crc = 0x8800 + htonl(0x8800);
	raw = (uint8_t *)buffer.ifr_hwaddr.sa_data;
	for (i = 0; i < 6; i++) {
		crc += (uint64_t)raw[i] << i;
		if (run) crc *= run;
		run = (uint64_t)(raw[i] & 0x7f);
	}

	return (crc);
}

zobj_t *zio_obj_new(int flags)
{
	zobj_t *obj;

	_zio_obj_index++;

	obj = (zobj_t *)calloc(1, sizeof(zobj_t));
	if (!obj)
		return (NULL);

	/* intialize */
	obj->id = _zio_obj_index;
	obj->stamp = zio_time();

	/* prepend to object list. */
	obj->next = _zio_object_table;
	_zio_object_table = obj;

	return (obj);
}

zobj_t *zio_obj_get(int id)
{
	zobj_t *obj;

	for (obj = _zio_object_table; obj; obj = obj->next) {
		if (obj->id == id)
			return (obj);
	}

	return (NULL);
}

zobj_t *zio_obj_self(void)
{
	zobj_t *obj;

	obj = zio_obj_get(ZIO_OBJ_SELF);
	if (!obj) {
		obj = zio_obj_new(0);
		if (!obj)
			return (NULL); /* error */

		/* self attributes */
		obj->rad = 0.254; /* m^2 spherical chicken */
		obj->hashid = _zio_self_id(); 
	}

	return (obj);
}

