#pragma once

#include "MainWindow.hpp"

class App {
public:
  void start();

private:
  MainWindow window{{1920, 1080}, "Raytracing"};
};