#include <string>
#include <vector>
#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <filesystem>
#include <set>
#include <cstdio>
#include <unistd.h>


std::vector<std::filesystem::path> dir_paths;

std::filesystem::path path_and_time(const char *path) {
  std::filesystem::path relative_path(path + 1);
  std::filesystem::path real_path = relative_path;
  std::filesystem::path abs_path;
  bool not_yet = true;
  decltype(last_write_time(relative_path)) last_time;
  for (std::filesystem::path &directory : dir_paths) {
    if (relative_path == "") {
      abs_path = directory;
    } else abs_path = directory / relative_path;
    if (exists(abs_path)) {
      if (not_yet || last_write_time(abs_path) > last_time) {
        last_time = last_write_time(abs_path);
        real_path = abs_path;
        not_yet = false;
      }
    }
  }
  return real_path;
}

int my_stat(const char *path, struct stat *st, struct fuse_file_info *fi) {
  stat(absolute(path_and_time(path)).c_str(), st);
  if (S_ISDIR(st->st_mode)) st->st_mode &= S_IFDIR | 0555;
  if (S_ISREG(st->st_mode)) st->st_mode &= S_IFREG | 0444;
  return 0;
}

int my_read(const char *path, char *buf, size_t size, off_t off, struct fuse_file_info *fi) {
  std::filesystem::path real_path = path_and_time(path);
  if (is_regular_file(real_path)) {
    uint64_t sz = file_size(real_path);
    if (off >= sz) return 0;
    if (off + size > sz) size = sz - off;
    FILE* fs = fopen(real_path.c_str(), "r");
    fseek(fs, off, SEEK_SET);
    fgets(buf, (int)size + 1, fs);
    return (int)size;
  }
  return 0;
}

int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
               off_t off, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
  std::vector<std::filesystem::path> paths;
  ++path;
  std::set<std::string> names_set;
  filler(buf, ".", nullptr, 0, (enum fuse_fill_dir_flags)0);
  filler(buf, "..", nullptr, 0, (enum fuse_fill_dir_flags)0);
  for (std::filesystem::path &directory : dir_paths) {
    std::filesystem::path abs_path = directory / path;
    if (exists(abs_path) && is_directory(std::filesystem::directory_entry(abs_path).status())) {
      for (const std::filesystem::directory_entry &properties: std::filesystem::directory_iterator(abs_path)) {
        std::string file_name = std::filesystem::path(properties).filename().c_str();
        if (names_set.count(file_name) == 0) {
          names_set.insert(file_name);
          filler(buf, file_name.data(), nullptr, 0, (enum fuse_fill_dir_flags)0);
        }
      }
    }
  }
  return 0;
}

struct fuse_operations operations = {
    .getattr = my_stat,
    .read = my_read,
    .readdir = my_readdir
};

int main(int argc, char **argv) {
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

  typedef struct {
    char *src;
  } my_options_t;
  my_options_t my_options;

  struct fuse_opt opt_specs[] = {
      {"--src %s", offsetof(my_options_t, src), 0},
      { nullptr      , 0                             , 0     }
  };

  fuse_opt_parse(&args, &my_options, opt_specs, nullptr);

  std::string name;
  std::istringstream stream(my_options.src);

  while (std::getline(stream, name, ':')) {
    std::string path = absolute(std::filesystem::path(name));
    dir_paths.emplace_back(path);
  }

  int ret = fuse_main(args.argc, args.argv, &operations, nullptr);
  fuse_opt_free_args(&args);
  return ret;
}