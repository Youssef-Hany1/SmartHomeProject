package com.example.app;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.TabPane;
import javafx.scene.layout.AnchorPane;

import java.util.Random;

public class MainController {

    @FXML
    private TabPane mainTabPane;

    private UARTManager uartManager;
    private LampTabController lampTabController;
    private DoorTabController doorTabController;
    private TemperatureTabController temperatureTabController;

    @FXML
    public void initialize() {
        Platform.runLater(() -> {
            AnchorPane lampPane = (AnchorPane) mainTabPane.getTabs().get(0).getContent();
            lampTabController = (LampTabController) lampPane.getUserData();

            AnchorPane doorPane = (AnchorPane) mainTabPane.getTabs().get(1).getContent();
            doorTabController = (DoorTabController) doorPane.getUserData();

            AnchorPane tempPane = (AnchorPane) mainTabPane.getTabs().get(2).getContent();
            temperatureTabController = (TemperatureTabController) tempPane.getUserData();

            simulateData();
        });
    }

    private void simulateData() {
        Random random = new Random();
        new Thread(() -> {
            char lampStatus = '0';    // Lamp initially off
            char plugStatus = '2';    // Plug initially off
            char doorStatus = '2';    // Door initially closed
            boolean alarmOn = false;
            int temperature = 25;     // initial temperature

            while (true) {
                try {
                    // Toggle lamp
                    Thread.sleep(1000);
                    lampStatus = (lampStatus == '0') ? '1' : '0';
                    lampTabController.updateLampStatus(lampStatus);

//                    // Toggle plug
//                    Thread.sleep(2000);
//                    plugStatus = (plugStatus == '2') ? '3' : '2';
//                    lampTabController.updatePlugStatus(plugStatus);

                    // Toggle door
                    Thread.sleep(2000);
                    doorStatus = (doorStatus == '2') ? '3' : '2';
                    doorTabController.updateDoorStatus(doorStatus);

                    // Update temperature
                    Thread.sleep(2000);
                    temperature = 20 + random.nextInt(11); // 20-30°C
                    temperatureTabController.updateTemperature(temperature);

                    // Simulate alarm signal
                    Thread.sleep(1000);
                    // Randomly toggle alarm
                    alarmOn = !alarmOn;
                    if (alarmOn) {
                        // simulate '5' received from UART: alarm on
                        temperatureTabController.setAlarm(true);
                    } else {
                        // simulate '4' received from UART: alarm off
                        temperatureTabController.setAlarm(false);
                    }

                } catch (InterruptedException e) {
                    break;
                }
            }
        }).start();
    }
}
