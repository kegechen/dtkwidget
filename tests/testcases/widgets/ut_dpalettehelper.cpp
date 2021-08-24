/*
* Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     Ye ShanShan <yeshanshan@uniontech.com>
*
* Maintainer: Ye ShanShan <yeshanshan@uniontech.com>>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>

#include <QWidget>

#include "dpalettehelper.h"
DWIDGET_USE_NAMESPACE
class ut_DPaletteHelper : public testing::Test
{
protected:
    void SetUp() override
    {
        target = DPaletteHelper::instance();
    }
    void TearDown() override
    {
    }
    DPaletteHelper *target = nullptr;
};

TEST_F(ut_DPaletteHelper, resetPalette)
{
    QWidget *widget = new QWidget();
    DPalette originPalette = target->palette(widget);
    DPalette pallette;
    pallette.setColor(DPalette::TextTitle, Qt::red);
    target->setPalette(widget, pallette);
    ASSERT_EQ(target->palette(widget).color(DPalette::TextTitle), Qt::red);

    target->resetPalette(widget);
    ASSERT_EQ(target->palette(widget).color(DPalette::TextTitle), originPalette.color(DPalette::TextTitle));
    widget->deleteLater();
};

TEST_F(ut_DPaletteHelper, setPalette)
{
    QWidget *widget = new QWidget();
    DPalette pallette;
    pallette.setColor(DPalette::TextTitle, Qt::red);
    target->setPalette(widget, pallette);
    ASSERT_EQ(target->palette(widget).color(DPalette::TextTitle), Qt::red);
    widget->deleteLater();
};
