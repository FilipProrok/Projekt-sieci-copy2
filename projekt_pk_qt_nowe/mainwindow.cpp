#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialog.h"
#include <QFileDialog>

// ==========================================
// Konstruktor i Destruktor
// ==========================================

MainWindow::MainWindow(Services* services, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , services_main(services)
{
    ui->setupUi(this);

    connect(services_main, &Services::simulationUpdated, this, &MainWindow::updateGUIState);
    connect(services_main, &Services::configurationLoaded, this, &MainWindow::syncGUIParameters);

    // Wykres Wartości Zadanej (W) i Wyjścia (Y)
    wykresWY = new Wykres(ui->plot_WY, "Regulacja (W vs Y)", "Wartość");
    wykresWY->add_series("Wartość Zadana (W)", Qt::red, false);  // Index 0
    wykresWY->add_series("Wyjście Obiektu (Y)", Qt::blue, false); // Index 1

    // Wykres Uchybu (E)
    wykresE = new Wykres(ui->plot_E, "Uchyb Regulacji (e)", "e");
    wykresE->add_series("Uchyb", Qt::darkGreen);

    // Wykres Sterowania (U) + sterowania po nasyceniu
    wykresU = new Wykres(ui->plot_U, "Sygnał Sterujący (u)", "u");
    wykresU->add_series("Sterowanie po nasyceniu", Qt::darkMagenta);
    wykresU->add_series("Sterowanie", Qt::magenta);
    wykresU->reverse_legend_order();


    // Wykres Składowych PID
    wykresPID = new Wykres(ui->plot_PID, "Składowe PID", "Wartość");
    wykresPID->add_series("P", Qt::red);
    wykresPID->add_series("I", Qt::green);
    wykresPID->add_series("D", Qt::blue);

    syncGUIParameters();
    updateGUIState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==========================================
// Logika Główna (Aktualizacja GUI)
// ==========================================

void MainWindow::updateGUIState()
{
    double time = services_main->getCurrentSimulationTime();
    double w = services_main->getCurrentW(); // Wartość zadana
    double y = services_main->getCurrentY(); // Wyjście (regulowana)
    double u = services_main->getCurrentU(); // Sterowanie
    double u_sat = services_main->getCurrentUSat(); // Sterowanie po nasyceniu
    double e = services_main->getCurrentE(); // Uchyb

    ui->SIM_TIME->setText(QString::number(time, 'f', 2) + " s");

    // Wykres W i Y
    wykresWY->add_point(0, time, w);
    wykresWY->add_point(1, time, y);

    // Wykres E
    wykresE->add_point(0, time, e);

    // Wykres U
    wykresU->add_point(0, time, u_sat);
    wykresU->add_point(1, time, u);

    // Wykres PID
    wykresPID->add_point(0, time, services_main->get_PID_P());
    wykresPID->add_point(1, time, services_main->get_PID_I());
    wykresPID->add_point(2, time, services_main->get_PID_D());
}

// ==========================================
// Panel: Symulacja i Wykresy (Lewa strona)
// ==========================================

void MainWindow::on_SIM_start_clicked()
{
    services_main->set_ARX_U_borders(ui->ARX_u_top_border->value(), ui->ARX_u_bottom_border->value());
    services_main->set_ARX_Y_borders(ui->ARX_y_top_border->value(), ui->ARX_y_bottom_border->value());

    int interval_ms = ui->SIM_DELAY->value();

    services_main->startSimulation(interval_ms);
    ui->SIM_start->setEnabled(false);
    ui->SIM_stop->setEnabled(true);
}


void MainWindow::on_SIM_stop_clicked()
{
    services_main->stopSimulation();
    ui->SIM_start->setEnabled(true);
    ui->SIM_stop->setEnabled(false);
}

void MainWindow::on_SIM_reset_clicked()
{
    services_main->resetSimulation();

    services_main->set_ARX_U_borders(ui->ARX_u_top_border->value(), ui->ARX_u_bottom_border->value());
    services_main->set_ARX_Y_borders(ui->ARX_y_top_border->value(), ui->ARX_y_bottom_border->value());

    wykresWY->clear();
    wykresE->clear();
    wykresU->clear();
    wykresPID->clear();

    wykresWY->add_series("Wartość Zadana (W)", Qt::red, false);
    wykresWY->add_series("Wyjście Obiektu (Y)", Qt::blue, false);

    wykresE->add_series("Uchyb", Qt::darkGreen);

    wykresU->add_series("Sterowanie po nasyceniu", Qt::darkMagenta);
    wykresU->add_series("Sterowanie", Qt::magenta);
    wykresU->reverse_legend_order();


    wykresPID->add_series("P", Qt::red);
    wykresPID->add_series("I", Qt::green);
    wykresPID->add_series("D", Qt::blue);

    if (ui->rectangleRadioBtn->isChecked()) {
        wykresWY->setStepStyle(0, true);
    }

    ui->SIM_start->setEnabled(true);
    ui->SIM_stop->setEnabled(false);
    updateGUIState();
}

void MainWindow::on_SIM_DELAY_editingFinished()
{
    int new_interval = ui->SIM_DELAY->value();

    if(services_main->isSimulationRunning()){
        services_main->stopSimulation();
        services_main->startSimulation(new_interval);
    }
    double currentPeriod = ui->gen_period_spinBox->value();
    services_main->set_Generator_PeriodT(new_interval, currentPeriod);
}

void MainWindow::on_SIM_PLOT_AREA_editingFinished()
{
    double time_window = ui->SIM_PLOT_AREA->value();
    wykresWY->setTimeWindow(time_window);
    wykresE->setTimeWindow(time_window);
    wykresU->setTimeWindow(time_window);
    wykresPID->setTimeWindow(time_window);
}

// ==========================================
// Panel: Pliki (Zapis/Odczyt)
// ==========================================

void MainWindow::on_FILE_SAVE_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
    tr("Zapisz konfigurację"),
    "",
    tr("Pliki JSON (*.json);;Wszystkie pliki (*)"));

    if (fileName.isEmpty()) return; // Anulowano

    if (!fileName.endsWith(".json")) fileName += ".json";

    services_main->saveConfiguration(fileName);
}

