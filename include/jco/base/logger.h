/*
* This file is part of JCO
*
* JCO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* JCO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with JCO.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file logger.h
 * @author Jake Shilling
 * @brief Declares functions for logging
 */

#ifndef INCLUDE_OBJECT_LOGGER_H_
#define INCLUDE_OBJECT_LOGGER_H_

#define jco_log(x, ...)							    \
  _Generic ((x),							    \
	    enum LogLevel: jco_logger_log,				    \
		      int: jco_logger_log,				    \
		  default: jco_logger_log_default_level) (x, ##__VA_ARGS__)

enum LogLevel
{
  FINEST,
  FINER,
  FINE,
  CONFIG,
  INFO,
  WARNING,
  SEVERE,

  N_LEVELS
};

void jco_logger_log (enum LogLevel level, char const *const restrict fmt, ...);
void jco_logger_log_default_level (char const *const restrict fmt, ...);

#endif /* INCLUDE_OBJECT_LOGGER_H_ */
