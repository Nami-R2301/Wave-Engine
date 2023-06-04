//
// Created by nami on 5/20/22.
//
#include <Utilities/resource_loader.h>
#include <Utilities/logger.h>

namespace Wave
{
  
  std::string Resource_loader::load_shader_source(const char *file_path)
  {
    FILE *file_ptr = fopen(file_path, "rb");
    if (!file_ptr)
    {
      alert(WAVE_LOG_ERROR, "[Resource loader] --> Error opening shader source file! "
                            "Make sure the file path exists and is in text format!",
            file_path);
      return {};
    }
    if (fseek(file_ptr, 0, SEEK_END) < 0)  // Get last character position in file.
    {
      alert(WAVE_LOG_ERROR, "[Resource loader] --> Error opening shader source file! "
                            "Make sure the file path exists and is in text format!",
            file_path);
      return {};
    }
    long file_size = ftell(file_ptr) + 1;
    rewind(file_ptr);
    char *shader_source_buffer = (char *) calloc(1, file_size);
    if (shader_source_buffer)
    {
      if (!fread(shader_source_buffer, sizeof(char), file_size, file_ptr))
      {
        Wave::alert(WAVE_LOG_ERROR, "[Resource loader] --> Error while reading source (fread returned %d)",
                    WAVE_INTERNAL_ERROR_READ_SHADER);
        fclose(file_ptr);
        free(shader_source_buffer);
        return {};
      }
      std::string shader_source_cpy(shader_source_buffer);
      free(shader_source_buffer);
      return shader_source_cpy;
    }
    fclose(file_ptr);
    free(shader_source_buffer);
    alert(WAVE_LOG_ERROR, "[Resource loader] --> Error while parsing shader file %s! "
                          "Make sure the file path exists and is in text format!", file_path);
    return {};
  }
  
  std::shared_ptr<Texture> Resource_loader::load_texture_source(const char *file_path)
  {
    return Texture::create(file_path);
  }
  
