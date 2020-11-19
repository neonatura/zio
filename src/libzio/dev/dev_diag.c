/* Copyright 2018 Neo Natura */

#include "zio.h"
 
int zio_diag_open(zdev_t *dev)
{
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	zio_dev_on(dev);

	return (0);
}
 
int zio_diag_write(zdev_t *dev, uint8_t *data, size_t data_len)
{
	dev_t *mod;

	if (!is_zio_dev_on(dev))
		return (ERR_AGAIN);

#if 0
	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_DIAG);
  if (mod)
    zio_write(mod, data, data_len);
#endif

	zio_mod_write(dev, data, data_len);
	return (0);
}

int zio_diag_close(zdev_t *dev)
{

	if (!is_zio_dev_on(dev))
		return (0);

	zio_dev_off(dev);

	return (0);
}

zdev_t zio_diag_device =
{
	"diag", PIN_NULL, STRATUM_MAX, /* contoller: activity diag */
	ZDEV_DIAG, DEVF_START | DEVF_OUTPUT | DEVF_MODULE, ZMOD_INTERNAL, 
	/* op */
	{ zio_diag_open, NULL, zio_diag_write, NULL, zio_diag_close },
	/* param */
	{ /* freq_min */ 0.5, /* freq_max */ 5, 0, PIN_NULL }
};

