//////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015-2016 Daniel Allendorf                                   //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "UIShop.h"

#include "../../Data/ItemData.h"
#include "../../Net/Packets/NpcInteractionPackets.h"
#include "../../Util/Misc.h"
#include "../Components/AreaButton.h"
#include "../Components/Charset.h"
#include "../Components/MapleButton.h"
#include "../Components/TwoSpriteButton.h"
#include "../UI.h"
#include "UINotice.h"
#include "nlnx/nx.hpp"

namespace jrc
{
UIShop::UIShop(const CharLook& in_charlook, const Inventory& in_inventory)
    : charlook(in_charlook), inventory(in_inventory)
{
    nl::node src = nl::nx::ui["UIWindow2.img"]["Shop"];
    nl::node src2 = nl::nx::ui["UIWindow2.img"]["Shop2"];

    sprites.emplace_back(src["backgrnd"]);
    sprites.emplace_back(src["backgrnd2"]);
    sprites.emplace_back(src["backgrnd3"]);

    buttons[BUY_ITEM] = std::make_unique<MapleButton>(src["BtBuy"]);
    buttons[SELL_ITEM] = std::make_unique<MapleButton>(src["BtSell"]);
    buttons[EXIT] = std::make_unique<MapleButton>(src["BtExit"]);

    nl::node sellen = src2["TabSell"]["enabled"];
    nl::node selldis = src2["TabSell"]["disabled"];

    constexpr Point<std::int16_t> stshift(-43, -9);
    for (std::uint16_t i = EQUIP; i <= CASH; ++i) {
        std::string tabnum = std::to_string(i - EQUIP);
        buttons[i] = std::make_unique<TwoSpriteButton>(
            selldis[tabnum], sellen[tabnum], stshift);
    }

    for (std::uint16_t i = BUY0; i <= BUY4; ++i) {
        Point<std::int16_t> pos(8, 116 + 42 * (i - BUY0));
        Point<std::int16_t> dim(200, 36);
        buttons[i] = std::make_unique<AreaButton>(pos, dim);
    }
    for (std::uint16_t i = SELL0; i <= SELL4; ++i) {
        Point<std::int16_t> pos(242, 116 + 42 * (i - SELL0));
        Point<std::int16_t> dim(200, 36);
        buttons[i] = std::make_unique<AreaButton>(pos, dim);
    }

    selection = src["select"];
    impossible = src["ShopSpecial"]["impossible"];
    meso = src["meso"];

    mesolabel = {Text::A11M, Text::RIGHT, Text::LIGHTGREY};

    buyslider = {11, {115, 308}, 214, 5, 1, [&](bool upwards) {
                     std::int16_t shift = upwards ? -1 : 1;
                     bool above = buystate.offset + shift >= 0;
                     bool below =
                         buystate.offset + shift <= buystate.lastslot - 5;
                     if (above && below) {
                         buystate.offset += shift;
                     }
                 }};
    sellslider = {11, {115, 308}, 445, 5, 1, [&](bool upwards) {
                      std::int16_t shift = upwards ? -1 : 1;
                      bool above = sellstate.offset + shift >= 0;
                      bool below =
                          sellstate.offset + shift <= sellstate.lastslot - 5;
                      if (above && below) {
                          sellstate.offset += shift;
                      }
                  }};

    active = false;
    dimension = Texture(src["backgrnd"]).get_dimensions();
    position = {400 - dimension.x() / 2, 240 - dimension.y() / 2};
}

void UIShop::draw(float alpha) const
{
    UIElement::draw(alpha);

    npc.draw({position + Point<std::int16_t>(64, 76), true});
    charlook.draw(position + Point<std::int16_t>(294, 76),
                  false,
                  Stance::STAND1,
                  Expression::DEFAULT);

    mesolabel.draw(position + Point<std::int16_t>(450, 62));

    buystate.draw(position, selection);
    sellstate.draw(position, selection);

    buyslider.draw(position);
    sellslider.draw(position);
}

void UIShop::update()
{
    std::int64_t num_mesos = inventory.get_meso();
    std::string mesostr = std::to_string(num_mesos);
    string_format::split_number(mesostr);
    mesolabel.change_text(mesostr);
}

Button::State UIShop::button_pressed(std::uint16_t buttonid)
{
    clear_tooltip();

    constexpr Range<std::uint16_t> buy(BUY0, BUY4);
    constexpr Range<std::uint16_t> sell(SELL0, SELL4);
    if (buy.contains(buttonid)) {
        std::int16_t selected = buttonid - BUY0;
        buystate.select(selected);

        return Button::NORMAL;
    } else if (sell.contains(buttonid)) {
        std::int16_t selected = buttonid - SELL0;
        sellstate.select(selected);

        return Button::NORMAL;
    } else {
        switch (buttonid) {
        case BUY_ITEM:
            buystate.buy();
            return Button::NORMAL;
        case SELL_ITEM:
            sellstate.sell();
            return Button::NORMAL;
        case EXIT:
            active = false;
            NpcShopActionPacket().dispatch();
            return Button::PRESSED;
        case EQUIP:
            changeselltab(InventoryType::EQUIP);
            return Button::IDENTITY;
        case USE:
            changeselltab(InventoryType::USE);
            return Button::IDENTITY;
        case ETC:
            changeselltab(InventoryType::ETC);
            return Button::IDENTITY;
        case SETUP:
            changeselltab(InventoryType::SETUP);
            return Button::IDENTITY;
        case CASH:
            changeselltab(InventoryType::CASH);
            return Button::IDENTITY;
        default:
            break;
        }
    }
    return Button::PRESSED;
}

bool UIShop::remove_cursor(bool clicked, Point<std::int16_t> cursorpos)
{
    if (buyslider.remove_cursor(clicked)) {
        return true;
    }

    if (sellslider.remove_cursor(clicked)) {
        return true;
    }

    return UIElement::remove_cursor(clicked, cursorpos);
}

Cursor::State UIShop::send_cursor(bool clicked, Point<std::int16_t> cursorpos)
{
    Point<std::int16_t> cursoroffset = cursorpos - position;
    if (buyslider.isenabled()) {
        Cursor::State bstate = buyslider.send_cursor(cursoroffset, clicked);
        if (bstate != Cursor::IDLE) {
            clear_tooltip();
            return bstate;
        }
    }

    if (sellslider.isenabled()) {
        Cursor::State sstate = sellslider.send_cursor(cursoroffset, clicked);
        if (sstate != Cursor::IDLE) {
            clear_tooltip();
            return sstate;
        }
    }

    std::int16_t xoff = cursoroffset.x();
    std::int16_t yoff = cursoroffset.y();
    std::int16_t slot = slot_by_position(yoff);
    if (slot >= 0 && slot <= 4) {
        if (xoff > 7 && xoff < 209) {
            show_item(slot, true);
        } else if (xoff > 241 && xoff < 443) {
            show_item(slot, false);
        } else {
            clear_tooltip();
        }
    } else {
        clear_tooltip();
    }
    return UIElement::send_cursor(clicked, cursorpos);
}

void UIShop::clear_tooltip()
{
    UI::get().clear_tooltip(Tooltip::SHOP);
}

void UIShop::show_item(std::int16_t slot, bool buy)
{
    if (buy) {
        buystate.show_item(slot);
    } else {
        sellstate.show_item(slot);
    }
}

void UIShop::changeselltab(InventoryType::Id type)
{
    std::uint16_t oldtab = tabbyinventory(sellstate.tab);
    if (oldtab > 0) {
        buttons[oldtab]->set_state(Button::NORMAL);
    }

    std::uint16_t newtab = tabbyinventory(type);
    if (newtab > 0) {
        buttons[newtab]->set_state(Button::PRESSED);
    }

    sellstate.change_tab(inventory, type, meso);

    sellslider.setrows(5, sellstate.lastslot);
}

void UIShop::reset(std::int32_t npcid)
{
    std::string strid = string_format::extend_id(npcid, 7);
    npc = nl::nx::npc[strid + ".img"]["stand"]["0"];

    for (auto& button : buttons) {
        button.second->set_state(Button::NORMAL);
    }
    buttons[EQUIP]->set_state(Button::PRESSED);

    buystate.reset();
    sellstate.reset();

    changeselltab(InventoryType::EQUIP);

    active = true;
}

void UIShop::modify(InventoryType::Id type)
{
    if (type == sellstate.tab) {
        changeselltab(type);
    }
}

void UIShop::add_item(std::int32_t id,
                      std::int32_t price,
                      std::int32_t pitch,
                      std::int32_t time,
                      std::int16_t buyable)
{
    add_rechargable(id, price, pitch, time, 0, buyable);
}

void UIShop::add_rechargable(std::int32_t id,
                             std::int32_t price,
                             std::int32_t pitch,
                             std::int32_t time,
                             std::int16_t chargeprice,
                             std::int16_t buyable)
{
    auto buyitem = BuyItem(meso, id, price, pitch, time, chargeprice, buyable);
    buystate.add(buyitem);

    buyslider.setrows(5, buystate.lastslot);
}

std::int16_t UIShop::slot_by_position(std::int16_t y)
{
    std::int16_t yoff = y - 115;
    if (yoff > 0 && yoff < 38) {
        return 0;
    } else if (yoff > 42 && yoff < 80) {
        return 1;
    } else if (yoff > 84 && yoff < 122) {
        return 2;
    } else if (yoff > 126 && yoff < 164) {
        return 3;
    } else if (yoff > 168 && yoff < 206) {
        return 4;
    } else {
        return -1;
    }
}

std::uint16_t UIShop::tabbyinventory(InventoryType::Id type)
{
    switch (type) {
    case InventoryType::EQUIP:
        return EQUIP;
    case InventoryType::USE:
        return USE;
    case InventoryType::ETC:
        return ETC;
    case InventoryType::SETUP:
        return SETUP;
    case InventoryType::CASH:
        return CASH;
    default:
        return 0;
    }
}

UIShop::BuyItem::BuyItem(Texture cur,
                         std::int32_t i,
                         std::int32_t p,
                         std::int32_t pt,
                         std::int32_t t,
                         std::int16_t cp,
                         std::int16_t b)
    : currency(cur),
      id(i),
      price(p),
      pitch(pt),
      time(t),
      chargeprice(cp),
      buyable(b)
{
    namelabel = {Text::A11M, Text::LEFT, Text::DARKGREY};
    pricelabel = {Text::A11M, Text::LEFT, Text::DARKGREY};

    const ItemData& item = ItemData::get(id);
    if (item.is_valid()) {
        icon = item.get_icon(false);
        namelabel.change_text(item.get_name());
    }

    std::string mesostr = std::to_string(price);
    string_format::split_number(mesostr);
    pricelabel.change_text(mesostr + " Mesos");
}

void UIShop::BuyItem::draw(Point<std::int16_t> pos) const
{
    icon.draw(pos + Point<std::int16_t>(0, 32));
    namelabel.draw(pos + Point<std::int16_t>(40, -1));
    currency.draw(pos + Point<std::int16_t>(38, 20));
    pricelabel.draw(pos + Point<std::int16_t>(53, 17));
}

std::int32_t UIShop::BuyItem::get_id() const
{
    return id;
}

std::int16_t UIShop::BuyItem::get_buyable() const
{
    return buyable;
}

UIShop::SellItem::SellItem(std::int32_t item_id,
                           std::int16_t count,
                           std::int16_t s,
                           bool sc,
                           Texture cur)
{
    const ItemData& idata = ItemData::get(item_id);

    icon = idata.get_icon(false);
    id = item_id;
    sellable = count;
    slot = s;
    showcount = sc;
    currency = cur;

    namelabel = {Text::A11M, Text::LEFT, Text::DARKGREY};
    pricelabel = {Text::A11M, Text::LEFT, Text::DARKGREY};

    std::string name = idata.get_name();
    namelabel.change_text(name);

    std::int32_t price = idata.get_price();
    std::string mesostr = std::to_string(price);
    string_format::split_number(mesostr);
    pricelabel.change_text(mesostr + " Mesos");
}

void UIShop::SellItem::draw(Point<std::int16_t> pos) const
{
    icon.draw(pos + Point<std::int16_t>(0, 32));
    if (showcount) {
        static const Charset countset = {nl::nx::ui["Basic.img"]["ItemNo"],
                                         Charset::LEFT};
        countset.draw(std::to_string(sellable),
                      pos + Point<std::int16_t>(0, 20));
    }
    namelabel.draw(pos + Point<std::int16_t>(40, -1));
    currency.draw(pos + Point<std::int16_t>(38, 20));
    pricelabel.draw(pos + Point<std::int16_t>(53, 17));
}

std::int32_t UIShop::SellItem::get_id() const
{
    return id;
}

std::int16_t UIShop::SellItem::get_slot() const
{
    return slot;
}

std::int16_t UIShop::SellItem::get_sellable() const
{
    return sellable;
}

void UIShop::BuyState::reset()
{
    items.clear();

    offset = 0;
    lastslot = 0;
    selection = -1;
}

void UIShop::BuyState::draw(Point<std::int16_t> parentpos,
                            const Texture& selected) const
{
    for (std::int16_t i = 0; i < 5; ++i) {
        std::int16_t slot = i + offset;
        if (slot >= lastslot) {
            break;
        }

        auto itempos = Point<std::int16_t>(12, 116 + 42 * i);
        if (slot == selection) {
            selected.draw(parentpos + itempos + Point<std::int16_t>(35, -1));
        }
        items[slot].draw(parentpos + itempos);
    }
}

void UIShop::BuyState::show_item(std::int16_t slot)
{
    std::int16_t absslot = slot + offset;
    if (absslot < 0 || absslot >= lastslot) {
        return;
    }

    std::int32_t itemid = items[absslot].get_id();
    UI::get().show_item(Tooltip::SHOP, itemid);
}

void UIShop::BuyState::add(BuyItem item)
{
    items.push_back(item);

    lastslot++;
}

void UIShop::BuyState::buy() const
{
    if (selection < 0 || selection >= lastslot) {
        return;
    }

    const BuyItem& item = items[selection];
    std::int16_t buyable = item.get_buyable();
    std::int16_t slot = selection;
    std::int32_t itemid = item.get_id();
    if (buyable > 1) {
        constexpr auto question = "How many would you like to buy?";
        auto onenter = [slot, itemid](std::int32_t qty) {
            auto shortqty = static_cast<std::int16_t>(qty);

            NpcShopActionPacket(slot, itemid, shortqty, true).dispatch();
        };
        UI::get().emplace<UIEnterNumber>(question, onenter, 1, buyable, 1);
    } else if (buyable > 0) {
        constexpr auto question = "Would you like to buy the item?";
        auto ondecide = [slot, itemid](bool yes) {
            if (yes) {
                NpcShopActionPacket(slot, itemid, 1, true).dispatch();
            }
        };
        UI::get().emplace<UIYesNo>(question, ondecide);
    }
}

void UIShop::BuyState::select(std::int16_t selected)
{
    std::int16_t slot = selected + offset;
    if (slot == selection) {
        buy();
    } else {
        selection = slot;
    }
}

void UIShop::SellState::reset()
{
    items.clear();

    offset = 0;
    lastslot = 0;
    selection = -1;
    tab = InventoryType::NONE;
}

void UIShop::SellState::change_tab(const Inventory& inv,
                                   InventoryType::Id newtab,
                                   Texture meso_texture)
{
    tab = newtab;

    offset = 0;
    selection = -1;

    items.clear();

    std::int16_t slots = inv.get_slotmax(tab);
    for (std::int16_t i = 1; i <= slots; ++i) {
        if (std::int32_t item_id = inv.get_item_id(tab, i)) {
            std::int16_t count = inv.get_item_count(tab, i);
            items.emplace_back(
                item_id, count, i, tab != InventoryType::EQUIP, meso_texture);
        }
    }

    lastslot = static_cast<std::int16_t>(items.size());
}

void UIShop::SellState::draw(Point<std::int16_t> parentpos,
                             const Texture& selected) const
{
    for (std::int16_t i = 0; i < 5; ++i) {
        std::int16_t slot = i + offset;
        if (slot >= lastslot) {
            break;
        }

        Point<std::int16_t> itempos(243, 116 + 42 * i);
        if (slot == selection) {
            selected.draw(parentpos + itempos + Point<std::int16_t>(35, -1));
        }
        items[slot].draw(parentpos + itempos);
    }
}

void UIShop::SellState::show_item(std::int16_t slot)
{
    std::int16_t absslot = slot + offset;
    if (absslot < 0 || absslot >= lastslot) {
        return;
    }

    if (tab == InventoryType::EQUIP) {
        std::int16_t realslot = items[absslot].get_slot();
        UI::get().show_equip(Tooltip::SHOP, realslot);
    } else {
        std::int32_t itemid = items[absslot].get_id();
        UI::get().show_item(Tooltip::SHOP, itemid);
    }
}

void UIShop::SellState::sell() const
{
    if (selection < 0 || selection >= lastslot) {
        return;
    }

    const SellItem& item = items[selection];
    std::int32_t itemid = item.get_id();
    std::int16_t sellable = item.get_sellable();
    std::int16_t slot = item.get_slot();
    if (sellable > 1) {
        constexpr auto question = "How many would you like to sell?";
        auto onenter = [itemid, slot](std::int32_t qty) {
            auto shortqty = static_cast<std::int16_t>(qty);

            NpcShopActionPacket(slot, itemid, shortqty, false).dispatch();
        };
        UI::get().emplace<UIEnterNumber>(question, onenter, 1, sellable, 1);
    } else if (sellable > 0) {
        constexpr auto question = "Would you like to sell the item?";
        auto ondecide = [itemid, slot](bool yes) {
            if (yes) {
                NpcShopActionPacket(slot, itemid, 1, false).dispatch();
            }
        };
        UI::get().emplace<UIYesNo>(question, ondecide);
    }
}

void UIShop::SellState::select(std::int16_t selected)
{
    std::int16_t slot = selected + offset;
    if (slot == selection) {
        sell();
    } else {
        selection = slot;
    }
}
} // namespace jrc
