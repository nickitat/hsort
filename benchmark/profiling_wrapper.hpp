#pragma once

#include <gperftools/profiler.h>

namespace hsort {
struct PerfProfilingWrapper {
  PerfProfilingWrapper(const char* profileName) {
    ProfilerStart(profileName);
  }

  ~PerfProfilingWrapper() {
    ProfilerStop();
  }
};
}  // namespace hsort
