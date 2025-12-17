#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "renderers/clay_renderer_raylib.c"

// Colors and Font
#define BG_COLOR (Clay_Color) {50, 50, 50, 255}
#define HEADER_BG_COLOR (Clay_Color) {90, 90, 90, 255}
#define HEADER_BUTTON_COLOR (Clay_Color) {120, 120, 120, 255}
#define HEADER_BUTTON_COLOR_HOVER (Clay_Color) {110, 110, 110, 255}

const uint32_t FONT_ID_BODY_16 = 0;
const char FONT_PATH[] = "resources/Orbitron Light.ttf";
Font fonts[1];

Clay_Dimensions MeasureTextRaylib(
    Clay_StringSlice text,
    Clay_TextElementConfig *config,
    void *userData
) {
    Font* fonts = (Font*)userData;

    // Safety checks
    if (!text.chars || text.length == 0) return (Clay_Dimensions){0,0};
    if (config->fontId >= 2) return (Clay_Dimensions){0,0}; // adjust max fonts

    Font font = fonts[config->fontId];

    // Dynamically allocate buffer for string slice
    char* buffer = (char*)malloc(text.length + 1);
    if (!buffer) return (Clay_Dimensions){0,0};

    memcpy(buffer, text.chars, text.length);
    buffer[text.length] = '\0';

    Vector2 size = MeasureTextEx(font, buffer, (float)config->fontSize, 0);

    free(buffer);

    return (Clay_Dimensions){ .width = size.x, .height = size.y };
}



// Error Handler
void HandleClayErrors(Clay_ErrorData errorData) {
        printf("%s", errorData.errorText.chars);
}


// UI Elements

void buildHeaderButton(Clay_ElementId id, Clay_String text) {
        CLAY( id, {
                .layout = {
                        .sizing = {
                                .width = CLAY_SIZING_FIT(0),
                                .height = CLAY_SIZING_FIT(0)
                        },
                        .padding = CLAY_PADDING_ALL(5)
                },
                .cornerRadius = CLAY_CORNER_RADIUS(8),
                .backgroundColor = HEADER_BUTTON_COLOR
        }) {
                CLAY_TEXT( text, CLAY_TEXT_CONFIG({
                        .fontId = FONT_ID_BODY_16,
                        .fontSize = 24,
                        .textColor = (Clay_Color) {0, 0, 0, 255}
                }));
        }
}

void buildUI() {
        CLAY( CLAY_ID("OuterContainer"), {
                        .layout = {
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .sizing = {
                                        .width = CLAY_SIZING_GROW(0),
                                        .height = CLAY_SIZING_GROW(0)
                                },
                                .childGap = 16
                        },
                        .backgroundColor = BG_COLOR,
                }) {
                        CLAY( CLAY_ID("HeaderBar"), {
                                .layout = {
                                        .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                        .sizing = {
                                                .width = CLAY_SIZING_GROW(0),
                                                .height = CLAY_SIZING_FIT(0)
                                        },
                                        .padding = CLAY_PADDING_ALL(8),
                                        .childGap = 12
                                },
                                .backgroundColor = HEADER_BG_COLOR,
                                .border = {
                                        .width = {
                                                .bottom = 2
                                        },
                                        .color = (Clay_Color) {0, 0, 0, 255}
                                } 
                        }) {
                                buildHeaderButton(CLAY_ID("tabExplore"), CLAY_STRING("Explore"));
                                buildHeaderButton(CLAY_ID("tabBattle"), CLAY_STRING("Battle"));
                                buildHeaderButton(CLAY_ID("tabBuild"), CLAY_STRING("Build"));
                        }
                }
}






// Main Funct Call
void main() {
        Clay_Raylib_Initialize(600, 800, "Game_01", FLAG_WINDOW_TOPMOST);

        fonts[FONT_ID_BODY_16] = LoadFont(FONT_PATH);

        // Memory Alloc
        uint64_t clayRequiredMemory = Clay_MinMemorySize();
        Clay_Arena clayMemory = (Clay_Arena) {
                .memory = malloc(clayRequiredMemory),
                .capacity = clayRequiredMemory
        };

        // Window Creation
        Clay_Initialize(clayMemory, (Clay_Dimensions) {
                .width = GetScreenWidth(),
                .height = GetScreenHeight()
        }, (Clay_ErrorHandler) {HandleClayErrors});
        Clay_SetMeasureTextFunction(MeasureTextRaylib, fonts);

        // While Game is Running
        while (!WindowShouldClose()) {

                // Code here runs once per frame
                Clay_BeginLayout();

                // UI Code
                buildUI();

                Clay_RenderCommandArray renderCommands = Clay_EndLayout();

                // Render Call
                BeginDrawing();
                ClearBackground((Color) {50, 50, 50, 255});
                Clay_Raylib_Render(renderCommands, fonts);
                EndDrawing();
        }
}