  Object_2D_data_s Resource_loader::load_object_2D_source(const char *file_path)
  {
    std::vector<Vertex_2D> vertices;
    std::vector<Face_2D_s> face_indices;
    std::vector<Vector_3f> normals;
    std::vector<Vector_2f> tex_coords;
    
    FILE *file_ptr = fopen(file_path, "rb");
    if (!file_ptr)
    {
      alert(WAVE_LOG_ERROR, "[Resource loader] --> Error opening object 3D source file! "
                            "Make sure the file path exists and is in text format!",
            file_path);
      return {};
    }
    
    char line_c[FILENAME_MAX]{0};
    std::string line;
    line.reserve(FILENAME_MAX);
    
    while (fscanf(file_ptr, "%s", line_c) != EOF)  // Get current line.
    {
      line = std::string(line_c);
      
      if (line == "v")
        vertices.emplace_back(load_object_2D_vertex(file_ptr), Vector_2f(0, 0));  // Load vertex coordinates.
      if (line == "f") face_indices.emplace_back(load_object_2D_face(file_ptr));  // Load index coordinates.
      if (line == "vn") normals.emplace_back(load_object_2D_normal(file_ptr));  // Load normal coordinates.
      if (line == "vt") tex_coords.emplace_back(load_object_texture_coords(file_ptr));  // Load texture coordinates.
      
      memset(line_c, '\0', sizeof(line_c));  // Reset line.
    }
    
    if (vertices.empty())
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Object 3D source '%s' doesn't contain any vertices!",
                  file_path);
    }
    
    if (face_indices.empty())
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Object 3D source '%s' doesn't contain any indices!",
                  file_path);
    }
    
    if (tex_coords.empty())
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Object 3D source '%s' doesn't contain any texture coordinates!",
                  file_path);
      tex_coords.resize(face_indices.size(), Vector_2f(-1));
    }
    
    fclose(file_ptr);
    return {Vector_2f(0.0f), vertices, face_indices, tex_coords, {}};
  }
  
  Object_3D_data_s Resource_loader::load_object_3D_source(const char *file_path)
  {
    std::vector<Vertex_3D> vertices;
    std::vector<Face_3D_s> face_indices;
    std::vector<Vector_3f> normals;
    std::vector<Vector_2f> tex_coords;
    
    FILE *file_ptr = fopen(file_path, "rb");
    if (!file_ptr)
    {
      alert(WAVE_LOG_ERROR, "[Resource loader] --> Error opening object 3D source file! "
                            "Make sure the file path exists and is in text format!",
            file_path);
      return {};
    }
    
    char line_c[FILENAME_MAX]{0};
    std::string line;
    line.reserve(FILENAME_MAX);
    
    while (fscanf(file_ptr, "%s", line_c) != EOF)  // Get current line.
    {
      line = std::string(line_c);
      
      if (line == "v")
        vertices.emplace_back(load_object_3D_vertex(file_ptr), Vector_2f(0, 0));  // Load vertex coordinates.
      if (line == "f") face_indices.emplace_back(load_object_3D_face(file_ptr));  // Load index coordinates.
      if (line == "vn") normals.emplace_back(load_object_3D_normal(file_ptr));  // Load normal coordinates.
      if (line == "vt") tex_coords.emplace_back(load_object_texture_coords(file_ptr));  // Load texture coordinates.
      
      memset(line_c, '\0', sizeof(line_c));  // Reset line.
    }
    
    if (vertices.empty())
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Object 3D source '%s' doesn't contain any vertices!",
                  file_path);
    }
    
    if (face_indices.empty())
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Object 3D source '%s' doesn't contain any indices!",
                  file_path);
    }
    
    if (tex_coords.empty())
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Object 3D source '%s' doesn't contain any texture coordinates!",
                  file_path);
      tex_coords.resize(face_indices.size(), Vector_2f(-1));
    }
    
    fclose(file_ptr);
    return {Vector_3f(0), vertices, face_indices, normals, tex_coords, {}};
  }
  
  Vector_2f Resource_loader::load_object_2D_vertex(FILE *file_ptr)
  {
    char x[sizeof(double) * 4]{0}, y[sizeof(double) * 4]{0}, z[sizeof(double) * 4]{0};
    if (fscanf(file_ptr, "%s %s %s\n", x, y, z) == EOF)
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Reached end of file while loading vertex positions (v)");
      return {};
    }
    return {strtof(x, nullptr), strtof(y, nullptr)};
  }
  
  Vector_3f Resource_loader::load_object_3D_vertex(FILE *file_ptr)
  {
    char x[sizeof(double) * 4]{0}, y[sizeof(double) * 4]{0}, z[sizeof(double) * 4]{0};
    if (fscanf(file_ptr, "%s %s %s\n", x, y, z) == EOF)
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Reached end of file while loading vertex positions (v)");
      return {};
    }
    return {strtof(x, nullptr), strtof(y, nullptr), strtof(z, nullptr)};
  }
  
  Face_2D_s Resource_loader::load_object_2D_face(FILE *file_ptr)
  {
    char face_data[3][FILENAME_MAX]{{0},
                                {0},
                                {0}};
    if (fscanf(file_ptr, "%s %s %s\n", face_data[0], face_data[1], face_data[2]) == EOF)
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Reached end of file while loading 3D face indices (f)");
      return {};
    }
    std::string line(face_data[0]);
    line += "/";
    line += face_data[1];
    line += "/";
    line += face_data[2];
    line += "/";
    
    std::string delim = "/";
    size_t pos;
    std::string token;
    std::vector<uint32_t> indices;
    
    while ((pos = line.find(delim)) != std::string::npos)
    {
      token = line.substr(0, pos);
      line.erase(0, pos + delim.length());
      if (token.empty()) indices.push_back(0);
      else indices.push_back((uint32_t) strtoul(token.c_str(), nullptr, 10));
    }
    
    Face_2D_s face_2D_indices = {indices[0] - 1,
                                 indices[1] - 1,
                                 indices[2] - 1,
                                 indices[3] - 1,
                                 indices[4] - 1,
                                 indices[5] - 1};
    
    return face_2D_indices;
  }
  
  Face_3D_s Resource_loader::load_object_3D_face(FILE *file_ptr)
  {
    char face_data[3][FILENAME_MAX]{{0},
                                {0},
                                {0}};
    if (fscanf(file_ptr, "%s %s %s\n", face_data[0], face_data[1], face_data[2]) == EOF)
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Reached end of file while loading 3D face indices (f)");
      return {};
    }
    std::string line(face_data[0]);
    line += "/";
    line += face_data[1];
    line += "/";
    line += face_data[2];
    line += "/";
    
    std::string delim = "/";
    size_t pos;
    std::string token;
    std::vector<uint32_t> indices;
    
    while ((pos = line.find(delim)) != std::string::npos)
    {
      token = line.substr(0, pos);
      line.erase(0, pos + delim.length());
      if (token.empty()) indices.push_back(0);
      else indices.push_back((uint32_t) strtoul(token.c_str(), nullptr, 10));
    }
    
    Face_3D_s face_3D_indices = {indices[0] - 1,
                                 indices[1] - 1,
                                 indices[2] - 1,
                                 indices[3] - 1,
                                 indices[4] - 1,
                                 indices[5] - 1,
                                 indices[6] - 1,
                                 indices[7] - 1,
                                 indices[8] - 1};
    
    return face_3D_indices;
  }
  
  Vector_2f Resource_loader::load_object_2D_normal(FILE *file_ptr)
  {
    char x[sizeof(double) * 4]{0}, y[sizeof(double) * 4]{0}, z[sizeof(double) * 4]{0};
    if (fscanf(file_ptr, "%s %s %s\n", x, y, z) == EOF)
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Reached end of file while loading normal positions (vn)");
      return {};
    }
    return {strtof(x, nullptr), strtof(y, nullptr)};
  }
  
  Vector_3f Resource_loader::load_object_3D_normal(FILE *file_ptr)
  {
    char x[sizeof(double) * 4]{0}, y[sizeof(double) * 4]{0}, z[sizeof(double) * 4]{0};
    if (fscanf(file_ptr, "%s %s %s\n", x, y, z) == EOF)
    {
      Wave::alert(WAVE_LOG_WARN, "[Resource loader] --> Reached end of file while loading normal positions (vn)");
      return {};
    }
    return {strtof(x, nullptr), strtof(y, nullptr), strtof(z, nullptr)};
  }
  
  Vector_2f Resource_loader::load_object_texture_coords(FILE *file_ptr)
  {
    char x[sizeof(double) * 4]{0}, y[sizeof(double) * 4]{0};
    if (fscanf(file_ptr, "%s %s", x, y) == EOF)
    {
      Wave::alert(WAVE_LOG_WARN,
                  "[Resource loader] --> Reached end of file while loading texture coordinate positions (vt)");
      return {};
    }  // Get next string after space.
    return {strtof(x, nullptr), strtof(y, nullptr)};
  }
}