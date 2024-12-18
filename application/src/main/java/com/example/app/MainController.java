package com.example.app;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.TabPane;
import javafx.scene.layout.AnchorPane;

public class MainController {

    @FXML
    private TabPane mainTabPane;

    private UARTManager uartManager;
    private LampTabController lampTabController;
    private DoorTabController doorTabController;

    @FXML
    public void initialize() {
        // Delay controller retrieval until after initialization to ensure userData is set
        Platform.runLater(() -> {
            AnchorPane lampPane = (AnchorPane) mainTabPane.getTabs().get(0).getContent();
            lampTabController = (LampTabController) lampPane.getUserData();

            AnchorPane doorPane = (AnchorPane) mainTabPane.getTabs().get(1).getContent();
            doorTabController = (DoorTabController) doorPane.getUserData();

            // Assign UARTManager if needed
            // uartManager = new UARTManager("COM3", 9600);

            lampTabController.setUARTManager(uartManager);
            doorTabController.setUARTManager(uartManager);

            // Simulate data if no hardware is connected
            simulateData();
        });
    }

    private void simulateData() {
        new Thread(() -> {
            char lampStatus = '0';
            char doorStatus = '2';

            while (true) {
                try {
                    Thread.sleep(2000);
                    lampStatus = (lampStatus == '0') ? '1' : '0';
                    lampTabController.updateLampStatus(lampStatus);

                    Thread.sleep(3000);
                    doorStatus = (doorStatus == '2') ? '3' : '2';
                    doorTabController.updateDoorStatus(doorStatus);

                } catch (InterruptedException e) {
                    break;
                }
            }
        }).start();
    }
}
