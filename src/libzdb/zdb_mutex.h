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

#ifndef __ZDB__ZDB_MUTEX_H__
#define __ZDB__ZDB_MUTEX_H__


int zdb_lock(zdb_t *db);

int zdb_trylock(zdb_t *db);

void zdb_unlock(zdb_t *db);

void zdb_mutex_init(zdb_t *db);

void zdb_mutex_term(zdb_t *db);


#endif /* ndef __ZDB__ZDB_MUTEX_H__ */
