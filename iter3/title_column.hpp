#ifndef TITLE_COLUMN_HPP
#define TITLE_COLUMN_HPP

#include "column.hpp"
#include <QString>

class TitleColumn : public Column
{
    QString stylesheet_template = "QLineEdit { color: %1; border-color: %2; background-color: %3; border-style: solid; border-width: %4; border-bottom: none; border-top: none; border-left: none; font-family: %5; padding-left: 6px; }";
    inline static QString text_color = "#FFFFFF";
    inline static QString background_color = "#222222";
    inline static QString border_width = "15px";
    inline static QString font_family = "DejaVu Sans Mono";
    inline static QString border_color = "#000000";

public:
    TitleColumn(const int& column_height, const QString& text)
    {
        setFixedHeight(column_height);
        setAlignment(Qt::AlignLeft);
	setText(text);
	home(true);
        setReadOnly(true);
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }

    void SetColor(const QString &color) override
    {
	border_color = color;
	setStyleSheet(stylesheet_template.arg(text_color, border_color, background_color, border_width, font_family));
    }
};

#endif
