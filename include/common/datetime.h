/*!
* @file datetime.h
* @brief Functions for working with date and time
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#pragma once
#include <Arduino.h>

/*!
* @brief Structure describes the main parameters of date and time
*/
struct DateTime {
    /*!
    * @brief The year of datetime (0000-9999)
    */
    int Year;
    /*!
    * @brief The month of datetime (1-12)
    */
    int Month;
    /*!
    * @brief The day of datetime (1-31)
    */
    int Day;
    /*!
    * @brief The hour of datetime (0-23)
    */
    int Hour;
    /*!
    * @brief The minute of datetime (0-59)
    */
    int Minute;
    /*!
    * @brief The second of datetime (0-59)
    */
    int Second;
};

/*!
* @brief Is leap year
* @param[in] year Specified year (0000-9999)
* @return <b>TRUE</b> Specified year is leap year, otherwise <b>FALSE</b> 
*/
inline bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
};

/*!
* @brief Convert DateTime structure to Unix-time
* @param[in] dt DateTime structure with specified date and time
* @return Unix-time
*/
uint32_t DateTimeToUnix(const DateTime& dt) {
    const uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint32_t totalSeconds = 0;

    for (int y = 1970; y < dt.Year; ++y) {
        totalSeconds += isLeapYear(y) ? 31622400UL : 31536000UL; 
    }

    for (int m = 1; m < dt.Month; ++m) {
        uint8_t d = daysInMonth[m - 1];
        if (m == 2 && isLeapYear(dt.Year)) {
            d = 29;
        }
        totalSeconds += (uint32_t)d * 86400UL;
    }

    totalSeconds += (uint32_t)(dt.Day - 1) * 86400UL;
    totalSeconds += (uint32_t)dt.Hour * 3600UL;
    totalSeconds += (uint32_t)dt.Minute * 60UL;
    totalSeconds += dt.Second;

    return totalSeconds;
}

/*!
* @brief Convert Unix-time to DateTime structure
* @param[in] unixtime Specified date and time in Unix-format
* @return DateTime structure
*/
DateTime UnixToDateTime(uint32_t unixtime) {
    DateTime dt;
    const uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    dt.Second = unixtime % 60;
    unixtime /= 60;

    dt.Minute = unixtime % 60;
    unixtime /= 60;

    dt.Hour = unixtime % 24;
    unixtime /= 24;

    // unixTime теперь — количество дней с 1 января 1970
    int year = 1970;
    while (true) {
        uint16_t daysInThisYear = isLeapYear(year) ? 366 : 365;
        if (unixtime >= daysInThisYear) {
            unixtime -= daysInThisYear;
            year++;
        } else {
            break;
        }
    }
    dt.Year = year;

    int month = 1;
    while (true) {
        uint8_t daysInThisMonth = daysInMonth[month - 1];
        if (month == 2 && isLeapYear(dt.Year)) {
            daysInThisMonth = 29;
        }
        if (unixtime >= daysInThisMonth) {
            unixtime -= daysInThisMonth;
            month++;
        } else {
            break;
        }
    }
    dt.Month = month;
    dt.Day = unixtime + 1;

    dt.Year = year;
    return dt;
}

/*!
* @brief Auxiliary function for converting HH:MM:SS to seconds since the start of the day
* @param[in] hour The hour value of time (0-23)
* @param[in] minute The minute value of time (0-59)
* @param[in] second The second value of time (0-59)
* @return Unix-time
*/
inline uint32_t hmsToSeconds(int hour, int minute, int second) {
    return (uint32_t)hour * 3600UL + (uint32_t)minute * 60UL + (uint32_t)second;
}

