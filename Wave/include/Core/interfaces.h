//
// Created by nami on 17/01/23.
//

#pragma once

#include <Math/vector.h>

namespace Wave
{
  enum Print_type
  {
    Info = 0,
    Warn,
    Error,
    Debug
  };
  
  // Entity identifiable via an uuid.
  class I_identifiable
  {
    public:
    virtual ~I_identifiable() = default;
    virtual void register_id(int32_t entity_id) = 0;
  };
  
  // Able to initialize its members explicitly without relying on the ambiguity of constructors.
  class I_sendable
  {
    public:
    virtual ~I_sendable() = default;
    [[nodiscard]] virtual bool is_sent() const = 0;
    virtual void send_gpu(uint64_t instance_count) = 0;
    virtual void free_gpu(uint64_t instance_count) = 0;
  };
  
  // Display properties applied to entities to debug their states and/or show them in the editor.
  class I_printable
  {
    public:
    virtual ~I_printable() = default;
    [[nodiscard]] virtual std::string to_string() const = 0;
    virtual void print(Print_type type) const = 0;
  };
  
  // Translate dynamic entities from one screen coordinate to the other.
  class I_movable
  {
    public:
    virtual ~I_movable() = default;
    virtual void translate(const Vector_3f &position) = 0;
    virtual void translate(float x, float y, float z) = 0;
  };
  
  // Able to be rotated on the x and y-axis.
  class I_rotatable
  {
    public:
    virtual ~I_rotatable() = default;
    virtual void rotate(const Vector_3f &position) = 0;
    virtual void rotate(float x, float y, float z) = 0;
  };
  
  // Able to get its size scaled on the x, y and z-axis.
  class I_scalable
  {
    public:
    virtual ~I_scalable() = default;
    virtual void scale(const Vector_3f &scalar) = 0;
    virtual void scale(float x, float y, float z) = 0;
  };
  
  // Transfer data from one entity onto the other, useful for duplication and testing of various similar entities.
  class I_copiable
  {
    public:
    virtual ~I_copiable() = default;
    [[nodiscard]] virtual void *copy() const = 0;
  };
  
  
  // Encode entities in a compact format and decode them from files to save their states between executions.
  class I_serializable
  {
    public:
    virtual ~I_serializable() = default;
    virtual void encode() = 0;
    virtual void *decode() = 0;
  };

#ifdef DEBUG
// Display properties applied to entities to debug their states and/or show them in the editor.
#define INTERFACE_PRINTABLE  [[nodiscard]] std::string to_string() const override;\
                         void print([[maybe_unused]] Print_type type = Print_type::Info) const override                        \
                         {                                                    \
                          std::string printed(this->to_string());             \
                          if (type == Print_type::Info)     \
                            Wave::alert(WAVE_LOG_INFO, "%s", this->to_string().c_str());  \
                          else if (type == Print_type::Warn)   \
                            Wave::alert(WAVE_LOG_WARN, "%s", this->to_string().c_str());                                \
                          else if (type == Print_type::Debug)            \
                            Wave::alert(WAVE_LOG_DEBUG, "%s", this->to_string().c_str()); \
                         else Wave::alert(WAVE_LOG_ERROR, "%s", this->to_string().c_str());  \
                         }
#else
#ifdef RELEASE
#define INTERFACE_PRINTABLE  [[nodiscard]] std::string to_string() const override;\
                             void print([[maybe_unused]] Print_type type) const override                          \
                             {                                                    \
                             };
#endif
#endif

// Identifiable via an uuid.
#define INTERFACE_IDENTIFIABLE  inline void register_id(int32_t entity_id) override \
                                  {                                  \
                                    this->id = entity_id; \
                                  }
// Translate dynamic entities from one screen coordinate to the other.
#define INTERFACE_MOVABLE  void translate(const Vector_3f &position) override; \
                           void translate(float x, float y, float z) override

// Able to be rotated on the x and y-axis.
#define INTERFACE_ROTATABLE  void rotate(const Vector_3f &angle) override; \
                           void rotate(float x, float y, float z) override

// Transfer data from one entity onto the other, useful for duplication and testing of various similar entities.
#define INTERFACE_COPIABLE [[nodiscard]] void *copy() const override

// Able to get its size scaled on the x, y and z-axis.
#define INTERFACE_SCALABLE void scale(const Vector_3f &scalar) override; \
                           void scale(float x, float y, float z) override

// Able to initialize its members explicitly without relying on the ambiguity of constructors.
#define INTERFACE_SENDABLE void send_gpu(uint64_t instance_count) override; \
                            void free_gpu(uint64_t instance_count) override; \
                           [[nodiscard]] bool is_sent() const override \
                           {                     \
                             return this->sent;                      \
                           }
}