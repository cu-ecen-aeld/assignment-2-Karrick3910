# Writer Application - Build and Usage Instructions

## 📝 Overview

The `writer` application is a C program that writes text content to a file, serving as an alternative to the shell script `writer.sh`. It includes comprehensive syslog logging and cross-compilation support for embedded systems development.

## 🏗️ Build System

### Makefile Features

- ✅ **Cross-compilation support** for multiple architectures
- ✅ **Clean builds** with artifact removal  
- ✅ **Automatic dependency tracking** based on file timestamps
- ✅ **Compiler warning flags** for code quality
- ✅ **C99 standard compliance** for portability

## 🚀 Quick Start

### Native Compilation (Host System)

```bash
# Build the application
make

# Run the application
./writer /tmp/test.txt "Hello World"

# Clean build artifacts
make clean
```

### Cross-Compilation for ARM64

```bash
# Clean previous builds
make clean

# Cross-compile for ARM64
make CROSS_COMPILE=aarch64-none-linux-gnu-

# Transfer to target system and run
scp writer user@target:/usr/local/bin/
```

## 📋 Detailed Usage

### Available Make Targets

| Target | Description | Example |
|--------|-------------|---------|
| `all` (default) | Build writer executable | `make` or `make all` |
| `writer` | Explicit build target | `make writer` |  
| `clean` | Remove all build artifacts | `make clean` |

### Cross-Compilation Examples

#### ARM 64-bit (AArch64)
```bash
make CROSS_COMPILE=aarch64-none-linux-gnu-
```

#### ARM 32-bit
```bash
make CROSS_COMPILE=arm-linux-gnueabihf-
```

#### Native x86-64
```bash
make
# or explicitly
make CROSS_COMPILE=
```

### Application Usage

```bash
./writer <writefile> <writestr>
```

**Parameters:**
- `writefile` - Path to the output file
- `writestr` - Text content to write to the file

**Examples:**
```bash
# Write to a file in /tmp
./writer /tmp/example.txt "This is a test message"

# Write to a file in current directory  
./writer output.log "Application started successfully"

# Write to a file with spaces in content
./writer /var/log/app.log "User login: $(date)"
```

## 🔍 Verification Commands

### Check Build Output

```bash
# Show file type and architecture
file writer

# Show dynamic library dependencies (native builds)
ldd writer

# Show ELF header information
readelf -h writer

# Show file size
ls -lh writer
```

### Example Output

```bash
$ file writer
writer: ELF 64-bit LSB executable, x86-64, version 1 (SYSV)

$ file writer  # (ARM64 cross-compiled)
writer: ELF 64-bit LSB executable, ARM aarch64, version 1 (SYSV)
```

## 📊 Syslog Integration

The application uses syslog for comprehensive logging:

### Log Levels Used

| Level | Purpose | Example Message |
|-------|---------|-----------------|
| `LOG_DEBUG` | Normal operations | `Writing Hello World to /tmp/test.txt` |
| `LOG_ERR` | Error conditions | `Failed to open file /tmp/test.txt for writing: Permission denied` |

### View Logs

```bash
# View recent syslog entries
sudo journalctl -f

# Filter for writer application logs
sudo journalctl -t writer

# View logs with specific priority
sudo journalctl -p err
```

## 🛠️ Development Workflow

### Recommended Development Process

1. **Edit Source Code**
   ```bash
   vim writer.c
   ```

2. **Clean Build** (recommended for significant changes)
   ```bash
   make clean
   ```

3. **Compile Native Version**
   ```bash
   make
   ```

4. **Test Locally**
   ```bash
   ./writer test.txt "Debug message"
   cat test.txt  # Verify content
   ```

5. **Cross-Compile for Target**
   ```bash
   make clean
   make CROSS_COMPILE=aarch64-none-linux-gnu-
   ```

6. **Deploy to Target System**
   ```bash
   scp writer user@target:/usr/local/bin/
   ssh user@target 'chmod +x /usr/local/bin/writer'
   ```

### Testing Different Scenarios

```bash
# Test normal operation
./writer /tmp/normal.txt "Normal operation test"

# Test with existing file (should overwrite)
./writer /tmp/normal.txt "Overwrite test"

# Test error handling - invalid directory (should fail gracefully)
./writer /nonexistent/path/file.txt "This should fail"

# Test with empty string
./writer /tmp/empty.txt ""

# Test with special characters
./writer /tmp/special.txt "Special chars: !@#$%^&*()"
```

## 🚨 Troubleshooting

### Common Build Issues

| Error | Cause | Solution |
|-------|-------|----------|
| `command not found` | Cross-compiler not installed | Install toolchain: `sudo apt-get install gcc-aarch64-linux-gnu` |
| `No rule to make target` | Incorrect file names | Verify `writer.c` exists in current directory |
| `Permission denied` | Insufficient permissions | Check file permissions or use `sudo` if needed |

### Common Runtime Issues

| Error | Cause | Solution |
|-------|-------|----------|
| `Error: Two arguments required` | Incorrect usage | Use: `./writer <file> <content>` |
| `Could not create file` | Permission/path issues | Check directory exists and is writable |
| `Segmentation fault` | Binary architecture mismatch | Ensure binary matches target architecture |

### Debug Build

For debugging, you can add debug flags:

```bash
make clean
make CFLAGS="-Wall -Wextra -std=c99 -g -DDEBUG"
```

## 🔧 Customization

### Compiler Flags

Modify `CFLAGS` in Makefile for specific needs:

```makefile
# Debug build
CFLAGS = -Wall -Wextra -std=c99 -g -DDEBUG

# Optimized build  
CFLAGS = -Wall -Wextra -std=c99 -O2

# Static linking (for embedded systems)
CFLAGS = -Wall -Wextra -std=c99 -static
```

### Additional Targets

Add custom targets to Makefile:

```makefile
# Install target
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	chmod +x /usr/local/bin/$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)
```

## 📚 Additional Resources

- [GNU Make Manual](https://www.gnu.org/software/make/manual/)
- [Cross-Compilation Guide](https://elinux.org/Toolchains)
- [Syslog Programming](https://man7.org/linux/man-pages/man3/syslog.3.html)
- [C99 Standard Reference](https://en.cppreference.com/w/c)

## 📄 License

This project is part of the Advanced Embedded Linux Development course assignments.