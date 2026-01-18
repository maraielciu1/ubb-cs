# Architecture Diagram - DSM Lab 8

## System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                    Distributed Shared Memory                     │
│                     4 MPI Processes (Ranks)                      │
└─────────────────────────────────────────────────────────────────┘

┌───────────────┐  ┌───────────────┐  ┌───────────────┐  ┌───────────────┐
│   Rank 0      │  │   Rank 1      │  │   Rank 2      │  │   Rank 3      │
│               │  │               │  │               │  │               │
│ ┌───────────┐ │  │ ┌───────────┐ │  │ ┌───────────┐ │  │ ┌───────────┐ │
│ │Variables  │ │  │ │Variables  │ │  │ │Variables  │ │  │ │Variables  │ │
│ │[0,1,2,3,4]│ │  │ │[0,1,2,3,4]│ │  │ │[0,1,2,3,4]│ │  │ │[0,1,2,3,4]│ │
│ └───────────┘ │  │ └───────────┘ │  │ └───────────┘ │  │ └───────────┘ │
│               │  │               │  │               │  │               │
│ ┌───────────┐ │  │ ┌───────────┐ │  │ ┌───────────┐ │  │ ┌───────────┐ │
│ │Lamport    │ │  │ │Lamport    │ │  │ │Lamport    │ │  │ │Lamport    │ │
│ │Clock      │ │  │ │Clock      │ │  │ │Clock      │ │  │ │Clock      │ │
│ └───────────┘ │  │ └───────────┘ │  │ └───────────┘ │  │ └───────────┘ │
│               │  │               │  │               │  │               │
│ ┌───────────┐ │  │ ┌───────────┐ │  │ ┌───────────┐ │  │ ┌───────────┐ │
│ │Pending    │ │  │ │Pending    │ │  │ │Pending    │ │  │ │Pending    │ │
│ │Queue      │ │  │ │Queue      │ │  │ │Queue      │ │  │ │Queue      │ │
│ └───────────┘ │  │ └───────────┘ │  │ └───────────┘ │  │ └───────────┘ │
│               │  │               │  │               │  │               │
│ ┌───────────┐ │  │ ┌───────────┐ │  │ ┌───────────┐ │  │ ┌───────────┐ │
│ │Background │ │  │ │Background │ │  │ │Background │ │  │ │Background │ │
│ │Thread     │ │  │ │Thread     │ │  │ │Thread     │ │  │ │Thread     │ │
│ └───────────┘ │  │ └───────────┘ │  │ └───────────┘ │  │ └───────────┘ │
└───────────────┘  └───────────────┘  └───────────────┘  └───────────────┘
       ↕                  ↕                  ↕                  ↕
       └──────────────────┴──────────────────┴──────────────────┘
                        MPI Communication
```

## Write Operation Flow

```
Step 1: Write Initiated
═══════════════════════

Rank 0 (User Thread)                     Rank 1,2,3 (Background Thread)
─────────────────────                    ───────────────────────────────
dsm.write(var0, 100)
  │
  ├─ Increment Lamport Clock: T=10
  ├─ Create message (WRITE, var0, 100, T=10)
  ├─ Add to own pending queue
  ├─ Self-ACK
  │
  └─ MPI_Send ──────────────────────────→ Receive WRITE message
                                           │
                                           ├─ Update clock: T=max(T_local,10)+1
                                           ├─ Add to pending queue
                                           ├─ Send ACK to all ────┐
                                           └─────────────────────┐ │
                                                                 ↓ ↓
Step 2: ACK Exchange
════════════════════

Rank 0 (Background)                      Rank 1,2,3 (Background)
───────────────────                      ───────────────────────
Receive ACKs ←─────────────────────────── Send ACKs
  │
  └─ Update ACK set
     ACK_set[(0,msgid)] = {0,1,2,3}


Step 3: Delivery
════════════════

All Ranks (Background Thread)
─────────────────────────────
Check pending queue:
  │
  ├─ Message at head: (WRITE, var0, 100, T=10)
  ├─ haveAllAcks? → YES
  │
  ├─ Pop from queue
  ├─ old_value = variables[0]  // Save old
  ├─ variables[0] = 100         // Update
  │
  └─ invoke_callback(0, old_value, 100, T=10)
        │
        └─→ User sees: "Var 0: 0 → 100 | T=10"

