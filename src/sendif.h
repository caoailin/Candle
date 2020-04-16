#ifndef SENDIF_H
#define SENDIF_H
#include <qserialport.h>
#include <QtNetwork/QTcpSocket>
#include <qobject.h>

class sendIF : public QIODevice
{
    Q_OBJECT
public:
    explicit sendIF(QObject *parent = Q_NULLPTR);
    ~sendIF();
    bool isTcp(){return m_isTCP;}

    void setTcp(bool value){ m_isTCP = value;}
    bool setBaudRate(qint32 baudRate, QSerialPort::Directions directions = QSerialPort::AllDirections);
    qint32 baudRate(QSerialPort::Directions directions = QSerialPort::AllDirections) const;

    bool setDataBits(QSerialPort::DataBits dataBits){return m_serialPort.setDataBits(dataBits);}
    QSerialPort::DataBits dataBits() const{return m_serialPort.dataBits();}

    bool setParity(QSerialPort::Parity parity){return m_serialPort.setParity(parity);}
    QSerialPort::Parity parity() const{return m_serialPort.parity();}

    bool setStopBits(QSerialPort::StopBits stopBits){return m_serialPort.setStopBits(stopBits);}
    QSerialPort::StopBits stopBits() const {return m_serialPort.stopBits();}

    bool setFlowControl(QSerialPort::FlowControl flowControl){return m_serialPort.setFlowControl(flowControl);}
    QSerialPort::FlowControl flowControl() const{return m_serialPort.flowControl();}

    void setPortName(const QString &name);
    QString portName() const{return m_portName;}

    void setPort(const QSerialPortInfo &info){ m_serialPort.setPort(info);}

    bool open(OpenMode mode) Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    bool atEnd() const Q_DECL_OVERRIDE;
    bool isSequential() const Q_DECL_OVERRIDE;
    qint64 bytesAvailable() const Q_DECL_OVERRIDE;
    qint64 bytesToWrite() const Q_DECL_OVERRIDE;
    bool canReadLine() const Q_DECL_OVERRIDE;
    bool waitForReadyRead(int msecs) Q_DECL_OVERRIDE;
    bool waitForBytesWritten(int msecs) Q_DECL_OVERRIDE;
Q_SIGNALS:
    void readyRead();
    void error(QSerialPort::SerialPortError serialPortError);
protected:
    qint64 readData(char *data, qint64 maxSize) Q_DECL_OVERRIDE;
    qint64 readLineData(char *data, qint64 maxSize) Q_DECL_OVERRIDE;
    qint64 writeData(const char *data, qint64 maxSize) Q_DECL_OVERRIDE;

private slots:
    void onSerialPortReadyRead();
    void onSerialPortError(QSerialPort::SerialPortError err);
    void onSocketError(QTcpSocket::SocketError err);
private:
    QTcpSocket m_pTcpSocket;
    QSerialPort m_serialPort;
    QString m_portName;
    bool m_isTCP;
    qint32 m_BaudRate;
};

#endif // SENDIF_H
