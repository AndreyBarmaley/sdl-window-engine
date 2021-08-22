/***************************************************************************
 *   Copyright (C) 2017 by SWE team <sdl.window.engine@gmail.com>          *
 *                                                                         *
 *   Part of the SWE: SDL Window Engine:                                   *
 *   https://github.com/AndreyBarmaley/sdl-window-engine                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SWE_DISABLE_TERMGUI

#include <numeric>
#include <algorithm>

#include "swe_display.h"
#include "swe_inputs_keys.h"
#include "swe_translations.h"
#include "swe_termgui.h"

namespace SWE
{
    namespace TermGUI
    {
        std::array<ColorIndex, ColorsCounts>
                                themeColors = {{ Color::RoyalBlue, Color::Yellow, Color::Red, Color::LightCyan, Color::RoyalBlue, Color::Silver, Color::Red, Color::Silver, Color::Yellow, 
                                                Color::Silver, Color::Red, Color::Blue, Color::LightYellow, Color::Yellow, Color::Navy, Color::Navy, Color::Yellow, Color::Navy }};
        LineType                        themeLine = LineThin;


        void ThemeColors::setColor(const type_t & type, const ColorIndex & col)
        {
            themeColors[type] =  col;
        }

        void ThemeColors::setTheme(const LineType & line, std::array<ColorIndex, ColorsCounts> colors)
        {
            themeColors.swap(colors);
        }

        void ThemeColors::resetTheme(void)
        {
            themeLine = LineThin;

            themeColors[ColorBackground] = Color::RoyalBlue;
            themeColors[ColorBorderLine] = Color::Yellow;
            themeColors[ColorHeaderText] = Color::Red;
            themeColors[ColorBodyText] = Color::LightCyan;
            themeColors[ColorButtonBackground] = Color::RoyalBlue;
            themeColors[ColorButtonBracket] = Color::Silver;
            themeColors[ColorButtonFirstText] = Color::Red;
            themeColors[ColorButtonBodyText] = Color::Silver;
            themeColors[ColorFocusedButtonBackground] = Color::Yellow;
            themeColors[ColorFocusedButtonBracket] = Color::Silver;
            themeColors[ColorFocusedButtonFirstText] = Color::Red;
            themeColors[ColorFocusedButtonBodyText] = Color::Blue;
            themeColors[ColorInputFieldBackground] = Color::LightYellow;
            themeColors[ColorFocusedInputFieldBackground] = Color::Yellow;
            themeColors[ColorInputFieldText] = Color::Navy;
            themeColors[ColorInputCursor] = Color::Navy;
            themeColors[ColorListSelectedBackground] = Color::Yellow;
            themeColors[ColorListSelectedText] = Color::Navy;
        }

        LineType ThemeColors::lineType(void) const
        {
            return themeLine;
        }

        ColorIndex ThemeColors::colorBackground(void) const
        {
            return themeColors[ColorBackground];
        }

        ColorIndex ThemeColors::colorText(void) const
        {
            return themeColors[ColorBodyText];
        }

        ColorIndex ThemeColors::colorLine(void) const
        {
            return themeColors[ColorBorderLine];
        }

        /* LabelAction */
        LabelAction::LabelAction(const UnicodeString & str, int action, TermWindow & term)
            : TermWindow(& term), hotkey(Key::NONE)
        {
            resetState(FlagModality);
            setState(FlagKeyHandle);
            setAction(action);
            setLabel(str);
            setVisible(true);
        }

        LabelAction::LabelAction(const UnicodeString & str, int action, const TermPos & pos, TermWindow & term)
            : TermWindow(& term), hotkey(Key::NONE)
        {
            resetState(FlagModality);
            setState(FlagKeyHandle);
            setAction(action);
            setLabel(str);
            setTermPos(term, pos);
            setVisible(true);
        }

        LabelAction::LabelAction(TermWindow* term) : TermWindow(term), hotkey(Key::NONE)
        {
            resetState(FlagModality);
            setState(FlagKeyHandle);
            setVisible(false);
        }

        int LabelAction::parseHotKey(const UnicodeString & str) const
        {
            auto it1 = std::find(str.begin(), str.end(), '<');

            if(it1 != str.end())
            {
                it1 = std::next(it1);
                auto it2 = std::next(it1);

                if(it2 != str.end() && *it2 == '>')
                    return Key::toKey(std::string(1, ::toupper(*it1)));
            }

            return Key::NONE;
        }

        TermSize LabelAction::labelSize(const UnicodeString & str) const
        {
            return TermSize(str.size() - (Key::NONE != hotKey() ? 2 : 0), 1);
        }

        void LabelAction::setLabel(const UnicodeString & str)
        {
            setHotKey(parseHotKey(str));
            setTermSize(labelSize(str));
            content = str;
            setDirty(true);
        }

        bool LabelAction::isLabel(const UnicodeString & str) const
        {
            return content == str;
        }

        const UnicodeString & LabelAction::label(void) const
        {
            return content;
        }

        void LabelAction::setAction(int act)
        {
            setResultCode(act);
        }

        int LabelAction::action(void) const
        {
            return resultCode();
        }

        bool LabelAction::isAction(int act) const
        {
            return action() == act;
        }

        void LabelAction::setHotKey(int key)
        {
            hotkey = key;
        }

