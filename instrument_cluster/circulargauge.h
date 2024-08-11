#ifndef CIRCULARGAUGE_H
#define CIRCULARGAUGE_H

#include <QWidget>
#include <QPropertyAnimation>

class CircularGauge : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue)

public:
    explicit CircularGauge(QWidget *parent = nullptr);

    int value() const { return m_value; }
    void setValue(int value);                // Set the gauge's value
    void setRange(int min, int max);         // Set the gauge's minimum and maximum values
    void startAnimation(double targetValue, int duration = 1000);  // Start animation to a target value
    void paintPen(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event) override;  // Paint the gauge

private:
    int m_value;  // Current gauge value
    int m_min;    // Minimum gauge value
    int m_max;    // Maximum gauge value
    QPropertyAnimation *m_animation;  // Animation for smooth value transition
};

#endif // CIRCULARGAUGE_H
