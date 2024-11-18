#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QLabel>
#include "Raindrop.h"
#include "Umbrella.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void spawnRaindrop();
    void updateGame();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView * view;
    Umbrella *umbrella;
    QList<Raindrop*> raindrops;
    QTimer *spawnTimer;
    QTimer *gameTimer;
    QLabel *liveLabel; // To display the score
    int live; // Variable to track the score
    int score;
    QLabel * scoreLabel;
    void endGame();
    void updateLive(int points); // Method to update the score
    void updateScore(int points);
};

#endif // MAINWINDOW_H
