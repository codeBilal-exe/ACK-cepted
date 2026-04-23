#pragma once

#include "detection_engine.h"
#include <QString>
#include <QTableWidget>
#include <vector>

class ConnectionTable : public QTableWidget {
  Q_OBJECT

public:
  explicit ConnectionTable(QWidget *parent = nullptr);

  void update_data(const std::vector<ConnectionInfo> &connections,
                   const std::vector<std::string> &flagged_ips);

  QString selected_ip() const;

signals:
  void ip_selected(const QString &ip);

private slots:
  void on_selection_changed();
};