package com.example.app;

public class UARTManager {
    // Example with jSerialComm
    /*
    private SerialPort comPort;

    public UARTManager(String portName, int baudRate) {
        comPort = SerialPort.getCommPort(portName);
        comPort.setBaudRate(baudRate);
        comPort.openPort();
    }

    public int read() {
        if (comPort.bytesAvailable() > 0) {
            byte[] readBuffer = new byte[1];
            int numRead = comPort.readBytes(readBuffer, 1);
            if (numRead > 0) {
                return readBuffer[0];
            }
        }
        return -1;
    }

    public void write(char c) {
        comPort.writeBytes(new byte[]{(byte)c}, 1);
    }

    public void close() {
        comPort.closePort();
    }
    */

    // For now, empty.
    public UARTManager(String portName, int baudRate) {
        // do nothing
    }

    public int read() {
        return -1; // no data
    }

    public void write(char c) {
        System.out.println("UART Write: " + c);
    }

    public void close() {
        // no operation
    }
}
