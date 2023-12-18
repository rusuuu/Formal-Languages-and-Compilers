#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProiectGramaticaIDC.h"

class ProiectGramaticaIDC : public QMainWindow
{
    Q_OBJECT

public:
    ProiectGramaticaIDC(QWidget *parent = nullptr);
    ~ProiectGramaticaIDC();

private:
    Ui::ProiectGramaticaIDCClass ui;
};
