# Philosophers

> Dining Philosophers Problem — 42 School Project

## Description

Simulation of the **Dining Philosophers Problem**, a classic concurrency exercise.
A set of philosophers sit at a round table, alternating between thinking, eating, and sleeping.
Each philosopher needs two forks to eat, but there is one fork between each pair of adjacent philosophers.

The program monitors the philosophers and stops as soon as one dies of starvation — or when all philosophers have eaten enough times.

---

## Usage

```bash
make
./philo nb_philos time_to_die time_to_eat time_to_sleep [max_meals]
```

### Arguments

| Argument       | Description                                                  |
|----------------|--------------------------------------------------------------|
| `nb_philos`    | Number of philosophers (and forks)                           |
| `time_to_die`  | Time in ms before a philosopher dies if they haven't eaten   |
| `time_to_eat`  | Time in ms a philosopher takes to eat                        |
| `time_to_sleep`| Time in ms a philosopher sleeps after eating                 |
| `max_meals`    | *(Optional)* Stop the simulation when all philosophers have eaten this many times |

### Examples

```bash
# 5 philosophers, die at 800ms, eat for 200ms, sleep for 200ms
./philo 5 800 200 200

# Same but stop after each philosopher has eaten 7 times
./philo 5 800 200 200 7

# Edge case: 1 philosopher with only 1 fork (will die)
./philo 1 800 200 200
```

---

## Output format

Each event is printed as:

```
[timestamp_ms] [philosopher_id] [action]
```

Possible actions:
- `has taken a fork`
- `is thinking`
- `is eating`
- `is sleeping`
- `died`

No message is printed after a death is announced.

---

## Project structure

```
.
├── philo.h       # Structs and function prototypes
├── main.c        # Entry point, thread creation, cleanup
├── init.c        # Argument parsing, data and philosopher initialization
├── launch.c      # Philosopher thread routine
├── dying.c       # Death and meal monitoring loop
├── print.c       # Timestamped output, sleep, get_time utilities
└── Makefile
```

---

## Implementation details

### Concurrency model

- Each philosopher runs in its own **POSIX thread** (`pthread`).
- A dedicated **monitoring loop** (`dying`) runs on the main thread and continuously checks whether any philosopher has exceeded `time_to_die` since their last meal.

### Mutex usage

| Mutex          | Purpose                                             |
|----------------|-----------------------------------------------------|
| `forks[i]`     | One mutex per fork, prevents two philosophers from grabbing the same fork simultaneously |
| `print_mutex`  | Serializes all output to avoid interleaved messages |
| `death_mutex`  | Protects `someone_died`, `last_meal_time`, and `meals_eaten` from race conditions |

### Deadlock prevention

Philosophers with an **even ID** pick up their right fork first; those with an **odd ID** pick up their left fork first. This asymmetry breaks the circular wait condition and prevents deadlock.

### Starvation prevention

Even-numbered philosophers are delayed by `id * 1ms` at startup (`usleep(philo->id * 1000)`), giving odd-numbered philosophers a head start and allowing pairs to interleave naturally.

### Precise sleep

`usleep` alone is not accurate enough for short durations. `precise_sleep` busy-waits in 100µs increments, checking `get_time()` until the target duration has elapsed.

---

## Build

```bash
make        # Compile
make clean  # Remove object files
make fclean # Remove object files and binary
make re     # Full rebuild
```

---

## Notes

- All times are in **milliseconds**.
- Arguments must be **strictly positive integers**.
- The single-philosopher case is handled separately: the philosopher picks up one fork and waits until death, since the second fork is never available.
