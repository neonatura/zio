/* Copyright 2018 Neo Natura */

#ifndef __ZIO_CTL_H__
#define __ZIO_CTL_H__


#define ZCTL_PING 0
#define ZCTL_RESET 1
#define ZCTL_WAKE 2
#define ZCTL_SLEEP 3
#define ZCTL_FREQ 4
#define ZCTL_HUMIDITY 5

typedef struct zioctl_t
{
        uint32_t reg;
        uint32_t set;
} zioctl_t;


int zioctl_get(zdev_t *dev, int reg);

int zioctl(zdev_t *dev, int reg, int set, uint8_t *raw, size_t raw_len);


#endif /* ndef __ZIO_CTL_H__ */

