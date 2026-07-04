#include <obs-module.h>
#include <util/platform.h>
#include <util/dstr.h>
#include <string>
#include <sstream>

#define PLUGIN_ELEMENT_ID "game_badge_source"
#define BROWSER_SOURCE_TARGET "Game Badge Source"

struct game_badge_plugin_context {
    obs_source_t *source;
    
    std::string api_key;
    std::string search_name;
    std::string custom_text;
    
    bool show_cover, show_title, show_platform, show_dev, show_pub, show_region, show_country, show_custom;
    int cover_x, cover_y, meta_x, meta_y, title_size, text_size;
    uint32_t text_color;
};

std::string get_hex_color(uint32_t obs_color) {
    int b = (obs_color >> 16) & 0xFF;
    int g = (obs_color >> 8) & 0xFF;
    int r = obs_color & 0xFF;
    char hex[8];
    snprintf(hex, sizeof(hex), "#%02x%02x%02x", r, g, b);
    return std::string(hex);
}

void push_css_updates(game_badge_plugin_context *ctx) {
    obs_source_t *browser_src = obs_get_source_by_name(BROWSER_SOURCE_TARGET);
    if (!browser_src) return;

    std::stringstream css;
    css << ":root {"
        << "  --cover-x: " << ctx->cover_x << "px; --cover-y: " << ctx->cover_y << "px;"
        << "  --meta-x: " << ctx->meta_x << "px; --meta-y: " << ctx->meta_y << "px;"
        << "  --title-size: " << ctx->title_size << "px; --text-size: " << ctx->text_size << "px;"
        << "  --text-color: " << get_hex_color(ctx->text_color) << ";"
        << "  --show-cover: " << (ctx->show_cover ? "block" : "none") << ";"
        << "  --show-title: " << (ctx->show_title ? "block" : "none") << ";"
        << "  --show-platform: " << (ctx->show_platform ? "block" : "none") << ";"
        << "  --show-dev: " << (ctx->show_dev ? "block" : "none") << ";"
        << "  --show-pub: " << (ctx->show_pub ? "block" : "none") << ";"
        << "  --show-region: " << (ctx->show_region ? "block" : "none") << ";"
        << "  --show-country: " << (ctx->show_country ? "block" : "none") << ";"
        << "  --show-custom: " << (ctx->show_custom ? "block" : "none") << ";"
        << "}";

    obs_data_t *settings = obs_source_get_settings(browser_src);
    obs_data_set_string(settings, "css", css.str().c_str());
    obs_source_update(browser_src, settings);
    
    obs_data_release(settings);
    obs_source_release(browser_src);
}

static bool on_search_clicked(obs_properties_t *props, obs_property_t *prop, void *data) {
    (void)props;
    (void)prop;
    game_badge_plugin_context *ctx = (game_badge_plugin_context *)data;
    blog(LOG_INFO, "[Game Badge] Initiating search for: %s", ctx->search_name.c_str());
    return true;
}

static const char *gb_get_name(void *unused) {
    (void)unused;
    return "Game Badge Controller";
}

static void gb_destroy(void *data) {
    delete (game_badge_plugin_context *)data;
}

static obs_properties_t *gb_get_properties(void *data) {
    (void)data;
    obs_properties_t *props = obs_properties_create();
    obs_properties_add_text(props, "api_key", "API Key", OBS_TEXT_PASSWORD);
    obs_properties_add_text(props, "search_name", "Game Name", OBS_TEXT_DEFAULT);
    obs_properties_add_button(props, "search_btn", "Search Game Data", on_search_clicked);
    obs_properties_add_text(props, "custom_text", "Custom Overlay Text", OBS_TEXT_DEFAULT);
    
    obs_properties_add_bool(props, "show_cover", "Show Cover Art");
    obs_properties_add_bool(props, "show_title", "Show Title");
    obs_properties_add_bool(props, "show_platform", "Show Platform");
    obs_properties_add_bool(props, "show_dev", "Show Developer");
    obs_properties_add_bool(props, "show_pub", "Show Publisher");
    obs_properties_add_bool(props, "show_region", "Show Region");
    obs_properties_add_bool(props, "show_country", "Show Country");
    obs_properties_add_bool(props, "show_custom", "Show Custom Text");

    obs_properties_add_int_slider(props, "cover_x", "Cover X", -500, 1920, 1);
    obs_properties_add_int_slider(props, "cover_y", "Cover Y", -500, 1080, 1);
    obs_properties_add_int_slider(props, "meta_x", "Text X", -500, 1920, 1);
    obs_properties_add_int_slider(props, "meta_y", "Text Y", -500, 1080, 1);
    obs_properties_add_int_slider(props, "title_size", "Title Size", 10, 100, 1);
    obs_properties_add_int_slider(props, "text_size", "Details Size", 10, 80, 1);
    obs_properties_add_color(props, "text_color", "Text Color");

    return props;
}

static void gb_update(void *data, obs_data_t *settings) {
    game_badge_plugin_context *ctx = (game_badge_plugin_context *)data;
    ctx->api_key = obs_data_get_string(settings, "api_key");
    ctx->search_name = obs_data_get_string(settings, "search_name");
    ctx->custom_text = obs_data_get_string(settings, "custom_text");
    ctx->show_cover = obs_data_get_bool(settings, "show_cover");
    ctx->show_title = obs_data_get_bool(settings, "show_title");
    ctx->show_platform = obs_data_get_bool(settings, "show_platform");
    ctx->show_dev = obs_data_get_bool(settings, "show_dev");
    ctx->show_pub = obs_data_get_bool(settings, "show_pub");
    ctx->show_region = obs_data_get_bool(settings, "show_region");
    ctx->show_country = obs_data_get_bool(settings, "show_country");
    ctx->show_custom = obs_data_get_bool(settings, "show_custom");
    ctx->cover_x = (int)obs_data_get_int(settings, "cover_x");
    ctx->cover_y = (int)obs_data_get_int(settings, "cover_y");
    ctx->meta_x = (int)obs_data_get_int(settings, "meta_x");
    ctx->meta_y = (int)obs_data_get_int(settings, "meta_y");
    ctx->title_size = (int)obs_data_get_int(settings, "title_size");
    ctx->text_size = (int)obs_data_get_int(settings, "text_size");
    ctx->text_color = (uint32_t)obs_data_get_int(settings, "text_color");
    push_css_updates(ctx);
}

static void *gb_create(obs_data_t *settings, obs_source_t *source) {
    game_badge_plugin_context *ctx = new game_badge_plugin_context();
    ctx->source = source;
    gb_update(ctx, settings);
    return ctx;
}

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("game-badge", "en-US")

bool obs_module_load(void) {
    obs_source_info info = {};
    info.id = PLUGIN_ELEMENT_ID;
    info.type = OBS_SOURCE_TYPE_INPUT;
    info.output_flags = OBS_SOURCE_VIDEO;
    info.get_name = gb_get_name;
    info.create = gb_create;
    info.destroy = gb_destroy;
    info.update = gb_update;
    info.get_properties = gb_get_properties;
    
    obs_register_source(&info);
    return true;
}