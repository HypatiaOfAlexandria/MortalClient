//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
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
// along with this program.  If not, see <https://www.gnu.org/licenses/>.   //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../Constants.h"
#include "../Error.h"
#include "../Template/Rectangle.h"
#include "../Template/Singleton.h"
#include "../Template/nullable_ptr.h"
#include "../Util/QuadTree.h"
#include "DrawArgument.h"
#include "GL/glew.h"
#include "Text.h"
#include "ft2build.h"
#include "nlnx/bitmap.hpp"
#include FT_FREETYPE_H

#include <string_view>
#include <unordered_map>
#include <vector>

namespace jrc
{
//! Graphics engine which uses OpenGL.
class GraphicsGL : public Singleton<GraphicsGL>
{
public:
    GraphicsGL() noexcept;

    //! Initialise all resources.
    Error init();
    //! Re-initialise after changing screen modes.
    void reinit();

    //! Clear all bitmaps if most of the space is used up.
    void clear();

    //! Add a bitmap to the available resources.
    void add_bitmap(const nl::bitmap& bmp);
    //! Draw the bitmap with the given parameters.
    void draw(const nl::bitmap& bmp,
              const Rectangle<std::int16_t>& rect,
              const Color& color,
              float angle);

    //! Create a layout for the text with the parameters specified.
    Text::Layout create_layout(const utf8_string& text,
                               Text::Font font,
                               Text::Alignment alignment,
                               std::int16_t max_width,
                               bool formatted);
    //! Draw a text with the given parameters.
    void draw_text(const DrawArgument& args,
                   const utf8_string& text,
                   const Text::Layout& layout,
                   Text::Font font,
                   Text::Color color,
                   Text::Background back);

    //! Draw a rectangle filled with the specified color.
    void draw_rectangle(std::int16_t x,
                        std::int16_t y,
                        std::int16_t w,
                        std::int16_t h,
                        float r,
                        float g,
                        float b,
                        float a);
    //! Fill the screen with the specified color.
    void draw_screen_fill(float r, float g, float b, float a);

    //! Lock the current scene.
    void lock();
    //! Unlock the scene.
    void unlock();

    //! Draw the buffer contents with the specified scene opacity.
    void flush(float opacity);
    //! Clear the buffer contents.
    void clearscene();
    //! Set the screen rectangle.
    static void set_screen(Rectangle<std::int16_t>&& new_screen) noexcept;
    //! Set the screen rectangle.
    static void set_screen(std::int16_t l,
                           std::int16_t r,
                           std::int16_t t,
                           std::int16_t b) noexcept;

private:
    void clear_internal();
    bool
    addfont(const char* name, Text::Font id, FT_UInt width, FT_UInt height);

    struct Offset {
        GLshort l;
        GLshort r;
        GLshort t;
        GLshort b;

        Offset(GLshort x, GLshort y, GLshort w, GLshort h) noexcept
            : l{x}, r{x + w}, t{y}, b{y + h}
        {
        }

        Offset() noexcept : l{0}, r{0}, t{0}, b{0}
        {
        }
    };

    //! Add a bitmap to the available resources.
    const Offset& get_offset(const nl::bitmap& bmp);

    struct Leftover {
        GLshort l;
        GLshort r;
        GLshort t;
        GLshort b;

        Leftover(GLshort x, GLshort y, GLshort w, GLshort h) noexcept
            : l{x}, r{x + w}, t{y}, b{y + h}
        {
        }

        Leftover() noexcept : l{0}, r{0}, t{0}, b{0}
        {
        }

        GLshort width() const noexcept
        {
            return r - l;
        }

        GLshort height() const noexcept
        {
            return b - t;
        }
    };

    struct Quad {
        struct Vertex {
            GLshort x;
            GLshort y;
            GLshort s;
            GLshort t;

            Color c;
        };

        static const std::size_t LENGTH = 4;
        Vertex vertices[LENGTH];

        Quad(GLshort l,
             GLshort r,
             GLshort t,
             GLshort b,
             const Offset& o,
             const Color& color,
             GLfloat rot)
        {
            vertices[0] = {l, t, o.l, o.t, color};
            vertices[1] = {l, b, o.l, o.b, color};
            vertices[2] = {r, b, o.r, o.b, color};
            vertices[3] = {r, t, o.r, o.t, color};

            if (rot != 0.0f) {
                float cos = std::cos(rot);
                float sin = std::sin(rot);
                GLshort cx = (l + r) / 2;
                GLshort cy = (t + b) / 2;

                for (int i = 0; i < 4; ++i) {
                    GLshort vx = vertices[i].x - cx;
                    GLshort vy = vertices[i].y - cy;
                    GLfloat rx = std::roundf(vx * cos - vy * sin);
                    GLfloat ry = std::roundf(vx * sin + vy * cos);
                    vertices[i].x = static_cast<GLshort>(rx + cx);
                    vertices[i].y = static_cast<GLshort>(ry + cy);
                }
            }
        }
    };

