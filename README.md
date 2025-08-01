# CS2 External Cheat

A modern, external cheat for Counter-Strike 2 (CS2) built with C++ and featuring automated build pipelines, memory dumping, and module-based architecture.

## 🌟 Features

- **External Memory Access** - No injection required, safer detection profile
- **Automated Build Pipeline** - One-command setup and execution
- **Dynamic Offset Updates** - Automatically fetches latest CS2 memory offsets
- **Modular Architecture** - Clean, maintainable code structure
- **Cross-Platform Scripts** - Works on Windows, WSL, and Linux environments

### Cheat Modules

- **ESP Module** - Visual overlays and information display
- **Aimbot Module** - Aim assistance functionality
- **Root Module** - Core cheat management and coordination

## 🚀 Quick Start

### Prerequisites

- **Windows OS** with CS2 installed
- **Visual Studio Build Tools** or **Visual Studio** (for MSBuild)
- **Git Bash** or **WSL** (recommended for script execution)
- **curl** (for downloading dependencies)

### One-Command Setup

```bash
# Clone and setup everything
git clone <repository-url>
cd External-cheat
./run.sh --build
```

This will:

1. 📥 Download the latest CS2 dumper
2. 🔍 Generate current memory offsets
3. 🔨 Build the application
4. 🚀 Launch the cheat

### Quick Launch (After Setup)

```bash
# Quick run - uses existing build
./run.sh
```

## 📋 Usage Modes

### Build Mode

```bash
./run.sh --build    # Force rebuild with latest offsets
./run.sh -b         # Short form
```

### Quick Mode

```bash
./run.sh           # Skip dumper, use existing application
```

### Individual Components

```bash
./scripts/update-dumper.sh    # Update CS2 dumper
./scripts/dump.sh             # Generate memory offsets
./scripts/build.sh            # Build application only
```

## 🏗️ Project Structure

```
External-cheat/
├── 📁 Application/           # Main cheat application
│   ├── Application.cpp       # Entry point
│   ├── Application.h         # Main application header
│   ├── 📁 Gui/              # User interface components
│   ├── 📁 Instances/        # Game object representations
│   └── 📁 Modules/          # Cheat functionality modules
├── 📁 Engine/               # Core engine and utilities
│   ├── Engine.h             # Main engine header
│   ├── Memory.h             # Memory manipulation
│   ├── 📁 Classes/          # Utility classes
│   ├── 📁 GameDumper/       # CS2 offset dumping tools
│   ├── 📁 Math/             # Mathematical utilities
│   └── 📁 Shell/            # System interaction
├── 📁 ImGui/                # Dear ImGui integration
├── 📁 Build/                # Compiled outputs
│   └── 📁 Release/          # Release build artifacts
├── 📁 scripts/              # Build and utility scripts
│   ├── build.sh             # Application builder
│   ├── dump.sh              # Memory dumper
│   ├── update-dumper.sh     # Dumper updater
│   └── README.md            # Scripts documentation
├── run.sh                   # Main pipeline orchestrator
└── Application.sln          # Visual Studio solution
```

## 🔧 Development

### Building

The project uses **MSBuild** with a Visual Studio solution:

```bash
# Debug build
./scripts/build.sh --debug

# Release build (default)
./scripts/build.sh
```

### Memory Offset Management

CS2 updates frequently change memory layouts. The project uses [cs2-dumper](https://github.com/a2x/cs2-dumper) to automatically generate current offsets:

```bash
# Update dumper tool
./scripts/update-dumper.sh

# Generate fresh offsets (requires CS2 running)
./scripts/dump.sh
```

Generated offsets are stored in `Engine/GameDumper/Dumps/` as C++ header files.

### Module Development

Create new cheat modules in `Application/Modules/`:

```cpp
// Example module structure
class NewModule : public Module {
public:
    void Initialize() override;
    void Update() override;
    void Render() override;
};
```

## ⚙️ Configuration

### Build Configuration

- **Platform**: x64 (64-bit)
- **Configuration**: Release (optimized) / Debug (development)
- **Compiler**: MSVC via MSBuild

### Runtime Requirements

- **Administrator Privileges** - Required for memory access
- **CS2 Running** - Target process must be active
- **Antivirus Exclusions** - May be needed for memory access

## 🛡️ Safety & Ethics

This project is for **educational purposes** and **legitimate testing environments** only.

### Important Notes

- **External Implementation** - Safer than injection-based cheats
- **Detection Risks** - Use responsibly and at your own risk
- **Game Integrity** - Respect other players and game rules
- **Educational Focus** - Primarily for learning memory manipulation techniques

## 🔍 Troubleshooting

### Common Issues

**Build Failures**

```bash
# Check MSBuild availability
msbuild --version

# Verify Visual Studio installation
./scripts/build.sh --debug
```

**Dumper Issues**

```bash
# Run as administrator
# Ensure CS2 is running
# Check antivirus settings
```

**Memory Access Errors**

- Run as Administrator
- Verify CS2 process is active
- Update memory offsets with fresh dump

### Debug Mode

```bash
# Build with debug symbols
./run.sh --build --debug

# Check build logs
./scripts/build.sh --debug 2>&1 | tee build.log
```

## 📜 Scripts Documentation

All build and utility scripts are documented in [`scripts/README.md`](scripts/README.md).

### Script Architecture

- **Modular Design** - Single responsibility per script
- **Error Handling** - Graceful failure recovery
- **User Feedback** - Clear progress and status messages
- **Cross-Platform** - Git Bash, WSL, Linux compatible

## 🤝 Contributing

1. **Fork** the repository
2. **Create** a feature branch
3. **Implement** your changes with proper error handling
4. **Test** on multiple environments
5. **Submit** a pull request with clear description

### Code Style

- **C++17** standard compliance
- **RAII** memory management
- **Clear naming** conventions
- **Comprehensive comments** for complex logic

## 📄 License

This project is provided for educational purposes. Use responsibly and in accordance with applicable laws and game terms of service.

## 🔗 Related Projects

- [cs2-dumper](https://github.com/a2x/cs2-dumper) - Memory offset dumping tool
- [Dear ImGui](https://github.com/ocornut/imgui) - Immediate mode GUI library

---

**⚠️ Disclaimer**: This software is for educational and research purposes only. Users are responsible for complying with applicable laws and game terms of service.
