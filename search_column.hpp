#ifndef SEARCH_COLUMN_HPP
#define SEARCH_COLUMN_HPP

#include "column.hpp"
#include <QObject>
#include <QKeyEvent>

class SearchColumn : public Column
{
    Q_OBJECT
private:
    QString stylesheet_template = "QLineEdit { color: %1; border-color: %2; background-color: %3; border-style: solid; border-width: %4; border-bottom: none; border-top: none; border-left: none; font-family: %5; padding-left: 6px; }";
    inline static QString text_color = "#222222";
    inline static QString background_color = "#F5F5F5";
    inline static QString border_width = "15px";
    inline static QString font_family = "DejaVu Sans Mono";
    inline static QString border_color = "#000000";

signals:
    void upArrowPressed();
    void downArrowPressed();

public:
    SearchColumn(const int& column_height)
    {
        setAlignment(Qt::AlignCenter);
        setFixedHeight(column_height);
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));

        setAlignment(Qt::AlignLeft);
    }

    void SetColor(const QString &color) override
    {
	border_color = color;
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }

    void keyPressEvent(QKeyEvent *e) override {
	if(e->key() == Qt::Key_Up)
	    emit upArrowPressed();
	if(e->key() == Qt::Key_Down)
	    emit downArrowPressed();
	QLineEdit::keyPressEvent(e);
    }
};

#endif
