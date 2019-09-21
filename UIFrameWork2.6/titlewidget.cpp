#include "titlewidget.h"
#include "ui_titlewidget.h"
#include <QDesktopWidget>
#include "IcoHelper/iconhelper.h"

TitleWidget::TitleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleWidget)
{
    ui->setupUi(this);
    isMousePress = false;
    maxFlag = false;
    isMax = false;
    isMsgBox = false;
}

TitleWidget::~TitleWidget()
{
    if (isMsgBox == false)
    {
        delete skinMenu;
/*        delete languageMenu;
        delete english;
        delete blackAction;
        delete blueAction;
        delete grayAction;
        delete navyAction;
        delete actionGroup;*/
    }
    delete ui;
}

/**
 * @brief TitleWidget::titleInit
 * @param _isMax
 * @param _isMsgBox
 */
void TitleWidget::titleInit(const QString titleName, bool _isMax, bool _isMsgBox,uint skin,int lan)
{
    isMax = _isMax;
    isMsgBox = _isMsgBox;

    if (isMax == true)
    {
        connect(ui->maxButton, SIGNAL(clicked()), this, SLOT(maxButtonClicked()));
        ui->maxButton->setToolTip(tr("max"));
        IconHelper::Instance()->SetIcon(ui->maxButton, QChar(MAX_ICON), 12);
    }
    else
    {
        ui->maxButton->setEnabled(false);
        ui->maxButton->hide();
    }

    connect(ui->minButton, SIGNAL(clicked()), this, SLOT(minButtonClicked()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));

    ui->closeButton->setToolTip(tr("close"));
    ui->minButton->setToolTip(tr("min"));


    IconHelper::Instance()->SetIcon(ui->closeButton, QChar(CLOSE_ICON), 12);
    IconHelper::Instance()->SetIcon(ui->minButton, QChar(MIN_ICON), 12);
    IconHelper::Instance()->SetIcon(ui->titleIco, QChar(EARTH_ICON), 12);

    if (isMsgBox == false)
    {
        ui->title->setText(titleName);
        ui->skinButton->setToolTip(tr("skin"));
        skinMenu = new QMenu(ui->skinButton);
        blackAction = new QAction(tr("black"), skinMenu);
        blueAction = new QAction(tr("blue"), skinMenu);
        grayAction = new QAction(tr("gray"), skinMenu);
        navyAction = new QAction(tr("navy"), skinMenu);

        actionGroup = new QActionGroup(skinMenu);//QActionGroup默认互斥
        actionGroup->addAction(blackAction);
        actionGroup->addAction(blueAction);
        actionGroup->addAction(grayAction);
        actionGroup->addAction(navyAction);

        skinMenu->addAction(blackAction);
        skinMenu->addAction(blueAction);
        skinMenu->addAction(grayAction);
        skinMenu->addAction(navyAction);

        blackAction->setCheckable(true);
        blueAction->setCheckable(true);
        grayAction->setCheckable(true);
        navyAction->setCheckable(true);

        languageMenu = new QMenu(tr("Language"),skinMenu);
        languageGroup = new QActionGroup(languageMenu);
        skinMenu->addMenu(languageMenu);
        english = new QAction(tr("English"),languageMenu);
        chinese = new QAction(tr("Chinese"),languageMenu);
        languageGroup->addAction(english);
        languageGroup->addAction(chinese);
        languageMenu->addAction(english);
        languageMenu->addAction(chinese);
        english->setCheckable(true);
        chinese->setCheckable(true);

        ui->skinButton->setMenu(skinMenu);

//        IconHelper::Instance()->SetIcon(ui->skinButton, QChar(0xf0ca), 12);
        connect(blackAction, SIGNAL(triggered()), this, SLOT(blackActionTriggered()));
        connect(blueAction, SIGNAL(triggered()), this, SLOT(blueActionTriggered()));
        connect(grayAction, SIGNAL(triggered()), this, SLOT(grayActionTriggered()));
        connect(navyAction, SIGNAL(triggered()), this, SLOT(navyActionTriggered()));

        connect(chinese, SIGNAL(triggered()), this, SLOT(chineseActionTriggered()));
        connect(english, SIGNAL(triggered()), this, SLOT(englishActionTriggered()));
        changeStyleSheet(skin);

        switch(skin)
        {
            case BLACK: blackAction->setChecked(true);break;
            case BLUE:  blueAction->setChecked(true);break;
            case GRAY:  grayAction->setChecked(true);break;
            case NAVY:  navyAction->setChecked(true);break;
            default:    blackAction->setChecked(true);
        }
        switch(lan)
        {
            case CHINESE: chinese->setChecked(true);break;
            case ENGLISH: english->setChecked(true);break;
            default:      english->setChecked(true);
        }
    }
    else
    {
        ui->skinButton->setEnabled(false);
        ui->skinButton->hide();
    }

}

