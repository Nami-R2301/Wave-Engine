//
// Created by nami on 5/20/22.
//
#include <Utilities/res_loader.h>
#include <Utilities/logger.h>

namespace Wave
{
  
  Res_loader_3D::Res_loader_3D(const char *file_path)
  {
    // Add filename to path.
    char relative_file_path[FILENAME_MAX];
    if (snprintf(relative_file_path, FILENAME_MAX, "../Wave/res/Models/%s", file_path) < 0)
    {
      Wave::alert(WAVE_ERROR, "[SNPRINTF ERROR] : %d\n", ERROR_SNPRINTF);
    }
    this->file_path = relative_file_path;
    this->file_ptr = fopen(this->file_path, "rb");
    if (!this->file_ptr)
    {
      alert(WAVE_ERROR, "[RES LOADER] : ERROR READING FILE %s! MAKE SURE FILE EXISTS AND IS IN TEXT FORMAT!",
            relative_file_path);
    }
  }
  
  std::string Res_loader_3D::load_shader_source(const char *file_path)
  {
    // Add filename to path.
    char relative_file_path[FILENAME_MAX];
    if (snprintf(relative_file_path, FILENAME_MAX, "../Wave/res/Shaders/%s", file_path) < 0)
    {
      Wave::alert(WAVE_ERROR, "[SNPRINTF ERROR] : %d\n", ERROR_SNPRINTF);
    }
    FILE *file_ptr = fopen(relative_file_path, "rb");
    if (!file_ptr)
    {
      alert(WAVE_ERROR, "[RES LOADER] : ERROR READING SHADER FILE %s! MAKE SURE FILE EXISTS AND IS IN TEXT FORMAT!",
            relative_file_path);
      return {};
    }
    if (fseek(file_ptr, 0, SEEK_END) < 0)  // Get last character position in file.
    {
      alert(WAVE_ERROR, "[RES LOADER] : ERROR READING SHADER FILE %s! MAKE SURE FILE EXISTS AND IS IN TEXT FORMAT!",
            relative_file_path);
      return {};
    }
    long file_size = ftell(file_ptr) + 1;
    rewind(file_ptr);
    std::string shader_source_cpy;
    char *shader_source_buffer = (char *) calloc(1, file_size);
    if (shader_source_buffer)
    {
      if (!fread(shader_source_buffer, sizeof(char), file_size, file_ptr))
      {
        Wave::alert(WAVE_ERROR, "[FREAD ERROR] : %d\n", ERROR_READ_SHADER);
        fclose(file_ptr);
        free(shader_source_buffer);
        return {};
      }
      shader_source_cpy = std::string(shader_source_buffer);
    }
    fclose(file_ptr);
    free(shader_source_buffer);
    return shader_source_cpy;
  }
  
  Texture Res_loader_3D::load_texture(const char *file_path)
  {
    // Add filename to path.
    char relative_file_path[FILENAME_MAX];
    if (snprintf(relative_file_path, FILENAME_MAX, "../../Wave/res/Textures/%s", file_path) < 0)
    {
      Wave::alert(WAVE_ERROR, "[SNPRINTF ERROR] : %d\n", ERROR_SNPRINTF);
    }
    return Texture(relative_file_path);
  }
  
  Mesh_3D Res_loader_3D::load_3D_mesh()
  {
    while (this->file_ptr)  // Get current line.
    {
      char line_c[FILENAME_MAX];
      std::string line;
      int resource = fscanf(this->file_ptr, "%s", line_c);
      line_c[FILENAME_MAX - 1] = '\0';
      line = std::string(line_c);
      if (resource == EOF) break;
      
      if (strcmp(line.c_str(), "v") == 0) load_obj_vertex();  // Load vertex coordinates.
      if (strcmp(line.c_str(), "vt") == 0) load_obj_texture_coords();  // Load texture coordinates.
      if (strcmp(line.c_str(), "vn") == 0) load_obj_normal();  // Load normal coordinates.
      if (strcmp(line.c_str(), "f") == 0) load_obj_face();  // Load index coordinates.
    }
    
    if (this->vertices.empty())
    {
      Wave::alert(WAVE_WARN, "[RES ERROR] : OBJECT FILE (.OBJ) FOR VERTEX DATA (v) NOT FOUND!");
    }
    
    if (this->face_indices.empty())
    {
      Wave::alert(WAVE_WARN, "[RES ERROR] : OBJECT FILE (.OBJ) FOR FACE DATA (f) NOT FOUND!");
    }
    
    if (this->tex_coords.empty())
    {
      Wave::alert(WAVE_WARN, "[RES ERROR] : OBJECT FILE (.OBJ) FOR TEXTURE DATA (vt) NOT FOUND!");
      this->tex_coords.resize(this->face_indices.size(), Vector_2f(-1));
    }
    
    return {this->vertices,
            this->face_indices,
            this->normals,
            this->tex_coords};
  }
  
