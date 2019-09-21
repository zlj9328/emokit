#include "qlog.h"
#include "qdatetime.h"

/**
 * @brief LogOutput
 * @param type
 * @param Context
 * @param msg
 * 日志输出到文件函数
 */
void LogOutput(QtMsgType type, const QMessageLogContext& Context, const QString  &msg)
{
	QString txt;
    QString str;

	QDateTime time = QDateTime::currentDateTime();
	str = time.toString("yyyy-MM-dd hh:mm:ss");

	switch (type) 
	{
	case QtDebugMsg:
		txt = QString("Debug\t- %1").arg(msg);
		break;
	case QtWarningMsg:
		txt = QString("Warning\t- %1").arg(msg);
		break;
	case QtCriticalMsg:
		txt = QString("Critical\t- %1").arg(msg);
		break;
	case QtFatalMsg:
		txt = QString("Fatal\t- %1").arg(msg);
		abort();
	}

	str.append("\t");
	str.append(txt);


	QFile outFile("Log.log");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << str << endl;
}
