#include "circulargauge.h"
#include <QPainter>
#include <QConicalGradient>
#include <QVBoxLayout>
#include <QPushButton>
#include <cmath>

CircularGauge::CircularGauge(QWidget *parent)
    : QWidget(parent), m_value(0), m_min(0), m_max(260)
{
    setMinimumSize(200, 200);  // Setting the minimum size of a widget
    m_animation = new QPropertyAnimation(this, "value");    // Create animation for the “value” property
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);   // Define easing curve for a smooth transition of the animation
}

/**
 * @brief Set the gauge's value
 * @param value
 */
void CircularGauge::setValue(int value)
{
    // Adjust the value so that it is not outside the range
    if (value < m_min) value = m_min;
    if (value > m_max) value = m_max;

    m_value = value;
    update();  // Redraw widget if the value changes
}

void CircularGauge::setRange(int min, int max)
{
    m_min = min;
    m_max = max;
}

void CircularGauge::startAnimation(int targetValue, int duration)
{
    m_animation->stop();                    // Stop an existing animation
    m_animation->setDuration(duration);     // Set animation duration
    m_animation->setStartValue(m_value);    // Start at current value
    m_animation->setEndValue(targetValue);  // Animating to a Target Value
    m_animation->start();                   // Start animation
}

void CircularGauge::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);  // Gibt an, dass dieser Parameter nicht verwendet wird

    int side = qMin(width(), height());  // Legen Sie die Größe basierend auf der kleineren Breite und Höhe des Widgets fest
    int radius = side / 2;  // den Radius des Kreises festlegen

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // Reibungslose Grafik mit Anti-Aliasing

    // Hintergrund auf Schwarz setzen
    painter.fillRect(rect(), Qt::black);

    QRectF rect((width() - side) / 2, (height() - side) / 2, side, side);

    // Winkel des Fortschrittsbalkens festlegen
    // int startAngle = 240 * 16;
    int startAngle = 240 * 14.99; // 15
    int spanAngle = -int(240.0 * (double)(m_value - m_min) / (m_max - m_min) * 16); // 16

    // Farbe und Dicke des Fortschrittsbalkens festlegen
    QPen pen(Qt::red, 10); // Farbe für Leuchtreklameneffekt festlegen (Cyan)
    pen.setCapStyle(Qt::RoundCap); // Legen Sie fest, dass das Ende der Linie gerundet werden soll
    painter.setPen(pen);

    // balken anzeige
    //painter.drawArc(rect.adjusted(10, 10, -10, -10), startAngle, spanAngle);
    painter.drawArc(rect.adjusted(50, 50, -50, -50), startAngle, spanAngle);




    // Den aktuellen Wert als Text in der Mitte anzeigen
    painter.setPen(Qt::white);  // Textfarbe ist weiß
    painter.setFont(QFont("Arial", 20));  // Schriftarteinstellungen
    painter.drawText(rect, Qt::AlignCenter, QString::number(m_value) + " cm/s");  // 중앙에 텍스트 표시


    // 눈금 그리기
    painter.setPen(QPen(Qt::white, 4));
    painter.setFont(QFont("Arial", 10));

    int numTicks = (m_max - m_min) / 20 + 1;
    double angleStep = 243.5 / (numTicks - 1);  // 눈금 간격 각도 계산

    int startAngelBig = 240 *15;
    for (int i = 0; i < numTicks; ++i) {
        int angle = startAngelBig / 16 + i * angleStep;

        QPointF startPoint_line(radius * 0.80 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                                radius * 0.80 * sin((angle - 90) * M_PI / 180.0) + height() / 2);
        QPointF endPoint(radius * 0.85 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                         radius * 0.85 * sin((angle - 90) * M_PI / 180.0) + height() / 2);
        QPointF startPoint(radius * 0.95 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                           radius * 0.95 * sin((angle - 90) * M_PI / 180.0) + height() / 2);

        painter.drawLine(startPoint_line, endPoint);

        int value = m_min + i * (m_max - m_min) / (numTicks - 1);
        QString label = QString::number(value);
        QRectF textRect = QRectF(
            startPoint.x() -8,
            startPoint.y() -6,
            20, 10);
        painter.drawText(textRect, Qt::AlignCenter, label);
    }

    painter.setPen(QPen(Qt::white, 2)); // Farbe und Dicke des Rasters festlegen (weiß)


    // 작은 눈금
    int startAngle_small = 240 * 15;  // 작은 눈금의 시작 각도

    int numTicks_small = (m_max - m_min) / 10 + 1;  // 작은 눈금의 개수
    double angleStep_small = 243.5 / (numTicks_small - 1);  // 작은 눈금 간격 각도 계산

    for (int i = 0; i < numTicks_small; ++i) {
        // 작은 눈금의 각도 계산
        int angle = startAngle_small / 16 + i * angleStep_small;

        // Start- und Endkoordinaten entsprechend dem Winkel berechnen
        QPointF startPoint_line(radius * 0.81 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                                radius * 0.82 * sin((angle - 90) * M_PI / 180.0) + height() / 2);
        QPointF endPoint(radius * 0.84 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                         radius * 0.84 * sin((angle - 90) * M_PI / 180.0) + height() / 2);

        // 작은 눈금 그리기
        painter.drawLine(startPoint_line, endPoint);
    }
}
