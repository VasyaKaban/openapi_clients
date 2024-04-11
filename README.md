# OpenAPI CRUD clients service

## Installation: 
- git clone git@github.com:VasyaKaban/openapi_clients.git
- cd openapi_clients
- Execute deps.sh that will install and compile dependencies(they will be placed in the compiled folder)
- mkdir build/{Type of build}(Debug/Release)
- cd build/{Type of build}
- cmake -DCMAKE_BUILD_TYPE={Type of build} ../../
- cmake --build
- ctest (for tests execution)

## Requirements: 
- C++20
- Oat++
- Oat++-Swagger

## Dependencies resolving
You can use deps.sh script for dependencies installation and compilation. You can also pass first argument with amouns of jobs that will participate in "make" building

## Execution:
  Just execute compiled binary and use.
  For termination you can write "shutdown" in console.

## Endpoints:
- **` api/clients `** - GET request - returns all clients
- **` api/clients/{clientId} `** - GET request - returns client with target ID
- **` api/clients `** - POST request - creates new client. Client data must be passed as JSON data
- **` api/clients `** - PUT request - updates client. Client data must be passed as JSON data
- **` api/clients/{clientId} `** - DELETE request - deletes client with target ID

## More documented information about endpoints and models at **` /swagger/ui `**
