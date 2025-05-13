# PLAZZA - WHO SAID ANYTHING ABOUT PIZZAS?

![C++](https://img.shields.io/badge/language-C%2B%2B-blue.svg?logo=c%2B%2B&logoColor=white)
![Makefile](https://img.shields.io/badge/build-Makefile-brightgreen?logo=gnu&logoColor=white)
![Linux](https://img.shields.io/badge/platform-Linux-lightgrey?logo=linux)
![Nix](https://img.shields.io/badge/env-Nix-5277C3?logo=nixos&logoColor=white)
![Coverage](https://img.shields.io/badge/coverage-pending-yellow?logo=codecov)
![License](https://img.shields.io/badge/license-MIT-informational?logo=open-source-initiative)

## Overview

PLAZZA is a C++ simulation of a pizzeria, designed to teach advanced concepts in process management, thread synchronization, load balancing, and inter-process communication (IPC). The project models a reception that accepts pizza orders, manages multiple kitchens (processes), and coordinates several cooks (threads) per kitchen, each preparing pizzas according to user commands.

## Features

- **Interactive Reception Shell**: Accepts pizza orders and displays kitchen statuses.
- **Dynamic Kitchen Management**: Automatically creates new kitchens as demand increases.
- **Thread Pool per Kitchen**: Each kitchen manages its cooks using a thread pool.
- **Load Balancing**: Orders are distributed to balance workload across kitchens.
- **Ingredient Stock Management**: Kitchens manage and replenish their own ingredient stocks.
- **IPC Encapsulation**: Clean abstraction for inter-process communication.
- **Order Serialization**: Orders and pizza statuses are serialized for IPC.
- **Automatic Kitchen Shutdown**: Kitchens close after 5 seconds of inactivity.
- **Extensible Pizza Recipes**: Easily add new pizzas and ingredients via config.

## Requirements

- **gcc**
- **make**
- **Nix** (optional, for reproducible builds via `flake.nix`)

## Project Structure

```sh
.
├── src/
│   └── main.cpp
├── scripts/
│   ├── align_columns.py
│   ├── check_commit_message.py
│   ├── discard_headers.py
│   └── insert_headers.py
├── .clang-format
├── .clang-tidy
├── .gitignore
├── Makefile
├── base-config.mk
├── warning_flags.txt
├── flake.nix
├── flake.lock
├── plazza.nix
├── assignment.pdf
├── .github/
│   └── workflows/
│       └── ci.yml
└── (bonus/)
```

## Installation

1. **Clone the repository:**

   ```sh
   git clone <repository-url>
   cd <repository-directory>
   ```

2. **Build the project:**

   ```sh
   make
   ```

   - Use `make re` to force a rebuild.
   - Use `make clean` to remove object files.
   - Use `make fclean` to remove all build artifacts and binaries.

3. **(Optional) Using Nix:**

   ```sh
   nix develop
   make
   ```

## Usage

Run the reception shell with:

```sh
./plazza <multiplier> <cooks_per_kitchen> <ingredient_refill_time_ms>
```

- `<multiplier>`: Cooking time multiplier (can be a float between 0 and 1 for faster cooking).
- `<cooks_per_kitchen>`: Number of cooks (threads) per kitchen.
- `<ingredient_refill_time_ms>`: Time in milliseconds to refill one unit of each ingredient.

### Example

```sh
./plazza 1.5 3 500
```

### Reception Shell Commands

- **Order pizzas:**
  Example:

  ```txt
  regina XXL x2; fantasia M x3; margarita S x1
  ```

- **Show kitchen status:**

  ```txt
  status
  ```

## Testing

- **Coverage:**
  Run:

  ```sh
  make cov
  ```

  *(To be completed: Add test instructions and coverage details.)*

## Config File

Pizza recipes and ingredients can be configured via a `config.json` file:

```json
{
  "ingredients": [
    "ChiefLove",
    "Dough",
    "Eggplant",
    "GoatCheese",
    "Gruyere",
    "Ham",
    "Mushrooms",
    "Steak",
    "Tomato"
  ],
  "recipes": {
    "Margarita": {
      "ingredients": ["Dough", "Tomato", "Gruyere"],
      "cooking_time": 1000
    },
    "Regina": {
      "ingredients": ["Dough", "Tomato", "Gruyere", "Ham", "Mushrooms"],
      "cooking_time": 2000
    },
    "Americana": {
      "ingredients": ["Dough", "Tomato", "Gruyere", "Steak"],
      "cooking_time": 2000
    },
    "Fantasia": {
      "ingredients": ["Dough", "Tomato", "Eggplant", "GoatCheese", "ChiefLove"],
      "cooking_time": 4000
    }
  }
}
```

### Supported Elements

- **ingredients**:
    An array of strings listing all available ingredient names. Each string represents a unique ingredient that can be used in pizza recipes.

- **recipes**:
    An object where each key is a pizza name (e.g., `"Margarita"`, `"Regina"`), and the value is an object describing the recipe:
  - **ingredients**:
        An array of ingredient names (strings) required to make the pizza. All ingredients must be listed in the top-level `ingredients` array.
  - **cooking_time**:
        An integer representing the cooking time for the pizza in milliseconds.

**Example structure:**

```json
{
    "ingredients": [ ... ],
    "recipes": {
        "PizzaName": {
            "ingredients": [ ... ],
            "cooking_time": 1000
        }
    }
}
```

## Error Handling

- **Invalid arguments:** The program exits with an error if arguments are missing or invalid.
- **Config errors:** Malformed or missing config files are reported and halt execution.
- **IPC/Thread/Process errors:** All critical failures are logged and result in a clean exit.
- **REPL errors:** Invalid commands or malformed pizza orders entered in the reception shell are detected and reported with clear error messages, allowing the user to correct input without crashing or exiting the program.

## Arguments

- **multiplier**: Cooking time multiplier (float, required).
- **cooks_per_kitchen**: Number of cooks per kitchen (int, required).
- **ingredient_refill_time_ms**: Ingredient refill time in ms (int, required).

## Config

- **config.json**: Defines available ingredients and pizza recipes. See above for format.

## REPL

The reception provides an interactive shell for entering pizza orders and commands (e.g., `status`).

## Contributors

- Yohann Boniface
- Gabriel Hosquet
- Julien Bregent

For more details, see [assignment.pdf](assignment.pdf).
