#ifndef MSGBOX_H
#define MSGBOX_H

#include <QDialog>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

namespace Ui {
class MsgBox;
}

class MsgBox : public QDialog
{
    Q_OBJECT

public:
    explicit MsgBox(QWidget *parent = 0);
    ~MsgBox();

    enum msgType
    {
        WARING_MSG = 0xf06a,
        INFORMATION_MSG = 0xf05a,
        QUESTION_MSG = 0xf059
    };

    void SetInfoIcon(QChar c, const QColor &color);
    void SetInfo(const QString str);
    void SetTitle(const QString str);

    void Waring(const QString str);
    void Information(const QString str);
    void Question(const QString str);


private:
    Ui::MsgBox *ui;

};


#endif // MSGBOX_H
/*example
MsgBox m;
m.Question("ads");
if( m.exec()){
   qDebug("aaaaaaaaaaaaa");

}else
{
   qDebug("fffffffffffffff");
}
*/
