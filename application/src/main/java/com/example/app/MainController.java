package com.example.app;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.layout.VBox;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class MainController {

    public static UARTManager uartManager;
    private LampTabController lampController;
    private DoorTabController doorController;
    private TemperatureTabController temperatureController;

    @FXML
    private VBox rootVBox;

    private Thread uartReadThread;
    private final BlockingQueue<Character> dataQueue = new LinkedBlockingQueue<>();

    private StringBuilder messageBuffer = new StringBuilder();

    @FXML
    public void initialize() {
        Platform.runLater(this::findControllersAndStartUART);
    }

    private void findControllersAndStartUART() {
        VBox lampBox = (VBox) rootVBox.getChildren().get(0);
        lampController = (LampTabController) lampBox.getUserData();

        VBox doorBox = (VBox) rootVBox.getChildren().get(1);
        doorController = (DoorTabController) doorBox.getUserData();

        VBox tempBox = (VBox) rootVBox.getChildren().get(2);
        temperatureController = (TemperatureTabController) tempBox.getUserData();

        uartManager = new UARTManager("COM13", 9600);

        startUARTReadThread();
        startDataProcessingThread();
    }

    private void startUARTReadThread() {
        uartReadThread = new Thread(() -> {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    int data = uartManager.read();
                    if (data != -1) {
                        dataQueue.put((char) data);
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        });
        uartReadThread.setDaemon(true);
        uartReadThread.start();
    }

    private void startDataProcessingThread() {
        Thread dataProcessingThread = new Thread(() -> {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    char receivedChar = dataQueue.take();
                    processUARTMessage(receivedChar);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        });
        dataProcessingThread.setDaemon(true);
        dataProcessingThread.start();
    }

    private void processUARTMessage(char receivedChar) {
        if (receivedChar == '#') {
            // End of message
            String message = messageBuffer.toString().trim();
            messageBuffer.setLength(0); // Clear the buffer

            // Process the full message
            handleFullMessage(message);
        } else if (messageBuffer.length() == 0 && isSingleCharacterCommand(receivedChar)) {
            // Single-character commands should not be part of the buffer
            handleFullMessage(String.valueOf(receivedChar));
        } else {
            // Append the character to the buffer for multi-character messages
            messageBuffer.append(receivedChar);
        }
    }

    private boolean isSingleCharacterCommand(char c) {
        // Define valid single-character commands
        return c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f';
    }




    private void handleFullMessage(String message) {
        if (message.startsWith("t_")) {
            // Temperature data: t_XX.X#
            try {
                String tempString = message.substring(2); // Remove "t_"
                double temperature = Double.parseDouble(tempString);
                Platform.runLater(() -> temperatureController.updateTemperature(temperature));
            } catch (NumberFormatException e) {
                System.err.println("Invalid temperature format: " + message);
            }
        } else if (message.equals("a")) {
            // Lamp ON
            Platform.runLater(() -> lampController.updateLampStatus('a'));
        } else if (message.equals("b")) {
            // Lamp OFF
            Platform.runLater(() -> lampController.updateLampStatus('b'));
        } else if (message.equals("c")) {
            // Door OPEN
            Platform.runLater(() -> doorController.updateDoorStatus('c'));
        } else if (message.equals("d")) {
            // Door CLOSED
            Platform.runLater(() -> doorController.updateDoorStatus('d'));
        } else if (message.equals("e")) {
            // Alarm OFF
            Platform.runLater(() -> temperatureController.setAlarm(false));
        } else if (message.equals("f")) {
            // Alarm ON
            Platform.runLater(() -> temperatureController.setAlarm(true));
        } else {
            // Log unexpected or malformed messages
            System.err.println("Unknown or malformed message: " + message);
        }
    }



    public void stop() {
        if (uartReadThread != null && uartReadThread.isAlive()) {
            uartReadThread.interrupt();
        }
        if (uartManager != null) {
            uartManager.close();
        }
    }
}
