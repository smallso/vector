# Vector Helper

## What is Vector Helper?
Vector Helper is a Vector Array library written in C that provides a high-performance Vector Array that focuses on runtime security for C projects. If you've used C++ Std, you should know that the std::vector<T> template class declared in the <vector> header file provides a Vector Array, but std::vector<T> is only used in C++.

## Why is there a Vector Helper?
The Vector Helper was originally a sub-module in the SmallC Studios. internal pure C project, which implements the same functionality as the std::vector<T> template class in pure C, while the open source version is separated and collated from its project. Independent version. Vector Helper follows the new API specification, unlike the std::vector<T> template class. Also, Vector Helper is focused on runtime security design, which includes more API call checks, legal validation of access parameters, and more. In terms of performance, it is close to the std::vector<T> template class.
  
## How is Vector Helper different from other Vector Arrays?
On the whole, the main points are as follows:

- Vector Helper focuses on runtime security, but this is not absolute.
- Vector Helper focuses on runtime integrity, but this is not absolute.
- Vector Helper maximizes performance while ensuring runtime security and integrity.
- Vector Helper ensures that C/C++ can be used at the same time, providing preprocessor compatibility support.
- Vector Helper is based on the common C code across the mainstream platform (in theory), but some platforms are unproven.