#define FlagHotKeyDisabled 0x01

        void LabelAction::setHotKeyDisabled(bool f)
        {
            setState(FlagHotKeyDisabled, f);
        }

        bool LabelAction::isHotKeyDisabled(void) const
        {
            return checkState(FlagHotKeyDisabled);
        }

        int LabelAction::hotKey(void) const
        {
            return isHotKeyDisabled() ? Key::NONE : hotkey;
        }

        bool LabelAction::isHotKey(int key) const
        {
            return isHotKeyDisabled() ? false : hotKey() == key;
        }

        void LabelAction::setSelected(bool f)
        {
            if(! isDisabled())
            {
                if(f)
                {
                    setState(FlagSelected);
                    signalEmit(Signal::ButtonSetFocus);
                }
                else
                {
                    resetState(FlagSelected);
                    signalEmit(Signal::ButtonResetFocus);
                }

                setDirty(true);
            }
        }

        bool LabelAction::isSelected(void) const
        {
            return checkState(FlagSelected);
        }

        void LabelAction::setDisabled(bool f)
        {
            setState(FlagDisabled, f);
            setDirty(true);
        }

        bool LabelAction::isDisabled(void) const
        {
            return checkState(FlagDisabled);
        }

        void LabelAction::renderLabel(void)
        {
            auto hcol = colorHotKey(isSelected());
            auto hotch = hotKey() ? KeySym(hotKey(), 0).keychar() : 0;

            auto body = colorText(isSelected());
            auto back = colorBackground(isSelected());
            *this << set::colors(body, back);

            for(auto & ch : label())
            {
                if(ch == '<' || ch == '>')
                    continue;

                if(hotch == ::tolower(ch) && ! isDisabled())
                {
                    // first found hotch
                    *this << set::fgcolor(hcol) << ch << set::fgcolor(body);
                    hotch = 0;
                }
                else
                    *this << ch;
            }

            *this << reset::colors();
        }


        ColorIndex LabelAction::colorBackground(void) const
        {
            return themeColors[ColorButtonBackground];
        }

        ColorIndex LabelAction::colorText(void) const
        {
            return themeColors[ColorButtonBodyText];
        }

        ColorIndex LabelAction::colorBackground(bool focused) const
        {
            return focused ? themeColors[ColorFocusedButtonBackground] : colorBackground();
        }

        ColorIndex LabelAction::colorText(bool focused) const
        {
            return focused ? themeColors[ColorFocusedButtonBodyText] : colorText();
        }

        ColorIndex LabelAction::colorHotKey(bool focused) const
        {
            return themeColors[focused ? ColorFocusedButtonFirstText : ColorButtonFirstText];
        }

        void LabelAction::renderWindow(void)
        {
            *this << reset::defaults();

            renderLabel();

            *this << set::flush();
        }

        bool LabelAction::keyPressEvent(const KeySym & key)
        {
            if(isDisabled())
                return false;

            if(Key::NONE != hotKey() && hotKey() == key.keycode())
            {
                if(isSelected())
                    clickAction();
                else
                    setSelected(true);

                return true;
            }
            else if(isSelected() && key.keycode() == Key::RETURN)
            {
                clickAction();
                return true;
            }

            return false;
        }

        void LabelAction::mouseFocusEvent(void)
        {
            setSelected(true);
        }

        void LabelAction::mouseLeaveEvent(void)
        {
            setSelected(false);
        }

        bool LabelAction::mouseClickEvent(const ButtonsEvent & be)
        {
            if(! isDisabled())
                clickAction();

            return true;
        }

        void LabelAction::clickAction(void)
        {
            setSelected(false);
            signalEmit(Signal::ButtonClicked);

            if(action() && parent())
                pushEventAction(action(), parent(), this);
            setDirty(true);
        }

        /* LabelActionGroup */
        LabelAction* LabelActionGroup::addLabel(const std::string & label, int action, const TermPos & pos, TermWindow & term)
        {
            emplace_back(label, action, pos, term);
            return back().get();
        }

        LabelAction* LabelActionGroup::addLabel(LabelAction* ptr)
        {
            emplace_back(ptr);
            return back().get();
        }

        bool LabelActionGroup::findLabel(const LabelAction* lb) const
        {
            return std::any_of(begin(), end(), [=](auto & ptr){ return ptr.get() == lb; });
        }

        LabelAction* LabelActionGroup::findLabel(const UnicodeString & str) const
        {
            auto it = std::find_if(begin(), end(), [&](auto & ptr){ return ptr && ptr->isLabel(str); });
            return it != end() ? (*it).get() : nullptr;
        }

        LabelAction* LabelActionGroup::findAction(int action) const
        {
            auto it = std::find_if(begin(), end(), [=](auto & ptr){ return ptr && ptr->isAction(action); });
            return it != end() ? (*it).get() : nullptr;
        }

        LabelAction* LabelActionGroup::findHotKey(int key) const
        {
            auto it = std::find_if(begin(), end(), [=](auto & ptr){ return ptr && ptr->isHotKey(key); });
            return it != end() ? (*it).get() : nullptr;
        }

        LabelAction* LabelActionGroup::findSelected(void) const
        {
            auto it = std::find_if(begin(), end(), [](auto & ptr){ return ptr && ptr->isSelected(); });
            return it != end() ? (*it).get() : nullptr;
        }

        LabelAction* LabelActionGroup::findIndex(size_t dist) const
        {
            auto it = std::next(begin(), dist);
            return it != end() ? (*it).get() : nullptr;
        }

        int LabelActionGroup::index(LabelAction* lb) const
        {
            if(lb)
            {
                auto it = std::find_if(begin(), end(), [=](auto & ptr){ return ptr.get() == lb; });
                if(it != end()) return std::distance(begin(), it);
            }

            return -1;
        }

        bool LabelActionGroup::setSelected(const LabelAction* lb)
        {
            if(lb &&
                std::any_of(begin(), end(), [=](auto & ptr){ return ptr.get() == lb; }))
            {
                resetSelected();
                const_cast<LabelAction*>(lb)->setSelected(true);
                return true;
            }

            return false;
        }

        void LabelActionGroup::nextSelected(void)
        {
            if(1 < size())
            {
                auto it = std::find_if(begin(), end(),
                                [](auto & ptr){ return ptr.get() && ptr->isSelected(); });
                if(it != end())
                {
                    it++;

                    if(it == end())
                        it = begin();

                    resetSelected();
                    (*it)->setSelected(true);
                }
            }
        }

        void LabelActionGroup::prevSelected(void)
        {
            if(1 < size())
            {
                auto it = std::find_if(begin(), end(),
                                [](auto & ptr){ return ptr.get() && ptr->isSelected(); });
                if(it != end())
                {
                    if(it == begin())
                        it = end();

                    it--;
                    resetSelected();
                    (*it)->setSelected(true);
                }
            }
        }

        void LabelActionGroup::firstSelected(void)
        {
            if(! empty())
            {
                auto & ptr = front();

                if(ptr && ! ptr->isSelected())
                {
                    resetSelected();
                    ptr->setSelected(true);
                }
            }
        }

        bool LabelActionGroup::isFirstSelected(void) const
        {
            if(! empty())
            {
                auto & ptr = front();
                return ptr.get() == findSelected();
            }

            return false;
        }

        void LabelActionGroup::lastSelected(void)
        {
            if(! empty())
            {
                auto & ptr = back();

                if(ptr && ! ptr->isSelected())
                {
                    resetSelected();
                    ptr->setSelected(true);
                }
            }
        }

        bool LabelActionGroup::isLastSelected(void) const
        {
            if(! empty())
            {
                auto & ptr = back();
                return ptr.get() == findSelected();
            }

            return false;
        }

        void LabelActionGroup::resetSelected(const LabelAction* exclude)
        {
            for(auto & ptr : *this)
                if(ptr && (!exclude || exclude != ptr.get()))
                    ptr->setSelected(false);
        }

        size_t LabelActionGroup::rows(void) const
        {
            return std::accumulate(begin(), end(), 0, [](int v, auto & ptr){ return ptr ? v + ptr->rows() : v; });
        }

        size_t LabelActionGroup::cols(void) const
        {
            return std::accumulate(begin(), end(), 0, [](int v, auto & ptr){ return ptr ? v + ptr->cols() : v; });
        }

        /* TextButton */
        TextButton::TextButton(const UnicodeString & us, int action, TermWindow & term) : LabelAction(us, action, term)
        {
            // reserve for [ ]
            setTermSize(TermSize(cols() + 2, rows()));
        }

        TextButton::TextButton(const UnicodeString & us, int action, const TermPos & tpos, TermWindow & term) : LabelAction(us, action, tpos, term)
        {
            // reserve for [ ]
            setTermSize(TermSize(cols() + 2, rows()));
        }

        TextButton::TextButton(const buttons_t & type, TermWindow & term) : LabelAction(& term)
        {
            setAction(type);
            setButton(type);
            setVisible(true);
        }

        TermSize TextButton::labelSize(const UnicodeString & str) const
        {
            return TermSize(str.size() + (Key::NONE != hotKey() ? 0 : 2), 1);
        }

        void TextButton::setButton(const buttons_t & type)
        {
            const char* str = "UnknownButton";

            switch(type)
            {
                case ButtonOk:
                    str = _("<O>k");
                    break;

                case ButtonYes:
                    str = _("<Y>es");
                    break;

                case ButtonNo:
                    str = _("<N>o");
                    break;

                case ButtonCancel:
                    str = _("<C>ancel");
                    break;

                default:
                    break;
            }

            setLabel(str);
        }

        ColorIndex TextButton::colorBracket(bool focused) const
        {
            return themeColors[focused ? ColorFocusedButtonBracket : ColorButtonBracket];
        }

        void TextButton::renderWindow(void)
        {
            auto brkt = colorBracket(isSelected());
            auto back2 = colorBackground(false);

            *this << reset::defaults() <<
                set::colors(brkt, back2) << "[" << reset::colors();

            renderLabel();

            *this << set::colors(brkt, back2) << "]" << reset::colors() <<
                set::flush();
        }

