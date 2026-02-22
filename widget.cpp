#include "widget.h"
#include "ui_widget.h"

/**
 * @brief Widget类的构造函数
 * @param parent 父窗口指针
 * @return 无返回值
 * @note 初始化窗口界面，刷新可用串口列表，并设置消息标签颜色为红色
 */
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    on_pushButton_Refresh_Com_clicked();//刷新端口号
    ui->label_Message->setStyleSheet("color:red;");
}

/**
 * @brief Widget类的析构函数
 * @param 无参数
 * @return 无返回值
 * @note 释放UI资源
 */
Widget::~Widget()
{
    delete ui;
}

/**
 * @brief 刷新串口列表按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 清空当前串口列表，然后扫描并添加所有可用的串口设备
 */
void Widget::on_pushButton_Refresh_Com_clicked()
{
    ui->comboBox_Com->clear();//清空端口
    //foreach(C++11)遍历所有可用的串口设备
    //QSerialPortInfo &info 是一个引用变量，用于在循环中存储每个可用串口的信息，&info表示引用传递，避免了复制操作，const表示只读
    //QSerialPortInfo::availablePorts()返回一个QList<QSerialPortInfo>类型的列表，包含所有可用的串口设备信息
    //info.portName()返回当前串口设备的端口名（如COM1、COM2等）
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->comboBox_Com->addItem(info.portName());
    }
}

/**
 * @brief 打开/关闭串口按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 根据当前按钮状态执行打开或关闭串口操作，打开时设置串口参数并建立连接，关闭时断开连接
 */
void Widget::on_pushButton_Onoff_Com_clicked()
{
    if(ui->pushButton_Onoff_Com->text() == "打开串口")
    {
        //设置端口
        m_Serial.setPortName(ui->comboBox_Com->currentText());
        m_Serial.setBaudRate(ui->comboBox_Baud->currentText().toInt());
        m_Serial.open(QIODevice::ReadWrite);
        if(m_Serial.isOpen())
        {
            connect(&m_Serial,&QSerialPort::readyRead,this,&Widget::message_received);
            ui->pushButton_Led->setStyleSheet("background-color:red;");
            ui->pushButton_Onoff_Com->setText("关闭串口");
            ui->label_Message->setText("打开串口成功！");
        }
        else
        {
            ui->label_Message->setText("打开串口失败！");
        }
    }
    else
    {
        m_Serial.close();
        if(!m_Serial.isOpen())
        {
            ui->pushButton_Led->setStyleSheet("background-color:232,232,232;");
            ui->pushButton_Onoff_Com->setText("打开串口");
            ui->label_Message->setText("关闭串口成功！");
        }
        else
        {
            ui->label_Message->setText("关闭串口失败！");
        }
    }
}

/**
 * @brief 清空接收缓冲区按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 清空接收文本框内容，重置接收字节计数为0
 */
void Widget::on_pushButton_Clear_Rx_clicked()
{
    ui->textBrowser_Rx->clear();
    this->rx_num = 0;
    ui->label_Rx_Num->setText("");
}

/**
 * @brief 清空发送缓冲区按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 清空发送文本框内容，重置发送字节计数为0
 */
void Widget::on_pushButton_ClearTx_clicked()
{
    ui->textEdit_Tx->clear();
    this->tx_num = 0;
    ui->label_Tx_Num->setText("");
}

/**
 * @brief 串口数据接收处理函数
 * @param 无参数
 * @return 无返回值
 * @note 读取串口接收的数据，根据当前接收模式（字符串或HEX）显示数据，并更新接收字节计数
 */
void Widget::message_received()
{
    QByteArray buf = m_Serial.readAll();
    this->rx_num += buf.size();//实现接收字节数量的自增
    QString str;

    if(this->rx_mode == 0)//字符串显示
    {
        ui->textBrowser_Rx->insertPlainText(tr(buf));
    }
    else//hex显示
    {
        for(int i = 0;i < buf.size();i++)
        {
            str += QString("%1").arg(buf[i] & 0xff,2,16,QLatin1Char('0'));
        }
        ui->textBrowser_Rx->insertPlainText(str);
        ui->textBrowser_Rx->moveCursor(QTextCursor::End);//光标移动到最后
    }
    ui->label_Rx_Num->setText(QString::number(this->rx_num));
}

/**
 * @brief HEX接收模式按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 设置接收模式为HEX格式，更新按钮样式
 */
void Widget::on_pushButton_HEX_Rx_clicked()
{
    ui->pushButton_HEX_Rx->setStyleSheet("background-color:rgb(149,231,85);");
    this->rx_mode = 1;
    ui->pushButton_String_Rx->setStyleSheet("background-color:rgb(232,232,232);");
}

/**
 * @brief 字符串接收模式按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 设置接收模式为字符串格式，更新按钮样式
 */
void Widget::on_pushButton_String_Rx_clicked()
{
    ui->pushButton_String_Rx->setStyleSheet("background-color:rgb(149,231,85);");
    this->rx_mode = 0;
    ui->pushButton_HEX_Rx->setStyleSheet("background-color:rgb(232,232,232);");
}

/**
 * @brief HEX发送模式按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 设置发送模式为HEX格式，更新按钮样式
 */
void Widget::on_pushButton_HEX_Tx_clicked()
{
    ui->pushButton_HEX_Tx->setStyleSheet("background-color:rgb(149,231,85);");
    this->tx_mode = 1;
    ui->pushButton_String_Tx->setStyleSheet("background-color:rgb(232,232,232);");
}

/**
 * @brief 字符串发送模式按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 设置发送模式为字符串格式，更新按钮样式
 */
void Widget::on_pushButton_String_Tx_clicked()
{
    ui->pushButton_String_Tx->setStyleSheet("background-color:rgb(149,231,85);");
    this->tx_mode = 0;
    ui->pushButton_HEX_Tx->setStyleSheet("background-color:rgb(232,232,232);");
}

/**
 * @brief 发送按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 根据当前发送模式（字符串或HEX）发送数据，并更新发送字节计数
 */
void Widget::on_pushButton_Tx_clicked()
{
    if(m_Serial.isOpen())
    {
        if(this->tx_mode == 0)//字符串发送
        {
            this->tx_num += m_Serial.write(ui->textEdit_Tx->toPlainText().toLatin1());
        }
        else//hex发送
        {
            this->tx_num += m_Serial.write(QByteArray::fromHex(ui->textEdit_Tx->toPlainText().toLatin1()));
        }
        ui->label_Tx_Num->setText(QString::number(this->tx_num));
    }
}

/**
 * @brief 清空发送缓冲区按钮的槽函数
 * @param 无参数
 * @return 无返回值
 * @note 清空发送文本框内容
 */
void Widget::on_pushButton_Clear_Tx_clicked()
{
    ui->textEdit_Tx->clear();
}