    struct Font {
        struct Char {
            GLshort ax;
            GLshort ay;
            GLshort bw;
            GLshort bh;
            GLshort bl;
            GLshort bt;
            Offset offset;

            Char(GLshort ax_,
                 GLshort ay_,
                 GLshort bw_,
                 GLshort bh_,
                 GLshort bl_,
                 GLshort bt_,
                 Offset offset_) noexcept
                : ax{ax_},
                  ay{ay_},
                  bw{bw_},
                  bh{bh_},
                  bl{bl_},
                  bt{bt_},
                  offset{offset_}
            {
            }
        };

        FT_Face face;
        // GLshort width;
        GLshort height;

        Font(FT_Face face_, GLshort h = 0) noexcept : face{face_}, height{h}
        {
        }

        Font() noexcept : face{nullptr}, height{0}
        {
        }

        void set_height(GLshort h) noexcept
        {
            height = h;
        }

        std::int16_t line_space() const noexcept
        {
            return static_cast<std::int16_t>(static_cast<float>(height)
                                             * 1.35f)
                   + static_cast<std::int16_t>(1);
        }

        void add_char(char32_t c,
                      GLshort ax,
                      GLshort ay,
                      GLshort bw,
                      GLshort bh,
                      GLshort bl,
                      GLshort bt,
                      Offset offset) noexcept
        {
            chars.try_emplace(c, ax, ay, bw, bh, bl, bt, offset);
        }

        nullable_ptr<Char> get_or_insert_char(char32_t c) noexcept
        {
            if (auto iter = chars.find(c); iter != chars.end()) {
                return &iter->second;
            }

            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                return {};
            }

            const auto g = face->glyph;

            GLshort ax = static_cast<GLshort>(g->advance.x >> 6);
            GLshort ay = static_cast<GLshort>(g->advance.y >> 6);
            GLshort l = static_cast<GLshort>(g->bitmap_left);
            GLshort t = static_cast<GLshort>(g->bitmap_top);
            GLshort w = static_cast<GLshort>(g->bitmap.width);
            GLshort h = static_cast<GLshort>(g->bitmap.rows);

            auto& ggl = GraphicsGL::get();

            if (ggl.font_border.x() + w > ATLASW) {
                ggl.font_border.set_x(0);
                ggl.font_border.shift_y(ggl.font_y_max);
            }

            if (h > ggl.font_y_max) {
                ggl.font_y_max = h;
            }
            if (h > height) {
                height = h;
            }

            glTexSubImage2D(GL_TEXTURE_2D,
                            0,
                            ggl.font_border.x(),
                            ggl.font_border.y(),
                            w,
                            h,
                            GL_RED,
                            GL_UNSIGNED_BYTE,
                            g->bitmap.buffer);

            Offset offset{ggl.font_border.x(), ggl.font_border.y(), w, h};
            auto [iter, _] = chars.try_emplace(c, ax, ay, w, h, l, t, offset);

            ggl.font_border.shift_x(w);

            return &iter->second;
        }

    private:
        std::unordered_map<char32_t, Char> chars;
    };

    class LayoutBuilder
    {
    public:
        LayoutBuilder(Font& font,
                      Text::Alignment alignment,
                      std::int16_t maxwidth,
                      bool formatted) noexcept;

        utf8_string::size_type add(const utf8_string& text,
                                   utf8_string::size_type prev,
                                   utf8_string::size_type first,
                                   utf8_string::size_type last);
        Text::Layout finish(std::size_t first, std::size_t last);

    private:
        void add_word(std::size_t first,
                      std::size_t last,
                      Text::Font font,
                      Text::Color color);
        void add_line();

        Font& font;

        Text::Alignment alignment;
        Text::Font font_id;
        Text::Color color;
        std::int16_t max_width;
        bool formatted;

        std::int16_t ax;
        std::int16_t ay;

        std::vector<Text::Layout::Line> lines;
        std::vector<Text::Layout::Word> words;
        std::vector<std::int16_t> advances;
        std::int16_t width;
        std::int16_t endy;
    };

    static Rectangle<std::int16_t> screen;

    static constexpr const GLshort ATLASW = 8192;
    static constexpr const GLshort ATLASH = 8192;
    static constexpr const GLshort MINLOSIZE = 32;

    bool locked;

    std::vector<Quad> quads;
    GLuint vbo;
    GLuint atlas;

    GLint program;
    GLint attribute_coord;
    GLint attribute_color;
    GLint uniform_texture;
    GLint uniform_atlas_size;
    GLint uniform_screen_size;
    GLint uniform_y_offset;
    GLint uniform_font_region;

    std::unordered_map<std::size_t, Offset> offsets;
    Offset null_offset;

    QuadTree<std::size_t, Leftover> leftovers;
    std::size_t rlid;
    std::size_t wasted;
    Point<GLshort> border;
    Range<GLshort> y_range;

    FT_Library ft_library;
    Font fonts[Text::NUM_FONTS];
    Point<GLshort> font_border;
    GLshort font_y_max;
};

// constexpr Rectangle<std::int16_t> GraphicsGL::screen;
} // namespace jrc
