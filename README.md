Certainly! Here's a more polished and professional version of your `README.md` file:

---

# WebServer

A simple web server implementing the HTTP protocol, built primarily for learning purposes.

## Features

- **HTTP GET Requests**: The server currently handles only GET requests.
- **Static File Handling**: It can serve static files such as HTML, CSS, JavaScript, images, etc.
- **Concurrent Connections**: The server can handle multiple concurrent connections, although it closes each connection after sending the response.

## Future Improvements

While the server is functional, there are several areas for improvement and additional features that could be implemented:

- **Support for Additional HTTP Methods**: Implement other HTTP request methods such as `PUT`, `POST`, `DELETE`, `HEAD`, etc.
- **Enhanced Header Support**: Add support for more HTTP headers to improve functionality and compliance with the HTTP protocol.
- **Improved Parser**: Either build a more robust parser manually or integrate a third-party parser for better request handling.
- **Session Management**: Implement cookies and support for longer user sessions.
- **Performance Optimization**: 
  - Create multiple processes, each with multiple threads, to handle connections more efficiently.
  - Improve the listening thread to avoid busy waiting, allowing it to sleep while waiting for connections and wake up when a connection is pending (this can be achieved by default with `accept()`, but requires handling connection creation for awakening).

## Getting Started

To try out the server, follow these steps:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/phantomCat1/WebServer.git
   ```

2. **Create a Build Directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Run CMake**:
   ```bash
   cmake ..
   ```

4. **Build the Project**:
   ```bash
   make
   ```

5. **Start the Server**:
   ```bash
   ./web_server
   ```

6. **Stop the Server**:
   - To stop the server gracefully, type `stop` in the terminal.

## Contributing

Contributions are welcome! If you'd like to contribute, please fork the repository and submit a pull request with your changes. For major changes, please open an issue first to discuss what you'd like to add or modify.

## License

This project is open-source and available under the [MIT License](LICENSE).

---
