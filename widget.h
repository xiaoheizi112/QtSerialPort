#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QLatin1Char>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    // 构造函数：初始化窗口界面
    Widget(QWidget *parent = nullptr);
    // 析构函数：释放UI资源
    ~Widget();

private slots:
    // 刷新串口列表
    void on_pushButton_Refresh_Com_clicked();

    // 打开/关闭串口
    void on_pushButton_Onoff_Com_clicked();

    // 清空接收缓冲区
    void on_pushButton_Clear_Rx_clicked();

    // 清空发送缓冲区
    void on_pushButton_ClearTx_clicked();

    // 处理串口接收的数据
    void message_received();

    // 设置接收模式为HEX格式
    void on_pushButton_HEX_Rx_clicked();

    // 设置接收模式为字符串格式
    void on_pushButton_String_Rx_clicked();

    // 设置发送模式为HEX格式
    void on_pushButton_HEX_Tx_clicked();

    // 设置发送模式为字符串格式
    void on_pushButton_String_Tx_clicked();

    // 发送数据
    void on_pushButton_Tx_clicked();

    // 清空发送缓冲区
    void on_pushButton_Clear_Tx_clicked();

private:
    Ui::Widget *ui;
    QSerialPort m_Serial;

    int rx_num;
    int tx_num;
    char rx_mode = 0;
    char tx_mode = 0;
};
#endif // WIDGET_H
