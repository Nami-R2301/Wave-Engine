//
// Created by nami on 2023-02-23.
//

#include <Renderer/vertex.h>
#include <Utilities/logger.h>

namespace Wave
{
  
  Vertex_2D::Vertex_2D(float x_coord, float y_coord, float red, float green, float blue, float alpha)
    : position(x_coord, y_coord), color(red, green, blue, alpha)
  {
  }
  
  Vertex_2D::Vertex_2D(const Math::Vector_2f &position_, const Color &color_)
    : position(position_), color(color_)
  {
  }
  
  Vertex_2D::Vertex_2D(const Math::Vector_2f &position_, const Math::Vector_2f &tex_coord_)
    : position(position_), tex_coord(tex_coord_)
  {
  }
  
  Vertex_2D::Vertex_2D(const Math::Vector_2f &position_, const Color &color_, const Math::Vector_2f &tex_coord_)
    : position(position_), color(color_), tex_coord(tex_coord_)
  {
  }
  
  Vertex_2D::Vertex_2D(const Math::Vector_2f &position_, const Math::Vector_2f &normal_, const Color &color_,
                       const Math::Vector_2f &tex_coord_) : position(position_), normal(normal_),
                                                            color(color_), tex_coord(tex_coord_)
  {
  }
  
  Vertex_2D::Vertex_2D(const Vertex_2D &other_vertex)
  {
    if (this == &other_vertex) return;
    this->position = other_vertex.position;
    this->color = other_vertex.color;
    this->tex_coord = other_vertex.tex_coord;
  }
  
  float Vertex_2D::length() const
  {
    return this->position.length();
  }
  
  Math::Vector_2f Vertex_2D::get_position() const
  {
    return this->position;
  }
  
  Math::Vector_2f Vertex_2D::get_normal() const
  {
    return this->normal;
  }
  
  const Color &Vertex_2D::get_color() const
  {
    return this->color;
  }
  
  const Math::Vector_2f &Vertex_2D::get_tex_coord() const
  {
    return this->tex_coord;
  }
  
  void Vertex_2D::set_id(int32_t id_)
  {
    this->id = id_;
  }
  
  void Vertex_2D::set_position(const Math::Vector_2f &position_)
  {
    this->position = position_;
  }
  
  void Vertex_2D::set_position(float x, float y)
  {
    this->position = Math::Vector_2f(x, y);
  }
  
  void Vertex_2D::set_normal(const Math::Vector_2f &normal_)
  {
    this->normal = normal_;
  }
  
  void Vertex_2D::set_normal(float x, float y)
  {
    this->normal = Math::Vector_2f(x, y);
  }
  
  void Vertex_2D::set_color(const Color &color_)
  {
    this->color = color_;
  }
  
  void Vertex_2D::set_color(float r, float g, float b, float a)
  {
    this->color = Color(r, g, b, a);
  }
  
  void Vertex_2D::set_tex_coord(const Math::Vector_2f &texCoord_)
  {
    this->tex_coord = texCoord_;
  }
  
  void Vertex_2D::set_tex_coord(float x, float y)
  {
    this->tex_coord = Math::Vector_2f(x, y);
  }
  
  bool Vertex_2D::operator==(const Vertex_2D &other_vertex)
  {
    if (this == &other_vertex) return true;
    return this->position == other_vertex.position && this->color == other_vertex.color &&
           this->tex_coord == other_vertex.tex_coord;
  }
  
  Vertex_2D &Vertex_2D::operator=(const Vertex_2D &other_vertex)
  {
    if (this == &other_vertex) return *this;
    this->set_position(other_vertex.get_position());
    this->set_color(other_vertex.get_color());
    this->set_tex_coord(other_vertex.get_tex_coord());
    return *this;
  }
  
