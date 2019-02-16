/* Copyright 2018 Neo Natura */

#include "zio.h"


double zio_therm_k(double c)
{
	return (c + 273.15);
}

double zio_therm_f(double c)
{
	return ((c * 1.8) + 32);
}

