#include "msgbox.h"
#include "ui_msgbox.h"
#include <QFile>
#include "IcoHelper/iconhelper.h"

MsgBox::MsgBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MsgBox)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    ui->titleWidget->titleInit(tr("MessageBox"), false, true);
    connect(ui->OKButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->CannelButton,SIGNAL(clicked()),this,SLOT(reject()));
}

MsgBox::~MsgBox()
{
    delete ui;
}

/**
 * @brief MsgBox::SetInfoIcon
 * @param c
 * @param color
 */
void MsgBox::SetInfoIcon(QChar c, const QColor &color)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    ui->infoIcon->setPalette(palette);
    IconHelper::Instance()->SetIcon(ui->infoIcon, c, 45);
}

/**
 * @brief MsgBox::SetInfo
 * @param str
 */
void MsgBox::SetInfo(const QString str)
{
    ui->Info->setText(str);
}

/**
 * @brief MsgBox::SetTitle
 * @param str
 */
void MsgBox::SetTitle(const QString str)
{
    ui->titleWidget->setTitleName(str);
}


/**
 * @brief MsgBox::Waring
 * @param str
 */
void MsgBox::Waring(const QString str)
{
    SetInfoIcon(WARING_MSG, Qt::red);
    ui->Info->setText(str);
    ui->titleWidget->setTitleName(tr("Waring"));
//    ui->CannelButton->hide();
    ui->OKButton->setFocus();
}

/**
 * @brief MsgBox::Information
 * @param str
 */
void MsgBox::Information(const QString str)
{
    SetInfoIcon(INFORMATION_MSG, Qt::green);
    ui->Info->setText(str);
    ui->titleWidget->setTitleName(tr("Information"));
    ui->CannelButton->hide();
    ui->OKButton->setFocus();
}

/**
 * @brief MsgBox::Question
 * @param str
 */
void MsgBox::Question(const QString str)
{
    SetInfoIcon(QUESTION_MSG, Qt::yellow);
    ui->Info->setText(str);
    ui->titleWidget->setTitleName(tr("Question"));
    ui->CannelButton->setFocus();
}


