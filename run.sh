#!/bin/bash

set -e  # Exit on any error

# ========================================
# CS2 External Cheat Build & Run Script
# ========================================

echo "🚀 CS2 External Cheat Pipeline"

# ========================================
# Configuration & Argument Parsing
# ========================================

FORCE_BUILD=false
SKIP_DUMPER=true
LAUNCH_APP=true

case "$1" in
    "--build"|"-b")
        FORCE_BUILD=true
        echo "🔨 Build mode: Will rebuild application"
        ;;
    "")
        SKIP_DUMPER=true
        echo "⚡ Quick mode: Skipping dumper steps"
        ;;
    *)
        echo "💡 Full mode: All steps included"
        ;;
esac

# ========================================
# Helper Functions
# ========================================

check_app_exists() {
    [ -f "Build/Release/Application.exe" ]
}

check_dumps_required() {
    grep -r -q "Dumps\|offsets\|signatures" Application/ Engine/ 2>/dev/null
}

check_dumps_exist() {
    [ -d "Engine/GameDumper/Dumps" ]
}

run_dumper_update() {
    if [ -f "scripts/update-dumper.sh" ]; then
        echo ""
        read -p "📥 Download latest CS2 dumper? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            set +e
            echo "n" | ./scripts/update-dumper.sh
            local status=$?
            set -e
            
            if [ $status -ne 0 ]; then
                echo "⚠️  Dumper update failed - continuing with existing"
            fi
        else
            echo "⏭️  Using existing dumper"
        fi
    else
        echo "⚠️  update-dumper.sh not found"
    fi
}

run_dumper() {
    if [ -f "scripts/dump.sh" ]; then
        echo "🔄 Running CS2 dumper..."
        ./scripts/dump.sh &
        local dump_pid=$!
        echo "⏳ Waiting for dumper completion (PID: $dump_pid)..."
        
        wait $dump_pid
        local status=$?
        
        if [ $status -eq 0 ]; then
            echo "✅ Dumper completed successfully!"
            return 0
        else
            echo "⚠️  Dumper failed (exit code: $status)"
            echo "💡 Common issues:"
            echo "   • Run as administrator"
            echo "   • Ensure CS2 is running"
            echo "   • Check antivirus settings"
            return $status
        fi
    else
        echo "❌ dump.sh not found"
        return 1
    fi
}

force_dumper_if_needed() {
    if check_dumps_required && ! check_dumps_exist; then
        echo "⚠️  Build requires dump files but none found"
        echo "� Running dumper to generate offset files..."
        
        if run_dumper; then
            return 0
        else
            echo ""
            read -p "🔧 Continue building without dumps? (y/N): " -n 1 -r
            echo
            if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                echo "👋 Please ensure CS2 is running and try again"
                exit 1
            fi
        fi
    fi
}

build_application() {
    if [ -f "scripts/build.sh" ]; then
        echo "n" | ./scripts/build.sh
    else
        echo "❌ build.sh not found"
        exit 1
    fi
}

check_cs2_running() {
    if command -v tasklist &> /dev/null; then
        if tasklist | grep -q "cs2.exe"; then
            echo "✅ CS2 is running - External cheat ready!"
            return 0
        else
            echo "⚠️  CS2 not running"
            echo "💡 External cheat can run without CS2, but won't function until CS2 starts"
            echo ""
            read -p "Continue anyway? (y/N): " -n 1 -r
            echo
            if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                echo "👋 Exiting..."
                exit 0
            fi
        fi
    fi
}

launch_application() {
    echo "🎮 Launching application..."
    
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
        ./Build/Release/Application.exe
    else
        if command -v wine &> /dev/null; then
            wine ./Build/Release/Application.exe
        else
            ./Build/Release/Application.exe
        fi
    fi
    
    echo "👋 Application closed."
}

# ========================================
# Main Pipeline
# ========================================

# Step 1: Check current state
echo ""
echo "📋 Status Check:"
if check_app_exists; then
    echo "✅ Application.exe found"
    APP_EXISTS=true
else
    echo "❌ Application.exe not found"
    APP_EXISTS=false
fi

# Step 2: Dumper operations
if [ "$SKIP_DUMPER" = false ]; then
    echo ""
    echo "� Step 1: Dumper Management"
    run_dumper_update
    
    echo ""
    echo "📊 Step 2: Running Dumper"
    if ! run_dumper; then
        echo "🔄 Continuing with existing dumps..."
    fi
else
    echo ""
    echo "⏭️  Steps 1-2: Skipping dumper operations (quick mode)"
fi

# Step 3: Build logic
echo ""
echo "🔨 Step 3: Build Management"

BUILD_NEEDED=false

if [ "$FORCE_BUILD" = true ]; then
    echo "🔨 Rebuilding (forced by --build flag)"
    force_dumper_if_needed
    BUILD_NEEDED=true
elif [ "$APP_EXISTS" = false ]; then
    echo "🔨 Building (Application.exe not found)"
    
    if [ "$SKIP_DUMPER" = true ] && check_dumps_required && ! check_dumps_exist; then
        echo "⚠️  No dump files found in quick mode"
        echo "💡 For first-time setup, run: ./run.sh --build"
        echo ""
        read -p "🔧 Continue building anyway? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            BUILD_NEEDED=true
        else
            echo "👋 Run './run.sh --build' for proper setup"
            exit 0
        fi
    else
        BUILD_NEEDED=true
    fi
elif [ "$SKIP_DUMPER" = true ]; then
    echo "⏭️  Using existing application (quick mode)"
else
    echo "🔨 Application exists - checking rebuild need"
    read -p "🔧 Rebuild application? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        BUILD_NEEDED=true
    else
        echo "⏭️  Using existing application"
    fi
fi

# Step 4: Execute build if needed
if [ "$BUILD_NEEDED" = true ]; then
    echo ""
    echo "🏗️  Building application..."
    
    if build_application; then
        echo ""
        echo "✅ Build successful!"
        
        # Ask about launching after build
        read -p "🚀 Launch application now? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            LAUNCH_APP=false
            echo "ℹ️  Run './run.sh' later to launch"
        fi
    else
        echo "❌ Build failed!"
        exit 1
    fi
else
    echo "⏭️  Skipping build"
fi

# Step 5: Launch application
if [ "$LAUNCH_APP" = true ]; then
    echo ""
    echo "🎮 Step 4: Launch Preparation"
    check_cs2_running
    launch_application
else
    echo ""
    echo "👋 Setup complete. Run './run.sh' when ready to launch."
fi
