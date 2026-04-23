#pragma once

#include <QLabel>
#include <QListWidget>
#include <QPropertyAnimation>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>


class AlertPanel : public QWidget {
  Q_OBJECT
  Q_PROPERTY(QColor bgColor READ bg_color WRITE set_bg_color)

public:
  explicit AlertPanel(QWidget *parent = nullptr) : QWidget(parent) {
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(8, 8, 8, 8);
    lay->setSpacing(4);

    status_label_ = new QLabel("  ✅  No threats detected", this);
    status_label_->setStyleSheet(
        "font-size: 14px; font-weight: bold; padding: 8px; "
        "border-radius: 6px; background: #e8f5e9; color: #2e7d32;");
    status_label_->setAlignment(Qt::AlignCenter);
    lay->addWidget(status_label_);
    alert_list_ = new QListWidget(this);
    alert_list_->setMaximumHeight(80);
    alert_list_->setStyleSheet("font-size: 12px;");
    lay->addWidget(alert_list_);
  }

  // Called from the main window when an alert fires
  void trigger_alert(const QString &message) {
    // Turn the status label red
    status_label_->setText("  🚨  SYN FLOOD DETECTED — " + message);
    status_label_->setStyleSheet(
        "font-size: 14px; font-weight: bold; padding: 8px; "
        "border-radius: 6px; background: #ffebee; color: #c62828;");

    // Add to the alert list
    alert_list_->insertItem(0, QTime::currentTime().toString("[hh:mm:ss]  ") +
                                   message);
    if (alert_list_->count() > 50)
      alert_list_->takeItem(50);
  }

  // Mark an IP as blocked in the panel
  void add_blocked_ip(const QString &ip) {
    alert_list_->insertItem(0, QTime::currentTime().toString("[hh:mm:ss]  ") +
                                   "🚫 Blocked: " + ip);
  }

  void reset() {
    status_label_->setText("  ✅  No threats detected");
    status_label_->setStyleSheet(
        "font-size: 14px; font-weight: bold; padding: 8px; "
        "border-radius: 6px; background: #e8f5e9; color: #2e7d32;");
    alert_list_->clear();
  }

  // Property required by QPropertyAnimation (unused but declared to satisfy
  // MOC)
  QColor bg_color() const { return bg_color_; }
  void set_bg_color(const QColor &c) { bg_color_ = c; }

private:
  QLabel *status_label_;
  QListWidget *alert_list_;
  QColor bg_color_;
};