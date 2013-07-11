//////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2008-2010, Shane J. M. Liesegang
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright 
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright 
//       notice, this list of conditions and the following disclaimer in the 
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder nor the names of any 
//       contributors may be used to endorse or promote products derived from 
//       this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////

/** 
 * @file
 *  A low-level C-style interface for drawing text to the screen. Most of the
 *   functionality is wrapped in TextActor, which is where you should probably
 *   be focusing if you care about text. But if you want/need to wrangle your
 *   own text functionality, here you go. 
 */
#pragma once

#include "../Util/StringUtil.h"

/**
 * Register a font with our text rendering system. 
 * 
 * @param filename The path to the font file you want to use. Must be a format
 *   readable by FreeType (http://www.freetype.org). Most major font formats
 *   are acceptable. 
 * @param pointSize The size, in points, that you want the text to render. 
 * @param nickname How you want to refer to the font when telling it to draw
 * @return Whether or not it successfully registered (check the error log if
 *   this returns false)
 */
const bool RegisterFont(String filename, int pointSize, String nickname);

/**
 * If you're done using a font and are concerned about memory usage, you can
 *  remove it from active use. 
 * 
 * @param nickname The name you gave the font when you registered it
 * @return Whether it successfully unregistered. If false, check the error log. 
 */
const bool UnRegisterFont(String nickname);

/**
 * Draw text on the screen (but only once, so you need to call this every
 *  frame if you want the text to stay up).
 * 
 * @param text The string to write
 * @param nickname The name you've assigned to the font you want to use
 * @param pixelX The X-coordinate (in pixels) to start drawing
 * @param pixelY The Y-coordinate (in pixels) to start drawing
 * @param angle The angle at which to draw the text
 * @return The 2d point (in pixels) representing the bottom-most, right-most point
 *   where you could safely start drawing and not overwrite the string you
 *   just displayed. 
 */
Vector2 DrawGameText(String text, String nickname, int pixelX, int pixelY, float angle=0.0f);

/**
 * If you're just interested in how much space a certain string will take up, 
 *  but don't want to bother drawing it, this is the function for you. 
 * 
 * @param text The string you care about
 * @param nickname The font you would want to render it in
 * @return The 2d point (in pixels) representing the bottom-most, right-most point
 *   where you could safely start drawing and not overwrite the string if you
 *   had drawn it to the screen. 
 */
Vector2 GetTextExtents(String text, String nickname);
