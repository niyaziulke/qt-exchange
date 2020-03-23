#include "mainwindow.h"
#include <QApplication>
#include <QTableWidget>
#include <QTimer>
#include <QTime>
#include <QObject>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QElapsedTimer>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

  QObjectList k=w.centralWidget()->children();
  //showtable is the table widget in the main window.
  QTableWidget *showtable=qobject_cast<QTableWidget*>(k.last());
  //The user should not be able to edit the table.
  showtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  //Setting colour.
  showtable->setStyleSheet("background-color:lightgray;");

  //Making request.
  QUrl website("https://api.coingecko.com/api/v3/simple/price?ids=bitcoin,ethereum,litecoin,monero,ripple,zcash&vs_currencies=eur,gbp,usd");
  QNetworkAccessManager manager;
  QNetworkReply *response = manager.get(QNetworkRequest(website));
  QEventLoop event;
  QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
  event.exec();
  QString content = response->readAll();
  //Convert to json format.
  QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
  QJsonObject obj=doc.object();
  QStringList list=obj.keys();
  int row=1;
  int column=1;
  //Fill the table accordingly.
  for(QString blockchain :list ){
    if(blockchain=="bitcoin"){
        row=1;
    }
    else if(blockchain=="ethereum"){
        row=2;
    }
    else if(blockchain=="ripple"){
        row=3;
    }
    else if(blockchain=="litecoin"){
        row=4;
    }
    else if(blockchain=="monero"){
        row=5;
    }
    else{
        row=6;
    }

    QJsonObject subobject=obj[blockchain].toObject();
      QStringList sublist=subobject.keys();
      for(QString currency : sublist){
          if(currency=="usd"){
              column=1;
          }
          else if(currency=="eur"){
              column=2;
          }
          else{
              column=3;
          }
          //Writing the data to the position in table.
          QTableWidgetItem *item = showtable->item(row, column);
                     if(!item) {
                         item = new QTableWidgetItem();
                         showtable->setItem(row, column,item);
                     }
                     QString valueAsString = QString::number(subobject[currency].toDouble());
                     item->setText(valueAsString);


      }

  }


  QString timeTitle("Time: ");
  //Time is written.
  showtable->itemAt(0,0)->setText(timeTitle.append(QTime::currentTime().toString()));

  //A timer of 20 seconds.
  QTimer *tupdate=new QTimer();
  tupdate->setInterval(20000);
  tupdate->start();

  //Repeated every 20 seconds.
  QObject::connect(tupdate, &QTimer::timeout, [&]() {
      QString timeTitle("Time: ");
      //Time is written.
      showtable->itemAt(0,0)->setText(timeTitle.append(QTime::currentTime().toString()));
      //Making request.
      QNetworkReply *response = manager.get(QNetworkRequest(website));
      QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
      event.exec();
      QString content = response->readAll();
      //Convert to json format.
      QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
      QJsonObject obj=doc.object();

      QStringList list=obj.keys();
      int row=1;
      int column=1;
      //Fill the table accordingly.
      for(QString blockchain :list ){
        if(blockchain=="bitcoin"){
            row=1;
        }
        else if(blockchain=="ethereum"){
            row=2;
        }
        else if(blockchain=="ripple"){
            row=3;
        }
        else if(blockchain=="litecoin"){
            row=4;
        }
        else if(blockchain=="monero"){
            row=5;
        }
        else{
            row=6;
        }

        QJsonObject subobject=obj[blockchain].toObject();
          QStringList sublist=subobject.keys();
          for(QString currency : sublist){
              if(currency=="usd"){
                  column=1;
              }
              else if(currency=="eur"){
                  column=2;
              }
              else{
                  column=3;
              }
              //Writing the data to the position in table.
              QTableWidgetItem *item = showtable->item(row, column);
                         if(!item) {
                             item = new QTableWidgetItem();
                             showtable->setItem(row, column,item);
                         }
                         QString valueAsString = QString::number(subobject[currency].toDouble());
                         item->setText(valueAsString);


          }

      }


  });

    //Show the main window.
    w.show();
    return a.exec();
}
