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
#pragma once
#include "../../Character/Inventory/Inventory.h"
#include "../../Character/Look/CharLook.h"
#include "../../Graphics/Text.h"
#include "../../Graphics/Texture.h"
#include "../Components/Slider.h"
#include "../UIElement.h"

namespace jrc
{
class UIShop : public UIElement
{
public:
    static constexpr Type TYPE = SHOP;
    static constexpr bool FOCUSED = true;
    static constexpr bool TOGGLED = true;

    UIShop(const CharLook& charlook, const Inventory& inventory);

    void draw(float alpha) const override;
    void update() override;

    bool remove_cursor(bool clicked, Point<std::int16_t> cursorpos) override;
    Cursor::State send_cursor(bool clicked, Point<std::int16_t> position) override;

    void reset(std::int32_t npcid);
    void add_item(std::int32_t id,
                  std::int32_t price,
                  std::int32_t pitch,
                  std::int32_t time,
                  std::int16_t buyable);
    void add_rechargable(std::int32_t id,
                         std::int32_t price,
                         std::int32_t pitch,
                         std::int32_t time,
                         std::int16_t chargeprice,
                         std::int16_t buyable);

    void modify(InventoryType::Id type);

protected:
    Button::State button_pressed(std::uint16_t buttonid) override;

private:
    void clear_tooltip();
    void show_item(std::int16_t slot, bool sale);
    void changeselltab(InventoryType::Id tab);
    std::int16_t slot_by_position(std::int16_t y);
    std::uint16_t tabbyinventory(InventoryType::Id type);

    enum Buttons : std::int16_t {
        BUY_ITEM = 0,
        SELL_ITEM = 1,
        EXIT = 2,
        EQUIP = 3,
        USE = 4,
        ETC = 5,
        SETUP = 6,
        CASH = 7,
        BUY0 = 8,
        BUY4 = 12,
        SELL0 = 13,
        SELL4 = 17
    };

    const CharLook& charlook;
    const Inventory& inventory;

    Texture npc;
    Texture selection;
    Texture impossible;
    Texture meso;
    Text mesolabel;

    Slider buyslider;
    Slider sellslider;

    class BuyItem
    {
    public:
        BuyItem(Texture cur,
                std::int32_t i,
                std::int32_t p,
                std::int32_t pt,
                std::int32_t t,
                std::int16_t cp,
                std::int16_t b);

        void draw(Point<std::int16_t> position) const;

        std::int32_t get_id() const;
        std::int16_t get_buyable() const;

    private:
        Texture icon;
        Texture currency;
        std::int32_t id;
        std::int32_t price;
        std::int32_t pitch;
        std::int32_t time;
        std::int16_t chargeprice;
        std::int16_t buyable;
        Text namelabel;
        Text pricelabel;
    };

    class SellItem
    {
    public:
        SellItem(std::int32_t item_id,
                 std::int16_t count,
                 std::int16_t slot,
                 bool showcount,
                 Texture cur);

        void draw(Point<std::int16_t> position) const;

        std::int32_t get_id() const;
        std::int16_t get_slot() const;
        std::int16_t get_sellable() const;

    private:
        Texture icon;
        Texture currency;
        std::int32_t id;
        std::int16_t slot;
        std::int16_t sellable;
        bool showcount;
        Text namelabel;
        Text pricelabel;
    };

    struct BuyState {
        std::vector<BuyItem> items;
        std::int16_t offset;
        std::int16_t lastslot;
        std::int16_t selection;

        void reset();
        void draw(Point<std::int16_t> position, const Texture& selected) const;
        void show_item(std::int16_t slot);
        void add(BuyItem item);
        void buy() const;
        void select(std::int16_t selected);
    };
    BuyState buystate;

    struct SellState {
        std::vector<SellItem> items;
        std::int16_t offset;
        InventoryType::Id tab;
        std::int16_t lastslot;
        std::int16_t selection;

        void reset();
        void change_tab(const Inventory& inventory,
                        InventoryType::Id type,
                        Texture meso);
        void draw(Point<std::int16_t> position, const Texture& selected) const;
        void show_item(std::int16_t slot);
        void sell() const;
        void select(std::int16_t selected);
    };
    SellState sellstate;
};
} // namespace jrc
