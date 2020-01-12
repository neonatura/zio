/* Copyright 2018 Neo Natura */

#include "zio.h"

static double _zio_gps_latitude;
static double _zio_gps_longitude;


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


void zio_gps_setTime(char *time_str, char *day_str)
{
	struct tm tm;
	int yr = year();
	int mon = month();
	int d = day();
	int h, m;
	float s;
	char buf[8];

	if (day_str != NULL) {
		/* day */
		memset(buf, 0, sizeof(buf));
		strncpy(buf, time_str, 2);
		d = atoi(buf);

		/* month */
		memset(buf, 0, sizeof(buf));
		strncpy(buf, time_str + 2, 2);
		mon = atoi(buf);

		/* year */
		memset(buf, 0, sizeof(buf));
		strncpy(buf, time_str + 4, 2);
		yr = atoi(buf) + 2000;
	}

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

	if (d && mon && yr) {
		memset(&tm, 0, sizeof(tm));
		tm.tm_sec = (int)s;
		tm.tm_min = m;
		tm.tm_hour = h;
		tm.tm_mday = d;
		tm.tm_mon = mon;
		tm.tm_year = yr;
		tm.tm_isdst = -1;
		time_t t = mktime(&tm);
		zio_time_set(mktime(&tm));
	}

}


/* Decimal Degrees = Degrees + minutes/60 + seconds/3600 */
void zio_gps_set(zdev_t *dev, double lat, double lon)
{
	double rlat, rlon;
	double lat_sec;
	double lon_sec;

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

	dev->fifo.dvalue = (double *)dev->fifo.value;
	dev->fifo.dvalue[0] = rlat;
	dev->fifo.dvalue[1] = rlon;
	dev->fifo.dvalue[2] = 0;

	_zio_gps_latitude = rlat;
	_zio_gps_longitude = rlon;
}


void parseGGA(zdev_t *dev, char *text)
{
	double lat, lon;
	char **vars;
	int nr;

	lat = lon = 0;
	vars = nmea_split_var(text, &nr);
	if (nr < 10)
		return;

	lat = atof(vars[2]);
	lon = atof(vars[4]);
	if (lat != 0.0 && lon != 0.0) {
		zio_gps_set(dev, lat, lon);
	}

}


void parseGLL(zdev_t *dev, char *text)
{
	float lat, lon;
	char **vars;
	int nr;

	lat = lon = 0;
	vars = nmea_split_var(text, &nr);
	if (nr < 6)
		return;

	lat = atof(vars[1]);
	lon = atof(vars[3]);
	if (lat != 0.0 && lon != 0.0) {
		zio_gps_set(dev, lat, lon);
	}

}


void parseRMC(zdev_t *dev, char *text)
{
	float lat, lon;
	float t;
	char **vars;
	int nr;

	vars = nmea_split_var(text, &nr);
	if (nr < 10) {
		return;
	}

	t = atof(vars[1]);
	if (t != 0.0) {
		zio_gps_setTime(vars[1], vars[9]);
	}

	if (0 == strcmp(vars[2], "V")) {
		return;
	}

	lat = atof(vars[3]);
	lon = atof(vars[5]);
	if (lat != 0.0 && lon != 0.0) {
		zio_gps_set(dev, lat, lon);
	}

}


void zio_gps_parse(zdev_t *dev, char *text)
{

  if (0 == strncmp(text, "GPGSV", 5) ||
      0 == strncmp(text, "GPVTG", 5) ||
      0 == strncmp(text, "GPGSA", 5))
    return;

  if (0 == strncmp(text+5, ",,,,", 4))
    return;

  if (0 == strncmp(text, "GPGGA", 5)) {
    parseGGA(dev, text);
  } else if (0 == strncmp(text, "GPGLL", 5)) {
    parseGLL(dev, text);
  } else if (0 == strncmp(text, "GPRMC", 5)) {
    parseRMC(dev, text);
  }

}


int zio_gps_init(zdev_t *dev)
{
	int fd;

	if (is_zio_dev_on(dev))
		return (0);

	fd = ZIO_UART_INIT("/dev/ttyAMA0", 115200);
	if (fd < 0)
		return (ZERR_INVAL);

	zio_dev_on(dev);
	dev->dev_fd = fd; 

	return (0);
}

int zio_gps_read(zdev_t *dev)
{
	char buf[256];
	int len;
	int fd;

	if (!zio_gps_dev_on(dev)) {
		zio_gps_init(dev);
	}

	memset(buf, 0, sizeof(buf));
	len = ZIO_UART_READ(fd, buf, sizeof(buf)-1);
	if (len >= 0) {
		zio_gps_parse(dev, buf); /* parse nmea */
	}

	return (0);
}

void zio_gps_term(zdev_t *dev)
{
	if (!zio_gps_dev_on(dev))
		return;

	close(dev->dev_fd);
	dev->dev_fd = 0;
	zio_gps_dev_off(dev);
}

double zio_gps_latitude(void)
{
	return (_zio_gps_latitude);
}

double zio_gps_longitude(void)
{
	return (_zio_gps_longitude);
}


