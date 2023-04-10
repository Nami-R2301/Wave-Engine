//
// Created by nami on 25/03/23.
//

#include <gtest/gtest.h>
#include <example_app.h>

namespace Wave
{
  
  class Camera_tests : public ::testing::Test
  {
  public:
    
    Camera_tests()
    {
      this->app = new Example_app();
      this->app->init();
    }
    
    void SetUp() override
    {
      // Code here will be called immediately after the constructor (right
      // before each test).
      this->dummy_camera = Wave::create_shared_pointer<Wave::Perspective_camera>(90.0f,
                                                                                 static_cast<float>(Engine::get_main_window()->get_width()),
                                                                                 static_cast<float>(Engine::get_main_window()->get_height()),
                                                                                 0.1f,
                                                                                 1000.0f).get();
    }
    
    void TearDown() override
    {
      // Code here will be called immediately after each test (right
      // before the destructor).
      this->app->shutdown();
    }
    
    // shared user data
    Example_app *app = nullptr;
    Perspective_camera *dummy_camera = nullptr;
  };
  
  TEST_F(Camera_tests, orbit_around_target)
  {
//    Camera_tests::dummy_camera->print(Print_type::Default);
  }
}