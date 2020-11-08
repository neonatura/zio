
/*
 *  Copyright 2020 Brian Burrell
 *
 *  This file is part of libhtm.
 *  (https://github.com/neonatura/zio)
 *
 *  libhtm is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Shionbot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libhtm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "htm.h"

htime htime_now(void)
{
  struct timeval tv;
  htime ret_stamp;
  uint64_t secs;
  uint64_t ms;
  
  memset(&tv, 0, sizeof(tv));
  gettimeofday(&tv, NULL);
  secs = (uint64_t)(tv.tv_sec - HTIME_EPOCH);
  ms = (uint64_t)(tv.tv_usec / 10000);

  return ( (secs * 100) + ms );
}

void htm_yield(void)
{
	pthread_yield();
}

void htm_msleep(int ms)
{
	usleep(ms * 1000);
}

