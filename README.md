# Introduction

[![video_deconstration](https://img.youtube.com/vi/oADotso-z_E/0.jpg)](https://www.youtube.com/watch?v=oADotso-z_E)


"Markdown Viewer" is a simple GTK-based application to view Markdown files. This application allows users to:

- Convert the displayed Markdown content to HTML.
- Save the rendered HTML content to a file.
- Preview the HTML content in a web browser.

## Prerequisites

- `gtk3` - The GTK+ graphical UI library.
- `cmark` - A C implementation of CommonMark.

Inside the application:

1. Click on "Open .md File" to select a Markdown file you wish to view.
2. The Markdown content will be displayed in the main view.
3. Click on "Save HTML" to save the rendered HTML content to a file.
4. Click on "Open in Browser" to preview the rendered content in your default web browser.

## Features

1. **Markdown Display**: The application reads and displays the content of Markdown files.
2. **HTML Rendering**: Uses the `cmark` library to convert Markdown content to an HTML string.
3. **File Interactions**: Provides options to open Markdown files and save the rendered HTML content.
4. **Browser Preview**: Allows users to view the rendered HTML in a browser.

## Known Issues

- The "Open in Browser" function uses `xdg-open temp.html` which works for Linux-based systems. For cross-platform compatibility, this might need adjustments.

## Contribution

Feel free to fork, modify, and submit pull requests. Ensure you test your code before submitting any changes.

## License

This project is open-source. Ensure you reference it if used for any of your projects.

