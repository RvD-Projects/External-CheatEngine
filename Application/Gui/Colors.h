#pragma once

typedef struct
{
    float R;
    float G;
    float B;
    float A;
} GuiColor;

const GuiColor Transparent{0, 0, 0, 0};

const GuiColor Black{0, 0, 0, 255};
const GuiColor Black12{0, 0, 0, 32.0};
const GuiColor Black25{0, 0, 0, 64.0};
const GuiColor Black50{0, 0, 0, 128.0};
const GuiColor Black64{0, 0, 0, 160.0};
const GuiColor Black75{0, 0, 0, 192.0};

const GuiColor White{255, 255, 255, 255};
const GuiColor White12{255, 255, 255, 32.0};
const GuiColor White25{255, 255, 255, 64.0};
const GuiColor White50{255, 255, 255, 128.0};
const GuiColor White64{255, 255, 255, 160.0};
const GuiColor White75{255, 255, 255, 192.0};

const GuiColor Red{255, 0, 0, 255};
const GuiColor Red12{255, 0, 0, 32.0};
const GuiColor Red25{255, 0, 0, 64.0};
const GuiColor Red50{255, 0, 0, 128.0};
const GuiColor Red64{255, 0, 0, 160.0};
const GuiColor Red75{255, 0, 0, 192.0};

const GuiColor Green{0, 255, 0, 255};
const GuiColor Green12{0, 255, 0, 32.0};
const GuiColor Green25{0, 255, 0, 64.0};
const GuiColor Green50{0, 255, 0, 128.0};
const GuiColor Green64{0, 255, 0, 160.0};
const GuiColor Green75{0, 255, 0, 192.0};

const GuiColor Blue{0, 0, 255, 255};
const GuiColor Blue12{0, 0, 255, 32.0};
const GuiColor Blue25{0, 0, 255, 64.0};
const GuiColor Blue50{0, 0, 255, 128.0};
const GuiColor Blue64{0, 0, 255, 160.0};
const GuiColor Blue75{0, 0, 255, 192.0};

const GuiColor Orange{255, 130, 30, 255};
const GuiColor Orange12{255, 130, 30, 32.0};
const GuiColor Orange25{255, 130, 30, 64.0};
const GuiColor Orange50{255, 130, 30, 128.0};
const GuiColor Orange64{255, 130, 30, 160.0};
const GuiColor Orange75{255, 130, 30, 192.0};

const GuiColor Yellow{250, 218, 80, 255};
const GuiColor Yellow12{250, 218, 80, 32.0};
const GuiColor Yellow25{250, 218, 80, 64.0};
const GuiColor Yellow50{250, 218, 80, 128.0};
const GuiColor Yellow64{250, 218, 80, 160.0};
const GuiColor Yellow75{250, 218, 80, 192.0};

const GuiColor GameRed{199, 51, 59, 128};
const GuiColor GameGreen{56, 109, 41, 128};
const GuiColor GameBlue{51, 93, 114, 128};
const GuiColor GameBlack{32, 36, 38, 128};
