#!/bin/bash

set -e  # Exit on any error

# ========================================
# CS2 External Cheat Build Script
# ========================================

echo "🔨 CS2 External Cheat Builder"

# ========================================
# Configuration
# ========================================

CONFIG="Release"
PLATFORM="x64"
SOLUTION_FILE="Application.sln"

# Parse arguments
case "$1" in
    "--debug"|"-d")
        CONFIG="Debug"
        echo "🐛 Debug build mode"
        ;;
    "")
        echo "🚀 Release build mode"
        ;;
    *)
        echo "💡 Usage: $0 [--debug|-d]"
        echo "💡 Default: Release build"
        ;;
esac

# ========================================
# Helper Functions
# ========================================

check_solution_exists() {
    if [ ! -f "$SOLUTION_FILE" ]; then
        echo "❌ $SOLUTION_FILE not found"
        echo "💡 Run this script from the project root directory"
        exit 1
    fi
}

clean_previous_build() {
    read -p "🧹 Clean previous build? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "🧹 Cleaning previous build..."
        if [ -d "Build" ]; then
            echo "🗑️  Removing Build directory..."
            rm -rf Build
        fi
        echo "✅ Clean completed"
    else
        echo "⏭️  Skipping clean"
    fi
}

find_msbuild() {
    if command -v msbuild &> /dev/null; then
        echo "msbuild"
    elif command -v msbuild.exe &> /dev/null; then
        echo "msbuild.exe"
    else
        echo "❌ MSBuild not found"
        echo "💡 Install Visual Studio Build Tools or Visual Studio"
        echo "💡 Or run from Visual Studio Developer Command Prompt"
        exit 1
    fi
}

execute_build() {
    local msbuild_cmd=$(find_msbuild)
    echo "✅ Using $msbuild_cmd"
    echo "🔄 Building $CONFIG configuration..."
    echo "⏳ This may take a moment..."
    
    # Execute build and capture output
    set +e  # Temporarily disable exit on error
    BUILD_OUTPUT=$($msbuild_cmd "$SOLUTION_FILE" \
        "//p:Configuration=$CONFIG" \
        "//p:Platform=$PLATFORM" \
        "//t:Rebuild" \
        "//m" 2>&1)
    BUILD_STATUS=$?
    set -e  # Re-enable exit on error
    
    return $BUILD_STATUS
}

show_build_errors() {
    echo ""
    echo "❌ Build failed (exit code: $BUILD_STATUS)"
    echo "� Error details:"
    echo "----------------------------------------"
    
    # Filter and show only relevant errors
    local error_output=$(echo "$BUILD_OUTPUT" | grep -i "error\|failed\|exception" || echo "")
    
    if [ -n "$error_output" ]; then
        echo "$error_output"
    else
        # If no specific errors found, show last 10 lines
        echo "$BUILD_OUTPUT" | tail -10
    fi
    
    echo "----------------------------------------"
    echo "💡 Check error messages above for details"
}

verify_build_output() {
    local output_file="Build/$CONFIG/Application.exe"
    
    if [ -f "$output_file" ]; then
        echo "📁 Output: $output_file"
        
        # Get file size
        local file_size=$(stat -c%s "$output_file" 2>/dev/null || stat -f%z "$output_file" 2>/dev/null || echo "unknown")
        echo "� Size: $file_size bytes"
        
        # Get timestamp
        if command -v stat &> /dev/null; then
            local timestamp=$(stat -c %y "$output_file" 2>/dev/null || stat -f %Sm "$output_file" 2>/dev/null || echo "unknown")
            echo "⏰ Built: $timestamp"
        fi
        
        return 0
    else
        echo "⚠️  Expected output not found: $output_file"
        echo "💡 Check Build directory for output files"
        return 1
    fi
}

offer_run_application() {
    local output_file="Build/$CONFIG/Application.exe"
    
    read -p "🚀 Run the application now? (y/N): " -n 1 -r
    echo
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "🎮 Launching application..."
        
        if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
            ./"$output_file"
        else
            ./"$output_file"
        fi
    else
        echo "ℹ️  Ready to run: ./$output_file"
    fi
}

# ========================================
# Main Build Process
# ========================================

echo ""
echo "📋 Build Configuration:"
echo "   Solution: $SOLUTION_FILE"
echo "   Configuration: $CONFIG"
echo "   Platform: $PLATFORM"

# Step 1: Validate environment
echo ""
echo "🔍 Step 1: Environment Check"
check_solution_exists

# Step 2: Clean previous build
echo ""
echo "🧹 Step 2: Clean Management"
clean_previous_build

# Step 3: Execute build
echo ""
echo "🔨 Step 3: Building Solution"
if execute_build; then
    echo ""
    echo "✅ Build completed successfully!"
    
    # Step 4: Verify output
    echo ""
    echo "🔍 Step 4: Output Verification"
    if verify_build_output; then
        # Step 5: Offer to run
        echo ""
        echo "🎮 Step 5: Launch Application"
        offer_run_application
    fi
else
    show_build_errors
    exit 1
fi

echo ""
echo "🎯 Build process completed!"
