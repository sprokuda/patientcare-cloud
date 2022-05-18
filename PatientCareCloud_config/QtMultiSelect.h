#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QDate>

#include "QtMultiSelectPopup.h"

#include <map>
#include <string>

using namespace std;

class QtMultiSelect : public QWidget
{
    Q_OBJECT

public:
    QtMultiSelect(QWidget* parent = Q_NULLPTR);

    void adjustPopupPosition();
    QtMultiSelectPopup& getPopup() { return *popup; };
    QString getText();
    void setText(QString text);
    QStringList getCheckedNames();
    QStringList getBooks(map<string, string> books);
    void selectAllBooks();
    void selectBooks(const QStringList& list);
public slots:

private slots:
    void onShowPopupButtonClicked();
    void onAddItem(const QString& item_text);
    void onRemoveItem(const QString& item_text);
    void onClickCatch(int value);
signals:
    void editingFinished();
private:

    QtMultiSelectPopup* popup;
    QLineEdit* edit;
    QToolButton* button;

    QFont font;

    QHBoxLayout* ctrlLayout;

};