/**
 * @brief TitleWidget::minButtonClicked
 */
void TitleWidget::minButtonClicked()
{
    this->nativeParentWidget()->showMinimized();
}

/**
 * @brief TitleWidget::maxButtonClicked
 */
void TitleWidget::maxButtonClicked()
{
    if (maxFlag)
    {
        this->nativeParentWidget()->setGeometry(location);
        ui->maxButton->setToolTip(tr("max"));
        IconHelper::Instance()->SetIcon(ui->maxButton, QChar(MAX_ICON), 12);
    }
    else
    {
        location = this->nativeParentWidget()->geometry();
        this->nativeParentWidget()->setGeometry(QApplication::desktop()->availableGeometry());
        ui->maxButton->setToolTip(tr("restore"));
        IconHelper::Instance()->SetIcon(ui->maxButton, QChar(RESTORE_ICON), 12);
    }
    maxFlag = !maxFlag;
}

/**
 * @brief TitleWidget::closeButtonClicked
 */
void TitleWidget::closeButtonClicked()
{
    if (isMsgBox == true)
    {
        this->nativeParentWidget()->close();
    }
    else
    {
        qApp->exit();
    }
}

/**
 * @brief TitleWidget::mouseMoveEvent
 * @param e
 */
void TitleWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (isMousePress && e->buttons() && Qt::LeftButton && !maxFlag)
    {
        this->nativeParentWidget()->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

/**
 * @brief TitleWidget::mousePressEvent
 * @param e
 */
void TitleWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        isMousePress = true;
        mousePoint = e->globalPos() - this->nativeParentWidget()->pos();
        e->accept();
    }
}

/**
 * @brief TitleWidget::mouseReleaseEvent
 * @param e
 */
void TitleWidget::mouseReleaseEvent(QMouseEvent *e)
{
    isMousePress = false;
}

/**
 * @brief TitleWidget::blackActionTriggered
 */
void TitleWidget::blackActionTriggered()
{
    changeStyleSheet(BLACK);
    emit SkinChangedSignal(BLACK);
}

/**
 * @brief TitleWidget::blueActionTriggered
 */
void TitleWidget::blueActionTriggered()
{
    changeStyleSheet(BLUE);
    emit SkinChangedSignal(BLUE);
}

/**
 * @brief TitleWidget::grayActionTriggered
 */
void TitleWidget::grayActionTriggered()
{
    changeStyleSheet(GRAY);
    emit SkinChangedSignal(GRAY);
}

/**
 * @brief TitleWidget::navyActionTriggered
 */
void TitleWidget::navyActionTriggered()
{
    changeStyleSheet(NAVY);
    emit SkinChangedSignal(NAVY);
}

/**
 * @brief TitleWidget::chineseActionTriggered
 */
void TitleWidget::chineseActionTriggered()
{
    emit LanguageChangedSignal(CHINESE);
}


/**
 * @brief TitleWidget::englishActionTriggered
 */
void TitleWidget::englishActionTriggered()
{
    emit LanguageChangedSignal(ENGLISH);
}

/**
 * @brief TitleWidget::setLocation
 * @param _location
 */
void TitleWidget::setLocation(QRect _location)
{
    location = _location;
}

/**
 * @brief TitleWidget::setTitleName
 * @param name
 */
void TitleWidget::setTitleName(const QString name)
{
    ui->title->setText(name);
}


/**
 * @brief TitleWidget::changeStyleSheet
 * @param name
 */
void TitleWidget::changeStyleSheet(uint skinKey )
{
    QString name = NULL;
    switch(skinKey)
    {
        case BLACK: name.append("black");break;
        case BLUE: name.append("blue");break;
        case GRAY: name.append("gray");break;
        case NAVY: name.append("navy");break;
        default:name.append("black");
    }
    QFile file(QString(":/QSS/%1.css").arg(name));
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
    qApp->setPalette(QPalette(QColor("#F0F0F0")));
}
