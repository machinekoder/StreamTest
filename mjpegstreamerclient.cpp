#include "mjpegstreamerclient.h"

MjpegStreamerClient::MjpegStreamerClient(QQuickPaintedItem *parent) :
    QQuickPaintedItem(parent)
{
    this->setRenderTarget(QQuickPaintedItem::FramebufferObject);
    this->setPerformanceHint(QQuickPaintedItem::FastFBOResizing, true);
    this->setAntialiasing(false);

    m_url = "";
    m_fps = 0.0;
    m_running = false;
    m_frameCount = 0;
    m_timestamp = 0.0;
    m_componentCompleted = false;

    m_framerateTimer = new QTimer(this);
    connect(m_framerateTimer, SIGNAL(timeout()),
            this, SLOT(updateFramerate()));
    m_framerateTimer->start(1000);

    m_context = createDefaultContext(this);
    m_context->start();

    m_updateSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_updateSocket->setLinger(0);
}

/** componentComplete is executed when the QML component is fully loaded */
void MjpegStreamerClient::componentComplete()
{
    m_componentCompleted = true;

    if (m_running == true)    // the component was set to ready before it was completed
    {
        connectSocket();
    }

    QQuickItem::componentComplete();
}

void MjpegStreamerClient::paint(QPainter *painter)
{
    QRect r = this->boundingRect().toRect();

    // Show view finder
    if(!m_frameImg.isNull())
    {
        painter->drawImage(r, m_frameImg, m_frameImg.rect());
    }
}

/** If the running property has a rising edge we try to connect
 *  if it is has a falling edge we disconnect and cleanup
 */
void MjpegStreamerClient::setRunning(bool arg)
{
    if (m_running != arg) {
        m_running = arg;
        emit runningChanged(arg);

        if (m_componentCompleted == false)
        {
            return;
        }

        if (m_running)
        {
            connectSocket();
        }
        else
        {
            disconnectSocket();
        }
    }
}

void MjpegStreamerClient::connectSocket()
{
     m_updateSocket->connectTo(m_url);
     m_updateSocket->subscribeTo("frames");

     connect(m_updateSocket, SIGNAL(messageReceived(QList<QByteArray>)),
             this, SLOT(updateMessageReceived(QList<QByteArray>)));
}

void MjpegStreamerClient::disconnectSocket()
{
    disconnect(m_updateSocket, SIGNAL(messageReceived(QList<QByteArray>)),
               this, SLOT(updateMessageReceived(QList<QByteArray>)));
}

void MjpegStreamerClient::updateMessageReceived(QList<QByteArray> messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

#ifdef QT_DEBUG
    qDebug() << "received frame" << topic;
    qDebug() << "timestamp: " << m_rx.timestamp_unix() << m_rx.timestamp_s() << m_rx.timestamp_us();
#endif

    QByteArray data = QByteArray(m_rx.blob().data(), m_rx.blob().size());

    m_frameCount++;
    m_frameImg = QImage::fromData(data, "JPG");
    m_timestamp = (double)m_rx.timestamp_s() +  (double)m_rx.timestamp_us() / 1000000.0;
    update();
    emit timestampChanged(m_timestamp);

    QDateTime dateTime;
    QTime time;
    dateTime.setMSecsSinceEpoch((quint64)m_rx.timestamp_unix()*1000);
    dateTime = dateTime.toLocalTime();
    time = dateTime.time();
    m_time = time.addMSecs(m_rx.timestamp_us()/1000.0);
    emit timeChanged(m_time);
}

void MjpegStreamerClient::updateFramerate()
{
    m_fps = m_frameCount;
    m_frameCount = 0;
    emit fpsChanged(m_fps);

#ifdef QT_DEBUG
    qDebug() << "fps: " << m_fps;
#endif
}
