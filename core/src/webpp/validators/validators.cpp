#include "../../../include/webpp/validators/validators.h"

#include "../../../include/webpp/utils/strings.h"

#include <string>
#include <string_view>

using namespace webpp;

bool is::name_color(std::string str) noexcept {
    // converting to lower case
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    static constexpr auto names = {"algae green",
                                   "aliceblue",
                                   "alien green",
                                   "antiquewhite",
                                   "aquamarine",
                                   "army brown",
                                   "ash gray",
                                   "avocado green",
                                   "aztech purple",
                                   "azure",
                                   "baby blue",
                                   "bashful pink",
                                   "basket ball orange",
                                   "battleship gray",
                                   "bean red",
                                   "bee yellow",
                                   "beer",
                                   "beetle green",
                                   "beige",
                                   "black",
                                   "black cat",
                                   "black cow",
                                   "black eel",
                                   "blanchedalmond",
                                   "blonde",
                                   "blood red",
                                   "blossom pink",
                                   "blue angel",
                                   "blue diamond",
                                   "blue dress",
                                   "blue eyes",
                                   "blue gray",
                                   "blue green",
                                   "blue hosta",
                                   "blue ivy",
                                   "blue jay",
                                   "blue koi",
                                   "blue lagoon",
                                   "blue lotus",
                                   "blue orchid",
                                   "blue ribbon",
                                   "blue whale",
                                   "blue zircon",
                                   "blueberry blue",
                                   "blush pink",
                                   "blush red",
                                   "brass",
                                   "bright gold",
                                   "bright neon pink",
                                   "bronze",
                                   "brown bear",
                                   "brown sugar",
                                   "bullet shell",
                                   "burgundy",
                                   "burlywood",
                                   "burnt pink",
                                   "butterfly blue",
                                   "cadillac pink",
                                   "camel brown",
                                   "camouflage green",
                                   "cantaloupe",
                                   "caramel",
                                   "carbon gray",
                                   "carnation pink",
                                   "celeste",
                                   "champagne",
                                   "charcoal",
                                   "chartreuse",
                                   "cherry red",
                                   "chestnut",
                                   "chestnut red",
                                   "chilli pepper",
                                   "chocolate",
                                   "cinnamon",
                                   "cloudy gray",
                                   "clover green",
                                   "cobalt blue",
                                   "coffee",
                                   "columbia blue",
                                   "construction cone orange",
                                   "cookie brown",
                                   "copper",
                                   "coral",
                                   "coral blue",
                                   "corn yellow",
                                   "cornflower blue",
                                   "cornsilk",
                                   "cotton candy",
                                   "cranberry",
                                   "cream",
                                   "crimson",
                                   "crocus purple",
                                   "crystal blue",
                                   "cyan opaque",
                                   "cyan or aqua",
                                   "dark carnation pink",
                                   "dark forest green",
                                   "dark goldenrod",
                                   "dark orange",
                                   "dark orchid",
                                   "dark salmon",
                                   "dark sea green",
                                   "dark slate blue",
                                   "dark slate grey",
                                   "dark turquoise",
                                   "dark violet",
                                   "day sky blue",
                                   "deep peach",
                                   "deep pink",
                                   "deep sky blue",
                                   "denim blue",
                                   "denim dark blue",
                                   "desert sand",
                                   "dimorphotheca magenta",
                                   "dodger blue",
                                   "dollar bill green",
                                   "dragon green",
                                   "dull purple",
                                   "earth blue",
                                   "eggplant",
                                   "electric blue",
                                   "emerald green",
                                   "fall leaf brown",
                                   "fern green",
                                   "ferrari red",
                                   "fire engine red",
                                   "firebrick",
                                   "flamingo pink",
                                   "forest green",
                                   "frog green",
                                   "ginger brown",
                                   "glacial blue ice",
                                   "golden brown",
                                   "goldenrod",
                                   "granite",
                                   "grape",
                                   "grapefruit",
                                   "gray",
                                   "gray cloud",
                                   "gray dolphin",
                                   "gray goose",
                                   "gray wolf",
                                   "grayish turquoise",
                                   "green",
                                   "green apple",
                                   "green onion",
                                   "green peas",
                                   "green snake",
                                   "green thumb",
                                   "green yellow",
                                   "greenish blue",
                                   "gunmetal",
                                   "halloween orange",
                                   "harvest gold",
                                   "hazel green",
                                   "heliotrope purple",
                                   "hot pink",
                                   "hummingbird green",
                                   "iceberg",
                                   "iguana green",
                                   "indigo",
                                   "iridium",
                                   "jade green",
                                   "jasmine purple",
                                   "jeans blue",
                                   "jellyfish",
                                   "jet gray",
                                   "jungle green",
                                   "kelly green",
                                   "khaki",
                                   "khaki rose",
                                   "lapis blue",
                                   "lava red",
                                   "lavender blue",
                                   "lavender pinocchio",
                                   "lawn green",
                                   "lemon chiffon",
                                   "light aquamarine",
                                   "light blue",
                                   "light coral",
                                   "light cyan",
                                   "light jade",
                                   "light pink",
                                   "light salmon",
                                   "light sea green",
                                   "light sky blue",
                                   "light slate",
                                   "light slate blue",
                                   "light slate gray",
                                   "light steel blue",
                                   "lilac",
                                   "lime green",
                                   "lipstick pink",
                                   "love red",
                                   "lovely purple",
                                   "macaroni and cheese",
                                   "macaw blue green",
                                   "magenta",
                                   "mahogany",
                                   "mango orange",
                                   "marble blue",
                                   "maroon",
                                   "mauve",
                                   "medium aquamarine",
                                   "medium forest green",
                                   "medium orchid",
                                   "medium purple",
                                   "medium sea green",
                                   "medium spring green",
                                   "medium turquoise",
                                   "medium violet red",
                                   "metallic silver",
                                   "midnight",
                                   "midnight blue",
                                   "milk white",
                                   "mint green",
                                   "mist blue",
                                   "misty rose",
                                   "moccasin",
                                   "mocha",
                                   "mustard",
                                   "navy blue",
                                   "nebula green",
                                   "neon pink",
                                   "night",
                                   "northern lights blue",
                                   "oak brown",
                                   "ocean blue",
                                   "oil",
                                   "orange gold",
                                   "orange salmon",
                                   "pale blue lily",
                                   "pale violet red",
                                   "papaya orange",
                                   "parchment",
                                   "pastel blue",
                                   "peach",
                                   "pearl",
                                   "periwinkle",
                                   "pig pink",
                                   "pine green",
                                   "pink",
                                   "pink bow",
                                   "pink bubble gum",
                                   "pink cupcake",
                                   "pink daisy",
                                   "pink lemonade",
                                   "pink rose",
                                   "pistachio green",
                                   "platinum",
                                   "plum",
                                   "plum pie",
                                   "plum purple",
                                   "plum velvet",
                                   "powder blue",
                                   "puce",
                                   "pumpkin orange",
                                   "purple",
                                   "purple amethyst",
                                   "purple daffodil",
                                   "purple dragon",
                                   "purple flower",
                                   "purple haze",
                                   "purple iris",
                                   "purple jam",
                                   "purple mimosa",
                                   "purple monster",
                                   "purple sage bush",
                                   "red",
                                   "red dirt",
                                   "red fox",
                                   "red wine",
                                   "robin egg blue",
                                   "rogue pink",
                                   "rose",
                                   "rose gold",
                                   "rosy brown",
                                   "rosy finch",
                                   "royal blue",
                                   "rubber ducky yellow",
                                   "ruby red",
                                   "rust",
                                   "saffron",
                                   "sage green",
                                   "salad green",
                                   "sand",
                                   "sandstone",
                                   "sandy brown",
                                   "sangria",
                                   "sapphire blue",
                                   "scarlet",
                                   "school bus yellow",
                                   "sea blue",
                                   "sea green",
                                   "sea turtle green",
                                   "seashell",
                                   "seaweed green",
                                   "sedona",
                                   "sepia",
                                   "shamrock green",
                                   "shocking orange",
                                   "sienna",
                                   "silk blue",
                                   "sky blue",
                                   "slate blue",
                                   "slate gray",
                                   "slime green",
                                   "smokey gray",
                                   "spring green",
                                   "steel blue",
                                   "stoplight go green",
                                   "sun yellow",
                                   "sunrise orange",
                                   "tan brown",
                                   "tangerine",
                                   "taupe",
                                   "tea green",
                                   "teal",
                                   "thistle",
                                   "tiffany blue",
                                   "tiger orange",
                                   "tron blue",
                                   "tulip pink",
                                   "turquoise",
                                   "tyrian purple",
                                   "valentine red",
                                   "vampire gray",
                                   "vanilla",
                                   "velvet maroon",
                                   "venom green",
                                   "viola purple",
                                   "violet",
                                   "violet red",
                                   "water",
                                   "watermelon pink",
                                   "white",
                                   "windows blue",
                                   "wisteria purple",
                                   "wood",
                                   "yellow",
                                   "yellow green",
                                   "zombie green"};

    auto it = std::lower_bound(std::cbegin(names), std::end(names), str,
                               [](const auto& l, const auto& r) {
                                   return l < r;
                               });
    return it != std::end(names) && *it == str;
}

