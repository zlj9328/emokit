#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QFile>

#define  BLACK  0x292929
#define  BLUE   0x1077B5
#define  GRAY   0x7A7A7A
#define  NAVY   0x1582C3

#define CHINESE 0x01
#define ENGLISH 0x02



namespace Ui {
class TitleWidget;
}

class TitleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TitleWidget(QWidget *parent = 0);
    ~TitleWidget();

    enum iconKey
    {
        MIN_ICON        = 0xf068,
        MAX_ICON        = 0xf096,
        CLOSE_ICON      = 0xf00d,
        RESTORE_ICON    = 0xf079,
        EARTH_ICON      = 0xf0ac
    };

    void setLocation(QRect _location);
    void setTitleName(const QString name);
    void titleInit(const QString titleName, bool _isMax, bool _isMsgBox,uint skin = BLACK, int lan = 0x01);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    void changeStyleSheet(uint skinKey);

private slots:
    void minButtonClicked();
    void maxButtonClicked();
    void closeButtonClicked();

    void blackActionTriggered();
    void blueActionTriggered();
    void grayActionTriggered();
    void navyActionTriggered();
    void chineseActionTriggered();
    void englishActionTriggered();

signals:
    void SkinChangedSignal(uint skin);
    void LanguageChangedSignal(int value);

private:
    Ui::TitleWidget *ui;

    QRect location;
    bool maxFlag;
    QPoint mousePoint;
    bool isMousePress;
    bool isMax;
    bool isMsgBox;

    QMenu *skinMenu;
    QMenu *languageMenu;
    QAction *blackAction;
    QAction *blueAction;
    QAction *grayAction;
    QAction *navyAction;
    QAction *english;
    QAction *chinese;
    QActionGroup *actionGroup;
    QActionGroup *languageGroup;
};

#endif // TITLEWIDGET_H


