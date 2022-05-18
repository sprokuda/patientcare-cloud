#include "QtSingleSelect.h"


QtSingleSelect::QtSingleSelect( QWidget* parent)
    : QWidget(parent) 
{
    QFont workingFont = QFont("Calibri", 10);
    this->setFont(workingFont);
    popup = new QtSingleSelectPopup(this);
    popup->installEventFilter(this);
    popup->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    edit = new QLineEdit("", this);
    edit->setReadOnly(true);
    edit->setContentsMargins(0, 0, 0, 0);

    button = new QToolButton(this);
    button->setContentsMargins(0, 0, 0, 0);
    QIcon icon;
    icon.addPixmap(QPixmap(":Client/resources/down-100.png"), QIcon::Normal, QIcon::On); //QString::fromUtf8("N")
    icon.addPixmap(QPixmap(":Client/resources/down-100.png"), QIcon::Active, QIcon::On);
    icon.addPixmap(QPixmap(":Client/resources/down-100.png"), QIcon::Selected, QIcon::On);
    button->setIcon(icon);

    ctrlLayout = new QHBoxLayout(this);
    ctrlLayout->addWidget(edit);
    ctrlLayout->addWidget(button);
    ctrlLayout->setSpacing(0);
    ctrlLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(ctrlLayout);

    connect(button, SIGNAL(clicked()), SLOT(onShowPopupButtonClicked()));
    connect(popup, SIGNAL(addItem(QString)), SLOT(onAddItem(QString)));
    connect(popup, SIGNAL(removeItem(QString)), SLOT(onRemoveItem(QString)));
    connect(popup, SIGNAL(clickCatched(int)), SLOT(onClickCatch(int)));
    connect(popup, SIGNAL(editingFinished()),popup, SLOT(hide()));
}


void QtSingleSelect::selectFirstItem()
{
    auto& box_list = popup->getBoxList();
    auto& edit_list = popup->getEditList();
    (*box_list.begin())->setCheckState(Qt::Checked);
    edit->setText((*edit_list.begin())->text());
}

bool QtSingleSelect::selectItem(QString item_name)
{
    auto& box_list = popup->getBoxList();
    auto& edit_list = popup->getEditList();
 
    auto it1 = edit_list.begin();
    auto it2 = box_list.begin();

    while (it1 != edit_list.end())
    {
        if ((*it1)->text() == item_name) break;
        ++it1;
        ++it2;
    }

    if (it1 != edit_list.end())
    {
        unCheckAll();
        (*it2)->setCheckState(Qt::Checked);
        edit->setText((*it1)->text());
        return true;
    }
    else return false;
}


void QtSingleSelect::unCheckAll()
{
    auto& box_list = popup->getBoxList();

    auto it1 = box_list.begin();

    while (it1 != box_list.end())
    {
        (*it1)->setCheckState(Qt::Unchecked);
        ++it1;
    }

}

void QtSingleSelect::onClickCatch(int value)
{
    edit->setText(QString::number(value));
    popup->hide();
}

QString QtSingleSelect::getText()
{
    return edit->text();
}

QStringList QtSingleSelect::getItems(map<string,string> items)
{
    auto text = edit->text();

    auto parts = text.split(";");
    parts.removeAt(parts.size() - 1);

    QStringList result;
    for (auto it = parts.begin(); it != parts.end(); it++)
    {
        if (items.find(it->toStdString()) != items.end())
            result << QString::fromStdString(items.at(it->toStdString()));
    }

    result.sort();
    return result;
}


void QtSingleSelect::adjustPopupPosition()
{
    int l, t, r, b;
    popup->layout()->getContentsMargins(&l, &r, &t, &b);

    QRect rect = edit->rect();
    QPoint bottomLeft = this->mapToGlobal(rect.bottomLeft());
    popup->move(bottomLeft.x() - l, bottomLeft.y() - t);
}

void QtSingleSelect::onShowPopupButtonClicked()
{
        adjustPopupPosition();
        popup->show();

}

void QtSingleSelect::onAddItem(const QString& item_text)
{
    edit->setText(item_text);
    emit emitItemSelected(item_text);
}

void QtSingleSelect::onRemoveItem(const QString& item_text)
{
    edit->setText(item_text);
}



QString QtSingleSelect::getCheckedName()
{
    auto& boxes = popup->getBoxList();
    auto& edits = popup->getEditList();

    auto it1 = boxes.begin();
    auto it2 = edits.begin();
    for (; it1 != boxes.end() && it2 != edits.end(); it1++, it2++)
    {
        if ((*it1)->isChecked())  return (*it2)->text();
    }
    return "";
}



