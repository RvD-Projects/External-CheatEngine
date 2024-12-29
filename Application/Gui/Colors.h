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
const GuiColor Black64{0, 0, 0, 160.0f};
const GuiColor Black75{0, 0, 0, 192.0f};

const GuiColor White{255, 255, 255, 255};
const GuiColor White12{255, 255, 255, 32.0f};
const GuiColor White25{255, 255, 255, 64.0f};
const GuiColor White50{255, 255, 255, 128.0f};
const GuiColor White64{255, 255, 255, 160.0f};
const GuiColor White75{255, 255, 255, 192.0f};

const GuiColor Red{255, 0, 0, 255};
const GuiColor Red12{255, 0, 0, 32.0f};
const GuiColor Red25{255, 0, 0, 64.0f};
const GuiColor Red50{255, 0, 0, 128.0f};
const GuiColor Red64{255, 0, 0, 160.0f};
const GuiColor Red75{255, 0, 0, 192.0f};

const GuiColor Green{0, 255, 0, 255};
const GuiColor Green12{0, 255, 0, 32.0f};
const GuiColor Green25{0, 255, 0, 64.0f};
const GuiColor Green50{0, 255, 0, 128.0f};
const GuiColor Green64{0, 255, 0, 160.0f};
const GuiColor Green75{0, 255, 0, 192.0f};

const GuiColor Blue{0, 0, 255, 255};
const GuiColor Blue12{0, 0, 255, 32.0f};
const GuiColor Blue25{0, 0, 255, 64.0f};
const GuiColor Blue50{0, 0, 255, 128.0f};
const GuiColor Blue64{0, 0, 255, 160.0f};
const GuiColor Blue75{0, 0, 255, 192.0f};

const GuiColor GameRed{199, 51, 59, 128};
const GuiColor GameGreen{56, 109, 41, 128};
const GuiColor GameBlue{51, 93, 114, 128};
const GuiColor GameBlack{32, 36, 38, 128};
