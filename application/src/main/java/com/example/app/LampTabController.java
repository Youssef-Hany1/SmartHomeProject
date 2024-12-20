package com.example.app;

import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.VBox;

import static com.example.app.MainController.uartManager;

public class LampTabController {

    @FXML
    private Label lampStatusLabel;

    @FXML
    private Label plugStatusLabel;

    @FXML
    private ImageView lampImageView;

    @FXML
    private ImageView plugImageView;

    @FXML
    private VBox lampRoot;


    @FXML
    public void initialize() {
        lampRoot.setUserData(this);
        // Set default images
        setLampImage(false); // Lamp off by default
        setPlugImage(false); // Plug off by default
    }

//    public void setUARTManager(UARTManager manager) {
//        this.uartManager = manager;
//    }

    @FXML
    private void handleLampOn() {
        sendCommand('1'); // Lamp ON
        updateLampStatus('1');
    }

    @FXML
    private void handleLampOff() {
        sendCommand('0'); // Lamp OFF
        updateLampStatus('0');
    }

    @FXML
    private void handlePlugOn() {
        sendCommand('3'); // Plug ON
        updatePlugStatus('3');

    }

    @FXML
    private void handlePlugOff() {
        sendCommand('2'); // Plug OFF
        updatePlugStatus('2');
    }

    public void updateLampStatus(char status) {
        // status: '0' = off, '1' = on
        boolean isOn = (status == '1');
        String text = isOn ? "ON" : "OFF";
        javafx.application.Platform.runLater(() -> {
            lampStatusLabel.setText(text);
            setLampImage(isOn);
        });
    }

    public void updatePlugStatus(char status) {
        // status: '2' = off, '3' = on
        boolean isOn = (status == '3');
        String text = isOn ? "ON" : "OFF";
        javafx.application.Platform.runLater(() -> {
            plugStatusLabel.setText(text);
            setPlugImage(isOn);
        });
    }

    private void setLampImage(boolean on) {
        String imageName = on ? "lamp_on.png" : "lamp_off.png";
        lampImageView.setImage(new Image(
                getClass().getResourceAsStream("/com/example/app/images/" + imageName)
        ));
    }

    private void setPlugImage(boolean on) {
        String imageName = on ? "plug_on.jpg" : "plug_off.jpg";
        plugImageView.setImage(new Image(
                getClass().getResourceAsStream("/com/example/app/images/" + imageName)
        ));
    }

    private void sendCommand(char c) {
        if (uartManager != null) {
            uartManager.write(c);
        }
        System.out.println("Sent command: " + c);
    }
}
