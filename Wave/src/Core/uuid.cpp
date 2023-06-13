//
// Created by nami on 10/06/23.
//


#include <Core/uuid.h>

#include <random>

namespace Wave
{
  
  static std::random_device s_device;
  static std::mt19937_64 s_random_engine(s_device());
  static std::uniform_int_distribution<uint64_t> s_uniform_distribution;
  
  UUID::UUID()
  {
    s_uniform_distribution(s_random_engine);
  }
  
  UUID::UUID(int64_t uuid_)
  {
    this->uuid = uuid_;
  }
  
  void UUID::re_generate()
  {
  }
  
  int64_t UUID::get_id() const
  {
    return this->uuid;
  }
  
  bool UUID::operator<(const UUID &other_uuid) const
  {
    return this->uuid < other_uuid.get_id();
  }
  
  UUID &UUID::operator=(const UUID &other_uuid)
  {
    if (this == &other_uuid) return *this;
    this->uuid = other_uuid.get_id();
    return *this;
  }
  
  UUID::operator uint64_t() const
  {
    return this->uuid;
  }
  
  bool UUID::operator==(const UUID &other_uuid) const
  {
    return this->uuid == other_uuid.get_id();
  }
}