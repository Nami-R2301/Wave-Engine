//
// Created by nami on 2023-02-23.
//

#include <Renderer/vertex.h>

namespace Wave
{
  
  Vertex_2D::Vertex_2D(float x_coord, float y_coord, float red, float green, float blue, float alpha)
  {
    this->position = Vector_2f(x_coord, y_coord);
    this->color = Color(red, green, blue, alpha);
  }
  
  Vertex_2D::Vertex_2D(const Vector_2f &position_, const Color &color_)
  {
    this->position = position_;
    this->color = color_;
  }
  
  Vertex_2D::Vertex_2D(const Vector_2f &position_, const Vector_2f &tex_coord_)
  {
    this->position = position_;
    this->tex_coord = tex_coord_;
  }
  
  Vertex_2D::Vertex_2D(const Vector_2f &position_, const Color &color_, const Vector_2f &tex_coord_)
  {
    this->position = position_;
    this->color = color_;
    this->tex_coord = tex_coord_;
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
  
  Vector_2f Vertex_2D::get_position() const
  {
    return this->position;
  }
  
  const Color &Vertex_2D::get_color() const
  {
    return this->color;
  }
  
  const Vector_2f &Vertex_2D::get_tex_coord() const
  {
    return this->tex_coord;
  }
  
  void Vertex_2D::set_position(const Vector_2f &position_)
  {
    this->position = position_;
  }
  
  void Vertex_2D::set_position(float x, float y)
  {
    this->position = Vector_2f(x, y);
  }
  
  void Vertex_2D::set_color(const Color &color_)
  {
    this->color = color_;
  }
  
  void Vertex_2D::set_color(float r, float g, float b, float a)
  {
    this->color = Color(r, g, b, a);
  }
  
  void Vertex_2D::set_tex_coord(const Vector_2f &texCoord_)
  {
    this->tex_coord = texCoord_;
  }
  
  void Vertex_2D::set_tex_coord(float x, float y)
  {
    this->tex_coord = Vector_2f(x, y);
  }
  
  bool Vertex_2D::operator ==(const Vertex_2D &other_vertex)
  {
    if (this == &other_vertex) return true;
    return this->position == other_vertex.position && this->color == other_vertex.color &&
           this->tex_coord == other_vertex.tex_coord;
  }
  
  Vertex_2D &Vertex_2D::operator =(const Vertex_2D &other_vertex)
  {
    if (this == &other_vertex) return *this;
    this->set_position(other_vertex.get_position());
    this->set_color(other_vertex.get_color());
    this->set_tex_coord(other_vertex.get_tex_coord());
    return *this;
  }
  
  std::string Vertex_2D::to_string() const
  {
    std::string output("[VERTEX 2D] :\n");
    
    output += "Position on screen --> " + this->position.to_string();
    output += "Color --> " + this->color.to_string();
    output += "Texture coordinates --> " + this->tex_coord.to_string();
    
    return output;
  }
  
  void Vertex_2D::print() const
  {
    std::string printed(this->to_string());
    auto error = printed.find("ERROR");
    if (error)
    {
      alert(WAVE_WARN, "%s", printed.c_str());
      return;
    }
    alert(WAVE_INFO, "%s", this->to_string().c_str());
  }
  
  /*************************** 3D *******************************/
  
  Vertex_3D::Vertex_3D(const Vector_2f &vector_2f, const Color &color)
  {
    this->position = Vector_3f(vector_2f.get_x(), vector_2f.get_y(), 0.0f);
    this->color = color;
  }
  
  Vertex_3D::Vertex_3D(const Vector_3f &vector_3f, const Color &color)
  {
    this->position = vector_3f;
    this->color = color;
  }
  
  Vertex_3D::Vertex_3D(const Vector_3f &position, const Vector_2f &tex_coord)
  {
    this->position = position;
    this->color = Color("#FFFFFFFF");
    this->tex_coord = tex_coord;
  }
  
  Vertex_3D::Vertex_3D(const Vector_3f &position_, const Color &color_, const Vector_2f &tex_coord_)
  {
    this->position = position_;
    this->color = color_;
    this->tex_coord = tex_coord_;
  }
  
  Vertex_3D::Vertex_3D(const Vector_3f &position_, const Color &color_, const Vector_3f &normal_)
  {
    this->position = position_;
    this->color = color_;
    this->normal = normal_;
  }
  
  Vertex_3D::Vertex_3D(const Vector_3f &position_, const Vector_3f &normal_, const Vector_2f &tex_coord_)
  {
    this->position = position_;
    this->normal = normal_;
    this->tex_coord = tex_coord_;
  }
  
  Vertex_3D::Vertex_3D(const Vector_3f &position_, const Color &color_, const Vector_3f &normal_,
                       const Vector_2f &tex_coord_)
  {
    this->position = position_;
    this->color = color_;
    this->normal = normal_;
    this->tex_coord = tex_coord_;
  }
  
  Vertex_3D::Vertex_3D(const Vertex_2D &vertex_2D)
  {
    this->position = Vector_3f(vertex_2D.get_position().get_x(), vertex_2D.get_position().get_y(), 0);
    this->color = vertex_2D.get_color();
    this->normal = Vector_3f(0);
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
  
  Vertex_3D::Vertex_3D(float x_coord, float y_coord, float z_coord, float _red_, float _green_,
                       float _blue_, float _alpha_)
  {
    this->position.set_x(x_coord);
    this->position.set_y(y_coord);
    this->position.set_z(z_coord);
    this->color = Color(_red_, _green_, _blue_, _alpha_);
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
    char buffer[FILENAME_MAX * 4] {0};
    if (snprintf(buffer, sizeof(buffer), "[VERTEX 3D] :\n%55sPosition on screen --> %s%55sNormal --> %s"
                                         "%55sColor --> %s%55sTexture coordinates --> %s",
                 DEFAULT, this->position.to_string().c_str(), DEFAULT,
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
      alert(WAVE_WARN, "%s", printed.c_str());
      return;
    }
    alert(WAVE_INFO, "%s", this->to_string().c_str());
  }
  
  Vector_3f Vertex_3D::get_position() const
  {
    return this->position;
  }
  
  Vector_3f Vertex_3D::get_normal() const
  {
    return this->normal;
  }
  
  const Vector_2f &Vertex_3D::get_tex_coord() const
  {
    return tex_coord;
  }
  
  const Color &Vertex_3D::get_color() const
  {
    return color;
  }
  
  void Vertex_3D::set_position(const Vector_3f &position_)
  {
    this->position = position_;
  }
  
  void Vertex_3D::set_position(float x, float y, float z)
  {
    this->position = Vector_3f(x, y, z);
  }
  
  void Vertex_3D::set_normal(const Vector_3f &normal_)
  {
    this->normal = normal_;
  }
  
  void Vertex_3D::set_normal(float x, float y, float z)
  {
    this->normal = Vector_3f(x, y, z);
  }
  
  void Vertex_3D::set_tex_coord(const Vector_2f &texCoord)
  {
    Vertex_3D::tex_coord = texCoord;
  }
  
  void Vertex_3D::set_tex_coord(float x, float y)
  {
    this->tex_coord = Vector_2f(x, y);
  }
  
  void Vertex_3D::set_color(const Color &color_)
  {
    Vertex_3D::color = color_;
  }
  
  void Vertex_3D::set_color(float r, float g, float b, float a)
  {
    this->color = Color(r, g, b, a);
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
  
  bool Vertex_3D::operator ==(const Vertex_3D &_vertex_)
  {
    if (this == &_vertex_) return true;
    return (this->position == _vertex_.position && this->color == _vertex_.color);
  }
  
  Vertex_3D &Vertex_3D::operator =(const Vertex_3D &_vertex_)
  {
    if (this == &_vertex_) return *this;
    this->set_position(_vertex_.get_position());
    this->set_color(_vertex_.get_color());
    this->set_normal(_vertex_.get_normal());
    this->set_tex_coord(_vertex_.get_tex_coord());
    return *this;
  }
}