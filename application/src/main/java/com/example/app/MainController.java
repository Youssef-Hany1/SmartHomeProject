package com.example.app;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TabPane;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.VBox;

import java.util.Random;

public class MainController {

    @FXML
    private TabPane mainTabPane;

    private UARTManager uartManager;
    private LampTabController lampTabController;
    private DoorTabController doorTabController;
    private TemperatureTabController temperatureTabController;
    @FXML
    private Label temperatureLabel; // A reference to find root
    @FXML
    private VBox rootVBox;
    @FXML
    public void initialize() {
        Platform.runLater(this::findControllersAndStartSimulation);
    }

    private void findControllersAndStartSimulation() {
        VBox root = rootVBox;

        //VBox root = (VBox) temperatureLabel.getScene().getRoot();

        VBox lampBox = (VBox) root.getChildren().get(0);
        System.out.println(lampBox.getUserData());
        lampTabController = (LampTabController) lampBox.getUserData();

        VBox doorBox = (VBox) root.getChildren().get(1);
        doorTabController = (DoorTabController) doorBox.getUserData();

        VBox tempBox = (VBox) root.getChildren().get(2);
        temperatureTabController = (TemperatureTabController) tempBox.getUserData();

        simulateData();
    }

    private void simulateData() {
        System.out.println("Simulation started");

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

                    // Simulate incoming UART temperature data
                    Thread.sleep(2000);
                    // Create a random temperature with a decimal
                    double simulatedTemp = 20 + Math.random() * 10; // 20.0 to 30.0
                    String uartData = "t_" + String.format("%.1f", simulatedTemp);
                    // Parse the UART data
                    String[] parts = uartData.split("_");
                    double parsedTemp = Double.parseDouble(parts[1]);
                    // Update the UI with the parsed temperature
                    temperatureTabController.updateTemperature(parsedTemp);

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
