# 🛠️ Build Scripts Documentation

Comprehensive build and utility scripts for the CS2 External Cheat project, featuring automated pipelines, error handling, and cross-platform compatibility.

## 📋 Scripts Overview

### 🔨 Core Build Scripts

| Script                 | Purpose                  | Features                                                        |
| ---------------------- | ------------------------ | --------------------------------------------------------------- |
| **`build.sh`**         | Application compilation  | MSBuild integration, Debug/Release modes, error filtering       |
| **`dump.sh`**          | Memory offset generation | Multi-method execution, verification, background processing     |
| **`update-dumper.sh`** | Dumper maintenance       | GitHub API integration, certificate handling, progress tracking |

### 🚀 Pipeline Orchestrator

- **`run.sh`** (Root) - Complete automation pipeline with intelligent workflow management

## 🎯 Usage Patterns

### Individual Script Execution

```bash
# Build application with specific configuration
./scripts/build.sh           # Release build (default)
./scripts/build.sh --debug   # Debug build with symbols

# Generate fresh memory offsets
./scripts/dump.sh            # Requires CS2 running + admin privileges

# Update dumper tool
./scripts/update-dumper.sh   # Downloads latest from GitHub
```

### Pipeline Automation

```bash
# Full automated pipeline
./run.sh --build             # Force rebuild with fresh offsets

# Quick launch mode
./run.sh                     # Skip dumper, use existing build

# First-time setup
git clone <repo> && cd External-cheat && ./run.sh --build
```

## 🏗️ Script Architecture

### Design Principles

- **🔧 Modular Design** - Single responsibility per script
- **⚡ Error Recovery** - Graceful failure handling with user choices
- **🌍 Cross-Platform** - Git Bash, WSL, Linux compatibility
- **📊 User Feedback** - Step-by-step progress with clear status indicators
- **🔄 Background Processing** - Non-blocking operations where appropriate

### Common Features

All scripts implement:

- **Configuration validation** before execution
- **Environment checks** for required tools
- **Progress tracking** with visual indicators
- **Error handling** with actionable recovery options
- **Summary reporting** with completion status

## 📚 Detailed Script Documentation

### 🔨 build.sh - Application Builder

**Purpose**: Compile the CS2 External Cheat using MSBuild

**Features**:

- Automatic MSBuild detection (`msbuild` / `msbuild.exe`)
- Configuration selection (Release/Debug)
- Build output filtering and error highlighting
- Post-build verification and launch options
- Clean build management with user prompts

**Usage Examples**:

```bash
./scripts/build.sh              # Release build
./scripts/build.sh --debug      # Debug build
./scripts/build.sh -d           # Debug build (short form)
```

**Dependencies**:

- MSBuild (Visual Studio Build Tools)
- Application.sln in project root
- Windows environment (MSVC compiler)

### 🔍 dump.sh - Memory Offset Generator

**Purpose**: Execute CS2 dumper to generate current memory offsets and signatures

**Features**:

- Multi-method execution for Windows compatibility
- Background process management with PID tracking
- Comprehensive output verification
- File listing with size information
- Fallback execution methods (direct, cmd, winpty)

**Execution Methods**:

1. Direct execution (`./cs2-dumper.exe`)
2. CMD wrapper (`cmd.exe //c`)
3. WinPTY wrapper (`winpty ./cs2-dumper.exe`)
4. CMD fallback (`cmd //c`)

**Usage Examples**:

```bash
./scripts/dump.sh              # Generate offsets (requires CS2 running)
```

**Dependencies**:

- cs2-dumper.exe in Engine/GameDumper/
- CS2 process running
- Administrator privileges
- Windows environment

**Output Location**: `Engine/GameDumper/Dumps/` (C++ header files)

### 📥 update-dumper.sh - Dumper Maintenance Tool

**Purpose**: Download and update the CS2 dumper from GitHub releases

**Features**:

- GitHub API integration with latest release detection
- Certificate handling for Git Bash compatibility
- Progress indication during download
- File verification with size and timestamp
- Immediate execution option post-download

**API Integration**:

- Endpoint: `https://api.github.com/repos/a2x/cs2-dumper/releases/latest`
- Certificate handling with `--insecure` flags
- Timeout management (connect: 10s, download: 300s)
- JSON parsing for release tag extraction

**Usage Examples**:

```bash
./scripts/update-dumper.sh     # Download latest dumper
```

**Dependencies**:

