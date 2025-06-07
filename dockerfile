# Dockerfile
FROM debian:bullseye

# Install deps
RUN apt update && apt install -y \
    g++ cmake make socat \
    libssl-dev \
    git curl \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy source
COPY . .

# Build
RUN cmake -B build -S . && cmake --build build

# Expose socket path via volume mount
VOLUME ["/tmp"]

# Default command runs TaskServer
CMD ["./build/TaskApp"]
