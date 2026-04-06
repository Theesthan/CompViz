# CompViz

A comprehensive implementation of the Huffman coding algorithm for data compression. This project provides both a command-line tool written in C++ and a browser-based visual simulator using HTML, CSS, and JavaScript.

## Features

- **Huffman Encoding & Decoding**: Efficient text compression using variable-length codes.
- **Cross-platform**: Works on modern web browsers and any system with a C++ compiler.
- **File Compression**: Compress and decompress files with the C++ tool.

## Project Structure

```
HuffmanCompressor-main/
├── Codec.js             # JavaScript Huffman codec functions
├── Huffman.cpp          # Core Huffman algorithm in C++
├── Huffman.pdf          # Detailed documentation
├── Minheap.js           # JavaScript helper for Huffman tree (min-heap)
├── README.md            # Project readme
├── index.html           # Web interface for simulation
├── script.js            # Frontend logic controller
├── styles.css           # Web styling
├── images/              # Visual assets
│   ├── bgHuff1.jpg
│   └── bgHuff2.jpg
```

## Usage Instructions

### Running the Web Interface

1. Navigate to the project folder.
2. Open `index.html` in any modern web browser (Chrome, Firefox, Edge).
3. Use the interface to:
   - Input a string
   - Generate the Huffman tree
   - View encoded results

No server is required; it's a static client-side app.

### Running the C++ Backend

1. Open a terminal and navigate to the directory.
2. Compile the source code:
   ```bash
   g++ Huffman.cpp -o huffman
   ```
3. Run the compiled program:
   ```bash
   ./huffman
   ```
   Follow on-screen prompts to encode or decode files.

## Dependencies

### JavaScript/Web
- No dependencies; pure HTML/CSS/JS.

### C++
- Requires a standard C++ compiler (e.g., GCC, Clang).

## Documentation

- For algorithm explanations, flowcharts, and usage examples, refer to `Huffman.pdf`.

```
this is an example for huffman encoding
```

Watch the tree build and codes generate dynamically.
