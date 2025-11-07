# Key-Value Store

This project implements an in-memory key-value store with WAL (Write-Ahead Logging) and checkpointing for crash recovery.



# Overview

This project implements a high-concurrency in-memory key-value store with durability guaranteed using Write-Ahead Logging (WAL) and Checkpointing. It is designed to simulate the internal mechanisms of modern databases like PostgreSQL.



# Features

-Multi-threaded Read/Write operations using POSIX threads  

\- Durability through WAL (Write-Ahead Log)  

\- Crash recovery by replaying WAL entries at startup  

\- Checkpointing thread to flush consistent transactions to disk  

\- Efficient synchronization using mutexes and atomic operations  

\- Performance profiling supported via `perf` and `strace`



# Architecture

# Components:

1\. Writer Threads  

&nbsp;  - Append updates to WAL log  

&nbsp;  - Update in-memory hash table  



2\. Checkpointer Thread 

&nbsp;  - Periodically reads WAL log  

&nbsp;  - Writes consistent state to `data.db`  

&nbsp;  - Ensures recovery point consistency  


3\. Reader Threads 

&nbsp;  - Retrieve values from in-memory store  

&nbsp;  - Synchronize safely using mutex locks 

4\. Write-Ahead Log (WAL)  

&nbsp;  - Every update is first written to the     WAL file before being applied to memory.  

&nbsp;  - Ensures durability — if a crash occurs, the database can recover by replaying unflushed WAL entries.  

&nbsp; 

Profiling output 

strace

<img width="956" height="593" alt="strace 1" src="https://github.com/user-attachments/assets/68588068-e1f2-432e-a9e2-a9c5661f1e6d" />
<img width="1114" height="565" alt="strace 3" src="https://github.com/user-attachments/assets/b9c7d912-4f7b-4349-8712-ed7fe69ec30b" />
<img width="1132" height="580" alt="strace 4" src="https://github.com/user-attachments/assets/464f92e1-3b8b-48b1-8cf7-e233d9a93bf7" />

perf 
<img width="926" height="562" alt="profiling-perf" src="https://github.com/user-attachments/assets/0ce69afe-7291-4acf-ab70-bdc93fa4a469" />









