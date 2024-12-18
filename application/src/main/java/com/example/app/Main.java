package com.example.app;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/com/example/app/main.fxml"));
        Parent root = loader.load();

        // Set initial scene size
        Scene scene = new Scene(root, 1000, 800);

        primaryStage.setTitle("Home Appliances Control");
        primaryStage.setScene(scene);
        primaryStage.setResizable(true); // or false, depending on your preference
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}

