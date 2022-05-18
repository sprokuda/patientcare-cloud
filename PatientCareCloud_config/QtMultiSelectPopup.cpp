#include "QtMultiSelectPopup.h"
#include <QHeaderView>


QtMultiSelectPopup::QtMultiSelectPopup( QWidget* parent)
    : QWidget(parent)  
{
    QFont workingFont = QFont("Calibri", 10);
    this->setFont(workingFont);

    content = new QWidget(this);
    content->setFont(workingFont);

    mainLayout = new QVBoxLayout(content);
    content->setLayout(mainLayout);
    content->setStyleSheet("QWidget {background-color:RGB(214,214,214);}");

    scrollLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea(this);

    scrollLayout->addWidget(scrollArea);
    scrollArea->setWidget(content);
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    setLayout(scrollLayout);
    this->setMinimumWidth(220);
    this->setMinimumHeight(300);

    this->setAttribute(Qt::WA_TranslucentBackground);


    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    installEventFilter(this);
    this->setStyleSheet("QWidget {background-color:RGB(214,214,214);}");

    delay = new callDelay();
    thread = new QThread();
    delay->moveToThread(thread);

    connect(delay, SIGNAL(delayFinished()), this, SLOT(onDelayFinished()));
    thread->start();
}


QtMultiSelectPopup::~QtMultiSelectPopup()
{
    for (auto it = layout_list.begin(); it != layout_list.end(); it++)
    {
        delete *it;
    }
    for (auto it = edit_list.begin(); it != edit_list.end(); it++)
    {
        delete* it;
    }

    thread->quit();
    thread->wait();
    delete delay;
}


void QtMultiSelectPopup::onDelayFinished()
{
    hide();
}


void QtMultiSelectPopup::setTable(const QStringList& list)
{

    for (auto& item : box_list) delete item;
    for (auto& item : edit_list) delete item;
    for (auto& item : layout_list)
    {
        mainLayout->removeItem(item); delete item;
    }
    box_list.clear();
    edit_list.clear();
    layout_list.clear();

    if (list.isEmpty()) return;

    for (int i = 0; i < list.size(); i++)
    {
        QLineEdit* edit = new QLineEdit(list.at(i), this);
        edit->setMinimumWidth(120);
        edit_list.push_back(edit);
        edit->setReadOnly(true);
        edit->setStyleSheet("QLineEdit {background-color:white;}");
        edit->installEventFilter(this);
        QCheckBox* box = new QCheckBox(this);
        box_list.push_back(box);
        QHBoxLayout* layout = new QHBoxLayout();
        //layout->addStrech();
        layout->addWidget(edit);
        layout->addWidget(box);
        QString word = list[i];
        connect(box, &QCheckBox::toggled, this, [=](bool if_checked) { selectItem(if_checked, word); });
        mainLayout->addLayout(layout);
        layout_list.push_back(layout);
    }

    mainLayout->installEventFilter(this);
    content->setMaximumHeight(120);
}

void QtMultiSelectPopup::selectItem(bool state, const QString& item_text)
{
    if (state) emit addItem(item_text);
    else emit removeItem(item_text);
}

void QtMultiSelectPopup::addDummyItem(const int state)
{
    auto cb = dynamic_cast<QCheckBox*>(sender());
    auto tw = dynamic_cast<QTableWidget*>(cb->parentWidget());

    emit addItem(QString::QString("some text"));
}

void QtMultiSelectPopup::paintEvent(QPaintEvent* event)
{
    event->accept();
}

bool QtMultiSelectPopup::eventFilter(QObject* object, QEvent* event)
{

    for (auto it = edit_list.begin(); it != edit_list.end(); it++)
    {
        if ((object == *it) && (event->type() == QEvent::MouseButtonRelease))
        {
//            emit clickCatched(event->type());
//            hide();
            QMetaObject::invokeMethod(delay, "doDelay", Qt::QueuedConnection);
        }
    }

    if ((object == this) && (event->type() == QEvent::KeyPress))
    {
        auto* key_event = dynamic_cast<QKeyEvent*>(event);
        if (key_event->key() == Qt::Key_Escape)
        {
            QMetaObject::invokeMethod(delay, "doDelay", Qt::QueuedConnection);
        }
    }

    if ((object == this) && (event->type() == QKeyEvent::WindowDeactivate))
    {
        //        emit clickCatched(event->type());
        //hide();
        QMetaObject::invokeMethod(delay, "doDelay", Qt::QueuedConnection);
    }

    if ((object == this) && (event->type() == QEvent::MouseButtonPress))
    {
//        emit clickCatched(event->type());
        QMetaObject::invokeMethod(delay, "doDelay", Qt::QueuedConnection);
    }

    if ((object == this) && (event->type() == QEvent::MouseButtonDblClick))
    {
        //        emit clickCatched(event->type());
        QMetaObject::invokeMethod(delay, "doDelay", Qt::QueuedConnection);
    }
    return QWidget::eventFilter(object, event);
}








