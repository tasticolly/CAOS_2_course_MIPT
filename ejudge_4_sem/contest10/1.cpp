#define FUSE_USE_VERSION 30
#include <string>
#include <cstddef>
#include <vector>
#include <filesystem>
#include <fstream>
#include <fuse.h>
#include <set>
#include <cstring>


using std::filesystem::file_time_type;
using std::filesystem::directory_entry;
using std::filesystem::directory_iterator;

typedef struct {
  char* directories_for_mount;
} simplefs_options_t;

std::vector<std::string> directories;
simplefs_options_t simplefs_options;

std::string get_full_path(const std::string& path) {
  std::filesystem::file_time_type last_time;
  std::string last_time_changed_path = path;
  bool is_exist = false;
  for (const std::string& full_path : directories) {
    std::string absolute_path(full_path);
    if (!path.empty()) {
      absolute_path += "/" + path;
    }
    if (std::filesystem::exists(absolute_path)
        && (std::filesystem::last_write_time(absolute_path) > last_time || !is_exist)) {
      last_time = std::filesystem::last_write_time(absolute_path);
      last_time_changed_path = absolute_path;
      is_exist = true;
    }
  }
  return last_time_changed_path;
}

int simplefs_getattr(const char* path, struct stat* out_stat, struct fuse_file_info* file_info) {
  stat(std::filesystem::absolute(get_full_path(path + 1)).c_str(), out_stat);


  if (S_ISDIR(out_stat->st_mode)) {
    out_stat->st_mode &= S_IFDIR | 0555;
  }
  if (S_ISREG(out_stat->st_mode)) {
    out_stat->st_mode &= S_IFREG | 0444;
  }
  return 0;
}

int simplefs_readdir(const char* path, void* buffet, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* file_info, enum fuse_readdir_flags flags) {

  filler(buffet, ".", nullptr, 0, (enum fuse_fill_dir_flags) 0);
  filler(buffet, "..", nullptr, 0, (enum fuse_fill_dir_flags) 0);

  std::set<std::string> file_names;
  std::vector<std::string> paths;
  for (const std::string& current_directory : directories) {
    std::string absolute_path = current_directory + "/" + (path + 1);
    if (std::filesystem::exists(absolute_path) && is_directory(directory_entry(absolute_path).status())) {
      for (const directory_entry& properties : directory_iterator(absolute_path)) {
        std::string file_name(std::filesystem::path(properties).filename());
        if (file_names.find(file_name) == file_names.end()) {
          file_names.insert(file_name);
          filler(buffet, file_name.data(), nullptr, 0, (enum fuse_fill_dir_flags) 0);
        }
      }
    }
  }

  return 0;
}

void tokenize(const std::string& str, const char devider, std::vector<std::string>& out) {
  size_t start;
  size_t end = 0;

  while ((start = str.find_first_not_of(devider, end)) != std::string::npos) {
    end = str.find(devider, start);
    out.push_back(absolute(std::filesystem::path(str.substr(start, end - start))));
  }
}

int simplefs_read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* file_info) {
  std::string relative_path = get_full_path(path + 1);
  if (!std::filesystem::is_regular_file(relative_path)) {
    return 0;
  }
  uint64_t len = std::filesystem::file_size(relative_path);
  if (offset >= len) {
    return 0;
  }

  if (offset + size > len) {
    size = len - offset;
  }

  FILE* fs = fopen(relative_path.c_str(), "r");
  fseek(fs, offset, SEEK_SET);
  fgets(buf, size + 1, fs);
  return size;
}

struct fuse_operations operations = {
    .getattr = simplefs_getattr,
    .read = simplefs_read,
    .readdir = simplefs_readdir
};

int main(int argc, char** argv) {
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
  struct fuse_opt opt_specs[] = {
      {"--src %s", offsetof(simplefs_options_t, directories_for_mount), 0},
      {NULL, 0, 0}
  };
  fuse_opt_parse(&args, &simplefs_options, opt_specs, NULL);

  tokenize(simplefs_options.directories_for_mount, ':', directories);

  int ret = fuse_main(args.argc, args.argv, &operations, nullptr);

  fuse_opt_free_args(&args);
  return ret;
}