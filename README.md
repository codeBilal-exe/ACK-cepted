# ACK-cepted (TCP SYN Flood Detector)

Short guide to build and run the project.

**Project layout**

- `CMakeLists.txt` — CMake project file
- `src/` — C++ source files and `gui/` subfolder

**Prerequisites**

- CMake >= 3.16
- A working C++ toolchain:
  - Windows: Visual Studio 2022 (recommended) or MSVC Build Tools / compatible Clang
  - Linux: gcc or clang
- Qt (Widgets) matching your compiler (MSVC/MinGW build of Qt on Windows)
- Packet capture library:
  - Linux: libpcap (development headers)
  - Windows: Npcap (and the developer SDK) — link against `wpcap.lib`

Notes:

- The code currently contains Linux-specific firewall and privilege-check code (`getuid()`, `iptables`). On Windows you will need to adapt or remove those parts before attempting a full build.
- Building and running packet capture requires elevated privileges (root on Linux or Administrator on Windows).

--

## Build (Windows - recommended: Visual Studio / MSVC)

1. Install Visual Studio with "Desktop development with C++" and install Qt for your MSVC toolset.
2. Install Npcap SDK and note the include/lib paths.
3. Open "x64 Native Tools Command Prompt for VS 2022" or Developer PowerShell.
4. From project root run:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 \
  -DCMAKE_PREFIX_PATH="C:/Qt/<version>/msvc<toolset>_64" \
  -DPCAP_INCLUDE="C:/Path/To/Npcap/Include" \
  -DPCAP_LIB="C:/Path/To/Npcap/Lib/wpcap.lib"
cmake --build build --config Release
```

After successful build the executable will be under `build\Release\` (e.g. `SynFloodDetector.exe`). Copy it to the project root if you want it there.

## Build (Linux)

Install Qt5 (dev), libpcap-dev, build-essential, and CMake. Example (Debian/Ubuntu):

```bash
sudo apt update
sudo apt install build-essential cmake libqt5widgets5 libqt5core5a libqt5gui5 libqt5-dev libpcap-dev
cmake -S . -B build
cmake --build build --config Release
```

The binary will be in `build/` (Release or default configuration).

## Running

- Run the binary with elevated privileges.
  - Linux: `sudo ./build/SynFloodDetector` (or `sudo ./SynFloodDetector` if you copied it to project root)
  - Windows: run the `.exe` as Administrator

## Quick options if you don't have dependencies yet

- I can create a minimal stub target (console-only) that builds without Qt or pcap so you can get a runnable EXE quickly. Tell me if you want that.
- Or I can help adapt the CMake file to detect and use Qt/Npcap on Windows and guard Linux-only code paths.

## Common issues

- "Compiler cannot compile a simple test program" — usually means the selected toolchain is misconfigured or missing runtime/linker components. For Clang on Windows prefer MSVC toolchain or install the full Visual Studio toolset.
- Missing Qt: set `CMAKE_PREFIX_PATH` to your Qt install root.
- Missing pcap: provide `PCAP_INCLUDE` and `PCAP_LIB` when invoking CMake.

If you'd like, tell me whether you want a quick stub executable now or full Windows adaptation; I can make the changes and attempt a build here.
