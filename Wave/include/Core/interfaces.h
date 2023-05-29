//
// Created by nami on 17/01/23.
//

#pragma once

#include <Math/vector.h>

namespace Wave
{
  enum Print_type
  {
    Default = 0,
    Warn,
    Error
  };
  
  // Display properties applied to entities to debug their states and/or show them in the editor.
  class Printable
  {
    public:
    virtual ~Printable() = default;
    [[nodiscard]] virtual std::string to_string() const = 0;
    virtual void print(Print_type type) const = 0;
  };
  
  // Translate dynamic entities from one screen coordinate to the other.
  class Movable
  {
    public:
    virtual ~Movable() = default;
    virtual void translate(const Vector_3f &position) = 0;
    virtual void translate(float x, float y, float z) = 0;
  };
  
  // Able to be rotated on the x and y-axis.
  class Rotatable
  {
    public:
    virtual ~Rotatable() = default;
    virtual void rotate(const Vector_3f &position) = 0;
    virtual void rotate(float x, float y, float z) = 0;
  };
  
  // Able to get its size scaled on the x, y and z-axis.
  class Scalable
  {
    public:
    virtual ~Scalable() = default;
    virtual void scale(const Vector_3f &scalar) = 0;
    virtual void scale(float x, float y, float z) = 0;
  };
  
  // Transfer data from one entity onto the other, useful for duplication and testing of various similar entities.
  class Copiable
  {
    public:
    virtual ~Copiable() = default;
    [[nodiscard]] virtual void *copy() const = 0;
  };
  
  
  // Encode entities in a compact format and decode them from files to save their states between executions.
  class Serializable
  {
    public:
    virtual ~Serializable() = default;
    virtual void encode() = 0;
    virtual void *decode() = 0;
  };
}