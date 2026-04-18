#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "Backend/Services.h"

class Services;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(Services *s, QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_d_ARX_A_left_clicked();
    void on_d_ARX_A_right_clicked();

    void on_d_ARX_B_left_clicked();
    void on_d_ARX_B_right_clicked();

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_d_ARX_A_value_valueChanged(double arg1);
    void on_d_ARX_B_value_valueChanged(double arg1);

private:
    Ui::Dialog *ui;

    Services *m_services;
    std::vector<double> arxA;
    std::vector<double> arxB;
    int m_current_index_A;
    int m_current_index_B;

    void update_view();

    double G_VERY_SMALL_NUMBER = 0.0001;
};

#endif // DIALOG_H
