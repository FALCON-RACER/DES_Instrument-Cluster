#include "circulargauge.h"
#include <QPainter>
#include <QConicalGradient>
#include <QVBoxLayout>
#include <QPushButton>
#include <cmath>

CircularGauge::CircularGauge(QWidget *parent)
    : QWidget(parent), m_value(0), m_min(0), m_max(300)
{
    setMinimumSize(200, 200);  // Set Widget's minimum size
    m_animation = new QPropertyAnimation(this, "value");    // Set Animation using "value"
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);   // Set easing curve for smooth animation
}

void CircularGauge::setValue(int value)
{
    if (value < m_min)
        value = m_min;
    if (value > m_max)
        value = m_max;

    m_value = value;
    update();  // Draw widget when value changed
}

void CircularGauge::setRange(int min, int max)
{
    m_min = min;
    m_max = max;
}

void CircularGauge::startAnimation(double targetValue, int duration)
{
    m_animation->stop();                    // Stop existing animation
    m_animation->setDuration(duration);     // Animation duration setting
    m_animation->setStartValue(m_value);
    m_animation->setEndValue(targetValue);
    m_animation->start();
}

void CircularGauge::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);  // Specify that this parameter is not used

    int side = qMin(width(), height());  // Set the size based on the smaller
    int radius = side / 2;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    painter.fillRect(rect(), Qt::black);    // Set the background color

    QRectF rect((width() - side) / 2, (height() - side) / 2, side, side);

    // Set the angle of progress bar
    int startAngle = 240 * 15;
    int spanAngle = -int(240.0 * (double)(m_value - m_min) / (m_max - m_min) * 16);


    QPen pen(Qt::cyan, 10);  // Set progress bar's color and thickness
    pen.setCapStyle(Qt::RoundCap);  // Set the end of line to round
    painter.setPen(pen);

    painter.drawArc(rect.adjusted(10, 10, -10, -10), startAngle, spanAngle);



    // text of the middle
    painter.setPen(Qt::white);  // text color
    painter.setFont(QFont("Arial", 40));  // set font
    painter.drawText(rect, Qt::AlignCenter, QString::number(m_value) + " cm/s");  // set the text in the middle




    // draw scale line & number text
    painter.setPen(QPen(Qt::white, 1));  // set scale line color, bold
    painter.setFont(QFont("Arial", 10));  // set number text font, size

    int numTicks = (m_max - m_min) / 50 + 1;  // number of scale line (50 range)
    int angleStep = 240 / (numTicks - 1);  // angle spacing between scales

    for (int i = 0; i < numTicks; ++i) {

        int angle = startAngle + i * angleStep * 16;    // calculate scale's angle

        // calculate the coordinate using angle
        QPointF startPoint(radius * 0.8 * cos((angle / 16.0 - 90) * M_PI / 180.0) + width() / 2,
                           radius * 0.8 * sin((angle / 16.0 - 90) * M_PI / 180.0) + height() / 2);
        QPointF endPoint(radius * 0.9 * cos((angle / 16.0 - 90) * M_PI / 180.0) + width() / 2,
                         radius * 0.9 * sin((angle / 16.0 - 90) * M_PI / 180.0) + height() / 2);

        painter.drawLine(startPoint, endPoint);

        // print the number of scale
        int value = m_min + i * (m_max - m_min) / (numTicks - 1);
        QString label = QString::number(value);
        QRectF textRect = QRectF(
            startPoint.x() - 10,
            startPoint.y() - 10,
            30, 30);
        painter.drawText(textRect, Qt::AlignCenter, label);
    }
}

