/* Copyright 2018 Neo Natura */

#ifndef __ZIO_RTC_H__
#define __ZIO_RTC_H__

extern zdev_t zio_rtc_device;

#define REGISTER_RTC_DEVICE() (zio_dev_register(&zio_rtc_device))

#endif /* ndef __ZIO_RTC_H__ */

