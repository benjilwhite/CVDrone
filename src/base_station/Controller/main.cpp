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
#include <ncurses.h>
#include "DroneController.h"

int main() {
    ::initscr(); // Initialize ncurses
    ::raw(); // Disable line buffering
    ::keypad(stdscr, TRUE); // Enable special key handling, including arrow keys
    ::noecho(); // Don't echo input characters to the screen

    DroneController controller("/dev/tty.usbmodem3485187B55B02", 115200);

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
                if(tilt < 1600) tilt += 20;
                controller.setPosition(pan, tilt);
                break;
            case 's':
                if(tilt > 1400) tilt -= 20;
                controller.setPosition(pan, tilt);
                break;
            case 'a':
                if(pan > 1000) pan -= 20;
                controller.setPosition(pan, tilt);
                break;
            case 'd':
                if(pan < 2000) pan += 20;
                controller.setPosition(pan, tilt);
                break;
            case KEY_UP:
                if(zoom < 2000) zoom += 20;
                controller.changeZoom(zoom);
                break;
            case KEY_DOWN:
                if(zoom > 1000) zoom -= 20;
                controller.changeZoom(zoom);
                break;
            case KEY_LEFT:
                if(focus > 1000) focus -= 20;
                controller.changeFocus(focus);
                break;
            case KEY_RIGHT:
                if(focus < 2000) focus += 20;
                controller.changeFocus(focus);
                break;
            default:
                break;
        }
        ::printw("Pan:  %d\nTilt: %d\nZoom: %d\nFocus: %d\n", pan, tilt, (zoom - 1000) * 20, (focus - 1000) * 20);
        ::refresh(); // Refresh the screen
    }
    ::endwin(); // Clean up ncurses
    return 0;
}
