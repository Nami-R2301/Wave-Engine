//
// Created by nami on 10/06/23.
//

#pragma once

#include <wave_pch.h>

namespace Wave
{
  
  class UUID
  {
    public:
    UUID();
    explicit UUID(int64_t uuid);
    ~UUID() = default;
    
    void re_generate();
    
    [[nodiscard]] int64_t get_id() const;
    
    explicit operator uint64_t() const;
    bool operator<(const UUID &other_uuid) const;
    UUID &operator=(const UUID &other_uuid);
    bool operator==(const UUID &other_uuid) const;
    private:
    int64_t uuid = 0;
  };
}

namespace std
{
  template<typename T>
  struct hash;
  
  template<>
  struct hash<Wave::UUID>
  {
    size_t operator()(const Wave::UUID &uuid) const
    {
      return (uint64_t) uuid;
    }
  };
}
