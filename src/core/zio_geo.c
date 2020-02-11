/* Copyright 2018 Neo Natura */

#include "zio.h"

#define _pi 3.14159265358979323846

#define MIN_GRID_SIZE 16

static double _deg2rad(double deg)
{
	return (deg * _pi / 180);
}

static double _rad2deg(double rad)
{
	return (rad * 180 / _pi);
}

/* meters between two geodetic locations. */
double zio_geo_dist(double lat1, double lon1, double lat2, double lon2)
{
	double theta, dist;

	if ((lat1 == lat2) && (lon1 == lon2)) {
		return 0;
	}

	theta = lon1 - lon2;
	dist = sin(_deg2rad(lat1)) * sin(_deg2rad(lat2)) + cos(_deg2rad(lat1)) * cos(_deg2rad(lat2)) * cos(_deg2rad(theta));
	dist = acos(dist);
	dist = _rad2deg(dist);
	dist = dist * 60 * 1.1515;
	/* -> meters */
	dist = dist * 1609.344;

	return (dist);
}

/* returns longitude based on meters distance. */
double zio_geo_dist_x(double lat, int meters)
{
	return (fabs((double)meters / (111.111 * cos(lat))));
}

/* returns latitude based on meters distance. */
double zio_geo_dist_y(int meters)
{
	return (fabs((double)meters / 111.111));
}

/* cenetered on lat,lon with r*2 of <span> meters */
void zio_grid_alloc(zgrid_t *grid, double lat, double lon, int span)
{
	zplot_t **plot;
	double lat_span;
	double lon_span;
	double cell_dim;
	double lat_off, lon_off;
	int cell_max;
	int x, y;

	cell_max = MAX(span, MIN_GRID_SIZE);
	lat_span = zio_geo_dist_y(span);
	lon_span = zio_geo_dist_x(lat, span);
	cell_dim = MIN(lat_span, lon_span) / (double)cell_max;
	lat_off = lat - lat_span/2;
	lon_off = lon - lon_span/2;

	/* allocate new grid cells */
	plot = (zplot_t **)calloc(cell_max, sizeof(zplot_t *));
	for (x = 0; x < cell_max; x++)
		plot[x] = (zplot_t *)calloc(cell_max, sizeof(zplot_t));
	for (x = 0; x < cell_max; x++) {
		for (y = 0; y < cell_max; y++) {
			plot[x][y].geo.lat = lat_off + (cell_dim * y) + (cell_dim / 2);
			plot[x][y].geo.lon = lon_off + (cell_dim * x) + (cell_dim / 2); 
		}
	}

	if (grid->plot) {
		/* reassign objs to new grid */
		for (x = 0; x < grid->cell_max; x++) {
			for (y = 0; y < grid->cell_max; y++) {
				zobj_t *obj;
				zgeo_t *geo;
				int trans_x;
				int trans_y;

				obj = GRID_OBJ(grid, x, y); 
				if (!obj) continue;

				geo = GRID_GEO(grid, x, y);
				trans_x = (geo->lon - lon_off) / cell_dim;
				trans_y = (geo->lat - lat_off) / cell_dim;
				if (trans_x >= 0 && trans_x < cell_max &&
						trans_y >= 0 && trans_y < cell_max)
					plot[trans_x][trans_y].obj = obj;
			}
		}

		/* free previous grid cells. */
		for (x = 0; x < grid->cell_max; x++) {
			free(grid->plot[x]);
		}
		free(grid->plot);
		grid->plot = NULL;
	}

	grid->plot = plot;
	grid->cell_max = cell_max;
	grid->cell_dim = cell_dim;
	grid->lat_off = lat_off;
	grid->lat_off = lon_off;
}

void zio_grid_expand(zgrid_t *grid, int x, int y, double lat, double lon)
{
	zgeo_t *geo;
	double x_fact;
	double y_fact;
	double fact;

	geo = &grid->plot[x][y].geo;
	x_fact = fabs(lon - geo->lon) / grid->cell_dim;
	y_fact = fabs(lat - geo->lat) / grid->cell_dim;
	fact = 2 - (x_fact + y_fact);

	if (fact >= 2)
		return;

	zio_grid_alloc(grid, lat, lon, (double)grid->cell_max * fact);
}

