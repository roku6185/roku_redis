# roku_redis
Redis client written in C from scratch.

Compile:
* cmake .
* make

Tests:
* make runtests

Test program (connects to a local redis-server on port 6379):

$ bin/main

Connected.

Command was sent successfully.

Received response:

550 (bulk string)
