
#ifndef VIEWCLASS_H
#define VIEWCLASS_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
QT_BEGIN_NAMESPACE

namespace Ui { class viewClass; }
QT_END_NAMESPACE

class viewClass : public QWidget {
Q_OBJECT

public:
    explicit viewClass(QWidget *parent = nullptr);
    ~viewClass() override;

private:
    Ui::viewClass *ui;

};
QT_END_NAMESPACE

#endif // VIEWCLASS_H
