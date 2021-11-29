#include "integer.hpp"

namespace frstd {
namespace driver {

void abortProgram();

void writeStdout(const char* data, const usz size);
void writeStderr(const char* data, const usz size);

void* allocateMemory(const usz size);

}
}
