#pragma once

void FlushTextureCache();

const int GetTextureReference(String name, bool optional = false);
const int GetTextureReference(String filename, GLint clampmode, GLint filtermode, bool optional = false);
