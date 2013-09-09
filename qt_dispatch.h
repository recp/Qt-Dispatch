//
//  Created by Recep ASLANTAS
//  Copyright (c) 2013 Recep ASLANTAS. All rights reserved.
//

#ifndef THREAD_DISPATCHER_H
#define THREAD_DISPATCHER_H

#include <QThread>
#include <QMetaObject>
#include <QThread>
#include <QCoreApplication>
#include <QObject>

#include <functional>

typedef std::function<void()> fnBlock;

class dummyObj : public QObject
{
  Q_OBJECT

public:
  dummyObj(QThread *thread) {
    moveToThread(thread);
  }
public slots:
  void doSomethings(fnBlock block) {
    block();
  }
};

static void q_dispatch_async_main(fnBlock block) {
  qRegisterMetaType<fnBlock>("fnBlock");

  QThread *mainThread = QCoreApplication::instance()->thread();
  dummyObj *worker = new dummyObj(mainThread);
  QMetaObject::invokeMethod(worker,
                            "doSomethings",
                            Qt::QueuedConnection,
                            Q_ARG(fnBlock, block)
                           );

}

static void q_dispatch_async(QThread* thread, fnBlock block) {
  qRegisterMetaType<fnBlock>("fnBlock");

  dummyObj *worker = new dummyObj(thread);
  QMetaObject::invokeMethod(worker,
                            "doSomethings",
                            Qt::QueuedConnection,
                            Q_ARG(fnBlock, block));
}

#endif // THREAD_DISPATCHER_H
