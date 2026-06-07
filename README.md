# img🦍svg

Kompaktowe narzędzie CLI napisane w C++ do zautomatyzowanej konwersji obrazów rastrowych na skalowalny format wektorowy.

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

### 5. Formatowanie kodu

Projekt używa `.clang-format`. Najlepiej formatować przez LSP w edytorze przy zapisie pliku.

Generowanie pliku `compile_commands.json` wymaganego przez LSP `clangd`:
```bash
scons compdb
```

Ręczne formatowanie pliku z konsoli:
```bash
clang-format -i src/main.cpp
```

### 6. Generowanie dokumentacji

Projekt używa narzędzia Doxygen, które tworzy dokumentację API na podstawie komentarzy w plikach źródłowych. Polecenie do generowania:
```bash
scons doc
```

## Instrukcja użytkowania

Po pomyślnej kompilacji, wektoryzator obsługuje się z poziomu wiersza poleceń. Należy podać ścieżkę do pliku źródłowego oraz opcjonalnie ścieżkę do pliku wyjściowego za pomocą flagi `-o` (domyślnie program zapisze wynik w pliku `output.svg`).

**Podstawowe wywołanie:**

```bash
./build/img-svg wejscie.png -o wyjscie.svg

```

**Przykład użycia flag konfiguracyjnych:**

```bash
./build/img-svg ./test-images/airplane.jpg -o samolot.svg --max-width 512 --tolerance 3 --colors 12 --stroke 3

```

**Pełna lista dostępnych opcji:**

* `-h`, `--help` — wyświetla komunikat pomocy.
* `-o`, `--output <plik>` — ścieżka do docelowego pliku SVG (domyślnie: `output.svg`).
* `--colors <liczba>` — docelowa liczba kolorów dla algorytmu kwantyzacji K-Means (domyślnie: 16).
* `--min-area <piksele>` — minimalne pole obszaru w pikselach; mniejsze plamy będą filtrowane jako szum (domyślnie: 10).
* `--tolerance <wartość>` — próg tolerancji dystansu błędu dla upraszczania wierzchołków RDP (domyślnie: 1).
* `--corner-threshold <kąt>` — próg kąta decydujący o zachowaniu ostrego rogu lub wygładzeniu ścieżki (domyślnie: 125).
* `--smoothing <wartość>` — mnożnik intensywności wygładzania krzywych (domyślnie: 1).
* `--max-width <piksele>` — maksymalna dopuszczalna szerokość; większe obrazy zostaną proporcjonalnie pomniejszone (domyślnie: 1200).
* `--stroke <wartość>` — `stroke-width` w wygenerowanym SVG; 0 aby wyłączyć (domyślnie: 1).
* `-v`, `--verbose` — włącza szczegółowe logowanie poszczególnych etapów w konsoli.

## Autorzy

* Karol Adamski
* Bartłomiej Masiak
