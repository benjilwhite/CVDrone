/**
 * Simple program that uses wasd to send pan, tilt, zoom, and focus commands to
 * an arduino via serial. Make sure that the RadioOutput sketch has been written
 * to the arduino, and that no other program is connecting to it.
 *
 * Use w/s to tilt up/down
 * Use a/d to pan left/right
 * Use up/down arrows to zoom in and out
 * Use left/right arrows to adjust focus
 * Press 'q' to exit
 *
 * Will add tracking via OpenCV in the future.
 */

#define NCURSES_NOMACROS // prevents ncurses and boost from killing each other
#include <iostream>
#include <ncurses.h>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <boost/asio/streambuf.hpp>

using namespace std;
using namespace boost::asio;

int main() {
    ::initscr(); // Initialize ncurses
    ::raw(); // Disable line buffering
    ::keypad(stdscr, TRUE); // Enable special key handling, including arrow keys
    ::noecho(); // Don't echo input characters to the screen

    io_service io;
    serial_port sp(io, "/dev/tty.usbmodem3485187B55B02");   //replace with filepath of arduino
    sp.set_option(serial_port_base::baud_rate(115200));
    sp.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
    sp.set_option(serial_port_base::parity(serial_port_base::parity::none));
    sp.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
    sp.set_option(serial_port_base::character_size(8));

    // Default values for pan, tilt, zoom, and focus
    int pan = 1500;
    int tilt = 1500;
    int zoom = 1500;
    int focus = 1500;

    printw("Pan:  %d\nTilt: %d\nZoom: %d\nFocus: %d\n", pan, tilt, zoom, focus);
    int ch = 0;
    while (ch != 'q') {
        ch = ::getch(); // Get a character from the keyboard
        ::clear();

        switch (ch) {
            case 'w':
                if(tilt < 2000) tilt += 20;
                break;
            case 's':
                if(tilt > 1000) tilt -= 20;
                break;
            case 'a':
                if(pan > 1000) pan -= 20;
                break;
            case 'd':
                if(pan < 2000) pan += 20;
                break;
            case KEY_UP:
                if(zoom < 2000) zoom += 20;
                break;
            case KEY_DOWN:
                if(zoom > 1000) zoom -= 20;
                break;
            case KEY_LEFT:
                if(focus > 1000) focus -= 20;
                break;
            case KEY_RIGHT:
                if(focus < 2000) focus += 20;
                break;
            default:
                break;
        }
        ::printw("Pan:  %d\nTilt: %d\nZoom: %d\nFocus: %d\n", pan, tilt, zoom, focus);
        ::refresh(); // Refresh the screen

        boost::asio::streambuf buf;
        string message = std::to_string(pan) + ":"
                         + std::to_string(tilt) + ":"
                         + std::to_string(zoom) + ":"
                         + std::to_string(focus);
        write(sp, buffer(message));
    }
    ::endwin(); // Clean up ncurses
    return 0;
}
