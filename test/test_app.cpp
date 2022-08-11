/*
  Unittest for gob_app
*/
#include "gtest/gtest.h"

#include <gob_app.hpp>
#include <gob_math.hpp>
#include <thread>
#include <chrono>

#if defined(ESP32) && defined(ARDUINO)
#include <gob_m5s_clock.hpp>
#include <gob_m5s_thread.hpp>
#include <Arduino.h>
using app_clock = goblib::m5s::esp_clock;
#else
using app_clock = std::chrono::steady_clock;
#endif

class TestApp : public goblib::App<app_clock, 30,60>
{
  public:
    TestApp() : goblib::App<app_clock,30,60>(), _cnt_fu(0), _cnt_u(0), _cnt_r(0) {}

    std::size_t countFU() const { return _cnt_fu; }
    std::size_t countU() const { return _cnt_u; }
    std::size_t countR() const { return _cnt_r; }
    
  protected:
    virtual void fixedUpdate() override { /*puts(__func__);*/ ++_cnt_fu; }
    virtual void update(float /*delta*/) override { /*puts(__func__);*/ ++_cnt_u; }
    virtual void render() override { /*puts(__func__);*/ ++_cnt_r; }

    #if defined(ESP32) && defined(ARDUINO)
    virtual void sleep_until(const std::chrono::time_point<app_clock, UpdateDuration>& abs_time) override
    {
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(abs_time - app_clock::now()).count();
        delay(us > 0 ? us / 1000 : 0);
        while(app_clock::now() < abs_time){ taskYIELD(); }
    }
    #endif
  protected:
    std::size_t _cnt_fu;
    std::size_t _cnt_u;
    std::size_t _cnt_r;
};

class TestApp2 : public TestApp
{
  public:
    TestApp2() : TestApp() {}

  protected:
    virtual void update(float delta) override
    {
        TestApp::update(delta);
        // time-consuming process...
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
};
    

TEST(App, Basic)
{
    TestApp app;
    TestApp2 app2;
    
    for(int i=0;i<30;++i)
    {
        app.pump();
        printf("%f:%f\n", app.fps(),app.delta());
    }
    EXPECT_EQ(30U, app.frames());
    EXPECT_GE(app.countFU(), 60U); 
    EXPECT_EQ(app.frames(), app.countU());
    EXPECT_EQ(app.frames(), app.countR());
    EXPECT_FLOAT_EQ(30.0f, goblib::math::round(app.fps()));

    for(int i=0;i<10;++i)
    {
        app2.pump();
    }
    EXPECT_EQ(10U, app2.frames());
    EXPECT_GE(app.countFU(), 60U-60U/10U);
    EXPECT_EQ(app2.frames(), app2.countU());
    EXPECT_EQ(app2.frames(), app2.countR());
    EXPECT_FLOAT_EQ(10.0f, goblib::math::round(app2.fps()));
}
