package com.example.app;

import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.AnchorPane;

public class DoorTabController {

    @FXML
    private Label doorStatusLabel;

    @FXML
    private ListView<String> doorEventsList;

    @FXML
    private ImageView doorImageView;

    @FXML
    private AnchorPane doorRoot;

    private UARTManager uartManager;
    private char lastDoorStatus = '2'; // Assume closed initially

    @FXML
    public void initialize() {
        doorRoot.setUserData(this);
        setDoorImage(false); // door closed by default
    }

    public void setUARTManager(UARTManager manager) {
        this.uartManager = manager;
    }

    public void updateDoorStatus(char status) {
        // '2' = closed, '3' = open
        boolean isOpen = (status == '3');
        String text = isOpen ? "OPEN" : "CLOSED";

        javafx.application.Platform.runLater(() -> {
            doorStatusLabel.setText(text);
            setDoorImage(isOpen);

            // If status changed, record event
            if (status != lastDoorStatus) {
                String event = String.format("[%s] Door %s",
                        new java.text.SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new java.util.Date()),
                        isOpen ? "OPENED" : "CLOSED");
                doorEventsList.getItems().add(event);
                lastDoorStatus = status;
            }
        });
    }

    private void setDoorImage(boolean open) {
        String imageName = open ? "door_open.png" : "door_closed.png";
        doorImageView.setImage(new Image(getClass().getResourceAsStream("images/" + imageName)));
    }
}
