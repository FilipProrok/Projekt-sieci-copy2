#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(Services *s, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog),
    m_services(s),
    m_current_index_A(0),
    m_current_index_B(0)
{
    ui->setupUi(this);

    if(m_services){
        arxA = m_services->getARX_ACoefficients();
        arxB = m_services->getARX_BCoefficients();
    }

    if(arxA.empty()){
        arxA.push_back(0.0);
    }
    if(arxB.empty()){
        arxB.push_back(0.0);
    }

    update_view();

    ui->d_ARX_delay_value->setValue(m_services->get_ARX_K());

    if (m_services) {
        ui->d_ARX_odchylenie_value->setValue(m_services->get_ARX_noise_deviation());
    }
}

void Dialog::update_view(){
    ui->d_ARX_A_id->setText(QString("A %1").arg(m_current_index_A+1));
    ui->d_ARX_B_id->setText(QString("B %1").arg(m_current_index_B+1));

    ui->d_ARX_A_value->blockSignals(true);
    ui->d_ARX_A_value->setValue(arxA[m_current_index_A]);
    ui->d_ARX_A_value->blockSignals(false);

    ui->d_ARX_B_value->blockSignals(true);
    ui->d_ARX_B_value->setValue(arxB[m_current_index_B]);
    ui->d_ARX_B_value->blockSignals(false);

    ui->d_ARX_A_left->setEnabled(m_current_index_A > 0);
    ui->d_ARX_B_left->setEnabled(m_current_index_B > 0);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_d_ARX_A_left_clicked()
{
    if(m_current_index_A > 0){
        m_current_index_A--;
        update_view();
    }
}


void Dialog::on_d_ARX_A_right_clicked()
{
    if(m_current_index_A == arxA.size() - 1){
        arxA.push_back(0.0);
    }
    m_current_index_A++;
    update_view();
}

void Dialog::on_d_ARX_A_value_valueChanged(double arg1)
{
    arxA[m_current_index_A] = arg1;
}


void Dialog::on_d_ARX_B_left_clicked()
{
    if(m_current_index_B > 0){
        m_current_index_B--;
        update_view();
    }
}


void Dialog::on_d_ARX_B_right_clicked()
{
    if(m_current_index_B == arxB.size() - 1){
        arxB.push_back(0.0);
    }
    m_current_index_B++;
    update_view();
}

void Dialog::on_d_ARX_B_value_valueChanged(double arg1)
{
    arxB[m_current_index_B] = arg1;
}

void Dialog::on_buttonBox_accepted()
{
    //Usunięcie wszystich 0.0 z końca wektorów (aż do pierwszej niezerowej wartości)
    while(arxA.size() > 1 && arxA.back() == 0.0){
        arxA.pop_back();
    }
    while(arxB.size() > 1 && arxB.back() == 0.0){
        arxB.pop_back();
    }

    //check wartości -0.0
    for (int i = 0; i < arxA.size(); ++i) {
        if(abs(arxA[i]) < G_VERY_SMALL_NUMBER ){
            arxA[i] = 0.0;
        }
    }
    for (int i = 0; i < arxB.size(); ++i) {
        if(abs(arxB[i]) < G_VERY_SMALL_NUMBER ){
            arxB[i] = 0.0;
        }
    }
    m_services->set_ARX_Coefficients(arxA,arxB);

    m_services->set_ARX_K(ui->d_ARX_delay_value->value());
    m_services->set_ARX_noise_deviation(ui->d_ARX_odchylenie_value->value());
}


void Dialog::on_buttonBox_rejected()
{}
