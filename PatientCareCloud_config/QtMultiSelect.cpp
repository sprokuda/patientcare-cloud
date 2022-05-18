#include "QtMultiSelect.h"


QtMultiSelect::QtMultiSelect( QWidget* parent)
    : QWidget(parent) 
{
    QFont workingFont = QFont("Calibri", 10);
    this->setFont(workingFont);
    popup = new QtMultiSelectPopup(this);
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


void QtMultiSelect::selectAllBooks()
{

    auto& list = popup->getBoxList();
    for (auto it = list.begin(); it != list.end(); it++)
    {
        (*it)->setCheckState(Qt::Checked);
    }
}

void QtMultiSelect::selectBooks(const QStringList& list)
{

    auto& box_list = popup->getBoxList();
    auto& edit_list = popup->getEditList();

    auto it1 = box_list.begin();
    auto it2 = edit_list.begin();
    for (int i = 0 ; i < list.size() && it1 != box_list.end() && it2 != edit_list.end(); it1++, it2++)
    {
        if (list.at(i) == (*it2)->text())
        {
            (*it1)->setCheckState(Qt::Checked);
            i++;
        }
        else 
            (*it1)->setCheckState(Qt::Unchecked);
    }
}

void QtMultiSelect::onClickCatch(int value)
{
    edit->setText(QString::number(value));
    popup->hide();
}

QString QtMultiSelect::getText()
{
    return edit->text();
}

void QtMultiSelect::setText(QString text)
{
    return edit->setText(text);
}

QStringList QtMultiSelect::getBooks(map<string,string> books)
{
    auto text = edit->text();

    auto parts = text.split(";");
    parts.removeAt(parts.size() - 1);

    QStringList result;
    for (auto it = parts.begin(); it != parts.end(); it++)
    {
        if (books.find(it->toStdString()) != books.end())
            result << QString::fromStdString(books.at(it->toStdString()));
    }

    result.sort();
    return result;
}


QStringList QtMultiSelect::getCheckedNames()
{
    auto& boxes = popup->getBoxList();
    auto& edits = popup->getEditList();

    QStringList result;

    auto it1 = boxes.begin();
    auto it2 = edits.begin();
    for (; it1 != boxes.end() && it2 != edits.end(); it1++, it2++)
    {
        if ((*it1)->isChecked())  result << (*it2)->text();
    }
    return result;
}



void QtMultiSelect::adjustPopupPosition()
{
    int l, t, r, b;
    popup->layout()->getContentsMargins(&l, &r, &t, &b);

    QRect rect = edit->rect();
    QPoint bottomLeft = this->mapToGlobal(rect.bottomLeft());
    popup->move(bottomLeft.x() - l, bottomLeft.y() - t);
}

void QtMultiSelect::onShowPopupButtonClicked()
{
        adjustPopupPosition();
        popup->show();

}

void QtMultiSelect::onAddItem(const QString& item_text)
{
    auto text = edit->text();
    text.append(item_text);
    text.append(";");
    auto parts = text.split(";");
//    parts.removeAt(parts.lastIndexOf(";"));
    parts.removeAt(parts.size() - 1);
    parts.sort();
    auto out_text = parts.join(";");
    if (!out_text.isEmpty()) out_text.append(";");
    edit->setText(out_text);
}

void QtMultiSelect::onRemoveItem(const QString& item_text)
{
    auto text = edit->text();
    auto parts = text.split(";");
    parts.removeAt(parts.size()-1);
    parts.removeAt(parts.indexOf(item_text));
    //parts.removeAt(parts.lastIndexOf(";"));
    parts.sort();
    auto out_text = parts.join(";");
    if (!out_text.isEmpty()) out_text.append(";");
    edit->setText(out_text);
}



