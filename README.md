
# 🧩 TaskManager CLI System

TaskManager is a C++ backend system consisting of:
- 🛠️ `TaskServer`: A TCP server daemon that manages tasks via JSON commands
- 🖥️ `taskctl`: A CLI tool for interacting with the server (add, list, complete, remove)
- 🧪 Fully tested using Google Test
- 🐳 Containerized with Docker and docker-compose

---

## 🏗️ Build Instructions

```bash
git clone <this_repo>
cd TaskManager
mkdir -p build && cd build
cmake ..
make
```

---

## 🚀 Running the Server

### Run locally:

```bash
./TaskApp
```

It will start the server on port `5050`.

### Run with Docker:

```bash
docker-compose build
docker-compose up -d
```

You can check logs with:

```bash
docker logs -f taskserver
```

---

## 🖥️ Using `taskctl`

`taskctl` connects to the TaskServer at `localhost:5050` and lets you:

```bash
./taskctl list
./taskctl add "Buy milk" 2 15
./taskctl done 1
./taskctl remove 1
```

---

## 🔗 Add `taskctl` as a Global Command

You can add `taskctl` to your shell globally:

### Temporary (for current session):
```bash
alias taskctl="/full/path/to/TaskManager/build/taskctl"
```

### Permanent (add to your shell config):

For bash:
```bash
echo 'alias taskctl="/full/path/to/TaskManager/build/taskctl"' >> ~/.bashrc
source ~/.bashrc
```

For zsh:
```bash
echo 'alias taskctl="/full/path/to/TaskManager/build/taskctl"' >> ~/.zshrc
source ~/.zshrc
```

Now you can just type:

```bash
taskctl list
taskctl add "Write code"
```

---

## 🧪 Running Tests

```bash
cd build
make GTestTaskTests
./GTestTaskTests
```

---

## 📦 Folder Structure

```
TaskManager/
├── src/                # All .cpp source files
├── include/            # All header files
├── test/               # Unit tests (Google Test)
├── build/              # CMake build output
├── dockerfile
├── docker-compose.yml
├── CMakeLists.txt
└── README.md
```

---

## ✅ Features

- TCP server using `AF_INET`
- JSON protocol via `nlohmann::json`
- CLI client with colorful output
- Persistent task storage in `tasks.json`
- Full Docker support
- Modular architecture
- Unit tested with GoogleTest

---

## 📬 Coming Soon

- Configurable socket/port
- Task due dates
- REST interface
