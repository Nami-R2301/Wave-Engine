//
// Created by nami on 23/03/23.
//

#include <gtest/gtest.h>
#include <example_app.h>

namespace Wave
{
  
  class Object_tests : public ::testing::Test
  {
  public:
    
    Object_tests()
    {
      this->app = new Example_app();
      this->app->init();
    }

    ~Object_tests() override
    {
      Example_app::shutdown();
      delete app;
    }
    
    void SetUp() override
    {
      // Code here will be called immediately after the constructor (right
      // before each test).
      this->dummy_2d = (Object_2D *) std::make_shared<Wave::Object_3D>(
        Wave::Res_loader_3D("cube.obj").load_3D_mesh()).get();
    }
    
    // shared user data
    Example_app *app = nullptr;
    Object_2D *dummy_2d = nullptr;
  };
  
  TEST_F(Object_tests, check_object_size)
  {
    ASSERT_LT(sizeof(*dummy_2d), sizeof(*this->app->demo_objects[1].get()));
  }
  
  TEST_F(Object_tests, check_vertex_size)
  {
    ASSERT_NE(dummy_2d->get_vertex_size(), sizeof(dummy_2d->get_vertices()));
    ASSERT_NE(this->app->demo_objects[1]->get_vertex_size(), sizeof(this->app->demo_objects[1]->get_vertices()));
  }
  
  TEST_F(Object_tests, check_object_integrity)
  {
    auto dummy_cube = Wave::Object_3D(Wave::Res_loader_3D("cube.obj").load_3D_mesh());
    EXPECT_TRUE(*this->app->demo_objects[1].get() == dummy_cube);
    
    Object_2D dummy_square_1 = (Object_2D) dummy_cube;
    dummy_cube.convert_in_2D();
    
    ASSERT_NE(sizeof(dummy_square_1.get_vertex(0).get_position()), sizeof(dummy_cube.get_vertex(0).get_position()));
  }
}