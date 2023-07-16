#include <glog/logging.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_ASSERT(x) DCHECK(x)
#include "stb_image_write.h"