  Mesh_2D Res_loader_3D::load_2D_mesh()
  {
    while (this->file_ptr)  // Get current line.
    {
      char line_c[FILENAME_MAX];
      std::string line;
      int resource = fscanf(this->file_ptr, "%s", line_c);
      line_c[FILENAME_MAX - 1] = '\0';
      line = std::string(line_c);
      if (resource == EOF) break;
      
      if (strcmp(line.c_str(), "v") == 0) load_obj_vertex();  // Load vertex coordinates.
      if (strcmp(line.c_str(), "vt") == 0) load_obj_texture_coords();  // Load texture coordinates.
    }
    if (this->vertices.empty())
    {
      Wave::alert(WAVE_WARN, "[RES ERROR] : OBJECT FILE (.OBJ) FOR VERTEX DATA (v) NOT FOUND!");
    }
    
    if (this->tex_coords.empty())
    {
      Wave::alert(WAVE_WARN, "[RES ERROR] : OBJECT FILE (.OBJ) FOR TEXTURE DATA (vt) NOT FOUND!");
      this->tex_coords.resize(this->vertices.size(), Vector_2f(-1));
    }
    
    return {this->vertices,
            this->tex_coords};
  }
  
  void Res_loader_3D::load_obj_vertex()
  {
    char x[sizeof(double) * 4] {0}, y[sizeof(double) * 4] {0}, z[sizeof(double) * 4] {0};
    if (fscanf(this->file_ptr, "%s %s %s\n", x, y, z) == EOF)
    {
      Wave::alert(WAVE_WARN, "[RES ERROR] : Loading vertex positions (v)");
    }
    else
    {
      this->vertices.emplace_back(Vector_3f(strtof(x, nullptr),
                                            strtof(y, nullptr),
                                            strtof(z, nullptr)), Vector_2f(0, 0));
    }
  }
  
  void Res_loader_3D::load_obj_face()
  {
    char face_data[3][FILENAME_MAX];
    if (fscanf(this->file_ptr, "%s %s %s\n", face_data[0], face_data[1], face_data[2]) == EOF)
    {
      Wave::alert(WAVE_WARN, "[RES ERROR] : Loading face indices (f)");
    }
    else
    {
      std::string line(face_data[0]);
      line += "/";
      line += face_data[1];
      line += "/";
      line += face_data[2];
      line += "/";
      
      std::string delim = "/";
      size_t pos;
      std::string token;
      std::vector<int> face_indexes;
      
      while ((pos = line.find(delim)) != std::string::npos)
      {
        token = line.substr(0, pos);
        line.erase(0, pos + delim.length());
        if (token.empty()) { face_indexes.push_back(0); }
        else { face_indexes.push_back((int) strtoul(token.c_str(), nullptr, 10)); }
      }
      
      Face face = {face_indexes[0] - 1,
                   face_indexes[1] - 1,
                   face_indexes[2] - 1,
                   face_indexes[3] - 1,
                   face_indexes[4] - 1,
                   face_indexes[5] - 1,
                   face_indexes[6] - 1,
                   face_indexes[7] - 1,
                   face_indexes[8] - 1};
      
      this->face_indices.push_back(face);
    }
  }
  
  void Res_loader_3D::load_obj_normal()
  {
    char x[sizeof(double) * 4], y[sizeof(double) * 4], z[sizeof(double) * 4];
    if (fscanf(this->file_ptr, "%s %s %s\n", x, y, z) == EOF)
    {
      Wave::alert(WAVE_WARN, "[RES ERROR] : Loading normal positions (v)");
    }
    else
    {
      this->normals.emplace_back(strtof(x, nullptr),
                                 strtof(y, nullptr),
                                 strtof(z, nullptr));
    }
  }
  
  void Res_loader_3D::load_obj_texture_coords()
  {
    char x[sizeof(double) * 4], y[sizeof(double) * 4];
    if (fscanf(this->file_ptr, "%s %s", x, y) == EOF)
    {
      Wave::alert(WAVE_WARN, "[RES ERROR] : Loading Texture coordinates (vt)");
    }  // Get next string after space.
    this->tex_coords.emplace_back(strtof(x, nullptr), strtof(y, nullptr));
  }
  
  Res_loader_3D::~Res_loader_3D()
  {
    if (this->file_ptr) fclose(this->file_ptr);
    this->file_ptr = nullptr;
  }
}