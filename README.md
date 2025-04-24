# WebServ

One of last projects of 42's common-core curriculum, webserv is a mini-webserver capable of serving static content and execute `python` and `php` CGI scripts.

It is a single-threaded application that uses `epoll API` for I/O multiplexing in order to allow for multiple concurrent connections.

The application implements `GET`, `POST` and `DELETE` HTTP verbs and aims to be as compliant as possible with RFC 9112. Multiple virtual servers can be set up to listen on different ports (or the same ports, but respond to requests regarding different virtual hosts).

# Installation

Clone this repo to a directory and run `make` on it. Then `./webserv <config_file.conf>` (note that some suggested configuration are provided in [config dir](./config/basic.conf)

# Config file
Refer to [this file](./config/advanced.conf) for all possible configuration possibilities. Also, the contents of example website is provided in this repo.

# Docs
Some development charts are provided in the [docs](./docs/) dir

# Team
Developed by [iury](https://github.com/iuryr), [thais](https://github.com/thaisnishimoto) and [andre](https://github.com/AndreGomesSilva)