All ranks deliver at same position in queue ✅
All ranks invoke callback with same values ✅
```

## CAS Operation Flow

```
Step 1: Multiple CAS Attempts
══════════════════════════════

Rank 0: CAS(var3, 0, 20) at T=10
Rank 1: CAS(var3, 0, 30) at T=15
Rank 2: CAS(var3, 0, 40) at T=12

Step 2: Total Order
═══════════════════

All ranks' pending queues eventually contain:
  ┌─────────────────────────────┐
  │ (CAS, var3, 20, exp=0, T=10)│ ← Head (smallest T)
  ├─────────────────────────────┤
  │ (CAS, var3, 40, exp=0, T=12)│
  ├─────────────────────────────┤
  │ (CAS, var3, 30, exp=0, T=15)│
  └─────────────────────────────┘

Step 3: Sequential Delivery
════════════════════════════

All Ranks Execute (in order):

1. CAS(var3, 0, 20) at T=10
   ├─ Check: variables[3] == 0? → YES ✓
   ├─ variables[3] = 20
   └─ Rank 0 returns: SUCCESS

2. CAS(var3, 0, 40) at T=12
   ├─ Check: variables[3] == 0? → NO (it's 20)
   └─ Rank 2 returns: FAILED

3. CAS(var3, 0, 30) at T=15
   ├─ Check: variables[3] == 0? → NO (it's 20)
   └─ Rank 1 returns: FAILED

Result: Only Rank 0 succeeds ✅
```

## Subscription Groups

```
Variable Subscription Model
════════════════════════════

Variable 0 (Global)                Variable 2 (Partial)
───────────────────                ────────────────────
Subscribers: {0, 1, 2, 3}          Subscribers: {0, 2, 3}

Write to var 0:                    Write to var 2:
  Rank 0 ──→ {1, 2, 3}              Rank 0 ──→ {2, 3}
  ACKs: All 4 must ACK              ACKs: Only 3 must ACK

Callback on all 4 ranks            Callback only on {0, 2, 3}
                                   Rank 1 never sees changes
```

## Message Format

```
MPI Message Structure (7 integers)
═══════════════════════════════════

┌─────────────┬─────────┬───────────┬──────────┬────────┬────────┬───────────┐
│ MessageType │ var_id  │ new_value │ expected │ sender │ msg_id │ timestamp │
├─────────────┼─────────┼───────────┼──────────┼────────┼────────┼───────────┤
│    1=WRITE  │   0-4   │    any    │    0     │  0-3   │   seq  │  Lamport  │
│    2=CAS    │   0-4   │    any    │   any    │  0-3   │   seq  │  Lamport  │
│    3=ACK    │   0-4   │    0      │    0     │ origin │  seq   │  Lamport  │
└─────────────┴─────────┴───────────┴──────────┴────────┴────────┴───────────┘

Example WRITE message:
  [1, 0, 100, 0, 0, 42, 101]
   │  │   │   │  │  │   │
   │  │   │   │  │  │   └── Lamport timestamp = 101
   │  │   │   │  │  └────── Message ID = 42
   │  │   │   │  └───────── Sender = Rank 0
   │  │   │   └──────────── Expected = 0 (unused for WRITE)
   │  │   └──────────────── New value = 100
   │  └──────────────────── Variable ID = 0
   └─────────────────────── Type = WRITE

Example CAS message:
  [2, 3, 777, 20, 2, 15, 250]
   │  │   │    │  │  │   │
   │  │   │    │  │  │   └── Lamport timestamp = 250
   │  │   │    │  │  └────── Message ID = 15
   │  │   │    │  └───────── Sender = Rank 2
   │  │   │    └──────────── Expected = 20 (CAS condition)
   │  │   └───────────────── New value = 777
   │  └───────────────────── Variable ID = 3
   └──────────────────────── Type = CAS
```

## Thread Architecture

```
Each MPI Process Has 2 Threads
═══════════════════════════════

┌─────────────────────────────────────┐
│         MPI Process (Rank i)         │
├─────────────────────────────────────┤
│                                     │
│  ┌────────────────────────────┐    │
│  │    Main/User Thread        │    │
│  ├────────────────────────────┤    │
│  │ • Runs main()              │    │
│  │ • Calls write()            │    │
│  │ • Calls CAS() [blocks]     │    │
│  │ • Calls read()             │    │
│  │ • Receives callbacks       │    │
│  └────────────────────────────┘    │
│              ↕                      │
│         (mutexes)                   │
│              ↕                      │
│  ┌────────────────────────────┐    │
│  │   Background Thread        │    │
│  ├────────────────────────────┤    │
│  │ • Polls MPI_Iprobe()       │    │
│  │ • Receives messages        │    │
│  │ • Sends ACKs               │    │
│  │ • Delivers operations      │    │
│  │ • Updates variables        │    │
│  │ • Invokes callbacks        │    │
│  │ • Signals CAS results      │    │
│  └────────────────────────────┘    │
│              ↕                      │
│         MPI_Send/Recv               │
└─────────────────────────────────────┘
```

## Data Flow Diagram

```
Write Operation Data Flow
══════════════════════════

User Code                DSM API               Background Thread           MPI Network
─────────                ───────               ─────────────────           ───────────

write(0,100) ────→ Lock mutexes
                   │
                   ├─ Clock++
                   ├─ Create PendingMessage
                   ├─ Add to queue
                   ├─ Self-ACK
                   │
                   └─ MPI_Send ──────────────→ [network] ──────────────→ Other ranks
                   │                                                      receive
                   └─ Return immediately                                 │
                                                                         ↓
                                                                    MPI_Iprobe
                                                                         │
                                                                         ↓
                                                                    MPI_Recv
                                                                         │
                                                                         ↓
                                                                    Add to queue
                                                                         │
                                                                         ↓
                                                                    Send ACKs
                                                                         │
             ←─────────────────────────────────────────────────────────┘
             │
             ↓
        Receive ACKs
             │
             ↓
        Update ACK_set
             │
             ↓
        All ACKed? ─── NO ──→ Wait for more
             │
             YES
             ↓
        Pop from queue
             │
             ↓
        Update variable
             │
             ↓
        Invoke callback ────→ User's lambda
                              │
                              ↓
                         Print/Log/Store
```

## Consistency Model

```
Sequential Consistency Guarantee
═════════════════════════════════

Property 1: Total Order
───────────────────────
All processes see operations in same order

Process 0: A → B → C → D
Process 1: A → B → C → D
Process 2: A → B → C → D
Process 3: A → B → C → D

✅ Same sequence everywhere


Property 2: Program Order
─────────────────────────
Operations from same process remain ordered

Process 0 executes:
  write(x, 1)  at T=10
  write(x, 2)  at T=20

All processes see:
  x: 0→1 (T=10)
  x: 1→2 (T=20)

Never:
  x: 0→2 (T=20)
  x: 2→1 (T=10)

✅ Causality preserved


Property 3: Real-time Order
────────────────────────────
If operation A completes before B starts (in real time),
A appears before B in total order

Process 0: write(x,1) completes at real-time 100ms
Process 1: write(x,2) starts   at real-time 200ms

All see: x:0→1, then x:1→2

✅ Lamport clocks enforce this
```

## Component Interaction

```
Lifecycle of a Variable Update
═══════════════════════════════

┌─────────┐
│ Initial │  variables[0] = 0
└────┬────┘
     │
     ↓
┌────────────┐
│ Write Call │  dsm.write(0, 100)
└────┬───────┘
     │
     ↓
┌────────────┐
│  Multicast │  Send to all subscribers
└────┬───────┘
     │
     ↓
┌────────────┐
│ ACK Phase  │  Wait for all ACKs
└────┬───────┘
     │
     ↓
┌────────────┐
│  Delivery  │  All subscribers deliver simultaneously (logically)
└────┬───────┘
     │
     ↓
┌────────────┐
│  Callback  │  User function called: callback(0, 0, 100, T)
└────┬───────┘
     │
     ↓
┌─────────┐
│  Final  │  variables[0] = 100
└─────────┘
```

## Summary

- **Replicated**: Every process has full copy
- **Distributed**: No central server
- **Consistent**: Sequential consistency via total-order
- **Scalable**: Per-variable subscription groups
- **Atomic**: CAS operations are linearizable
- **Efficient**: Local reads, coordinated writes

