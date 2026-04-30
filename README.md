# img🦍svg

## Wymagania systemowe

- Python 3
- Kompilator C++23
- Doxygen do wygenerowania dokumentacji:
  - **Ubuntu / Debian:** `sudo apt-get install doxygen`
  - **Arch Linux:** `sudo pacman -S doxygen`
  - **Windows:** `winget install doxygen.doxygen`

## Instrukcja budowania

### 1. Środowisko wirtualne i narzędzia

Windows:

```cmd
python -m venv venv
venv\Scripts\activate
pip install scons conan
```

Linux / macOS:

```bash
python3 -m venv venv
source venv/bin/activate
pip install scons conan
```

### 2. Konfiguracja Conana (tylko za pierwszym razem)

```bash
conan profile detect
```

### 3. Pobranie zależności

```bash
conan install . --build=missing
```

### 4. Budowanie i testy

Kompilacja projektu:

```bash
scons
```

Uruchomienie aplikacji:

```bash
# Linux/macOS
./build/img-svg

# Windows
.\build\img-svg.exe
```

Uruchomienie testów Catch2:

```bash
# Linux/macOS
./build/run-tests

# Windows
.\build\run-tests.exe
```

Czyszczenie plików budowania:

```bash
scons -c
```
