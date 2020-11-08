/* Copyright 2018 Neo Natura */

#include "zio.h"

static char **nmea_split_var(char *text, int *idx_p)
{
  static char *split_buf[16];
  static char raw[64];
  char buf[128];
  char *tok;
  int len;
  int idx;
  int of;

  memset(raw, 0, sizeof(raw));
  strncpy(raw, text, sizeof(raw)-1);

  idx = 1;
  split_buf[0] = raw;

  len = strlen(raw);
  for (of = 0; of < len; of++) {
    if (raw[of] == ',') {
      split_buf[idx++] = raw + (of + 1);
      raw[of] = '\000';
      if (idx == 16) break;
    }
  }

  *idx_p = idx;

  return ((char **)split_buf);
}

/* a gps device requesting to set the time. */
int zio_geo_time_set(zdev_t *dev, char *time_str, char *day_str)
{
	zdev_t *gtime_dev;
	struct tm tm;
	uint64_t ret_t;
	int yr;// = year();
	int mon;// = month();
	int d;// = day();
	int h, m;
	float s;
	char buf[8];

	gtime_dev = zio_dev_get_name(dev, "gtime");
	if (!gtime_dev)
		return (0);

	if (day_str == NULL)
		return (0);

	/* day */
	memset(buf, 0, sizeof(buf));
	strncpy(buf, day_str, 2);
	d = atoi(buf);

	/* month */
	memset(buf, 0, sizeof(buf));
	strncpy(buf, day_str + 2, 2);
	mon = atoi(buf);

	/* year */
	memset(buf, 0, sizeof(buf));
	strncpy(buf, day_str + 4, 2);
	yr = atoi(buf);

	/* hour */
	memset(buf, 0, sizeof(buf));
	strncpy(buf, time_str, 2);
	h = atoi(buf);

	/* min */
	memset(buf, 0, sizeof(buf));
	strncpy(buf, time_str + 2, 2);
	m = atoi(buf);

	/* sec */
	memset(buf, 0, sizeof(buf));
	strcpy(buf, time_str + 4);
	s = atof(buf);

	memset(&tm, 0, sizeof(tm));
	tm.tm_sec = (int)s;
	tm.tm_min = m;
	tm.tm_hour = h;
	tm.tm_mday = d;
	tm.tm_mon = mon - 1;
	tm.tm_year = yr + 100;
	tm.tm_isdst = -1;
	ret_t = zio_timegm(&tm);

	zio_value_set(gtime_dev, ret_t);

	return (0);
}

/* Decimal Degrees = Degrees + minutes/60 + seconds/3600 */
/* a gps device requesting to set the location. */
void zio_geo_set(zgeo_t *geo, double lat, double lon)
{
	double rlat, rlon;
	double lat_sec;
	double lon_sec;

	memset(geo, 0, sizeof(zgeo_t));

	rlat = floor(lat/100);
	rlon = floor(lon/100);

	lat -= (rlat * 100);
	lon -= (rlon * 100);
	lat_sec = (lat - floor(lat));
	lon_sec = (lon - floor(lon));

	/* minutes */
	rlat += (lat - lat_sec) / 60;
	rlon += (lon - lon_sec) / 60;

	/* seconds */
	rlat += ((lat_sec * 100) / 3600);
	rlon += ((lon_sec * 100) / 3600);

#if 0
	/* global reference */
	zio_geo_latitude_set(rlat);
	zio_geo_longitude_set(rlon);
#endif

	geo->lat = rlat;	
	geo->lon = rlon;
	geo->stamp = zio_time();

}

zgeo_t *zio_geo_parse_GGA(zdev_t *dev, char *text)
{
	static zgeo_t ret_geo;
	double lat, lon;
	char **vars;
	int nr;

	lat = lon = 0;
	vars = nmea_split_var(text, &nr);
	if (nr < 10)
		return (NULL);

	lat = atof(vars[2]);
	lon = atof(vars[4]);
	if (lat == 0.0 && lon == 0.0)
		return (NULL);

	zio_geo_set(&ret_geo, lat, lon);
	return (&ret_geo);
}

