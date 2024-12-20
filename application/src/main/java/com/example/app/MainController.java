package com.example.app;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.layout.VBox;

public class MainController {

    public static UARTManager uartManager;
    private LampTabController lampController;
    private DoorTabController doorController;
    private TemperatureTabController temperatureController;

    @FXML
    private VBox rootVBox;


    private Thread uartReadThread;

    @FXML
    public void initialize() {
        // Using runLater to ensure the scene is fully loaded and controllers are available
        Platform.runLater(this::findControllersAndStartUART);
    }

    private void findControllersAndStartUART() {
        // Retrieve controllers from userData
        VBox lampBox = (VBox) rootVBox.getChildren().get(0);
        lampController = (LampTabController) lampBox.getUserData();

        VBox doorBox = (VBox) rootVBox.getChildren().get(1);
        doorController = (DoorTabController) doorBox.getUserData();

        VBox tempBox = (VBox) rootVBox.getChildren().get(2);
        temperatureController = (TemperatureTabController) tempBox.getUserData();

        // Initialize UARTManager with COM13, 9600 baud (adjust as needed)
        uartManager = new UARTManager("COM13", 9600);

        // Start a thread to read data from UART continuously
        startUARTReadThread();
    }

    private void startUARTReadThread() {
        uartReadThread = new Thread(() -> {
            StringBuilder buffer = new StringBuilder();
            while (!Thread.currentThread().isInterrupted()) {
                int data = uartManager.read();
                if (data != -1) {
                    char c = (char) data;
                    // Assuming data is line-based or separated by '\n'
                    // Adjust this logic according to how data is sent by Tiva
                    if (c == '\n') {
                        String message = buffer.toString().trim();
                        buffer.setLength(0); // clear buffer
                        handleUARTMessage(message);
                    } else {
                        buffer.append(c);
                    }
                } else {
                    // No data available, small sleep to prevent CPU spinning
                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                }
            }
        });
        uartReadThread.setDaemon(true);
        uartReadThread.start();
    }

    private void handleUARTMessage(String message) {
        // Parse the message according to your protocol
        // Examples:
        // Lamp states: '0' or '1'
        // Plug states: '2' or '3'
        // Door states: '2' (closed) or '3' (open)
        // Temperature: "t_21.7"
        // Alarm: '4' (off), '5' (on)

        if (message.startsWith("t_")) {
            // Temperature data
            String[] parts1 = message.split("_");
            String[] parts=parts1[1].split("#");
            if (parts.length == 2) {
                try {
                    double tempVal = Double.parseDouble(parts[0]);
                    System.out.println(tempVal);
                    Platform.runLater(() -> temperatureController.updateTemperature(tempVal));
                } catch (NumberFormatException e) {
                    // handle parse error
                }
            }
        } else if (message.equals("0") || message.equals("1")) {
            // Lamp status
            char status = message.charAt(0);
            Platform.runLater(() -> lampController.updateLampStatus(status));
        } else if (message.equals("2") || message.equals("3")) {
            // Could be plug or door; you need a way to differentiate
            // Let's assume door vs plug are on separate lines or you have some prefix.
            // If no prefix, you need additional logic.
            // For example, if your device sends:
            // "L0"/"L1" for lamp
            // "P2"/"P3" for plug
            // "D2"/"D3" for door
            // "t_XX.X" for temp
            //
            // Adjust code accordingly.

            // If message is '2' or '3' for plug:
            // Platform.runLater(() -> lampController.updatePlugStatus(message.charAt(0)));

            // If message is '2' or '3' for door:
            Platform.runLater(() -> doorController.updateDoorStatus(message.charAt(0)));

            // Without a prefix, you must rely on your actual UART protocol.

        } else if (message.equals("4")) {
            // Alarm off
            Platform.runLater(() -> temperatureController.setAlarm(false));
        } else if (message.equals("5")) {
            // Alarm on
            Platform.runLater(() -> temperatureController.setAlarm(true));
        }
    }

    public void stop() {
        // Called when application is closing
        if (uartReadThread != null && uartReadThread.isAlive()) {
            uartReadThread.interrupt();
        }
        if (uartManager != null) {
            uartManager.close();
        }
    }
}
