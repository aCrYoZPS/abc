#include <hip/hip_runtime.h>

#include <cstdlib>
#include <iostream>

#define HIP_CHECK(expression)                                            \
    {                                                                    \
        hipError_t status = (expression);                                \
        if (status != hipSuccess) {                                      \
            std::cerr << "HIP error " << status << ": "                  \
                      << hipGetErrorString(status) << " at " << __FILE__ \
                      << ":" << __LINE__ << std::endl;                   \
            std::exit(EXIT_FAILURE);                                     \
        }                                                                \
    }

int main() {
    int deviceCount = 0;
    HIP_CHECK(hipGetDeviceCount(&deviceCount));
    std::cout << "Number of HIP devices: " << deviceCount << std::endl;

    for (int deviceId = 0; deviceId < deviceCount; ++deviceId) {
        hipDeviceProp_t deviceProp;
        HIP_CHECK(hipGetDeviceProperties(&deviceProp, deviceId));

        std::cout << "Device " << deviceId << " Properties:" << std::endl;
        std::cout << " Name: " << deviceProp.name << std::endl;
        std::cout << " Total Global Memory: "
                  << deviceProp.totalGlobalMem / (1024 * 1024) << " MiB"
                  << std::endl;
        std::cout << " Shared Memory per Block: "
                  << deviceProp.sharedMemPerBlock / 1024 << " KiB" << std::endl;
        std::cout << " Warp Size: " << deviceProp.warpSize << std::endl;
        std::cout << " Max Threads per Block: " << deviceProp.maxThreadsPerBlock
                  << std::endl;
        std::cout << " Number of Multiprocessors: "
                  << deviceProp.multiProcessorCount << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
