/**
 * Simple library that opens a serial port and transmits data to an arduino
 * with serial statements. Allows the user to update pan, tilt, zoom and focus
 * values.
 */
#include "DroneController.h"
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <boost/asio/streambuf.hpp>

DroneController::DroneController(const std::string& usb_device, int baud_rate) {

    // Default camera positions, in PWM pulse width microseconds
    _pan_position = 1500;
    _tilt_position = 1500;
    _zoom = 1000;
    _focus = 1000;

    // Initialize serial communication
    io = new boost::asio::io_service();
    buf = new boost::asio::streambuf();
    sp = new boost::asio::serial_port(*io, usb_device);
    sp->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    sp->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    sp->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    sp->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    sp->set_option(boost::asio::serial_port_base::character_size(8));
}

DroneController::~DroneController() {
    delete io;
    delete sp;
    delete buf;
}

// Send data to USB device over serial
void DroneController::transmitValues() {
    std::string message = std::to_string(_pan_position) + ":"
                        + std::to_string(_tilt_position) + ":"
                        + std::to_string(_zoom) + ":"
                        + std::to_string(_focus);
    boost::asio::write(*sp, boost::asio::buffer(message));
}

// Updates gimbal movement positions
void DroneController::setPosition(int pan, int tilt) {
    _pan_position = pan;
    _tilt_position = tilt;
    transmitValues();
}

// Reset position to facing forward and level
void DroneController::center() {
    this->setPosition(1500, 1500);
}

// Update zoom value
void DroneController::changeZoom(int zoom) {
    _zoom = zoom;
    transmitValues();
}

// Update focus value
void DroneController::changeFocus(int focus) {
    _focus = focus;
    transmitValues();
}