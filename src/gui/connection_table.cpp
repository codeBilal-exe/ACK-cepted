#include "gui/connection_table.h"

#include <QColor>
#include <QFont>
#include <QHeaderView>
#include <set>
#include <string>


ConnectionTable::ConnectionTable(QWidget *parent) : QTableWidget(parent) {

  setColumnCount(4);
  setHorizontalHeaderLabels({"Source IP", "Src Port", "Dst Port", "State"});
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setAlternatingRowColors(true);
  verticalHeader()->setVisible(false);
  setFont(QFont("Monospace", 10));

  connect(this, &QTableWidget::itemSelectionChanged, this,
          &ConnectionTable::on_selection_changed);
}

void ConnectionTable::update_data(
    const std::vector<ConnectionInfo> &connections,
    const std::vector<std::string> &flagged_ips) {
  std::set<std::string> flagged_set(flagged_ips.begin(), flagged_ips.end());

  QString prev_selected = selected_ip();

  setRowCount(static_cast<int>(connections.size()));

  for (int row = 0; row < static_cast<int>(connections.size()); ++row) {
    const auto &ci = connections[row];
    bool flagged = flagged_set.count(ci.src_ip) > 0;

    QColor bg = flagged ? QColor(255, 200, 200)
                        : (ci.state == ConnectionState::ESTABLISHED
                               ? QColor(200, 240, 200)
                               : QColor(255, 255, 255));

    auto make_item = [&](const QString &text) {
      auto *item = new QTableWidgetItem(text);
      item->setBackground(bg);
      if (flagged) {
        item->setForeground(QColor(180, 0, 0));
        QFont f = item->font();
        f.setBold(true);
        item->setFont(f);
      }
      return item;
    };

    setItem(row, 0, make_item(QString::fromStdString(ci.src_ip)));
    setItem(row, 1, make_item(QString::number(ci.src_port)));
    setItem(row, 2, make_item(QString::number(ci.dst_port)));
    setItem(row, 3, make_item(QString::fromStdString(ci.state_str())));

    if (QString::fromStdString(ci.src_ip) == prev_selected) {
      selectRow(row);
    }
  }
}

QString ConnectionTable::selected_ip() const {
  auto rows = selectedItems();
  if (rows.isEmpty())
    return "";
  return item(currentRow(), 0) ? item(currentRow(), 0)->text() : "";
}

void ConnectionTable::on_selection_changed() {
  emit ip_selected(selected_ip());
}
s