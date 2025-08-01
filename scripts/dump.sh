#!/bin/bash

set -e  # Exit on any error

# ========================================
# CS2 Memory Dumper Script
# ==============echo ""
echo "📋 Dump Configuration:"
echo "   Dumper: $DUMPER_PATH"
echo "   Output: $OUTPUT_PATH"
echo "   Format: C++ headers (.hpp)"=====================

echo "🔍 CS2 Memory Dumper"

# ========================================
# Configuration
# ========================================

DUMPER_DIR="Engine/GameDumper"
DUMPER_EXE="cs2-dumper.exe"
DUMPS_DIR="Dumps"
DUMPER_PATH="$DUMPER_DIR/$DUMPER_EXE"
OUTPUT_PATH="$DUMPER_DIR/$DUMPS_DIR"

# ========================================
# Helper Functions
# ========================================

check_dumper_exists() {
    if [ ! -f "$DUMPER_PATH" ]; then
        echo "❌ CS2 dumper not found: $DUMPER_PATH"
        echo "💡 Run './scripts/update-dumper.sh' to download the dumper"
        echo "💡 Or manually place cs2-dumper.exe in Engine/GameDumper/"
        exit 1
    fi
    echo "✅ CS2 dumper found"
}

prepare_environment() {
    echo "🔧 Preparing dump environment..."
    
    # Create dumps directory
    mkdir -p "$OUTPUT_PATH"
    echo "📁 Dumps directory ready: $OUTPUT_PATH"
    
    # Set execute permissions (Unix-like systems)
    chmod +x "$DUMPER_PATH" 2>/dev/null || true
    echo "🔐 Execute permissions set"
}

attempt_dump_method() {
    local method_name="$1"
    local command="$2"
    
    echo "🔄 Trying $method_name..."
    
    if eval "$command" 2>/dev/null; then
        echo "✅ Success with $method_name"
        return 0
    else
        echo "❌ Failed with $method_name"
        return 1
    fi
}

execute_dumper() {
    echo "📊 Starting CS2 memory dump..."
    echo "⏳ This may take a few moments..."
    echo "💡 Ensure CS2 is running and you have administrator privileges"
    
    # Change to dumper directory for execution
    cd "$DUMPER_DIR"
    
    # Try different execution methods for Windows compatibility
    local methods=(
        "Direct execution:./$DUMPER_EXE -f hpp -o $DUMPS_DIR -vvv"
        "CMD wrapper:cmd.exe //c \"$DUMPER_EXE -f hpp -o $DUMPS_DIR -vvv\""
        "WinPTY wrapper:winpty ./$DUMPER_EXE -f hpp -o $DUMPS_DIR -vvv"
        "CMD fallback:cmd //c \"$DUMPER_EXE -f hpp -o $DUMPS_DIR -vvv\""
    )
    
    for method in "${methods[@]}"; do
        local method_name="${method%%:*}"
        local command="${method#*:}"
        
        if attempt_dump_method "$method_name" "$command"; then
            cd ../..
            return 0
        fi
    done
    
    echo "❌ All execution methods failed!"
    echo "💡 Common issues:"
    echo "   • CS2 not running"
    echo "   • Missing administrator privileges"
    echo "   • Antivirus blocking execution"
    echo "   • Process protection enabled"
    
    cd ../..
    return 1
}

verify_dump_output() {
    echo "🔍 Verifying dump output..."
    
    if [ -d "$OUTPUT_PATH" ] && [ "$(ls -A "$OUTPUT_PATH" 2>/dev/null)" ]; then
        local file_count=$(ls -1 "$OUTPUT_PATH"/ | wc -l)
        echo "✅ Dump verification successful!"
        echo "📁 Generated files: $file_count"
        echo "📂 Location: $OUTPUT_PATH"
        
        # List generated files
        echo "📋 Generated dump files:"
        ls -la "$OUTPUT_PATH"/ | grep -v "^total" | awk '{print "   " $9 " (" $5 " bytes)"}'
        
        return 0
    else
        echo "❌ Dump verification failed!"
        echo "⚠️  No dump files found in $OUTPUT_PATH"
        return 1
    fi
}

show_dump_summary() {
    echo ""
    echo "📊 Dump Summary:"
    echo "   Dumper: $DUMPER_PATH"
    echo "   Output: $OUTPUT_PATH"
    echo "   Status: Complete"
    echo ""
    echo "💡 These files contain CS2 memory offsets and signatures"
    echo "💡 They are required for the external cheat to function"
}

# ========================================
# Main Dump Process
# ========================================

echo ""
echo "� Dump Configuration:"
echo "   Dumper: $DUMPER_PATH"
echo "   Output: $OUTPUT_PATH"
echo "   Format: C++ headers (.hpp)"

# Step 1: Validate environment
echo ""
echo "🔍 Step 1: Environment Check"
check_dumper_exists

# Step 2: Prepare environment
echo ""
echo "🔧 Step 2: Environment Setup"
prepare_environment

# Step 3: Execute dumper
echo ""
echo "📊 Step 3: Memory Dump Execution"
if execute_dumper; then
    # Step 4: Verify output
    echo ""
    echo "🔍 Step 4: Output Verification"
    if verify_dump_output; then
        show_dump_summary
        echo "🎯 Dump process completed successfully!"
        exit 0
    else
        echo "❌ Dump process failed during verification!"
        exit 1
    fi
else
    echo "❌ Dump process failed during execution!"
    exit 1
fi