void MainWindow::on_FILE_LOAD_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Wczytaj konfigurację"), "", tr("Pliki JSON (*.json);;Wszystkie pliki (*)"));

    if (fileName.isEmpty()) return;

    services_main->loadConfiguration(fileName);
}

void MainWindow::syncGUIParameters()
{

    ui->amplitude->setValue(services_main->get_Generator_Amplitude());
    ui->gen_offset_spinBox->setValue(services_main->get_Generator_Offset());
    ui->gen_period_spinBox->setValue(services_main->get_Generator_RealPeriod());
    ui->gen_filling_spinBox->setValue(services_main->get_Generator_Filling());
    ui->gen_activation_time->setValue(services_main->get_Generator_ActivationTime());

    if (services_main->get_Generator_Type() == SignalType::SINUSOIDAL) {
        ui->sinusRadiobtn->setChecked(true);
        on_sinusRadiobtn_clicked();
    } else {
        ui->rectangleRadioBtn->setChecked(true);
        on_rectangleRadioBtn_clicked();
    }

    ui->ARX_u_bottom_border->setValue(services_main->get_ARX_U_border_bottom());
    ui->ARX_u_top_border->setValue(services_main->get_ARX_U_border_top());
    ui->ARX_y_bottom_border->setValue(services_main->get_ARX_Y_border_bottom());
    ui->ARX_y_top_border->setValue(services_main->get_ARX_Y_border_top());
    ui->btn_ARX_ToggleU->setChecked(services_main->get_ARX_EnableBordersU());
    ui->btn_ARX_ToggleY->setChecked(services_main->get_ARX_EnableBordersY());

    updateButtonStyle(ui->btn_ARX_ToggleU);
    updateButtonStyle(ui->btn_ARX_ToggleY);

    ui->PID_wzmocnienie->setValue(services_main->get_PID_ProportionalGain());
    ui->PID_T_i->setValue(services_main->get_PID_IntegralTime());
    ui->PID_T_d->setValue(services_main->get_PID_DerivitiveTime());

    updatePIDButtonsState();

}

// ==========================================
// Panel: Generator (Dolny panel - lewo)
// ==========================================

void MainWindow::on_sinusRadiobtn_clicked()
{
    if(ui->sinusRadiobtn->isChecked()) {
        services_main->set_Generator_SignalType(SignalType::SINUSOIDAL);

        if(wykresWY) wykresWY->setStepStyle(0, false);

    }
    ui->gen_filling_spinBox->setEnabled(false);
}

void MainWindow::on_rectangleRadioBtn_clicked()
{
    if(ui->rectangleRadioBtn->isChecked()) {
        services_main->set_Generator_SignalType(SignalType::RECTANGULAR);

        if(wykresWY) wykresWY->setStepStyle(0, true);
    }
    ui->gen_filling_spinBox->setEnabled(true);
}

void MainWindow::on_amplitude_editingFinished()
{
    services_main->set_Generator_AmplitudeA(ui->amplitude->value());
}

void MainWindow::on_gen_offset_spinBox_editingFinished()
{
    services_main->set_Generator_FixedComponentS(ui->gen_offset_spinBox->value());
}

void MainWindow::on_gen_period_spinBox_editingFinished()
{
    int interval_ms = ui->SIM_DELAY->value();
    double period_s = ui->gen_period_spinBox->value();
    services_main->set_Generator_PeriodT(interval_ms, period_s);
}

