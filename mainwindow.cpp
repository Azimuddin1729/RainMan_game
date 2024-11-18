#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QDebug>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <cmath>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), score(10) {
    ui->setupUi(this);

    // Create a new QWidget to act as the container for the layout
    QWidget *container = new QWidget(this);

    // Create a QVBoxLayout to hold the view and the pushButton
    QVBoxLayout *layout = new QVBoxLayout(container);

    // Create the QGraphicsScene and view
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);  // Set scene to match view dimensions

    view = new QGraphicsView(scene, this);
    view->setFixedSize(800, 600);  // Ensure view is 800x600 as well
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::StrongFocus);

    // Add the view to the layout
    layout->addWidget(view);
    container->setLayout(layout);

    // Set the container as the central widget
    setCentralWidget(container);

    // Create the umbrella
    umbrella = new Umbrella(350, 550, 100, 20, this);  // Set initial x, y, width, and height
    umbrella->setPos(40, 30);  // Center umbrella within the bottom section of the scene
    scene->addItem(umbrella);
    scoreLabel = new QLabel(view);
    scoreLabel->setGeometry(640, 10, 150, 30);  // Position at the top-right in view
    scoreLabel->setText("Score: 0");
    scoreLabel->setStyleSheet("QLabel { color : white; font-size: 18px; }");
    // Set up the score label
    liveLabel = new QLabel(view);
    liveLabel->setGeometry(720, 10, 150, 30);  // Position at the top-right in view
    liveLabel->setText("Lives: 10");
    liveLabel->setStyleSheet("QLabel { color : red; font-size: 18px; }");

    // Position the start button below the score label
    ui->pushButton->setParent(view); // Set view as parent
    ui->pushButton->setGeometry(650, 50, 100, 30); // Position below the score label
    ui->pushButton->setText("Start");

    // Initialize other components like timers if necessary
}


MainWindow::~MainWindow() {
    delete ui;
}



void MainWindow::spawnRaindrop() {
    // Add the first raindrop with an 80% probability
    if (QRandomGenerator::global()->bounded(1, 11) <= 8) { // 80% probability
        int y = umbrella->location_right_now;
        int x = QRandomGenerator::global()->bounded(y+40, y + 140); // Random x position for first raindrop
        Raindrop *raindrop = new Raindrop(x, 0, 5, 20); // Create first raindrop
        raindrops.append(raindrop); // Add first raindrop to list
        scene->addItem(raindrop); // Add first raindrop to scene
    }

    // Add the second raindrop with a 30% probability
    if (QRandomGenerator::global()->bounded(1, 11) <= 3) { // 30% probability
        int x2 = QRandomGenerator::global()->bounded(150, 700); // Random x position for second raindrop
        Raindrop *raindrop1 = new Raindrop(x2, 0, 5, 20); // Create second raindrop
        raindrops.append(raindrop1); // Add second raindrop to list
        scene->addItem(raindrop1); // Add second raindrop to scene
    }
}



void MainWindow::updateGame() {
    // Move raindrops down and check for collisions
    for (int i = 0; i < raindrops.size(); ++i) {
        Raindrop *raindrop = raindrops[i];
        raindrop->moveDown(10); // Move the raindrop down by 15 units

        // Check collision with umbrella
        if (raindrop->collidesWithItem(umbrella)) {
            scene->removeItem(raindrop);
            raindrops.removeAt(i);
            delete raindrop;
            --i;
            updateLive(-1);
            // Update score here if needed

        } else if (raindrop->isOutOfScreen(scene->height())) {
            scene->removeItem(raindrop);
            raindrops.removeAt(i);
            updateScore(+1);
            delete raindrop;
            --i;
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    //qDebug() << "Key Pressed:" << event->key(); // Debug print to show the key code
    int distance = 30; // Distance the umbrella moves

    // Move the umbrella left or right using "A" and "D" keys
    if (event->key() == Qt::Key_A) { // A for left
        umbrella->moveLeft(distance);
        //qDebug() << "Moved left";
    } else if (event->key() == Qt::Key_D) { // D for right
        umbrella->moveRight(distance, scene->width());
        //qDebug() << "Moved right";
    }

}






#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

void MainWindow::updateLive(int points) {
    live += points;
    liveLabel->setText("Lives: " + QString::number(live));

    //Check if score has reached zero
    if (live <= 0) {
        endGame();
    }
}
void MainWindow::updateScore(int points) {
    score += points;
    scoreLabel->setText("Score: " + QString::number(score));
}

void MainWindow::endGame() {
    // Stop the timers to end the game
    if (spawnTimer) {
        spawnTimer->stop();
    }
    if (gameTimer) {
        gameTimer->stop();
    }

    // Show a message box to indicate the game is over
    QMessageBox::information(this, "Game Over", "You have lost. Zero remaining lives.");

    // Optionally, reset the game or disable further actions
    // score = 10; // Reset score or perform any other reset actions if desired
    // scoreLabel->setText("Score: " + QString::number(score));
    // raindrops.clear();
}

void MainWindow::on_pushButton_clicked() {
    // Reset the score at the start of the game
    live = 10;
    score=0;
    liveLabel->setText("Lives: " + QString::number(live));
    scoreLabel->setText("Score: " + QString::number(score));
    // Initialize and start timers
    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &MainWindow::spawnRaindrop);
    spawnTimer->start(400); // Spawn a raindrop every 400 ms

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateGame);
    gameTimer->start(100); // Update game state every 100 ms
}

