#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Backend/Services.h"
#include "wykres.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Dialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Services* services, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //Symulacja
    void on_SIM_start_clicked();
    void on_SIM_stop_clicked();
    void on_SIM_reset_clicked();

    void on_SIM_PLOT_AREA_editingFinished();
    void on_SIM_DELAY_editingFinished();

    //Wczytanie i zapis konfiguracji
    void on_FILE_SAVE_clicked();
    void on_FILE_LOAD_clicked();

    //ARX
    void on_OPEN_ARX_SIDEPANEL_clicked();
    void on_btn_ARX_ToggleU_toggled(bool checked);
    void on_btn_ARX_ToggleY_toggled(bool checked);

    void on_ARX_u_bottom_border_editingFinished();
    void on_ARX_u_top_border_editingFinished();
    void on_ARX_y_bottom_border_editingFinished();
    void on_ARX_y_top_border_editingFinished();

    //PID
    void on_PID_calk_zew_clicked();
    void on_PID_calk_wew_clicked();
    void on_PID_reset_uchyb_pamieci_clicked();

    void on_PID_wzmocnienie_editingFinished();
    void on_PID_T_i_editingFinished();
    void on_PID_T_d_editingFinished();


    //Generator
    void on_amplitude_editingFinished();

    void on_sinusRadiobtn_clicked();
    void on_rectangleRadioBtn_clicked();

    void on_gen_period_spinBox_editingFinished();
    void on_gen_offset_spinBox_editingFinished();
    void on_gen_filling_spinBox_editingFinished();
    void on_gen_activation_time_editingFinished();


    void updateGUIState();
    void syncGUIParameters();

private:
    Ui::MainWindow *ui;

    Services* services_main;

    Wykres* wykresWY;
    Wykres* wykresE;
    Wykres* wykresU;
    Wykres* wykresPID;

    Dialog* arxDialog = nullptr;

    void updateButtonStyle(QPushButton* btn);
    void updatePIDButtonsState();

};
#endif // MAINWINDOW_H
