#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProiectExpresieRegulata.h"

class ProiectExpresieRegulata : public QMainWindow
{
    Q_OBJECT

public:
    ProiectExpresieRegulata(QWidget *parent = nullptr);
    ~ProiectExpresieRegulata();

private:
    Ui::ProiectExpresieRegulataClass ui;
};
