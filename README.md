*This project has been created as part of the 42 curriculum by adkhalil.*

# Codexion

## Description

Codexion is a concurrent simulation of coders competing for shared USB dongles to compile quantum code. It is inspired by the classic Dining Philosophers problem. Each coder runs as a POSIX thread and must acquire two adjacent dongles simultaneously to compile. The simulation ends either when all coders reach the required number of compiles, or when one coder burns out from starvation.

The project demonstrates real-world concurrency challenges: deadlock prevention, starvation avoidance, fair resource arbitration, and precise timing — all implemented from scratch in C using pthreads and mutexes.

## Instructions

### Compilation

```bash
make
```

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- All time values are in milliseconds
- `scheduler` must be exactly `fifo` or `edf`
- `number_of_coders` must be at least 1

### Examples

```bash
# 5 coders, feasible with FIFO scheduling
./codexion 5 2000 200 200 200 10 0 fifo

# 5 coders with EDF scheduling and dongle cooldown
./codexion 5 4000 200 200 200 5 400 edf

# Single coder — will always burn out (cannot compile alone)
./codexion 1 800 100 100 100 3 0 fifo
```

### Cleanup

```bash
make clean   # remove object files
make fclean  # remove object files and binary
make re      # full rebuild
```

## Blocking cases handled

**Deadlock prevention** — The classic circular wait (Coffman condition) is broken by reversing the dongle acquisition order for the last coder in the ring. All other coders take their left dongle first, then right. The last coder takes right first, then left. This breaks the circular dependency and prevents deadlock entirely.

**Starvation prevention** — Each dongle maintains a min-heap priority queue of pending requests. Under FIFO, priority is the request timestamp so the earliest requester wins. Under EDF, priority is the coder's deadline (`last_compile_time + time_to_burnout`) so the coder closest to burning out wins. This guarantees liveness under feasible parameters.

**EDF tie-breaking** — When two coders share an identical deadline, the lower coder ID wins. This is enforced in both `heap_push` (bubble-up) and `bubble_down` comparisons, making EDF fully deterministic.

**Dongle cooldown** — After a dongle is released, it is unavailable for `dongle_cooldown` milliseconds. The `dongle_is_busy` function checks `released_time > 0 && elapsed < cooldown` before granting access. The `released_time > 0` guard prevents false triggering at simulation start.

**Burnout detection** — A dedicated monitor thread polls every 1ms. It reads each coder's `last_compile_time` under `time_mutex` and fires the burnout log within 10ms of the actual deadline. Coders that have already completed their required compiles keep refreshing `last_compile_time` every 10ms so the monitor never falsely kills them.

**Log serialization** — All output goes through `log_state`, which holds `log_mutex` for the duration of the `printf`. No two state messages can interleave on the same line.

**Compile count guard** — At the top of `compile_phase`, the coder checks its own compile count under `stop_mutex` before attempting to take dongles. This prevents a finished coder from re-entering the resource acquisition loop after the simulation should stop.

## Thread synchronization mechanisms

**`pthread_mutex_t` usage:**

- `dongle->mutex` — protects `in_use`, `released_time`, and the heap for each dongle. Held during heap push, the busy-wait condition check, heap pop, and `in_use` assignment.
- `sim->stop_mutex` — protects `sim->stop` and `compile_count`. All reads and writes to these fields go through this lock.
- `sim->log_mutex` — serializes all output. Held for the full duration of each `printf` call.
- `coder->time_mutex` — protects `last_compile_time`. Written by the coder thread after acquiring both dongles, and by the done-coder heartbeat loop. Read by the monitor thread.

**Busy-wait arbitration (`dongle_is_busy`):**

Rather than using `pthread_cond_t`, dongle waiting is implemented as a 500µs polling loop. Each iteration locks `dongle->mutex`, checks three conditions atomically — `in_use`, cooldown elapsed, and heap front matches — then unlocks. This avoids the Helgrind false positives that arise from condvar cross-thread association, while keeping latency well within the 10ms burnout detection window.

**Race condition prevention:**

- `last_compile_time` is always written and read under `time_mutex`, preventing the monitor from observing a torn value mid-update.
- `compile_count` increment and the all-done check are performed together under `stop_mutex`, so no coder can increment after another has already triggered the stop.
- `dongle->in_use = 1` is set inside the `dongle->mutex` lock (after `heap_pop`), so no two coders can both observe the dongle as free simultaneously.

## Resources

- [POSIX Threads Programming — Lawrence Livermore](https://hpc-tutorials.llnl.gov/posix/)
- [The Little Book of Semaphores — Allen B. Downey](https://greenteapress.com/wp/semaphores/)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Valgrind Helgrind Manual](https://valgrind.org/docs/manual/hg-manual.html)

**AI usage:** Claude (Anthropic) was used throughout this project to discuss concurrency bug diagnosis (Helgrind output interpretation, race condition identification), to reason through scheduling edge cases (EDF tie-breaking, cooldown false-trigger at start, instant burnout bug), and to suggest refactoring directions. All code was written and understood by the student; AI served as a debugging and reasoning partner, not a code generator.