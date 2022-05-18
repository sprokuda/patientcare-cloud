#pragma once
#include <QtWidgets/QWidget>
#include <QLayout.h>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QStringList>
#include <QCheckBox>

#include <QDesktopWidget>
#include <QApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QScrollArea>

#include <QThread>


#include <list>

using namespace std;

#include "callDelay.h"

class QtMultiSelectPopup : public QWidget//QFrame
{
    Q_OBJECT

public:
    QtMultiSelectPopup(QWidget* parent = Q_NULLPTR);
    ~QtMultiSelectPopup();
    void setTable(const QStringList& list);
    list<QCheckBox*>& getBoxList() { return box_list; };
    list<QLineEdit*>& getEditList() { return edit_list; };

signals:
    void addItem(const QString& text);
    void removeItem(const QString& text);
    void clickCatched(int value);
    void editingFinished();
private:

    QWidget* content;
    QVBoxLayout* scrollLayout;
    QScrollArea* scrollArea;

    QVBoxLayout* mainLayout;
    list<QHBoxLayout*> layout_list;
    list<QLineEdit*> edit_list;
    list<QCheckBox*> box_list;

    QThread* thread;
    callDelay* delay;

    void paintEvent(QPaintEvent* event);
    bool eventFilter(QObject* object, QEvent* event);

private slots:

    void selectItem(bool state, const QString& item_text);
    void onDelayFinished();
    void addDummyItem(const int state);
};

