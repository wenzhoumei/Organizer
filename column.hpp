#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <QLineEdit>
class Column : public QLineEdit
{
protected:
    virtual void SetColor(const QString &color) = 0;

public:
};

#endif
