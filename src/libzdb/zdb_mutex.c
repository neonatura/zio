/*
 * @copyright
 *
 *  Copyright 2016 Neo Natura
 *
 *  This file is part of the zio project.
 *  (https://github.com/neonatura/zio)
 *        
 *  The zio project is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version. 
 *
 *  The zio project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the zio project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @endcopyright
 */  

#include "zdb.h"

void zdb_mutex_init(zdb_t *db)
{
	if (!db) return;
#ifdef USE_LIBPTHREAD
	pthread_mutexattr_t attr;

	/* initialize mutex attributes */
	memset(&attr, 0, sizeof(attr));
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	/* initialize mutex */
	pthread_mutex_init(&db->lk, &attr);
	pthread_mutexattr_destroy(&attr);
#endif
}

void zdb_mutex_term(zdb_t *db)
{
	if (!db) return;
#ifdef USE_LIBPTHREAD
	pthread_mutex_destroy(&db->lk);
#endif
}

int zdb_lock(zdb_t *db)
{

	if (!db)
		return (TRUE); /* nothing to lock */

#ifdef USE_LIBPTHREAD
	if (0 != pthread_mutex_lock(&db->lk))
		return (FALSE);
#endif

	return (TRUE);
}

int zdb_trylock(zdb_t *db)
{

	if (!db)
		return (TRUE); /* nothing to lock */

#ifdef USE_LIBPTHREAD
	if (0 != pthread_mutex_trylock(&db->lk))
		return (FALSE);
#endif

	return (TRUE);
}

void zdb_unlock(zdb_t *db)
{

	if (!db)
		return;

#ifdef USE_LIBPTHREAD
	pthread_mutex_unlock(&db->lk);
#endif
}

