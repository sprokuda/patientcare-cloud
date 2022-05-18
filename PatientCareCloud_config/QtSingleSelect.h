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
    QStringList getBooks(map<string, string> books);
    void selectFirstBook();
    QString getCheckedName();
public slots:

private slots:
    void onShowPopupButtonClicked();
    void onAddItem(const QString& item_text);
    void onRemoveItem(const QString& item_text);
    void onClickCatch(int value);
signals:
    void editingFinished();
    void emitLocationSelected(QString item_text);
private:
//    friend class QtSingleSelectPopup;
    QtSingleSelectPopup* popup;
    QLineEdit* edit;
    QToolButton* button;

    QFont font;

    QHBoxLayout* ctrlLayout;

};