void MainWindow::on_gen_filling_spinBox_editingFinished()
{
    double filling = ui->gen_filling_spinBox->value();
    services_main->set_Generator_Filling(filling);
}

void MainWindow::on_gen_activation_time_editingFinished()
{
    services_main->set_Generator_ActivationTime(ui->gen_activation_time->value());
}

// ==========================================
// Panel: Granice ARX (Dolny panel - środek)
// ==========================================

void MainWindow::on_OPEN_ARX_SIDEPANEL_clicked()
{
    if (arxDialog == nullptr) {
        arxDialog = new Dialog(services_main, this);
        arxDialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(arxDialog, &QObject::destroyed, this, [=]() {
            arxDialog = nullptr;
        });

        arxDialog->show();
    }
    else {
        arxDialog->raise();
        arxDialog->activateWindow();
    }
}

void MainWindow::on_ARX_u_top_border_editingFinished()
{
    services_main->set_ARX_U_borders(ui->ARX_u_top_border->value(), ui->ARX_u_bottom_border->value());
    syncGUIParameters();
}

void MainWindow::on_ARX_u_bottom_border_editingFinished()
{
    services_main->set_ARX_U_borders(ui->ARX_u_top_border->value(), ui->ARX_u_bottom_border->value());
    syncGUIParameters();
}

void MainWindow::on_ARX_y_top_border_editingFinished()
{
    services_main->set_ARX_Y_borders(ui->ARX_y_top_border->value(), ui->ARX_y_bottom_border->value());
    syncGUIParameters();
}

void MainWindow::on_ARX_y_bottom_border_editingFinished()
{
    services_main->set_ARX_Y_borders(ui->ARX_y_top_border->value(), ui->ARX_y_bottom_border->value());
    syncGUIParameters();
}

void MainWindow::updateButtonStyle(QPushButton* btn) {
    if (btn->isChecked()) {
        // STAN WŁĄCZONY: Jasna zieleń, pogrubiony tekst
        btn->setStyleSheet("background-color: #90EE90; color: black; font-weight: bold; border: 2px solid green;");
        btn->setText("Granica: WŁ");

    } else {
        // STAN WYŁĄCZONY: Szary, zwykły tekst
        btn->setStyleSheet("background-color: #D3D3D3; color: grey;");
        btn->setText("Granica: WYŁ");
    }
}

void MainWindow::on_btn_ARX_ToggleU_toggled(bool checked)
{
    services_main->set_ARX_EnableBordersU(checked);
    updateButtonStyle(ui->btn_ARX_ToggleU);
}


void MainWindow::on_btn_ARX_ToggleY_toggled(bool checked)
{
    services_main->set_ARX_EnableBordersY(checked);
    updateButtonStyle(ui->btn_ARX_ToggleY);
}

// ==========================================
// Panel: Regulator PID (Dolny panel - prawo)
// ==========================================

void MainWindow::on_PID_wzmocnienie_editingFinished()
{
    services_main->set_PID_ProportionalGain(ui->PID_wzmocnienie->value());
}

void MainWindow::on_PID_T_i_editingFinished()
{
    services_main->set_PID_IntegralTime(ui->PID_T_i->value());
}

void MainWindow::on_PID_T_d_editingFinished()
{
    services_main->set_PID_DerivitiveTime(ui->PID_T_d->value());
}

void MainWindow::on_PID_calk_zew_clicked()
{
    services_main->set_PID_IntegrationMethod(RegulatorPID::LiczCalk::Zew);

    updatePIDButtonsState();
}

void MainWindow::on_PID_calk_wew_clicked()
{
    services_main->set_PID_IntegrationMethod(RegulatorPID::LiczCalk::Wew);
    updatePIDButtonsState();
}

void MainWindow::on_PID_reset_uchyb_pamieci_clicked()
{
    services_main->PID_reset_memory();
}

void MainWindow::updatePIDButtonsState()
{
    auto currentMode = services_main->get_PID_IntegrationMethod();
    bool isWew = (currentMode == RegulatorPID::LiczCalk::Wew);

    QString activeStyle = "background-color: #90EE90; color: black; font-weight: bold; border: 2px solid green;";
    QString inactiveStyle = "background-color: #D3D3D3; color: grey; border: 1px solid gray;";

    if (isWew) {
        ui->PID_calk_wew->setStyleSheet(activeStyle);
        ui->PID_calk_wew->setEnabled(false);
        ui->PID_calk_zew->setStyleSheet(inactiveStyle);
        ui->PID_calk_zew->setEnabled(true);
    } else {
        ui->PID_calk_wew->setStyleSheet(inactiveStyle);
        ui->PID_calk_wew->setEnabled(true);
        ui->PID_calk_zew->setStyleSheet(activeStyle);
        ui->PID_calk_zew->setEnabled(false);
    }
}
