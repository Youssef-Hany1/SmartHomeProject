package com.example.app;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Modality;

public class TemperatureTabController {

    @FXML
    private Label temperatureLabel;

    @FXML
    private ImageView alarmImageView; // If you no longer need the tab's alarm image, you can remove this from the FXML and controller.

    @FXML
    private AnchorPane temperatureRoot;

    private UARTManager uartManager;
    private boolean alarmOn = false;
    private int currentTemperature = 25;

    // Separate stage for the alarm window
    private Stage alarmStage;

    @FXML
    public void initialize() {
        temperatureRoot.setUserData(this);
        // You can remove setAlarm(false) if no need to set off image
        // setAlarm(false); // Alarm off by default
    }

    public void setUARTManager(UARTManager manager) {
        this.uartManager = manager;
    }

    public void updateTemperature(int temp) {
        this.currentTemperature = temp;
        // Wrap UI changes in runLater
        Platform.runLater(() -> {
            temperatureLabel.setText(temp + " °C");
            if (alarmOn) {
                updateAlarmWindow(temp); // also ensure updateAlarmWindow uses runLater for UI changes
            }
        });
    }

    public void setAlarm(boolean on) {
        this.alarmOn = on;
        Platform.runLater(() -> {
            if (on) {
                showAlarmWindow(currentTemperature);
            } else {
                hideAlarmWindow();
            }
        });
    }

    private void showAlarmWindow(int temp) {
        if (alarmStage == null) {
            alarmStage = new Stage();
            alarmStage.initModality(Modality.APPLICATION_MODAL);
            alarmStage.setTitle("ALARM!");

            ImageView alarmImg = new ImageView(new Image(
                    getClass().getResourceAsStream("/com/example/app/images/alarm_on.jpg")));

            // Set desired width and height
            alarmImg.setFitWidth(150);  // Set the width to 150 pixels
            alarmImg.setFitHeight(150); // Set the height to 150 pixels

            // Preserve image ratio (optional)
            alarmImg.setPreserveRatio(true);

            // Optional: Smooth scaling for better quality
            alarmImg.setSmooth(true);

            Label tempLabel = new Label("Current Temp: " + temp + " °C");
            tempLabel.setFont(Font.font("Arial", FontWeight.BOLD, 20));

            VBox layout = new VBox(20, alarmImg, tempLabel);
            layout.setStyle("-fx-padding: 20; -fx-alignment: center;");
            Scene scene = new Scene(layout, 300, 300);
            alarmStage.setScene(scene);
        } else {
            // If alarmStage already exists, just update it
            updateAlarmWindow(temp);
        }

        if (!alarmStage.isShowing()) {
            alarmStage.show();
        }
    }



    private void updateAlarmWindow(int temp) {
        // Ensure all UI changes happen on the FX thread
        Platform.runLater(() -> {
            if (alarmStage != null) {
                VBox layout = (VBox) alarmStage.getScene().getRoot();
                Label tempLabel = (Label) layout.getChildren().get(1);
                tempLabel.setText("Current Temp: " + temp + " °C");
            }
        });
    }


    private void hideAlarmWindow() {
        if (alarmStage != null) {
            alarmStage.close();
            alarmStage = null; // discard the reference
        }
    }
}
