
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

#ifndef __HTM_MOTOR_H__
#define __HTM_MOTOR_H__

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define MOTOR_HEAD
#define MOTOR_HEAD_TOOTH
#define MOTOR_HEAD_NOSE
#define MOTOR_HEAD_FOREHEAD

#define MOTOR_SPEECH

#define MOTOR_NECK

#define MOTOR_ARM

#define MOTOR_TORSO

#define MOTOR_THIGH

#define MOTOR_LEG

#define MOTOR_FOOT
#define MOTOR_FOOT_TOP
#define MOTOR_FOOT_BOTTOM
#define MOTOR_FOOT_ANKLE
#define MOTOR_FOOT_TOE

void htm_motor_sway(entity_t *ent, int axis, double speed);

#endif /* ndef __HTM_MOTOR_H__ */

