#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QTextStream>
#include <vector>
#include <iostream>
#include <string>
#include <unistd.h>
#include <functional>
#include <algorithm>
#include <cmath>
#include <QMediaPlayer>

using namespace std;

QString input;
vector<int> K;
vector<pair<int,int>> N;
int p;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

 //add background music
    QMediaPlayer *music=new QMediaPlayer();
    music->setMedia(QMediaContent(QUrl("/home/alirezafn/dev/gardeshgar/a.mp3")));
    music->setVolume(50);
    music->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("*-- Copyright (C) 2017 Alireza Forouzandeh Nezhad <alirezafn@gmx.us> --*");
    msgBox.exec();
}

void MainWindow::on_newinput_clicked()
{
    bool ok;
    QString text = QInputDialog::getMultiLineText(this,"ورودی جدید","ورودی جدید","", &ok);
        if (ok && !text.isEmpty()) {
            ui->log->setText("کجا برم؟");
            input = text;
        }
}

void parseInput() {
    QTextStream in(&input);
    int k,n;
    in >> n >> k >> p;
    K.resize(k);
    N.resize(n);
    for(int i = 0; i < k; i++) in >> K[i];
    for(int i = 0; i < n; i++) in >> N[i].first >> N[i].second;
}

void MainWindow::on_bgreedy_clicked() {
    parseInput();
    vector<int> result;
    for(int bestChoice=-1,bestCost = 0;;bestChoice = -1) {
        for(int i = 0; i < N.size(); i++) {
            auto &n = N[i];
            if(n.first != -1)
                if(bestChoice == -1 || n.first + K[n.second-1] < bestCost) {
                    cout << bestCost << " * " << bestChoice << endl;
                    bestCost = n.first + K[n.second-1];
                    bestChoice = i;
                }
        }
        if(bestChoice != -1) {
            p -= bestCost;
            N[bestChoice].first = -1;
            K[N[bestChoice].second] = 0;
            if(p >= 0) result.push_back(bestChoice + 1); else break;
        }
    }
    QString resultOutput;
    resultOutput.append(QString::number(result.size()) + "\n");
    for(int i : result) {
        resultOutput.append(QString::number(i) + " ");
    }
    ui->log->setText(resultOutput);
}

void MainWindow::on_bbandb_clicked()
{
    parseInput();
    vector<int> result,best;
    function<void(vector<int>,int)> branch;
    branch = [&] (vector<int> res,int bound) {
        if(bound < 0) {
            res.pop_back();
            if(res.size() > best.size()) {
                cout << res.size() << endl;
                best = res;
            }
        } else {
            for(int i = 0; i < N.size(); i++) {
                auto &n = N[i];
                if(n.first != -1) {
                    int F=n.first,FF=K[n.second-1],cost=n.first + K[n.second-1];
                    n.first = -1;
                    K[n.second-1] = 0;
                    res.push_back(i+1);
                    branch(res,bound - cost);
                    res.pop_back();
                    n.first = F;
                    K[n.second-1] = FF;
                }
            }
        }
    };
    branch(result,p);
    QString resultOutput;
    resultOutput.append(QString::number(best.size()) + "\n");
    for(int i : best) {
        resultOutput.append(QString::number(i) + " ");
    }
    ui->log->setText(resultOutput);
}

void MainWindow::on_bbrute_clicked()
{
    parseInput();
    vector<int> best;
     for(unsigned long long i = 0; i < pow(2,N.size()); i++) {
         vector<int> K2 = K,result;
         int cost = 0;
         for(int j = 0; j < N.size(); j++) {
             if(i & (1<<j)) {
                 cost += N[j].first + K2[N[j].second-1];
                 result.push_back(j+1);
                 K2[N[j].second-1] = 0;
             }
         }
         if(cost <= p && result.size() > best.size()){
             best = result;
         }
     }
     QString resultOutput;
     resultOutput.append(QString::number(best.size()) + "\n");
     for(int i : best) {
         resultOutput.append(QString::number(i) + " ");
     }
     ui->log->setText(resultOutput);
}




