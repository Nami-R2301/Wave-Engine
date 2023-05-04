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
  
  class Printable
  {
  public:
    virtual ~Printable() = default;
    [[nodiscard]] virtual std::string to_string() const = 0;
    virtual void print(Print_type type) const = 0;
  };
  
  class Movable
  {
  public:
    virtual ~Movable() = default;
    virtual void move(const Vector_3f &position) = 0;
  };
}