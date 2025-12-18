#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "renderers/clay_renderer_raylib.c"

// Colors and Font
#define CLAY_BLACK (Clay_Color) {0, 0, 0, 255}
#define BG_COLOR (Clay_Color) {50, 50, 42, 255}
#define CONTENT_BOX_BG_COLOR (Clay_Color) {80, 80, 80, 255}
#define HEADER_BG_COLOR (Clay_Color) {78, 78, 70, 255}
#define HEADER_BUTTON_COLOR (Clay_Color) {155, 155, 145, 255}
#define HEADER_BUTTON_COLOR_HOVER (Clay_Color) {120, 120, 110, 255}

#define RAYLIB_VECTOR2_TO_CLAY_VECTOR2(vector) (Clay_Vector2) { .x = vector.x, .y = vector.y }

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

// Mouse Handling

// UI

void buildHeaderButton(Clay_ElementId id, Clay_String text) {
        CLAY( id, {
                .layout = {
                        .sizing = {
                                .width = CLAY_SIZING_FIT(0),
                                .height = CLAY_SIZING_FIT(0)
                        },
                        .padding = CLAY_PADDING_ALL(8)
                },
                .border = {
                        .width = {
                                .top = 1,
                                .bottom = 1,
                                .left = 1,
                                .right = 1
                        },
                        .color = CLAY_BLACK
                },
                .cornerRadius = CLAY_CORNER_RADIUS(8),
                .backgroundColor = Clay_Hovered() ? HEADER_BUTTON_COLOR_HOVER : HEADER_BUTTON_COLOR
        }) {
                CLAY_TEXT( text, CLAY_TEXT_CONFIG({
                        .fontId = FONT_ID_BODY_16,
                        .fontSize = 22,
                        .textColor = CLAY_BLACK
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
                                .padding = CLAY_PADDING_ALL(8),
                                .childGap = 4
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
                                        .childGap = 6,
                                        .childAlignment = CLAY_ALIGN_X_LEFT
                                },
                                .backgroundColor = HEADER_BG_COLOR,
                                .cornerRadius = CLAY_CORNER_RADIUS(15),
                                .border = {
                                        .width = {
                                                .bottom = 2,
                                                .top = 2,
                                                .right = 2,
                                                .left = 2
                                        },
                                        .color = (Clay_Color) {0, 0, 0, 255}
                                } 
                        }) {
                                buildHeaderButton(CLAY_ID("tabExplore"), CLAY_STRING("Explore"));
                                buildHeaderButton(CLAY_ID("tabBattle"), CLAY_STRING("Battle"));
                                buildHeaderButton(CLAY_ID("tabBuild"), CLAY_STRING("Build"));
                        }

                        CLAY( CLAY_ID("BottomContainer"), {
                                .layout = {
                                        .sizing = {
                                                .width = CLAY_SIZING_GROW(0),
                                                .height = CLAY_SIZING_GROW(0)
                                        },
                                        .padding = CLAY_PADDING_ALL(0)
                                },
                                .backgroundColor = (Clay_Color) {22, 30, 60, 0}
                        }) {
                                CLAY( CLAY_ID("Content"), {
                                        .layout = {
                                                .sizing = {
                                                        .width = CLAY_SIZING_GROW(0),
                                                        .height = CLAY_SIZING_GROW(0)
                                                },
                                                .padding = CLAY_PADDING_ALL(8),
                                                .childGap = 8
                                        },
                                        .backgroundColor = HEADER_BG_COLOR,
                                        .cornerRadius = CLAY_CORNER_RADIUS(15),
                                        .border = {
                                                .width = {
                                                        .left = 2,
                                                        .right = 2,
                                                        .top = 2,
                                                        .bottom = 2
                                                },
                                                .color = CLAY_BLACK
                                        }
                                }) {}
                        }
                }
}






// Main Funct Call
void main() {
        Clay_Raylib_Initialize(600, 800, "Game_01", FLAG_WINDOW_TOPMOST | FLAG_WINDOW_RESIZABLE);
        SetTargetFPS(30);

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

                Clay_SetLayoutDimensions((Clay_Dimensions) {.width = GetScreenWidth(), .height = GetScreenHeight()});

                // Update Mouse Pointer
                Clay_SetPointerState(
                        RAYLIB_VECTOR2_TO_CLAY_VECTOR2(GetMousePosition()),
                        IsMouseButtonDown(MOUSE_LEFT_BUTTON)
                );

                // UI Layout Declaration
                Clay_BeginLayout();
                buildUI();
                Clay_RenderCommandArray renderCommands = Clay_EndLayout();

                // Interaction Logic

                // Render Call
                BeginDrawing();
                ClearBackground((Color) {50, 50, 50, 255});
                Clay_Raylib_Render(renderCommands, fonts);
                EndDrawing();
        }
}