  std::string Vertex_2D::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    if (snprintf(buffer, sizeof(buffer), "[Vertex 2D] :\n%55sObject ID --> %d\n%55sPosition --> %s%55sNormal --> %s"
                                         "%55sColor --> %s%55sTexture coordinates --> %s",
                 DEFAULT, this->id, DEFAULT, this->position.to_string().c_str(), DEFAULT,
                 this->normal.to_string().c_str(), DEFAULT,
                 this->color.to_string().c_str(),
                 DEFAULT, this->tex_coord.to_string().c_str()) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Vertex_2D]!";
    }
    return buffer;
  }
  
  void Vertex_2D::print() const
  {
    std::string printed(this->to_string());
    auto error = printed.find("ERROR");
    if (error)
    {
      alert(WAVE_LOG_WARN, "%s", printed.c_str());
      return;
    }
    alert(WAVE_LOG_INFO, "%s", this->to_string().c_str());
  }
  
  /*************************** 3D *******************************/
  
  Vertex_3D::Vertex_3D(const Math::Vector_2f &position_2D, const Color &color_)
    : position(position_2D), color(color_)
  {
  }
  
  Vertex_3D::Vertex_3D(const Math::Vector_3f &position_, const Color &color_)
    : position(position_), color(color_)
  {
  }
  
  Vertex_3D::Vertex_3D(const Math::Vector_3f &position_, const Math::Vector_2f &tex_coord_)
    : position(position_), tex_coord(tex_coord_)
  {
  }
  
  Vertex_3D::Vertex_3D(const Math::Vector_3f &position_, const Color &color_, const Math::Vector_2f &tex_coord_)
    : position(position_), color(color_), tex_coord(tex_coord_)
  {
  }
  
  Vertex_3D::Vertex_3D(const Math::Vector_3f &position_, const Color &color_, const Math::Vector_3f &normal_)
    : position(position_), normal(normal_), color(color_)
  {
  }
  
  Vertex_3D::Vertex_3D(const Math::Vector_3f &position_, const Math::Vector_3f &normal_,
                       const Math::Vector_2f &tex_coord_)
    : position(position_), normal(normal_), tex_coord(tex_coord_)
  {
  }
  
  Vertex_3D::Vertex_3D(const Math::Vector_3f &position_, const Color &color_, const Math::Vector_3f &normal_,
                       const Math::Vector_2f &tex_coord_) : position(position_), normal(normal_),
                                                            color(color_), tex_coord(tex_coord_)
  {
  }
  
  Vertex_3D::Vertex_3D(float x_coord, float y_coord, float z_coord, float red_, float green_,
                       float blue_, float alpha_) : position(x_coord, y_coord, z_coord),
                                                    color(red_, green_, blue_, alpha_)
  {
  }
  
  Vertex_3D::Vertex_3D(const Vertex_2D &vertex_2D)
  {
    this->position = Math::Vector_3f(vertex_2D.get_position().get_x(), vertex_2D.get_position().get_y(), 0);
    this->color = vertex_2D.get_color();
    this->normal = Math::Vector_3f(0);
    this->tex_coord = vertex_2D.get_tex_coord();
  }
  
  Vertex_3D::Vertex_3D(const Vertex_3D &other_vertex)
  {
    if (this == &other_vertex) return;
    this->position = other_vertex.position;
    this->color = other_vertex.color;
    this->normal = other_vertex.normal;
    this->tex_coord = other_vertex.tex_coord;
  }
  
  float Vertex_3D::length() const
  {
    return this->position.length();
  }
  
  void Vertex_3D::convert_in_2D()
  {
    this->set_position({this->position.get_x(),
                        this->position.get_y(),
                        0});
    this->set_normal(0, 0, 1);
  }
  
  std::string Vertex_3D::to_string() const
  {
    char buffer[FILENAME_MAX * 4]{0};
    if (snprintf(buffer, sizeof(buffer), "[Vertex 3D] :\n%55sObject ID --> %d\n%55sPosition --> %s%55sNormal --> %s"
                                         "%55sColor --> %s%55sTexture coordinates --> %s",
                 DEFAULT, this->id, DEFAULT, this->position.to_string().c_str(), DEFAULT,
                 this->normal.to_string().c_str(), DEFAULT,
                 this->color.to_string().c_str(),
                 DEFAULT, this->tex_coord.to_string().c_str()) < 0)
    {
      return "ERROR : Snprintf error when trying to display [Vertex_3D]!";
    }
    return buffer;
  }
  
  void Vertex_3D::print() const
  {
    std::string printed(this->to_string());
    auto error = printed.find("ERROR");
    if (error)
    {
      alert(WAVE_LOG_WARN, "%s", printed.c_str());
      return;
    }
    alert(WAVE_LOG_INFO, "%s", this->to_string().c_str());
  }
  
  Math::Vector_3f Vertex_3D::get_position() const
  {
    return this->position;
  }
  
  Math::Vector_3f Vertex_3D::get_normal() const
  {
    return this->normal;
  }
  
  const Math::Vector_2f &Vertex_3D::get_tex_coord() const
  {
    return tex_coord;
  }
  
  const Color &Vertex_3D::get_color() const
  {
    return color;
  }
  
  void Vertex_3D::set_id(int32_t id_)
  {
    this->id = id_;
  }
  
  void Vertex_3D::set_position(const Math::Vector_3f &position_)
  {
    this->position = position_;
  }
  
  void Vertex_3D::set_position(float x, float y, float z)
  {
    this->position = Math::Vector_3f(x, y, z);
  }
  
  void Vertex_3D::set_normal(const Math::Vector_3f &normal_)
  {
    this->normal = normal_;
  }
  
  void Vertex_3D::set_normal(float x, float y, float z)
  {
    this->normal = Math::Vector_3f(x, y, z);
  }
  
  void Vertex_3D::set_tex_coord(const Math::Vector_2f &texCoord)
  {
    Vertex_3D::tex_coord = texCoord;
  }
  
  void Vertex_3D::set_tex_coord(float x, float y)
  {
    this->tex_coord = Math::Vector_2f(x, y);
  }
  
  void Vertex_3D::set_color(const Color &color_)
  {
    Vertex_3D::color = color_;
  }
  
  void Vertex_3D::set_color(float r, float g, float b, float a)
  {
    this->color = Color(r, g, b, a);
  }
  
  void Vertex_3D::set_model_matrix(const Math::Matrix_4f &model_matrix_)
  {
    for (int i = 0; i < model_matrix_.get_num_rows(); ++i)
    {
      for (int j = 0; j < model_matrix_.get_num_cols(); ++j)
      {
        this->model_matrix[i][j] = model_matrix_.get_value(i, j);
      }
    }
  }
  
  Vertex_3D::operator Vertex_2D() const
  {
    Vertex_2D vertex;
    vertex.set_position({this->position.get_x(),
                         this->position.get_y()});
    vertex.set_tex_coord(this->tex_coord);
    vertex.set_color(this->color);
    return vertex;
  }
  
  bool Vertex_3D::operator==(const Vertex_3D &_vertex_)
  {
    if (this == &_vertex_) return true;
    return (this->position == _vertex_.position && this->color == _vertex_.color);
  }
  
  Vertex_3D &Vertex_3D::operator=(const Vertex_3D &_vertex_)
  {
    if (this == &_vertex_) return *this;
    this->set_position(_vertex_.get_position());
    this->set_color(_vertex_.get_color());
    this->set_normal(_vertex_.get_normal());
    this->set_tex_coord(_vertex_.get_tex_coord());
    return *this;
  }
}