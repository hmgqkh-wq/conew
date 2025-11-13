Xclipse940 Wrapper ICD for Winlator

Overview
This repository builds a user-space Vulkan wrapper ICD for Exynos 2400e / Xclipse 940 as a Winlator driver.
The wrapper forwards to the system Vulkan backend, exposes a conservative Vulkan surface, supports per-game profiles,
pipeline cache hooks, BCn fallback shader, and on-device logging/caches to iterate toward high compatibility and performance.

Warning
This wrapper cannot access privileged vendor firmware or kernel-only interfaces. For full vendor-level performance
(for example parity with Adreno drivers) you need vendor ICDs or kernel/firmware interfaces not available in user-space.
This project is designed to be a production-quality bridge and research platform.

Build
- Push to GitHub and run the provided GitHub Actions workflow, or build locally for Android arm64 with:
  mkdir -p build
  cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=31
  cmake --build build -j
  cmake --install build --prefix pkg

Package
- After install, package the contents with:
  python3 tools/package.py pkg xclipse940-driver.tar.zst

Deploy
- Copy/extract the packaged folder into:
  Android/data/com.winlator/files/drivers/xclipse940
- Open Winlator and select the driver

Next steps
- Replace HAL stubs with hardware-specific code when vendor interfaces are available.
- Add per-game pipeline cache precompilation and advanced shader tuning.
- Collect logs from /sdcard/Winlator/xclipse940/logs for iterative tuning.

License
MIT