void MainWindow::on_bdp_clicked()
{
    parseInput();
    QTextStream in(&input);
    int p,k,n;
    in >> n >> k >> p;
    vector<int> kE(k);
    for(int i = 0; i < k; i++)
        in >> kE[i];
    vector<vector<int>> kV(k);
    for(int i = 0; i < n; i++) {
        int v,a;
        in >> v >> a;
        kV[a-1].push_back(v);
    }

    for(int i = 0; i < k; i++)
        sort(kV[i].begin(),kV[i].end());

    for(int i = 0; i < k; i++) {
        cout << kE[i] << " : ";
        for(int j = 0; j < kV[i].size(); j++) cout << kV[i][j] << " ";
        cout << endl;
    }

    int dp[k][n+1];
    for(int i = 0; i < k; i++) {
        for (int j = 0; j <= n; j++) {
            dp[i][j] = 0;
        }
    }
    vector<int> dp2[k][n+1];
    for(int i = 0; i < k; i++) {
        for(int j = 0; j <= n; j++) {
            if(j > kV[i].size()) {
                if (i > 1 && kV[i - 1].size() < j) {
                        dp[i][j] = -1;
                } else if ( i == 0 ){
                    dp[i][j] = -1;
                } else {
                    goto normal;
                }
            }
            else {
                normal:
                int minimumCost = 0, minimumW = -1;
                int X;
                if(i == 0)
                    X = j;
                else
                    if(j > kV[i].size())
                        X = j-kV[i].size();
                    else
                        X = 0;
                for (int w = X; w <= j && w <= kV[i].size(); w++) {
                    int cost = 0;
                    if(w)
                        cost += kE[i];
                    if (i > 0) {
                        if(dp[i-1][j-w] == -1) continue;
                        cost += dp[i - 1][j - w];
                    }
                    for (int e = 0; e < w; e++)
                        cost += kV[i][e];
                    if (minimumW == -1 || cost < minimumCost) {
                        minimumCost = cost;
                        minimumW = w;
                    }
                }
                cout << "minimum w for dp(" << i << "," << j << ") = " << minimumW  << " => " << minimumCost << endl;
                dp[i][j] = minimumCost;
                if(i >= 1) {
                    dp2[i][j] = dp2[i-1][j-minimumW];
                    dp2[i][j].push_back(minimumW);
                }
                else
                    dp2[i][j].push_back(minimumW);
            }
        }
    }
    for(int i = k-1; i >= 0; i--) {
        for(int j = 0 ; j < n; j++) {
            cout << dp[i][j] << "\t";
        }
        cout << endl;
    }
    int bestJ=-1,bestI=-1;
    for(int i = k-1; i >= 0; i--) {
        for(int j = 0 ; j < n; j++) {
            if(dp[i][j] <= p && j>bestJ && dp[i][j] > 0) {
                bestJ = j;
                bestI = i;
            }
        }
        cout << endl;
    }
    int nnn = 0;
    for(int i = 0 ; i < dp2[bestI][bestJ].size(); i++){
        nnn += dp2[bestI][bestJ][i];
    }
    QString resultOutput;
    resultOutput.append(QString::number(nnn) + "\n");
    for(int i = 0 ; i < dp2[bestI][bestJ].size(); i++){
        for(int j = 0; j < dp2[bestI][bestJ][i]; j++) {
            cout << kV[i][j] << " ";
        }
        cout << endl;
    }
    for(int i = 0 ; i < dp2[bestI][bestJ].size(); i++){
        for(int j = 0; j < dp2[bestI][bestJ][i]; j++) {
            for(int w = 0; w < N.size();w++) {
                if(i == N[w].second-1 && N[w].first == kV[i][j]) {
                    cout << "H" <<endl;
                    resultOutput.append(QString::number(w+1) + " ");
                    N[w].first=-1;
                    break;
                }
            }
        }
    }

    ui->log->setText(resultOutput);

}
