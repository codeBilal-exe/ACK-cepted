#pragma once

#include <QMainWindow>
#include <QTimer>
#include <memory>

#include "detection_engine.h"
#include "gui/alert_panel.h"
#include "gui/connection_table.h"
#include "packet_capture.h"


QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QStatusBar;
class QComboBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

private slots:
  void on_start_stop_clicked();
  void on_block_ip_clicked();
  void refresh_ui();

private:
  void setup_ui();
  void setup_connections();

  // --- Core components ---
  std::shared_ptr<DetectionEngine> engine_;
  std::unique_ptr<PacketCapture> capture_;

  // --- GUI components ---
  ConnectionTable *connection_table_;
  AlertPanel *alert_panel_;
  QPushButton *start_stop_btn_;
  QPushButton *block_btn_;
  QComboBox *interface_combo_;
  QLabel *status_label_;

  // --- Refresh timer ---
  QTimer *refresh_timer_;

  bool capturing_ = false;
};