- curl command
- Internet connectivity
- GitHub API accessibility
- Write permissions to Engine/GameDumper/

### 🚀 run.sh - Pipeline Orchestrator

**Purpose**: Complete automation pipeline with intelligent workflow management

**Execution Modes**:

- **Build Mode** (`--build`, `-b`) - Force rebuild with fresh offsets
- **Quick Mode** (no args) - Skip dumper operations, use existing build
- **Full Mode** (any other args) - Complete pipeline with user prompts

**Workflow Logic**:

1. **Status Assessment** - Check existing application and dumps
2. **Dumper Management** - Update and execute dumper if needed
3. **Build Management** - Compile application with dependency checks
4. **Launch Preparation** - CS2 detection and application launch

**Smart Features**:

- Dump requirement detection via source code scanning
- Forced dump generation for build dependencies
- CS2 process detection with external cheat messaging
- Cross-platform application launching

## 🔧 Configuration & Environment

### Required Tools

| Tool         | Purpose                | Detection Command          |
| ------------ | ---------------------- | -------------------------- |
| **MSBuild**  | C++ compilation        | `msbuild --version`        |
| **curl**     | GitHub API / Downloads | `curl --version`           |
| **tasklist** | Process detection      | `tasklist \| grep cs2.exe` |

### Environment Variables

Scripts automatically detect:

- **OSTYPE** - Platform detection (msys, cygwin, linux)
- **PATH** - Tool availability checking
- **Working Directory** - Project root validation

### File Permissions

Scripts automatically handle:

- Execute permissions on dumper (`chmod +x`)
- Directory creation (`mkdir -p`)
- Cross-platform path handling

## 🔍 Troubleshooting Guide

### Common Issues & Solutions

**MSBuild Not Found**

```bash
# Install Visual Studio Build Tools
# Or run from Developer Command Prompt
# Verify: msbuild --version
```

**Dumper Execution Failures**

```bash
# Run as Administrator (required for memory access)
# Ensure CS2 is running
# Check antivirus exclusions
# Verify dumper exists: ls -la Engine/GameDumper/cs2-dumper.exe
```

**Download/Certificate Issues**

```bash
# Scripts use --insecure flags for Git Bash compatibility
# Check internet connectivity
# Manual download: https://github.com/a2x/cs2-dumper/releases/latest
```

**Build Failures**

```bash
# Check if dumps are required but missing
# Verify Visual Studio installation
# Try debug build: ./scripts/build.sh --debug
```

### Debug Mode

Enable verbose output for troubleshooting:

```bash
# Run individual scripts directly to see full output
bash -x ./scripts/build.sh     # Trace execution
./scripts/dump.sh 2>&1 | tee dump.log  # Capture output
```

## 🔄 Workflow Integration

### Git Hooks Integration

```bash
# Pre-commit hook example
#!/bin/bash
./scripts/build.sh --debug || exit 1
```

### IDE Integration

Scripts can be integrated into IDEs:

- **VS Code Tasks** - Use run_in_terminal for build tasks
- **Visual Studio** - External tools integration
- **Command Palette** - Quick script execution

### Continuous Integration

Scripts are designed for CI/CD integration:

- Exit codes for success/failure detection
- Automated dependency management
- Non-interactive execution modes

## 📊 Performance & Optimization

### Script Performance

- **Parallel Operations** - Background dumper execution
- **Cached Checks** - Avoid redundant validations
- **Selective Updates** - Only download when needed
- **Progress Feedback** - Non-blocking user interactions

### Build Optimization

- **Incremental Builds** - MSBuild handles dependency tracking
- **Parallel Compilation** - `/m` flag for multi-core builds
- **Configuration Caching** - Persistent build settings

## 🤝 Contributing to Scripts

### Adding New Scripts

1. Follow existing architecture patterns
2. Implement comprehensive error handling
3. Add progress indication and user feedback
4. Test on multiple environments (Git Bash, WSL)
5. Update this documentation

### Code Style Guidelines

```bash
# Use consistent formatting
set -e                          # Exit on error
echo "🔧 Clear status messages" # Emoji indicators
local variable_name             # Proper scoping
return 0                        # Explicit return codes
```

### Testing Checklist

- [ ] Works in Git Bash on Windows
- [ ] Handles missing dependencies gracefully
- [ ] Provides clear error messages
- [ ] Follows existing UI patterns
- [ ] Includes comprehensive help text

---

**💡 Tip**: All scripts include built-in help and error recovery. When in doubt, just run the script - it will guide you through any issues!
