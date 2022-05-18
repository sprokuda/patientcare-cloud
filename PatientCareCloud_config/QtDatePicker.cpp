#include "QtDatePicker.h"



QtDatePicker::QtDatePicker(QWidget* parent)
    : QWidget(parent)
{
    popup = new QtDatePickerPopup(this);

    edit = new QLineEdit(QDate::currentDate().toString("dd/MM/yyyy"), this);
    edit->setContentsMargins(0, 0, 0, 0);
    edit->setReadOnly(true);
    button = new QToolButton(this);
    button->setContentsMargins(0, 0, 0, 0);
    QIcon icon;
    icon.addPixmap(QPixmap(":/Client/resources/down-100.png"), QIcon::Normal, QIcon::On); //QString::fromUtf8("N")
    icon.addPixmap(QPixmap(":/Client/resources/down-100.png"), QIcon::Active, QIcon::On);
    icon.addPixmap(QPixmap(":/Client/resources/down-100.png"), QIcon::Selected, QIcon::On);
    button->setIcon(icon);

    QHBoxLayout* ctrlLayout = new QHBoxLayout(this);
    ctrlLayout->setContentsMargins(0, 0, 0, 0);
    ctrlLayout->setSpacing(0);
 //   ctrlLayout->addStretch();
    ctrlLayout->addWidget(edit);
//    ctrlLayout->addStretch();
    ctrlLayout->addWidget(button);

    setLayout(ctrlLayout);

    connect(button, SIGNAL(clicked()), SLOT(onShowPopupButtonClicked()));
    connect(popup, SIGNAL(dateSelected(QDate)), SLOT(setDate(QDate)));
}

QString QtDatePicker::getDate()
{
    return edit->text();
}

void QtDatePicker::setDate(const QDate date)
{
    edit->setText(date.toString("dd/MM/yyyy"));
    popup->setDate(date);
}


void QtDatePicker::adjustPopupPosition()
{
    int l, r, t, b;
    this->layout()->getContentsMargins(&l, &t, &r, &b);

    QRect rect = edit->rect();
    QPoint bottomLeft = this->mapToGlobal(rect.topLeft());
    popup->move(bottomLeft.x() - 6 + 1 + l, bottomLeft.y() + this->height() / 2 + edit->height() / 2 - 6 );

}

void QtDatePicker::onShowPopupButtonClicked()
{
        adjustPopupPosition();
//        popup->setDate(QDate::currentDate());
        popup->show();
}




