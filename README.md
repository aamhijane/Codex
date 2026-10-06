*This project has been created as part of the 42 curriculum by ayamhija.*

# Codexion

## Description

Codexion is a concurrency simulation written in C using POSIX threads. It models
a co-working space where several coders sit in a circle and share USB dongles.
Compiling requires two dongles at the same time (one in each hand). After
compiling, a coder debugs, then refactors, then tries to compile again.

A coder burns out if they don't start a new compile within time_to_burnout
milliseconds. The simulation stops either when every coder has compiled at least
number_of_compiles_required times, or when a coder burns out.

The goal is to coordinate access to the dongles fairly, avoid deadlocks, avoid
starvation, and detect burnout precisely.

## Instructions

### Build

    make

This produces the codexion binary at the root of the repository.

Other rules:

    make clean    - removes object files
    make fclean   - removes object files and the binary
    make re       - rebuilds from scratch

### Run

    ./codexion number_of_coders time_to_burnout time_to_compile \
                time_to_debug time_to_refactor number_of_compiles_required \
                dongle_cooldown scheduler

All eight arguments are required.

- number_of_coders: number of coders (and dongles).
- time_to_burnout: max ms between two compiles before burnout.
- time_to_compile: duration of a compile in ms.
- time_to_debug: duration of debug phase in ms.
- time_to_refactor: duration of refactor phase in ms.
- number_of_compiles_required: compiles each coder must complete.
- dongle_cooldown: ms a dongle stays unavailable after release.
- scheduler: either fifo or edf.

### Example

    ./codexion 4 800 200 200 200 3 100 fifo

## Blocking cases handled

Deadlock prevention.
Each coder is assigned a lower-indexed dongle (first) and a higher-indexed
dongle (second). Every coder requests their dongles in this fixed order, which
breaks the circular-wait condition (one of Coffman's four conditions). The last
coder's pointers are swapped so the rule holds for the whole circle. With only
one coder, the second dongle is NULL.

Starvation prevention.
Each dongle has its own priority queue implemented as a custom min-heap. In
fifo mode, requests are ordered by arrival time. In edf mode, requests are
ordered by deadline (last_compile_time + time_to_burnout). Ties are broken by
a monotonically increasing sequence number, which makes the order fully
deterministic.

Cooldown handling.
After a dongle is released, last_release_time is recorded. When a coder
reaches the top of the queue, the cooldown is checked. If it hasn't expired,
the coder releases the dongle mutex, sleeps for the remaining time, re-acquires
the lock, and re-checks. This avoids blocking other coders while waiting.

Precise burnout detection.
A dedicated monitor thread checks every coder's last_compile_time every
millisecond. Burnout is detected and logged within 10 ms of the actual event.
The monitor skips coders that already finished their required compiles.

Log serialization.
All log writes go through a single log_lock mutex. A state change is written
in one printf call under the lock, so two messages can never interleave.

## Thread synchronization mechanisms

The implementation uses these primitives:

- dongle->lock (pthread_mutex_t): one per dongle. Protects is_free,
  last_release_time, and the dongle's priority queue.
- dongle->cond (pthread_cond_t): one per dongle. Coders sleep here while
  waiting for the dongle to become available or for their turn in the queue.
- coder->lock (pthread_mutex_t): one per coder. Protects last_compile_time
  and compile_count, since both the coder and the monitor read them.
- sim->sim_lock (pthread_mutex_t): global. Protects is_running and next_seq.
- sim->log_lock (pthread_mutex_t): global. Serializes all output.

Lock order.
Every thread acquires locks in the same order: dongle->lock before
sim->sim_lock, and always before log_lock. This prevents deadlocks because no
thread can ever hold a later lock and then wait for an earlier one.

Race condition examples handled.

- is_running: written by the monitor, read by every coder. Always accessed
  under sim_lock.
- last_compile_time and compile_count: written by the coder, read by the
  monitor. Always accessed under coder->lock.
- next_seq: incremented by every coder when queuing a request. Protected by
  sim_lock to keep the sequence consistent.
- Dongle state (is_free, last_release_time, queue): always accessed under the
  dongle's own lock.
- Output: printf calls are always made under log_lock.

Thread-safe communication between coders and the monitor.
The monitor never blocks the coders. It briefly locks coder->lock to read one
coder's state, then releases it before moving to the next. When it decides to
stop the simulation, it sets is_running to 0 under sim_lock, then broadcasts on
every dongle's condition variable so any sleeping coder wakes up, re-checks
is_running, and exits cleanly.

## Resources

- POSIX Threads Programming (Google, Some playlists on Youtube).
- man pages: pthread_mutex_lock, pthread_cond_wait, pthread_create.
- AI (Deepseek)

AI usage.
AI was used as a mentor to explain concurrency concepts step by step:
condition variable semantics, min-heap operations, lock-ordering rules, and
the trade-offs between coarser and finer locking. AI did not write the final
code; every function was written, tested, and debugged manually. All
explanations were verified against the man pages and by running the program
under Valgrind and ThreadSanitizer.
