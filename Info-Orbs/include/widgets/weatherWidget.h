#ifndef WEAHTERWIDGET_H
#define WEAHTERWIDGET_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TJpg_Decoder.h>
#include <config.h>
#include <widget.h>

#include "icons.h"

class WeatherWidget : public Widget {
   public:
    WeatherWidget(ScreenManager& manager);
    ~WeatherWidget() override;
    void setup() override;
    void update() override;
    void update(bool force) override;
    void draw() override;
    void draw(bool force) override;
    void changeMode() override;

   private:
    void displayClock(int displayIndex, uint32_t background, uint32_t textColor);

    void showJPG(int displayIndex, int x, int y, const byte jpgData[], int size, int scale);
    void drawWeatherIcon(String condition, int displayIndex, int x, int y, int scale);
    void singleWeatherDeg(int displayIndex, uint32_t background, uint32_t textColor);
    void weatherText(int displayIndex, int16_t background, int16_t textColor);
    void threeDayWeather(int displayIndex);
    int getClockStamp();
    int getWeatherStamp();

    String m_hour = "";
    String m_minute = "";
    String m_month = "";
    String m_monthName = "";
    String m_weekday = "";
    String m_day = "";

    const long m_weatherDelay = 600000;  // weather refresh rate
    unsigned long m_lastWeather = 600000;

    const long m_timeDelay = 1000;  // time refresh rate
    unsigned long m_lastTime;

    const int centre = 120;  // centre location of the screen(240x240)

    int m_clockStamp = 0;
    int m_weatherStamp = 0;

    // Global Variables to track/store weather data
    int m_timeZoneOffSet;
    String m_cityName;
    String m_currentWeatherText = "";  // Weather Description
    String m_currentWeatherIcon = "";  // Text refrence for weather icon
    String m_currentWeatherDeg = "";
    String m_daysIcons[3];  // Icons/Temp For Next 3 days
    String m_daysDegs[3];

    String weatherLocation = WEATHER_LOCAION;
#ifdef WEATHER_UNITS_METRIC
    String weatherUnits = "metric";
#else
    String weatherUnits = "us";
#endif
    String weatherApiKey = WEATHER_API_KEY;

    String httpRequestAddress = "https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/" +
                                weatherLocation + "/next3days?key=" + weatherApiKey + "&unitGroup=" + weatherUnits +
                                "&include=days,current&iconSet=icons1";
};

#endif  // WEAHTERWIDGET_H