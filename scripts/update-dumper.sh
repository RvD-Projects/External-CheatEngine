#!/bin/bash

set -e  # Exit on any error

# ========================================
# CS2 Dumper Update Script
# ========================================

echo "📥 CS2 Dumper Updater"

# ========================================
# Configuration
# ========================================

REPO="a2x/cs2-dumper"
DUMPER_DIR="Engine/GameDumper"
DUMPER_FILE="cs2-dumper.exe"
DUMPER_PATH="$DUMPER_DIR/$DUMPER_FILE"
API_URL="https://api.github.com/repos/$REPO/releases/latest"
MANUAL_URL="https://github.com/$REPO/releases/latest"

# ========================================
# Helper Functions
# ========================================

check_curl_available() {
    if ! command -v curl &> /dev/null; then
        echo "❌ curl is not available"
        echo "💡 Please install curl or download manually from: $MANUAL_URL"
        exit 1
    fi
    echo "✅ curl is available"
}

prepare_environment() {
    echo "🔧 Preparing download environment..."
    mkdir -p "$DUMPER_DIR"
    echo "📁 Directory ready: $DUMPER_DIR"
}

fetch_latest_release_info() {
    echo "🔍 Fetching latest release information..." >&2
    
    local tag_name
    tag_name=$(curl -s -k --insecure \
        --connect-timeout 10 \
        --max-time 20 \
        "$API_URL" 2>/dev/null | \
        grep '"tag_name":' | \
        head -1 | \
        sed 's/.*"tag_name":[[:space:]]*"\([^"]*\)".*/\1/' | \
        tr -d '\r\n')
    
    if [ -n "$tag_name" ] && [ "$tag_name" != "" ]; then
        echo "✅ Found release tag: $tag_name" >&2
        echo "https://github.com/$REPO/releases/download/$tag_name/$DUMPER_FILE"
    else
        echo ""
    fi
}

download_dumper() {
    local download_url="$1"
    
    echo "⬇️  Downloading $DUMPER_FILE..."
    echo "🔗 Source: $download_url"
    
    # Use curl with certificate handling for Git Bash compatibility
    if curl -k -L --insecure \
        --connect-timeout 30 \
        --max-time 300 \
        --progress-bar \
        -o "$DUMPER_PATH" \
        "$download_url"; then
        echo "✅ Download completed"
        return 0
    else
        echo "❌ Download failed"
        return 1
    fi
}

set_executable_permissions() {
    echo "🔐 Setting executable permissions..."
    chmod +x "$DUMPER_PATH" 2>/dev/null || true
    echo "✅ Permissions set"
}

verify_download() {
    echo "🔍 Verifying download..."
    
    if [ -f "$DUMPER_PATH" ]; then
        local file_size
        file_size=$(stat -c%s "$DUMPER_PATH" 2>/dev/null || stat -f%z "$DUMPER_PATH" 2>/dev/null || echo "unknown")
        
        echo "✅ Download verification successful!"
        echo "📁 Location: $DUMPER_PATH"
        echo "📊 File size: $file_size bytes"
        
        # Show file timestamp
        if command -v stat &> /dev/null; then
            local timestamp
            timestamp=$(stat -c %y "$DUMPER_PATH" 2>/dev/null || stat -f %Sm "$DUMPER_PATH" 2>/dev/null || echo "unknown")
            echo "⏰ Downloaded: $timestamp"
        fi
        
        return 0
    else
        echo "❌ Download verification failed!"
        echo "⚠️  File not found: $DUMPER_PATH"
        return 1
    fi
}

offer_immediate_run() {
    echo ""
    read -p "🚀 Run the dumper now? (y/N): " -n 1 -r
    echo
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "🏃 Running dumper..."
        if [ -f "scripts/dump.sh" ]; then
            ./scripts/dump.sh
        else
            echo "❌ dump.sh script not found"
            echo "💡 You can run the dumper manually from: $DUMPER_PATH"
        fi
    else
        echo "ℹ️  Run later with: ./scripts/dump.sh"
    fi
}

show_update_summary() {
    echo ""
    echo "📊 Update Summary:"
    echo "   Repository: $REPO"
    echo "   File: $DUMPER_FILE"
    echo "   Location: $DUMPER_PATH"
    echo "   Status: Ready"
    echo ""
    echo "💡 This dumper extracts CS2 memory offsets and signatures"
    echo "💡 Run it whenever CS2 updates to get latest offsets"
}

# ========================================
# Main Update Process
# ========================================

echo ""
echo "📋 Update Configuration:"
echo "   Repository: $REPO"
echo "   Target: $DUMPER_PATH"
echo "   API: $API_URL"

# Step 1: Environment check
echo ""
echo "🔍 Step 1: Environment Check"
check_curl_available

# Step 2: Prepare environment
echo ""
echo "🔧 Step 2: Environment Setup"
prepare_environment

# Step 3: Fetch release information
echo ""
echo "🔍 Step 3: Release Information"
DOWNLOAD_URL=$(fetch_latest_release_info)

if [ -z "$DOWNLOAD_URL" ]; then
    echo "❌ Could not determine download URL"
    echo "💡 Try downloading manually from: $MANUAL_URL"
    echo "💡 Check your internet connection and try again"
    exit 1
fi

echo "✅ Download URL resolved"

# Step 4: Download dumper
echo ""
echo "⬇️  Step 4: Download Execution"
if download_dumper "$DOWNLOAD_URL"; then
    # Step 5: Set permissions
    echo ""
    echo "🔐 Step 5: Permission Setup"
    set_executable_permissions
    
    # Step 6: Verify download
    echo ""
    echo "🔍 Step 6: Download Verification"
    if verify_download; then
        show_update_summary
        
        # Step 7: Offer immediate run
        echo ""
        echo "🚀 Step 7: Immediate Execution"
        offer_immediate_run
        
        echo ""
        echo "🎯 CS2 dumper update completed successfully!"
        exit 0
    else
        echo "❌ Update failed during verification!"
        exit 1
    fi
else
    echo "❌ Update failed during download!"
    echo "💡 Check your internet connection and try again"
    echo "💡 Manual download: $MANUAL_URL"
    exit 1
fi
