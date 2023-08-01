Installing the compiler GCC


Ubuntu or Debian

sudo apt-get update
sudo apt-get install build-essential

Fedora

sudo dnf install gcc


CentOS or RHEL

sudo yum groupinstall "Development Tools"

macOS

brew install gcc

Windows

On Windows, the easiest way to install GCC is to use MinGW. You can download and install the MinGW package from their site(http://www6.mingw.org/?template=ARROW_3&tdfs=0&s_token=1690906948.0356440000&uuid=1690906948.0356440000&term=GCC%20Compiler&term=C%20Language%20Compiler&) and then run the command in the terminal:
term=Test%20Code%20Online&searchbox=0&showDomain=0&backfill=0

mingw-get install gcc

Program compilation


sudo apt-get update
sudo apt-get install libwebkit2gtk-4.0-dev

sudo apt-get install libcmark-dev

gcc main.c -o markdown_viewer `pkg-config --cflags --libs gtk+-3.0` -lcmark

./markdown_viewer
