#include <cstdlib>
#include <iostream>

#include "Application.hpp"
#include "glog/logging.h"

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostdout = true;
  FLAGS_colorlogtostdout = true;

  DLOG(WARNING) << "Using debug build";

  Prism::Application app(800, 600, "Prism Raytracing");
  app.run();

  return EXIT_SUCCESS;
}