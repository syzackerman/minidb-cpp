# minidb-cpp
A miniature relational database engine written in C++ supporting CREATE, INSERT, and SELECT operations.

# MiniDB

A miniature SQL-like database engine implemented in C++.

## Features

- CREATE TABLE
- INSERT
- SELECT
- In-memory storage

## Future goals

- WHERE clause
- DELETE
- File persistence
- Buffer manager
- B+ tree indexing
- Transactions

## Example

CREATE students id name gpa

INSERT students 1 Sophia 3.9

SELECT students

Output:

id    name    gpa
1     Sophia  3.9
