#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QDate>

#include "QtSingleSelectPopup.h"

#include <map>
#include <string>

using namespace std;

class QtSingleSelect : public QWidget
{
    Q_OBJECT

public:
    QtSingleSelect(QWidget* parent = Q_NULLPTR);

    void adjustPopupPosition();
    QtSingleSelectPopup& getPopup() { return *popup; };
    QString getText();
    QStringList getItems(map<string, string> items);
    void selectFirstItem();
    bool selectItem(QString item_name);
    void unCheckAll();
    QString getCheckedName();
public slots:

private slots:
    void onShowPopupButtonClicked();
    void onAddItem(const QString& item_text);
    void onRemoveItem(const QString& item_text);
    void onClickCatch(int value);
signals:
    void editingFinished();
    void emitItemSelected(QString item_text);
private:

    QtSingleSelectPopup* popup;
    QLineEdit* edit;
    QToolButton* button;

    QFont font;
    QHBoxLayout* ctrlLayout;
};

