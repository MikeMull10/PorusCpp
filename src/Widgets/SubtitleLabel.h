#pragma once

#include "TitleLabel.h"

class SubtitleLabel : public TitleLabel {
    Q_OBJECT

public:
    explicit SubtitleLabel(const QString& text, QWidget* parent = nullptr);
    explicit SubtitleLabel(const QString& text, bool clickable, QWidget* parent = nullptr);
};