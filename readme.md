## What is Vector Helper? [![Build Status](https://travis-ci.com/SMALLSO/Vector.svg?branch=master)](https://travis-ci.com/SMALLSO/Vector)
Vector Helper is a vector array library written in C that provides a vector array of C projects with a focus on runtime security and integrity. The project was initiated by SmallSO Studios.

## Why is there a Vector Helper?
The Vector Helper was originally a submodule in the SmallSO Studios internal C project, using C to implement roughly the same functionality as the `std::vector<T>` template class, while the open source version is a standalone version stripped from the internal project.

Vector Helper follows the new API specification, unlike the `std::vector<T>` template class. Also, Vector Helper is designed with a focus on runtime security and integrity, which will include more API call checks, legal validation of incoming and outgoing parameters, and more. In terms of performance, it is close to the `std::vector<T>` template class.
  
## How is Vector Helper different from other Vector Arrays?
On the whole, the main points are as follows:

- Vector Helper focuses on runtime security, but this is not absolute.
- Vector Helper focuses on runtime integrity, but this is not absolute.
- The API provided by Vector Helper is simple and easy to use.
- Vector Helper provides a fast enumerator.
- Vector Helper ensures the integrity of the operation, but this is not absolute.

## Who is currently serving Vector Helper?
The Vector Helper project was created by SmallSO and hosted on Github, with open source code under the MIT license. SmallSO will maintain regular maintenance of the project until it is explicitly stated, and other developers are welcome to join the development and maintenance of the project. If you have any questions or suggestions, please go to Github to submit the issue.

## Vector Helper Document
We hosted the project's documentation on a Gitbook with a detailed introduction and tutorial on each API for the project. In the project documentation, we provide a very detailed tutorial on building, installing, and using.

You can get more information by visiting https://smallso.gitbook.io/vector/.

## Other Questions
Please visit https://smallso.gitbook.io/vector/faq for more information.

## Contact
You are most welcome to submit a question through our repository page hosted on Github. You can find more information at https://github.com/SmallSO/Vector/issues.
