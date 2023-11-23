/**
 * Simple library that opens a serial port and transmits data to an arduino
 * with serial statements. Allows the user to update pan, tilt, zoom and focus
 * values.
 */
#ifndef DRONECONTROLLER_LIBRARY_H
#define DRONECONTROLLER_LIBRARY_H

#include <string>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <boost/asio/streambuf.hpp>

class DroneController {
public:
    DroneController(const std::string& usb_device, int baud_rate);
    ~DroneController();
    void center(void);
    void setPosition(int x, int y);
    void setPanAngle(int angle);
    void setTiltAngle(int angle);
    void changeZoom(int zoom);
    void changeFocus(int focus);
private:
    void transmitValues(void);
    int _pan_position;
    int _tilt_position;
    int _zoom;
    int _focus;
    boost::asio::io_service* io = nullptr;
    boost::asio::serial_port* sp = nullptr;
    boost::asio::streambuf* buf = nullptr;
};

#endif //DRONECONTROLLER_LIBRARY_H