void zio_grid_res_incr(zgrid_t *grid, int x, int y)
{
	zobj_t *obj;

	obj = grid->plot[x][y].obj;
	if (obj)
		obj->res++;
}

void zio_grid_res_decr(zgrid_t *grid, int x, int y)
{
	zobj_t *obj;

	obj = grid->plot[x][y].obj;
	if (obj)
		obj->res--;

}

/* @returns the object's unique index presiding in a grid cell. */
int zio_grid_obj(zgrid_t *grid, int x, int y)
{
	zobj_t *obj;

	if (x < 0 || x >= grid->cell_max ||
			y < 0 || y >= grid->cell_max)
		return (0); /* out of bounds */

	obj = grid->plot[x][y].obj;
	if (!obj)
		return (0); /* nothing present in cell. */

	return (obj->id);
}

void zio_grid_obj_set(zgrid_t *grid, zobj_t *obj, int x, int y)
{

	if (x < 0 || x >= grid->cell_max ||
			y < 0 || y >= grid->cell_max)
		return;

	grid->plot[x][y].obj = obj;

}

zgrid_t *zio_grid_init(double lat, double lon)
{
	zgrid_t *grid;

	grid = (zgrid_t *)calloc(1, sizeof(zgrid_t));
	if (!grid)
		return (NULL);

	zio_grid_alloc(grid, lat, lon, MIN_GRID_SIZE);

	return (0);
}

int zio_grid_plot(zgrid_t *grid, zobj_t *obj, double lat, double lon)
{
	double span;
	double x;
	double y;
	int cur_id;

	span = (double)grid->cell_dim * grid->cell_max;
	x = (lon - grid->lon_off) / grid->cell_dim;
	y = (lat - grid->lat_off) / grid->cell_dim;

	if (x < 0 || x >= grid->cell_max ||
			y < 0 || y >= grid->cell_max) {
		/* out of range */
		return (-1);
	}

	cur_id = zio_grid_obj(grid, x, y);
	if (cur_id == obj->id) {
		zio_grid_res_incr(grid, x, y);
		return (0);
	}

#if 0
	if (lon < (x_diff * 0.25) || lon > (x_diff * 0.75) ||
			lat < (y_diff * 0.25) || lat > (y_diff * 0.75)) {
		zio_grid_res_decr(grid, x, y);
		return (ZERR_AGAIN);
	}
#endif

	if (cur_id != 0)
		zio_grid_expand(grid, x, y, lat, lon);

	zio_grid_obj_set(grid, obj, x, y);
	return (0);	
}

int zio_grid_draw(zgrid_t *grid, uint8_t *buf, size_t buf_max)
{
	int x, y;

	*buf = '\000';
	for (y = 0; y < grid->cell_max; y++) { 
		for (x = 0; x < grid->cell_max; x++) { 
			int id = zio_grid_obj(grid, x, y); 
			if (id != 0)
				sprintf(buf+strlen(buf), "%-3.3d ", zio_grid_obj(grid, x, y));
			else
				strcat(buf, " .  ");
		}
		strcat(buf, "\n");
	}

}

int zio_grid_count(zgrid_t *grid)
{
	int total;
	int x, y;

	for (y = 0; y < grid->cell_max; y++) { 
		for (x = 0; x < grid->cell_max; x++) { 
			int id = zio_grid_obj(grid, x, y); 
			if (id != 0)
				total++;
		}
	}

	return (total);
}

zgeo_t *zio_geo(void)
{
	zdev_t *mod;
	zgeo_t *geo;

	mod = zio_dev_get_name(NULL, "geo");
	if (!mod)
		return (NULL);

  geo = zio_geo_value(mod);
	if (!geo)
		return (NULL);

	return (geo);
}