#define ButtonSelected 0x80000000

        /* ButtonsGroup */
        ButtonsGroup::ButtonsGroup(int buttons, TermWindow & term)
        {
            auto buttonsAll = { ButtonOk, ButtonYes, ButtonNo, ButtonCancel };

            for(auto & type : buttonsAll)
                if(buttons & type) addLabel(new TextButton(type, term));

            if(buttons & ButtonSelected) lastSelected();
        }

        /* HeaderAreaBox */
        HeaderAreaBox::HeaderAreaBox(const UnicodeString & us, TermWindow & term) : TermWindow(& term), header(us) 
        {
            // reserv 2 tee + 2 corner
            setTermSize(TermSize(header.size() + 4, 2));
            setVisible(true);
        }

        HeaderAreaBox::HeaderAreaBox(const UnicodeString & us, const FontRender & frs, Window* win) : TermWindow(frs, win), header(us)
        {
            // reserv 2 tee + 2 corner
            setTermSize(TermSize(header.size() + 4, 2));
            setVisible(true);
        }

        HeaderAreaBox::HeaderAreaBox(const UnicodeString & us, const TermSize & tsz, TermWindow & term) : TermWindow(& term), header(us)
        {
            // reserv: 2 line, 2 tee + 2 corner
            int columns = std::max(tsz.cols() + 2, header.size() + 4);
            // reserv: 2 line
            setTermSize(TermSize(columns, tsz.rows() + 2));
            setVisible(true);
        }


        ColorIndex HeaderAreaBox::colorHeader(void) const
        {
            return themeColors[ColorHeaderText];
        }

        LineType HeaderAreaBox::lineType(void) const
        {
            // force fixed type
            return TermWindow::fontRender == & systemFont() ? LineAscii : ThemeColors::lineType();
        }

        void HeaderAreaBox::renderBox(void)
        {
            *this << set::colors(colorLine(), colorBackground()) << 
                // border section
                draw::rect(0, 0, cols(), rows(), lineType()) << reset::colors();

            // header section
            if(header.size())
            {
                *this << cursor::set((cols() - header.size() - 2) / 2, 0) <<
                      acs::rtee(lineType()) << set::fgcolor(colorHeader()) << header << reset::fgcolor() << acs::ltee(lineType());
            }
        }

        void HeaderAreaBox::renderWindow(void)
        {
            *this << reset::defaults();

            renderBox();

            *this << set::flush();
        }

        bool HeaderAreaBox::keyPressEvent(const KeySym & key)
        {
            if(key.keycode() == Key::ESCAPE)
            {
                setVisible(false);
                return true;
            }

            return false;
        }

        /* ButtonsAreaBox */
        void ButtonsAreaBox::initAreaBox(void)
        {
            // reserv: 2 space + 2 line
            auto btnsColumns = btnsGroup.cols() + (btnsGroup.count() - 1) + 4;
            auto columns = std::max(cols(), btnsColumns);
            // reserv 2 line button section
            setTermSize(TermSize(columns, rows() + 2));

            setButtonsPosition();

            // signals
            setButtonsSubscribe(Signal::ButtonClicked);
            setButtonsSubscribe(Signal::ButtonSetFocus);
            signalSubscribe(*this, Signal::ButtonResetFocus);
        }

        void ButtonsAreaBox::signalReceive(int sig, const SignalMember* sm)
        {
            switch(sig)
            {
                case Signal::ButtonResetFocus:
                    btnsGroup.resetSelected();
                    break;

                case Signal::ButtonClicked:
                {
                    const TextButton* tb = dynamic_cast<const TextButton*>(sm);
                    if(tb) setResultCode(tb->action());

                    setVisible(false);
                }
                break;

                case Signal::ButtonSetFocus:
                {
                    const TextButton* tb = dynamic_cast<const TextButton*>(sm);
                    if(tb) btnsGroup.resetSelected(tb);
                }
                break;

                default:
                    break;
            }
        }

        void ButtonsAreaBox::setHotKeyDisabled(bool f)
        {
            for(auto & ptr : btnsGroup)
                if(ptr) ptr->setHotKeyDisabled(f);
        }

        void ButtonsAreaBox::setButtonsPosition(void)
        {
            size_t width = btnsGroup.cols() + (btnsGroup.count() - 1);
            Point pos = sym2gfx(TermPos((cols() - width) / 2, rows() - 2)) - position();

            for(auto & ptr : btnsGroup)
            {
                if(ptr)
                {
                    ptr->setPosition(pos);
                    ptr->setVisible(true);
                    pos += sym2gfx(TermSize(ptr->cols() + 1, 0));
                }
            }
        }

        void ButtonsAreaBox::setButtonsSubscribe(int sig)
        {
            for(auto & ptr : btnsGroup)
                if(ptr) signalSubscribe(*ptr, sig);
        }

        ButtonsGroup & ButtonsAreaBox::buttonsGroup(void)
        {
            return btnsGroup;
        }

        const ButtonsGroup & ButtonsAreaBox::buttonsGroup(void) const
        {
            return btnsGroup;
        }

        void ButtonsAreaBox::renderButtons(void)
        {
            // bottom section
            *this << cursor::set(0, rows() - 3) <<
                set::colors(colorLine(), colorBackground()) << 
                acs::ltee(lineType()) << draw::hline(cols() - 2, acs::hline(lineType())) << acs::rtee(lineType()) << reset::colors();

            // buttons section
            for(auto & btn : btnsGroup)
                if(btn) btn->renderWindow();
        }

        void ButtonsAreaBox::renderWindow(void)
        {
            *this << reset::defaults();

            renderBox();
            renderButtons();

            *this << set::flush();
        }

        bool ButtonsAreaBox::keyPressEvent(const KeySym & key)
        {
            switch(key.keycode())
            {
                case Key::LEFT:
                    if(btnsGroup.findSelected())
                    {
                        btnsGroup.prevSelected();
                        return true;
                    }

                    break;

                case Key::RIGHT:
                    if(btnsGroup.findSelected())
                    {
                        btnsGroup.nextSelected();
                        return true;
                    }

                    break;

                case Key::ESCAPE:
                    setVisible(false);
                    return true;

                default:
                    break;
            }

            return false;
        }

        /* MessageBox */
        MessageBox::MessageBox(const UnicodeString & header, const UnicodeString & message, int buttons, TermWindow & term)
            : UnicodeList(message.split('\n')), ButtonsAreaBox(header, TermSize(UnicodeList::maxStringWidth() + 2 /* space */, UnicodeList::size()), buttons | ButtonSelected, term)
        {
            // centered
            setPosition((term.size() - Window::size()) / 2);
            setVisible(true);
        }

        MessageBox::MessageBox(const UnicodeString & header, const UnicodeString & message, int buttons, const FontRender & frs, Window* win)
            : UnicodeList(message.split('\n')), ButtonsAreaBox(header, buttons | ButtonSelected, frs, win)
        {
            // reserv: 2 space + 2 line
            auto contentColumns = UnicodeList::maxStringWidth() + 4;
            auto columns = std::max(cols(), contentColumns);
            setTermSize(TermSize(columns, rows() + UnicodeList::size()));
            setButtonsPosition();

            // centered
            if(win)
                setPosition((win->size() - Window::size()) / 2);
            setVisible(true);
        }

        void MessageBox::renderWindow(void)
        {
            auto & term = static_cast<TermWindow &>(*this);

            term << reset::defaults();

            renderBox();
            renderButtons();

            for(auto it = UnicodeList::begin(); it != UnicodeList::end(); ++it)
                term << cursor::set((cols() - (*it).size()) / 2, 1 + std::distance(UnicodeList::begin(), it)) << *it;

            term << set::flush();
        }

        /* InputBox */
        InputBox::InputBox(const UnicodeString & header, size_t inputCols, const std::string & defstr, TermWindow & term)
            : ButtonsAreaBox(header, TermSize(inputCols, 1), ButtonYes | ButtonNo, term), sResult(defstr)
        {
            setInputFocused(true);
            setHotKeyDisabled(true);

            // centered
            setPosition((term.size() - Window::size()) / 2);
            setVisible(true);
        }

        InputBox::InputBox(const UnicodeString & header, size_t inputCols, const std::string & defstr, const FontRender & frs, Window* win)
            : ButtonsAreaBox(header, ButtonYes | ButtonNo, frs, win), sResult(defstr)
        {
            // reserv: 2 line
            auto columns = std::max(cols(), inputCols + 2);
            setTermSize(TermSize(columns, rows() + 1));
            setButtonsPosition();

            setInputFocused(true);
            setHotKeyDisabled(true);

            // centered
            if(win)
                setPosition((win->size() - Window::size()) / 2);

            setVisible(true);
        }

        void InputBox::setInputFocused(bool f)
        {
            setState(FlagSelected, f);
        }

        bool InputBox::checkInputFocused(void) const
        {
            return checkState(FlagSelected);
        }

        bool InputBox::keyPressEvent(const KeySym & key)
        {
            if(ButtonsAreaBox::keyPressEvent(key))
                return true;

            if(! checkInputFocused())
            {
                if(key.keycode() == Key::TAB)
                {
                    if(buttonsGroup().isFirstSelected())
                        buttonsGroup().lastSelected();
                    else
                    if(buttonsGroup().isLastSelected())
                    {
                        buttonsGroup().resetSelected();
                        setInputFocused(true);
                    }

                    return true;
                }

                return false;
            }

            switch(key.keycode())
            {
                case Key::RETURN:
                    setResultCode(ButtonYes);
                    setVisible(false);
                    return true;

                case Key::ESCAPE:
                    sResult.clear();
                    setResultCode(ButtonNo);
                    setVisible(false);
                    return true;

                case Key::TAB:
                    setInputFocused(false);
                    buttonsGroup().firstSelected();
                    setDirty(true);
                    return true;

                case Key::BACKSPACE:
                    if(0 < sResult.size())
                    {
                        sResult.erase(sResult.end() - 1);
                        setDirty(true);
                        return true;
                    }

                    break;

                default:
                {
                    int ch = key.keychar();

                    if(ch)
                    {
                        sResult.append(1, ch);
                        setDirty(true);
                        return true;
                    }

                    break;
                }
            }

            return false;
        }

        ColorIndex InputBox::colorFieldText(void) const
        {
            return themeColors[ColorInputFieldText];
        }

        ColorIndex InputBox::colorFieldCursor(void) const
        {
            return themeColors[ColorInputCursor];
        }

        ColorIndex InputBox::colorFieldBackground(bool focused) const
        {
            return themeColors[focused ? ColorFocusedInputFieldBackground : ColorInputFieldBackground];
        }

        void InputBox::renderWindow(void)
        {
            *this << reset::defaults();

            renderBox();
            renderButtons();

            const size_t iw = cols() - 2;

            // render input
            *this << cursor::set(1, 1) << set::colors(colorFieldText(), colorFieldBackground(checkInputFocused())) <<
                  fill::space(cols() - 2) << cursor::set(1, 1) << reset::colors();

            if(sResult.size() < iw)
                *this << sResult;
            else
                *this << sResult.substr(sResult.size() - iw + 1, iw - 1);

            *this << set::fgcolor(colorFieldCursor()) << set::blink() <<  "_" << reset::blink() << reset::fgcolor();
            *this << set::flush();
        }

        /* ListBox */
        ListBox::ListBox(const UnicodeString & header, const UnicodeList & list, size_t listRows, TermWindow & term)
            : ButtonsAreaBox(header, TermSize(list.maxStringWidth() + 2 /* space */, listRows), ButtonYes | ButtonNo | ButtonSelected, term), content(list), selected(-1), skipped(0)
        {
            // centered
            setPosition((term.size() - Window::size()) / 2);
            setVisible(true);
        }

        ListBox::ListBox(const UnicodeString & header, const UnicodeList & list, size_t listRows, const FontRender & frs, Window* win)
            : ButtonsAreaBox(header, ButtonYes | ButtonNo | ButtonSelected, frs, win), content(list), selected(-1), skipped(0)
        {
            // reserv: 2 space + 2 line
            auto contentColumns = content.maxStringWidth() + 4;
            setTermSize(TermSize(contentColumns, listRows));
            setButtonsPosition();

            // centered
            if(win)
                setPosition((win->size() - Window::size()) / 2);

            setVisible(true);
        }

        ColorIndex ListBox::colorText(bool selected) const
        {
            return selected ? themeColors[ColorListSelectedText] : ThemeColors::colorText();
        }

        ColorIndex ListBox::colorBackground(bool selected) const
        {
            return selected ? themeColors[ColorListSelectedBackground] : ThemeColors::colorBackground();
        }

        void ListBox::renderWindow(void)
        {
            *this << reset::defaults();

            renderBox();
            renderButtons();

            int max = rows() - 4;
            skipped = (max <= selected ? (selected - max + 1) : 0);
            auto it = std::next(content.begin(), skipped);

            for(; it != content.end(); ++it)
            {
                int dist = std::distance(content.begin(), it) - skipped;
                if(dist >= max) break;

                *this << cursor::set(1, 1 + dist);

                auto text = colorText(dist + skipped == selected);
                auto back = colorBackground(dist + skipped == selected);

                *this << cursor::set(1, 1 + dist) << set::colors(text, back) <<
                      cursor::set(1, 1 + dist) << *it << fill::space(cols() - 2 - (*it).size()) << reset::colors();
            }

            *this << set::flush();
        }

        void ListBox::windowVisibleEvent(bool show)
        {
            if(! show)
            {
                sResult.clear();

                if(ButtonYes == resultCode())
                {
                    auto it = std::next(content.begin(), selected);

                    if(it != content.end())
                        sResult = (*it).toString();
                }
            }
        }

        bool ListBox::mouseClickEvent(const ButtonsEvent & be)
        {
            Rect rt = sym2gfx(TermRect(1, 1, cols() - 2, rows() - 4));
            Point cur = be.press().position() + position();

            if(rt & cur)
            {
                signalEmit(Signal::ButtonResetFocus);
                int index = skipped + (cur.y - rt.y) / frs()->lineSkipHeight();

                // double click: close event
                if(index == selected)
                {
                    setResultCode(ButtonYes);
                    setVisible(false);
                }
                else
                {
                    selected = index;
                    setDirty(true);
                }
            }

            return false;
        }

        bool ListBox::scrollUpContent(void)
        {
            if(0 < selected)
            {
                selected--;
                signalEmit(Signal::ButtonResetFocus);
                setDirty(true);
                return true;
            }

            return false;
        }

        bool ListBox::scrollDownContent(void)
        {
            if(selected < static_cast<int>(content.size() - 1))
            {
                selected++;
                signalEmit(Signal::ButtonResetFocus);
                setDirty(true);
                return true;
            }

            return false;
        }

        bool ListBox::scrollUpEvent(void)
        {
            Rect rt = sym2gfx(TermRect(1, 1, cols() - 2, rows() - 4));

            if(rt & Display::mouseCursorPosition())
                return scrollUpContent();

            return false;
        }

        bool ListBox::scrollDownEvent(void)
        {
            Rect rt = sym2gfx(TermRect(1, 1, cols() - 2, rows() - 4));

            if(rt & Display::mouseCursorPosition())
                return scrollDownContent();

            return false;
        }

        bool ListBox::keyPressEvent(const KeySym & key)
        {
            if(ButtonsAreaBox::keyPressEvent(key))
                return true;

            switch(key.keycode())
            {
                case Key::UP:
                    return scrollUpContent();

                case Key::DOWN:
                    return scrollDownContent();

                default:
                    break;
            }

            return false;
        }

    } // namespace TermGUI

    /* CommandConsole */
    CommandConsole::CommandConsole(const TermSize & termsz, TermWindow & term) : TermWindow(termsz, term)
    {
        setModality(true);
        setVisible(true);
    }

    CommandConsole::CommandConsole(const Size & gfxsz, const FontRender & frs, Window & win) : TermWindow(gfxsz, frs, & win)
    {
        setModality(true);
        setVisible(true);
    }

    void CommandConsole::renderWindow(void)
    {
        const auto hdrcol = Color::White;
        const auto cmdcol = Color::White;
        const auto line = LineType::LineThin;
        const std::string header = "CommandConsole";

        int body = rows() - 4;
        int skipline = 0;

        if(content.size() > body)
            skipline = content.size() - body;

        int startline = 1;
        if(content.size() - skipline < body)
            startline = 1 + body - (content.size() - skipline);

        *this << fill::defaults(defaultColors(), 0x20, defaultProperty());

        // content
        for(auto & str : content)
        {
            if(0 == skipline)
            {
                *this << cursor::set(1, startline) << str;
                startline++;
            }
            else
                skipline--;
        }

        *this <<
            // border
            draw::rect(0, 0, cols(), rows(), line);

        // header
        if(header.size())
        {
            *this <<
                cursor::set((cols() - (header.size() + 4)) / 2, 0) << acs::rtee(line) << 0x20 << set::fgcolor(hdrcol) << header << reset::fgcolor() << 0x20 << acs::ltee(line);
        }

        *this <<
            // separator line
            cursor::set(0, rows() - 3) << acs::ltee(line) << draw::hline(cols() - 2, acs::hline(line)) << acs::rtee(line) <<
            // cursor emulator
            cursor::set(1, rows() - 2) << set::fgcolor(cmdcol) << command << set::blink() << 0x2581 << reset::blink() << reset::fgcolor() <<
            // flush
            set::flush();
    }

    bool CommandConsole::actionCommand(const std::string & cmd)
    {
        command.clear();
        setDirty(true);
        return true;
    }

    bool CommandConsole::keyPressEvent(const KeySym & ks)
    {
        if(ks.keycode() == Key::ESCAPE)
        {
            setVisible(false);
            return true;
        }
        else
        if(ks.keycode() == Key::RETURN)
        {
            return actionCommand(command);
        }
        else
        if(ks.keycode() == Key::BACKSPACE && command.size())
        {
            command.erase(command.size() - 1, 1);
            setDirty(true);
            return true;
        }
#ifndef SWE_SDL12
        else
        if(ks.keycode() == Key::INSERT && ks.keymod().isShift() &&
            SDL_HasClipboardText())
        {
            std::string str = SDL_GetClipboardText();
            textInputEvent(str);
            return true;
        }
#endif

#ifdef SWE_SDL12
        // text input SDL12: ascii only
        if(0x20 <= ks.keychar() && ks.keychar() < 255)
        {
            textInputEvent(std::string(1, ks.keychar()));
            return true;
        }
#endif

        return false;
    }

    bool CommandConsole::textInputEvent(const std::string & str)
    {
        if(str.size())
        {
            command.append(str);
            setDirty(true);
            return true;
        }

        return false;
    }

    FBColors CommandConsole::defaultColors(void) const
    {
        return FBColors(Color::Silver, Color::MidnightBlue);
    }

    void CommandConsole::commandLineClear(void)
    {
        command.clear();
        setDirty(true);
    }

    void CommandConsole::contentLinesClear(void)
    {
        content.clear();
        setDirty(true);
    }

    void CommandConsole::contentLinesAppend(const std::string & str)
    {
        content.append(str);
        setDirty(true);
    }

    void CommandConsole::contentLinesAppend(const StringList & list)
    {
        content.append(list);
        setDirty(true);
    }
} // namespace SWE

#endif
