package com.example.app;

import com.fazecast.jSerialComm.SerialPort;

public class UARTManager {
    private SerialPort port;

    public UARTManager(String portName, int baudRate) {
        port = SerialPort.getCommPort(portName);
        port.setBaudRate(baudRate);
        port.setNumDataBits(8); // Ensure 8 data bits
        port.setNumStopBits(SerialPort.ONE_STOP_BIT); // Ensure 1 stop bit
        port.setParity(SerialPort.NO_PARITY); // Ensure no parity
        port.setFlowControl(SerialPort.FLOW_CONTROL_DISABLED); // Disable flow control
        port.openPort();

        if (port.isOpen()) {
            System.out.println("UART opened successfully on " + portName);
        } else {
            System.err.println("Failed to open UART on " + portName);
        }
    }

    public int read() {
        if (port.bytesAvailable() > 0) {
            System.out.println("Bytes available: " + port.bytesAvailable());
            byte[] buffer = new byte[1];
            int numRead = port.readBytes(buffer, 1);
            if (numRead > 0) {
                System.out.println("Read byte: " + buffer[0]);
                return buffer[0] & 0xFF;
            }
        } else {
            System.out.println("No bytes available to read.");
        }
        return -1; // Return -1 if no data is available
    }

    public void write(char c) {
        port.writeBytes(new byte[]{(byte) c}, 1);
        System.out.println("Wrote byte: " + c);
    }

    public void close() {
        port.closePort();
        System.out.println("UART port closed.");
    }
}
