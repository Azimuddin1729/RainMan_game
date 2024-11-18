#include "Umbrella.h"
#include <QBrush>

Umbrella::Umbrella(int x, int y, int width, int height, QObject *parent)
    : QObject(parent), QGraphicsRectItem(x, y, width, height) {
    setBrush(Qt::green); // Green umbrella
    this->width = width;
    this->height = height;
    location_right_now = x;
}

void Umbrella::moveLeft(int distance) {
    // Check if moving left will keep the umbrella within the left boundary (0)
    if (location_right_now - distance >= 0) {
        setPos(x() - distance, y());
        location_right_now -= distance;
    }
}

void Umbrella::moveRight(int distance, int screenWidth) {
    // Check if moving right will keep the umbrella within the right boundary (screenWidth - umbrella width)
    if (location_right_now + distance + width <= screenWidth-40) {
        setPos(x() + distance, y());
        location_right_now += distance;
    }
}


