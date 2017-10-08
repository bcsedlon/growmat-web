/* Interval
 * Version 5-10-2014
 * Copyright (C) 2014  Pavel Brychta http://www.xpablo.cz
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses
 
Release Notes
5-10-2014  First public release
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}
#include <arduino.h>
#include "interval.h"

bool Interval::expired(void)
{
	uint32_t ticks = millis();
	bool status;

	if (timefrom <= timeto) // timer didn't overflow
		status = (ticks >= timeto) || (ticks < timefrom);
	else // timer overflow
		status = (ticks >= timeto) && (ticks < timefrom);

	return status;
}

void Interval::set(uint32_t timeout)
{

	timefrom = millis();
	timeto = timefrom + timeout;
}

// EOF
