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
        Platform.runLater(() -> {
            switch (receivedChar) {
                case '0':
                    lampController.updateLampStatus('0');
                    break;
                case '1':
                    lampController.updateLampStatus('1');
                    break;
                case '2':
                    doorController.updateDoorStatus('2');
                    break;
                case '3':
                    doorController.updateDoorStatus('3');
                    break;
                case '4':
                    temperatureController.setAlarm(false);
                    break;
                case '5':
                    temperatureController.setAlarm(true);
                    break;
                default:
                    System.out.println("Unknown data received: " + receivedChar);
            }
        });
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
