#include <Pack/RPKernel.h>

RP_SINGLETON_IMPL(RP_DEBUG_STUB_0);

void RP_DEBUG_STUB_0::Calculate() {
    // Debug calculation - no-op in release build
    // This would perform debug metrics calculation in debug builds
}

void RP_DEBUG_STUB_0::DebugDraw() {
    // Debug drawing - no-op in release build
    // This would render debug overlays in debug builds
}
