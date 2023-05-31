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
      this->dummy_2d = (Object_2D * )
      new Cube(Wave::Resource_loader::load_object_3D_source("../Wave/res/Models/cube.obj"));
      this->dummy_cube = new Cube(Wave::Resource_loader::load_object_3D_source("../Wave/res/Models/cube.obj"));
    }

    ~Object_tests() override
    {
      delete this->dummy_cube;
    }
    
    // shared user data
    Object_2D *dummy_2d = nullptr;
    Object_3D *dummy_cube = nullptr;
  };
  
  TEST_F(Object_tests, check_object_size)
  {
    ASSERT_LT(sizeof(*this->dummy_2d), sizeof(*this->dummy_cube));
  }
  
  TEST_F(Object_tests, check_vertex_size)
  {
    ASSERT_NE(this->dummy_2d->get_vertex_size(), sizeof(this->dummy_2d->get_vertices()));
    ASSERT_NE(this->dummy_cube->get_vertex_size(), sizeof(this->dummy_cube->get_vertices()));
  }
  
  TEST_F(Object_tests, check_object_integrity)
  {
    auto dummy_cube_to_object_3D = *((Object_3D *) (this->dummy_cube));
    EXPECT_TRUE(dummy_cube_to_object_3D == *this->dummy_cube);
    
    auto dummy_cube_2 = (Object_3D *) this->dummy_cube->copy();
    dummy_cube_2->convert_in_2D();
    
    ASSERT_NE(sizeof(this->dummy_2d->get_vertex(0).get_position()), sizeof(dummy_cube_2->get_vertex(0).get_position()));
  }
}