// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

//#include "mainwindow.h"

#include <QItemSelectionModel>

#include <DApplication>
#include <DApplicationSettings>
#include <DWidgetUtil>
#include <DLog>
#include <DSpinner>
#include <QDebug>
#include <QPushButton>
#define protected public
#include <DListView>

#include <libdtk-5.5.23/DWidget/DStyle>

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
#if defined(DTK_STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif
    DApplication *a = DApplication::globalApplication(argc, argv);

    DApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    DLogManager::registerConsoleAppender();

    a->loadTranslator();
#ifdef Q_OS_UNIX
    a->setOOMScoreAdj(500);
#endif

    a->setAutoActivateWindows(true);
    if (!a->setSingleInstance("deepin-tool-kit-examples")) {
        qDebug() << "another instance is running!!";
        return 0;
    }

    a->setApplicationName("dtk-example");
    a->setOrganizationName("deepin");
    a->setApplicationVersion("1.0");

    DApplicationSettings as;
    Q_UNUSED(as)

//    MainWindow w;
//    w.show();


    DListView v;
    QPushButton btn("move to first");
    v.addFooterWidget(&btn);

    QStandardItemModel *model = new QStandardItemModel(&v);
//    QObject::connect(model, &QStandardItemModel::rowsInserted, &v,
//                     [&](const QModelIndex &, int first, int last){
////        qInfo() << "rowsInserted" << first << last;
//        for (int i = first; i < last + 1; ++i) {
//            qInfo() << model->item(i)->text() << "inserted to index" << i;
//            if (auto item = static_cast<DStandardItem *>(model->item(i))) {
//                for (auto action : item->actionList(Qt::Edge::RightEdge)) {
//                    if (action->widget()) {
//                        auto itemRc = v.visualRect(model->indexFromItem(item));
//                        auto geo = action->widget()->geometry();
//                        int h = action->widget()->height();
//                        geo.setTop(itemRc.top());
//                        geo.setHeight(h);
//                        action->widget()->setGeometry(geo);
//                    }
//                }
//                qInfo() << "visualRect:" << v.visualRect(model->indexFromItem(item));
//            }
//        }

//    });
    for (int i = 0; i < 30; ++i) {
        DStandardItem *item = new DStandardItem (QString::number(i));
//        DViewItemAction *action2 = new DViewItemAction ();
//        action2->setIcon(helper.dstyle()->standardIcon(DStyle::SP_IndicatorChecked));
        DViewItemAction *action = new DViewItemAction ();
        DSpinner *sp = new DSpinner(v.viewport()) ;
        sp->setBackgroundColor(Qt::green);
        sp->setFixedWidth(30);
        action->setWidget(sp);
        item->setActionList(Qt::Edge::RightEdge, {action});
        action->setVisible(false);
        model->appendRow(item);
    }
    v.setModel(model);
    QAbstractItemModel *abModel = model;
    qInfo() << "dynamic_cast:" << model << dynamic_cast<QStandardItemModel *>(abModel);
    v.show();

    QObject::connect(&btn, &QPushButton::clicked, &v, [&](){
        auto current = v.currentIndex();
        auto cText = model->itemFromIndex(current) ? model->itemFromIndex(current)->text() : "";
        qInfo() << "current:" << cText ;

        if (auto item = model->takeItem(current.row(), current.column())) {
            model->removeRow(current.row());
            model->insertRow(0, item);

            if (auto item = static_cast<DStandardItem *>(model->itemFromIndex(current))) {
                for (auto action : item->actionList(Qt::Edge::RightEdge)) {
                    action->setVisible(action->widget() == nullptr);
                }
            }
        }
    });

    QObject::connect(v.selectionModel(), &QItemSelectionModel::currentChanged, &v,
                     [&](const QModelIndex &current, const QModelIndex &previous){
        auto cText = model->itemFromIndex(current) ? model->itemFromIndex(current)->text() : "";
        auto pText = model->itemFromIndex(previous) ? model->itemFromIndex(previous)->text() : "";
        qInfo() << "current:" << cText << "previous:" << pText;

        if (auto item = static_cast<DStandardItem *>(model->itemFromIndex(current))) {
            for (auto action : item->actionList(Qt::Edge::RightEdge)) {
                action->setVisible(true);
                if (auto widget = action->widget()) {
                    DSpinner *sp = static_cast<DSpinner *>(widget);
                    sp->start();
                }
            }
        }
        if (auto item = static_cast<DStandardItem *>(model->itemFromIndex(previous))) {
            for (auto action : item->actionList(Qt::Edge::RightEdge)) {
                action->setVisible(false);
                if (auto widget = action->widget()) {
                    DSpinner *sp = static_cast<DSpinner *>(widget);
                    sp->stop();
                }
            }
        }
    });

    Dtk::Widget::moveToCenter(&v);

    return a->exec();
}