/*!
* @brief Checking whether the current Unix-time falls within the specified time interval of the day
* @param[in] unixtime Specified date and time in Unix-time format
* @param[in] startH The hour of time of the begin period (0-23)
* @param[in] startM The minute of time of the begin period (0-59)
* @param[in] startS The second of time if the begin period (0-59)
* @param[in] endH The hour of time of the end period (0-23)
* @param[in] endtM The minute of time of the end period (0-59)
* @param[in] endS The second of time if the end period (0-59)
* @return <b>TRUE</b>Specified datetime is included of the specified period, otherwise <b>FALSE</b>
*/
bool IsTimeInRange(uint32_t unixtime, 
                   int startH, int startM, int startS, 
                   int endH, int endM, int endS) {
    uint32_t currentSec = unixtime % 86400UL;
    uint32_t startSec = hmsToSeconds(startH, startM, startS);
    uint32_t endSec = hmsToSeconds(endH, endM, endS);

    if (startSec <= endSec) {
        return (currentSec >= startSec && currentSec <= endSec);
    } else {
        return (currentSec >= startSec || currentSec <= endSec);
    }
}

/*!
* @brief Add or Substract specified seconds to specified datetime in Unix-time format
* @param[in] unixtime Specified datetime in Unix-time format
* @param[in] seconds Seconds count for add or substract
* @return Unix-time
*/
uint32_t AddSeconds(uint32_t unixtime, int32_t seconds) {
    return unixtime + seconds;
}

/*!
* @brief Add or Substract specified minutes to specified datetime in Unix-time format
* @param[in] unixtime Specified datetime in Unix-time format
* @param[in] minutes Minutes count for add or substract
* @return Unix-time
*/
uint32_t AddMinutes(uint32_t unixtime, int32_t minutes) {
    return unixtime + (minutes * 60L);
}

/*!
* @brief Add or Substract specified hours to specified datetime in Unix-time format
* @param[in] unixtime Specified datetime in Unix-time format
* @param[in] hours Hours count for add or substract
* @return Unix-time
*/
uint32_t AddHours(uint32_t unixtime, int32_t hours) {
    return unixtime + (hours * 3600L);
}

/*!
* @brief Add or Substract specified days to specified datetime in Unix-time format
* @param[in] unixtime Specified datetime in Unix-time format
* @param[in] days Days count for add or substract
* @return Unix-time
*/
uint32_t AddDays(uint32_t unixtime, int32_t days) {
    return unixtime + (days * 86400L);
}

/*!
* @brief A helper function to determine the number of days in a specific month
* @param[in] year The year of datetime (0000-9999)
* @param[in] month The month of datetime (1-12)
* @return Number of days in a specific month
*/
inline uint8_t getDaysInMonth(int year, int month) {
    const uint8_t days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month - 1];
}

/*!
* @brief Add or Substract specified months to specified datetime in Unix-time format
* @param[in] unixtime Specified datetime in Unix-time format
* @param[in] months Months count for add or substract
* @return Unix-time
*/
uint32_t AddMonths(uint32_t unixtime, int32_t months) {
    DateTime dt = UnixToDateTime(unixtime);
    
    int32_t totalMonths = (int32_t)dt.Year * 12 + (dt.Month - 1) + months;
    
    dt.Year = totalMonths / 12;
    dt.Month = (totalMonths % 12) + 1;
    
    if (dt.Month <= 0) {
        dt.Month += 12;
        dt.Year--;
    }

    uint8_t maxDays = getDaysInMonth(dt.Year, dt.Month);
    if (dt.Day > maxDays) {
        dt.Day = maxDays;
    }

    return DateTimeToUnix(dt);
}

/*!
* @brief Add or Substract specified years to specified datetime in Unix-time format
* @param[in] unixtime Specified datetime in Unix-time format
* @param[in] years Years count for add or substract
* @return Unix-time
*/
uint32_t AddYears(uint32_t unixtime, int32_t years) {
    DateTime dt = UnixToDateTime(unixtime);
    dt.Year += years;

    uint8_t maxDays = getDaysInMonth(dt.Year, dt.Month);
    if (dt.Day > maxDays) {
        dt.Day = maxDays;
    }

    return DateTimeToUnix(dt);
}
