#pragma once

typedef struct
{
    float R;
    float G;
    float B;
    float A;
} GuiColor;

const GuiColor Black{0, 0, 0, 255};
const GuiColor Black12{0, 0, 0, 32.0f};
const GuiColor Black25{0, 0, 0, 64.0f};
const GuiColor Black50{0, 0, 0, 128.0f};

const GuiColor White{255, 255, 255, 255};
const GuiColor White12{255, 255, 255, 32.0f};
const GuiColor White25{255, 255, 255, 64.0f};
const GuiColor White50{255, 255, 255, 128.0f};

const GuiColor Red{255, 0, 0, 255};
const GuiColor Red12{255, 0, 0, 32.0f};
const GuiColor Red25{255, 0, 0, 64.0f};
const GuiColor Red50{255, 0, 0, 128.0f};

const GuiColor Green{0, 255, 0, 255};
const GuiColor Green12{0, 255, 0, 32.0f};
const GuiColor Green25{0, 255, 0, 64.0f};
const GuiColor Green50{0, 255, 0, 128.0f};

const GuiColor Blue{0, 0, 255, 255};
const GuiColor Blue12{0, 0, 255, 32.0f};
const GuiColor Blue25{0, 0, 255, 64.0f};
const GuiColor Blue50{0, 0, 255, 128.0f};

const GuiColor GameRed{199, 51, 59, 255};
const GuiColor GameGreen{56, 109, 41, 255};
const GuiColor GameBlue{51, 93, 114, 255};
const GuiColor GameBlack{32, 36, 38, 255};
