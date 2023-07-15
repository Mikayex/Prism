#include <glog/logging.h>

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostdout = true;
  FLAGS_colorlogtostdout = true;

  DLOG(WARNING) << "Using debug build";

  std::cout << "Hello, world!" << std::endl;
  return EXIT_SUCCESS;
}