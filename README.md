# isaac-core [![wercker status](https://app.wercker.com/status/800ab4d210bdb25189a893cbf204d1e8/s/master "wercker status")](https://app.wercker.com/project/bykey/800ab4d210bdb25189a893cbf204d1e8) [![Coverage Status](https://coveralls.io/repos/github/agauniyal/isaac-core/badge.svg?branch=HEAD)](https://coveralls.io/github/agauniyal/isaac-core?branch=HEAD)

![logo](https://cloud.githubusercontent.com/assets/7630575/17446369/9f9145ee-5b66-11e6-915c-60aae097dead.png)

## About

Isaac is a platform for automating all sorts of machines, primarily focused on home appliances. It is currently composed of several different modules including **isaac-core**, **isaac-dashboard**, **isaac-server**, **isaac-web** & **isaac-integrations**. It runs on any development board capable of running linux and is written in c++. Isaac is currently under heavy development and is focusing on first alpha release for now.


## Key Features

- Multithreaded design for responsive operations
- Event loop based asynchronous IO
- Efficient websockets implementation instead of weak ajax based polling
- Uses json format for communication of messages and operations, making integrations easier
- Powerful logging integration, every bad operation/crash gets tracked
- Well defined gpio implementation providing abstractions for appliance integrations
- Async task-based parallelism for longer tasks
- *More coming soon...*


## Development
Isaac-core(current repository) is written in c++14 and is built using meson build system. It has some notable dependencies including -
- uWebsockets
- Json
- spdlog

which might have their own sub-dependencies. However the build configuration has been automatized and it builds the project fetching required dependencies on its own. Emulation of development board is also in progress and some parts like access to gpio pins have been emulated so far.

### How to build
The **recommended way** is to install docker and pull the docker image - agauniyal/isaac-app. It contains everything you need to work on the development of isaac app stack and will be updated regularly. Here's an workflow - 

[![asciicast](https://asciinema.org/a/85864.png)](https://asciinema.org/a/85864)

The **other way** is to setup your own machine with all the dependencies needed to build the project. The list includes but isn't limited to libuv1, openssl, zlib, git, meson, pkgconfig and ninja build system. You can take a look at wercker.yml file in project's root which contains all the necessary step to build this project.

## License

>MIT License

>Copyright (c) 2016 Abhinav Gauniyal

>Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