bool is::rgb_color(std::string_view sstr) noexcept {
    // TODO: there are better ways to do it, check performance

    constexpr auto numbers = "0123456789";

    trim(sstr);
    if (!starts_with(sstr, "rgb(") || !starts_with(sstr, "RGB("))
        return false;
    sstr.remove_prefix(4);
    sstr.remove_suffix(1);
    rtrim(sstr);
    auto it = sstr.find_first_not_of(numbers);
    if (!is::uint8(sstr.substr(0, it)))
        return false;
    sstr.remove_suffix(it);
    ltrim(sstr);
    if (starts_with(sstr, ','))
        return false;
    sstr.remove_prefix(1);
    ltrim(sstr);
    it = sstr.find_first_not_of(numbers);
    if (!is::uint8(sstr.substr(0, it)))
        return false;
    sstr.remove_prefix(it);
    ltrim(sstr);
    if (!starts_with(sstr, ','))
        return false;
    sstr.remove_prefix(1);
    ltrim(sstr);
    it = sstr.find_first_not_of(numbers);
    if (!is::uint8(sstr.substr(0, it)))
        return false;
    sstr.remove_prefix(it);
    ltrim(sstr);
    return sstr.empty();
}

bool is::rgba_color(std::string_view str) noexcept {
    // TODO: there are better ways to do it, check performance

    constexpr auto numbers = "0123456789";

    return true; // TODO: I'm just gonna make it compilable
}

bool is::email(std::string_view const& str) noexcept {
    // TODO: Do not use regular expression, it's slow; use CTRE
    static const std::regex pattern{
      "^[_A-Za-z0-9-\\+]+(\\.[_A-Za-z0-9-]+)*@[A-Za-z0-9-]+(\\.[A-Za-"
      "z0-9]+)*(\\.[A-Za-z]{2,})$"};
    return std::regex_match(std::string(str), pattern);
}

bool is::color(std::string_view const& str) noexcept {
    return hex_color(str) || name_color(std::string(str)) || rgb_color(str) ||
           rgba_color(str) || hsl_color(str);
}
bool is::hsla_color(std::string_view str) noexcept {
    return hsl_color(str);
}

bool is::hsl_color(std::string_view str) noexcept {
    return true; // FIXME: implement this
}
