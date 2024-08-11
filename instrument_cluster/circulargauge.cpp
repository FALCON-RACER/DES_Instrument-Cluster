#include "CircularGauge.h"
#include <QPainter>
#include <QConicalGradient>
#include <QVBoxLayout>
#include <QPushButton>

CircularGauge::CircularGauge(QWidget *parent)
    : QWidget(parent), m_value(0), m_min(0), m_max(100)
{
    setMinimumSize(200, 200);  // 위젯의 최소 크기 설정

    // "value" 속성에 대한 애니메이션을 생성
    m_animation = new QPropertyAnimation(this, "value");

    // 애니메이션의 부드러운 전환을 위한 이징 커브 설정
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);
}

void CircularGauge::setValue(int value)
{
    // 값이 범위를 벗어나지 않도록 조정
    if (value < m_min) value = m_min;
    if (value > m_max) value = m_max;

    m_value = value;
    update();  // 값 변경 시 위젯을 다시 그리기
}

void CircularGauge::setRange(int min, int max)
{
    m_min = min;
    m_max = max;
}

void CircularGauge::startAnimation(int targetValue, int duration)
{
    m_animation->stop();  // 기존 애니메이션 중지
    m_animation->setDuration(duration);  // 애니메이션 지속 시간 설정
    m_animation->setStartValue(m_value);  // 현재 값에서 시작
    m_animation->setEndValue(targetValue);  // 목표 값으로 애니메이션
    m_animation->start();  // 애니메이션 시작
}

void CircularGauge::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);  // 이 매개변수가 사용되지 않음을 명시

    int side = qMin(width(), height());  // 위젯의 가로와 세로 중 작은 쪽을 기준으로 크기 설정
    int radius = side / 2;  // 원의 반지름 설정

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 안티앨리어싱으로 부드러운 그래픽

    // 배경을 검은색으로 설정
    painter.fillRect(rect(), Qt::black);

    QRectF rect((width() - side) / 2, (height() - side) / 2, side, side);

    // 진행 바의 각도 설정
    // int startAngle = 240 * 16;
    int startAngle = 240 * 15;
    int spanAngle = -int(240.0 * (double)(m_value - m_min) / (m_max - m_min) * 16);

    // 프로그레스 바의 색상과 두께 설정
    QPen pen(Qt::cyan, 10);  // 네온사인 효과를 위한 색상 설정 (청록색)
    pen.setCapStyle(Qt::RoundCap);  // 선의 끝을 둥글게 설정
    painter.setPen(pen);

    // 원형 진행 바만 그리기
    painter.drawArc(rect.adjusted(10, 10, -10, -10), startAngle, spanAngle);



    // 현재 값을 중앙에 텍스트로 표시
    painter.setPen(Qt::white);  // 텍스트 색상은 흰색
    painter.setFont(QFont("Arial", 40));  // 폰트 설정
    painter.drawText(rect, Qt::AlignCenter, QString::number(m_value) + " cm/s");  // 중앙에 텍스트 표시




    // 눈금과 숫자 그리기
    painter.setPen(QPen(Qt::white, 1));  // 눈금의 색상과 두께 설정 (흰색)
    painter.setFont(QFont("Arial", 10));  // 눈금 숫자의 폰트 설정

    int numTicks = (m_max - m_min) / 10 + 1;  // 눈금의 개수 (10 단위)
    int angleStep = 240 / (numTicks - 1);  // 눈금 간의 각도 간격

    for (int i = 0; i < numTicks; ++i) {
        // 눈금의 각도 계산
        int angle = startAngle + i * angleStep * 16;
        // 각도에 따라 시작과 끝의 좌표를 계산
        QPointF startPoint(radius * 0.8 * cos((angle / 16.0 - 90) * M_PI / 180.0) + width() / 2,
                           radius * 0.8 * sin((angle / 16.0 - 90) * M_PI / 180.0) + height() / 2);
        QPointF endPoint(radius * 0.9 * cos((angle / 16.0 - 90) * M_PI / 180.0) + width() / 2,
                         radius * 0.9 * sin((angle / 16.0 - 90) * M_PI / 180.0) + height() / 2);

        // 눈금 그리기
        painter.drawLine(startPoint, endPoint);

        // 눈금의 숫자 그리기
        int value = m_min + i * (m_max - m_min) / (numTicks - 1);
        QString label = QString::number(value);
        QRectF textRect = QRectF(
            startPoint.x() - 10,
            startPoint.y() - 10,
            30, 30);
        painter.drawText(textRect, Qt::AlignCenter, label);
    }


}