zgeo_t *zio_geo_parse_GLL(zdev_t *dev, char *text)
{
	static zgeo_t ret_geo;
	float lat, lon;
	char **vars;
	int nr;

	lat = lon = 0;
	vars = nmea_split_var(text, &nr);
	if (nr < 6)
		return;

	lat = atof(vars[1]);
	lon = atof(vars[3]);
	if (lat == 0.0 && lon == 0.0)
		return (NULL);

	zio_geo_set(&ret_geo, lat, lon);
	return (&ret_geo);
}

zgeo_t *zio_geo_parse_RMC(zdev_t *dev, char *text)
{
	static zgeo_t ret_geo;
	float lat, lon;
	float t;
	char **vars;
	int nr;

	vars = nmea_split_var(text, &nr);
	if (nr < 10) {
		return (NULL);
	}

	t = atof(vars[1]);
	if (t != 0.0) {
		zio_geo_time_set(dev, vars[1], vars[9]);
	}

	if (0 == strcmp(vars[2], "V")) {
		return (NULL);
	}

	lat = atof(vars[3]);
	lon = atof(vars[5]);
	if (lat == 0.0 && lon == 0.0)
		return (NULL);

	zio_geo_set(&ret_geo, lat, lon);
	return (&ret_geo);
}

int zio_gps_parse(zdev_t *dev, char *text)
{
	zgeo_t *geo;

	if (*text != '$')
		return;
	text++;

	if (0 == strncmp(text, "GPGSV", 5) ||
			0 == strncmp(text, "GPVTG", 5) ||
			0 == strncmp(text, "GPGSA", 5))
		return (ZERR_AGAIN);

	if (0 == strncmp(text+5, ",,,,", 4))
		return (ZERR_AGAIN);

	geo = NULL;
	if (0 == strncmp(text, "GPGGA", 5)) {
		geo = zio_geo_parse_GGA(dev, text);
	} else if (0 == strncmp(text, "GPGLL", 5)) {
		geo = zio_geo_parse_GLL(dev, text);
	} else if (0 == strncmp(text, "GPRMC", 5)) {
		geo = zio_geo_parse_RMC(dev, text);
	}
	if (!geo)
		return (ZERR_AGAIN);

	zio_data_append(dev, (uint8_t *)geo, sizeof(zgeo_t));		

	return (0);
}

int zio_gps_open(zdev_t *dev)
{
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	fd = ZIO_UART_INIT(dev, dev->def_pin, 9600);
	if (fd < 0)
		return (ZERR_INVAL);

	dev->dev_fd = fd; 
	zio_dev_on(dev);

	return (0);
}

int zio_gps_read(zdev_t *dev)
{
	char buf[1024];
	char *line;
	char *cur;
	char *next;
	int err;
	int tot;
	int len;
	int fd;

	memset(buf, 0, sizeof(buf));
	len = ZIO_UART_READ(dev, buf, sizeof(buf)-1);
	if (len <= 0)
		return (ZERR_AGAIN);

	cur = buf;
	while (next = strchr(cur, '\n')) {
		err = zio_gps_parse(dev, cur); /* parse nmea */
		if (!err) tot++;
		cur = next + 1;
	}
	if (!tot)
		return (ZERR_AGAIN);

	return (0);
}

int zio_gps_close(zdev_t *dev)
{
	if (!zio_dev_on(dev))
		return (0);

	dev->dev_fd = 0;
	zio_dev_off(dev);

	return (0);
}

int zio_gps_poll(zdev_t *dev)
{
        int err;

        if (is_zio_dev_on(dev)) {
                err = zio_gps_read(dev);
                if (err)
                        return (err);
        }

        return (0);
}

int zio_gps_print(zdev_t *dev, int mode, void *retbuf)
{
	zgeo_t *geo;
        char buf[256];

	geo = zio_geo_value(dev);
	if (!geo)
		return (ZERR_AGAIN);

	sprintf(buf, "@%f,%f", geo->lat, geo->lon);
	strcpy((char *)retbuf, buf);

        return (0);
}

zdev_t zio_gps_device =
{
        "gps", ZIO_UART1, 0, /* contoller: local gps */
        ZDEV_GEO, DEVF_START, ZMOD_INTERNAL,
        /* op */
        { zio_gps_open, zio_gps_read, NULL, zio_gps_print, zio_gps_close, zio_gps_poll },
        /* param */
        { /* freq_min */ 0.25, /* freq_max */ 1, 0.15, PIN_NULL }
};


