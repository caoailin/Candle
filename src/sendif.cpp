#include "sendif.h"



sendIF::sendIF(QObject *parent):QIODevice(parent)
{
    m_isTCP = false;
    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(onSerialPortReadyRead()), Qt::QueuedConnection);
    connect(&m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(onSerialPortError(QSerialPort::SerialPortError)));
    connect(&m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(onSerialPortReadyRead()), Qt::QueuedConnection);
    connect(&m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

sendIF::~sendIF()
{

}

bool sendIF::setBaudRate(qint32 baudRate, QSerialPort::Directions directions)
{
    m_BaudRate = baudRate;
    if(m_isTCP){
        return true;
    }else{
       return m_serialPort.setBaudRate(baudRate,directions);
    }

}

qint32 sendIF::baudRate(QSerialPort::Directions directions) const
{
    if(m_isTCP){
        return m_BaudRate;
    }else{
       return m_serialPort.baudRate(directions);
    }
}

void sendIF::setPortName(const QString &name)
{
   m_portName = name;
   if(m_isTCP){
   }else{
       m_serialPort.setPortName(name);
   }
}

bool sendIF::open(QIODevice::OpenMode mode)
{
    if(m_isTCP){
        int state = m_pTcpSocket.state();
        if(state != QTcpSocket::ConnectingState && state != QTcpSocket::ConnectedState)
           m_pTcpSocket.connectToHost(m_portName,m_BaudRate);
        return (state == QTcpSocket::ConnectedState) && QIODevice::open(mode);
    }else{
        return m_serialPort.open(mode) && QIODevice::open(mode);
    }
}

void sendIF::close()
{
    if(m_isTCP){
        m_pTcpSocket.close();
    }else{
        m_serialPort.close();
    }
    QIODevice::close();
}

bool sendIF::atEnd() const
{
    if(m_isTCP){
       return QIODevice::atEnd() && m_pTcpSocket.atEnd();
    }else{
       return QIODevice::atEnd() && m_serialPort.atEnd();
    }
}

bool sendIF::isSequential() const
{
    if(m_isTCP){
        return m_pTcpSocket.isSequential();
    }else{
        return m_serialPort.isSequential();
    }
}

qint64 sendIF::bytesAvailable() const
{
    if(m_isTCP){
        return m_pTcpSocket.bytesAvailable();
    }else{
        return m_serialPort.bytesAvailable();
    }
}

qint64 sendIF::bytesToWrite() const
{
    if(m_isTCP){
        return m_pTcpSocket.bytesToWrite();
    }else{
        return m_serialPort.bytesToWrite();
    }
}

bool sendIF::canReadLine() const
{
    if(m_isTCP){
        return m_pTcpSocket.canReadLine();
    }else{
        return m_serialPort.canReadLine();
    }
}

bool sendIF::waitForReadyRead(int msecs)
{
    if(m_isTCP){
        return m_pTcpSocket.waitForReadyRead(msecs);
    }else{
        return m_serialPort.waitForReadyRead(msecs);
    }
}

bool sendIF::waitForBytesWritten(int msecs)
{
    if(m_isTCP){
        return m_pTcpSocket.waitForBytesWritten(msecs);
    }else{
        return m_serialPort.waitForBytesWritten(msecs);
    }
}

void sendIF::onSerialPortReadyRead()
{
    emit readyRead();
}

void sendIF::onSerialPortError(QSerialPort::SerialPortError err)
{
    emit error(err);
}

void sendIF::onSocketError(QAbstractSocket::SocketError err)
{
    switch(err){

    case QAbstractSocket::HostNotFoundError:
    case QAbstractSocket::ProxyNotFoundError:
        emit error(QSerialPort::DeviceNotFoundError);
        break;
    case QAbstractSocket::ConnectionRefusedError:
    case QAbstractSocket::ProxyConnectionRefusedError:
    case QAbstractSocket::ProxyConnectionClosedError:             /* 15 */
    case QAbstractSocket::ProxyConnectionTimeoutError:
        emit error(QSerialPort::OpenError);
        break;
//    case QAbstractSocket::SocketAccessError:
//    case QAbstractSocket::SocketResourceError:
//    case QAbstractSocket::SocketTimeoutError:                   /* 5 */
//    case QAbstractSocket::DatagramTooLargeError:
//    case QAbstractSocket::NetworkError:
//    case QAbstractSocket::AddressInUseError:
//    case QAbstractSocket::SocketAddressNotAvailableError:
//    case QAbstractSocket::UnsupportedSocketOperationError:        /* 10 */
//    case UnfinishedSocketOperationError:
//    case ProxyAuthenticationRequiredError:
//    case SslHandshakeFailedError:
//    case ProxyProtocolError:
//    case OperationError:
//    case  SslInternalError:                      /* 20 */
//    case  SslInvalidUserDataError:
//    case  TemporaryError:
//    case RemoteHostClosedError:
//    case UnknownSocketError:
    default:
        emit error(QSerialPort::UnknownError);
        break;
    }
}


qint64 sendIF::readData(char *data, qint64 maxSize)
{
    if(m_isTCP){
       return m_pTcpSocket.read(data,maxSize);
    }else{
        return m_serialPort.read(data,maxSize);
    }
}

qint64 sendIF::readLineData(char *data, qint64 maxSize)
{
    if(m_isTCP){
        return m_pTcpSocket.readLine(data,maxSize);
    }else{
        return m_serialPort.readLine(data,maxSize);
    }
}

qint64 sendIF::writeData(const char *data, qint64 maxSize)
{
    if(m_isTCP){
         return m_pTcpSocket.write(data,maxSize);
    }else{
        return m_serialPort.write(data,maxSize);
    }
}

