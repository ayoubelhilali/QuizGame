#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "ui_SettingsDialog.h"

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
private:
    Ui::SettingsDialog *ui;
    void saveSettings();
    void loadSettings() ;
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // SETTINGSDIALOG_H
