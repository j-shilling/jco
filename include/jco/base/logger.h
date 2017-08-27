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

/**
 * @brief Log a message
 *
 * Generic function marco which checks the type of the first argument. If
 * it is a log level, or an int, then that log level is used. Otherwise the
 * first argument is assumed to be a format string for the log message and
 * the default log level is used.
 */
#define jco_log(x, ...)							    \
  _Generic ((x),							    \
	    enum LogLevel: jco_logger_log,				    \
		      int: jco_logger_log,				    \
		  default: jco_logger_log_default_level) (x, ##__VA_ARGS__)


/**
 * @brief Used to rank the priority of a message.
 */
enum LogLevel
{
  FINEST,
  FINER,
  FINE,
  CONFIG,
  INFO,
  WARNING,
  SEVERE,

  N_LEVELS /** Internal use only */
};

/**
 * @brief Log a message at the specified log level
 * @param level	    Prioriety of the message
 * @param fmt	    A printf style format string
 */
void jco_logger_log (enum LogLevel level, char const *const restrict fmt, ...);

/**
 * @brief Log a message at the default log level
 * @param fmt	    A printf style format string
 */
void jco_logger_log_default_level (char const *const restrict fmt, ...);

/**
 * @brief Set whether the logger should print to a file.
 */
void jco_log_to_file (bool const val);

/**
 * @brief Set wheather the logger should print to stdout / stderr
 */
void jco_log_to_term (bool const val);

/**
 * @brief Set the minimum log level to be handled
 */
void jco_log_level (enum LogLevel const level);

/**
 * @brief Set the default message level
 */
void jco_default_log_level (enum LogLevel const level);

/**
 * @brief Set the path to a file to log messages
 * @param level	    The level to associate with this file
 * @param path	    The path to the file
 */
void jco_log_file (enum LogLevel const level, char const *const path);

#endif /* INCLUDE_OBJECT_LOGGER_H_ */
