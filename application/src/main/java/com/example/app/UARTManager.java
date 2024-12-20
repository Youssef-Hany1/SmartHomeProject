package com.example.app;

import com.fazecast.jSerialComm.SerialPort;

public class UARTManager {
    private SerialPort port;

    public UARTManager(String portName, int baudRate) {
        port = SerialPort.getCommPort(portName);
        port.setBaudRate(baudRate);
        port.openPort();
    }

    public int read() {
        if (port.bytesAvailable() > 0) {
            byte[] buffer = new byte[1];
            int numRead = port.readBytes(buffer, 1);
            if (numRead > 0) {
                return buffer[0] & 0xFF;
            }
        }
        return -1;
    }

    public void write(char c) {
        port.writeBytes(new byte[]{(byte)c}, 1);
    }

    public void close() {
        port.closePort();
    }
}
