# Vector Helper

## What is Vector Helper?
Vector Helper is a vector array library written in C that provides a vector array of C projects with a focus on runtime security and integrity. The project was initiated by SmallSO Studios.

## Why is there a Vector Helper?
The Vector Helper was originally a submodule in the SmallSO Studios internal C project, using C to implement roughly the same functionality as the std::vector<T> template class, while the open source version is a standalone version stripped from the internal project.

Vector Helper follows the new API specification, unlike the std::vector<T> template class. Also, Vector Helper is designed with a focus on runtime security and integrity, which will include more API call checks, legal validation of incoming and outgoing parameters, and more. In terms of performance, it is close to the std::vector<T> template class.
  
## How is Vector Helper different from other Vector Arrays?
On the whole, the main points are as follows:

- Vector Helper focuses on runtime security, but this is not absolute.
- Vector Helper focuses on runtime integrity, but this is not absolute.
- Vector Helper maximizes performance while ensuring runtime security and integrity.
- Vector Helper ensures that C/C++ can be used at the same time, providing preprocessor compatibility support.
- Vector Helper is based on the common C code across the mainstream platform (in theory), but some platforms are unproven.

## Who is currently serving Vector Helper?
...
