#version 450 core

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

// Original Font Data: 
//
// http://www.fial.com/~scott/tamsyn-font/ 
//
// Range 0x20 to 0x7f (inclusive)
//
// Every uvec4 holds the bitmap for one 8x16 bit character. 
//
const uvec4 font_data[96] = {

    { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
    { 0x00001010, 0x10101010, 0x00001010, 0x00000000 },
    { 0x00242424, 0x24000000, 0x00000000, 0x00000000 },
    { 0x00000024, 0x247E2424, 0x247E2424, 0x00000000 },
    { 0x00000808, 0x1E20201C, 0x02023C08, 0x08000000 },
    { 0x00000030, 0x494A3408, 0x16294906, 0x00000000 },
    { 0x00003048, 0x48483031, 0x49464639, 0x00000000 },
    { 0x00101010, 0x10000000, 0x00000000, 0x00000000 },
    { 0x00000408, 0x08101010, 0x10101008, 0x08040000 },
    { 0x00002010, 0x10080808, 0x08080810, 0x10200000 },
    { 0x00000000, 0x0024187E, 0x18240000, 0x00000000 },
    { 0x00000000, 0x0808087F, 0x08080800, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00001818, 0x08081000 },
    { 0x00000000, 0x0000007E, 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00001818, 0x00000000 },
    { 0x00000202, 0x04040808, 0x10102020, 0x40400000 },
    { 0x0000003C, 0x42464A52, 0x6242423C, 0x00000000 },
    { 0x00000008, 0x18280808, 0x0808083E, 0x00000000 },
    { 0x0000003C, 0x42020204, 0x0810207E, 0x00000000 },
    { 0x0000007E, 0x04081C02, 0x0202423C, 0x00000000 },
    { 0x00000004, 0x0C142444, 0x7E040404, 0x00000000 },
    { 0x0000007E, 0x40407C02, 0x0202423C, 0x00000000 },
    { 0x0000001C, 0x2040407C, 0x4242423C, 0x00000000 },
    { 0x0000007E, 0x02040408, 0x08101010, 0x00000000 },
    { 0x0000003C, 0x4242423C, 0x4242423C, 0x00000000 },
    { 0x0000003C, 0x4242423E, 0x02020438, 0x00000000 },
    { 0x00000000, 0x00181800, 0x00001818, 0x00000000 },
    { 0x00000000, 0x00181800, 0x00001818, 0x08081000 },
    { 0x00000004, 0x08102040, 0x20100804, 0x00000000 },
    { 0x00000000, 0x00007E00, 0x007E0000, 0x00000000 },
    { 0x00000020, 0x10080402, 0x04081020, 0x00000000 },
    { 0x00003C42, 0x02040810, 0x00001010, 0x00000000 },
    { 0x00001C22, 0x414F5151, 0x51534D40, 0x201F0000 },
    { 0x00000018, 0x24424242, 0x7E424242, 0x00000000 },
    { 0x0000007C, 0x4242427C, 0x4242427C, 0x00000000 },
    { 0x0000001E, 0x20404040, 0x4040201E, 0x00000000 },
    { 0x00000078, 0x44424242, 0x42424478, 0x00000000 },
    { 0x0000007E, 0x4040407C, 0x4040407E, 0x00000000 },
    { 0x0000007E, 0x4040407C, 0x40404040, 0x00000000 },
    { 0x0000001E, 0x20404046, 0x4242221E, 0x00000000 },
    { 0x00000042, 0x4242427E, 0x42424242, 0x00000000 },
    { 0x0000003E, 0x08080808, 0x0808083E, 0x00000000 },
    { 0x00000002, 0x02020202, 0x0242423C, 0x00000000 },
    { 0x00000042, 0x44485060, 0x50484442, 0x00000000 },
    { 0x00000040, 0x40404040, 0x4040407E, 0x00000000 },
    { 0x00000041, 0x63554949, 0x41414141, 0x00000000 },
    { 0x00000042, 0x62524A46, 0x42424242, 0x00000000 },
    { 0x0000003C, 0x42424242, 0x4242423C, 0x00000000 },
    { 0x0000007C, 0x4242427C, 0x40404040, 0x00000000 },
    { 0x0000003C, 0x42424242, 0x4242423C, 0x04020000 },
    { 0x0000007C, 0x4242427C, 0x48444242, 0x00000000 },
    { 0x0000003E, 0x40402018, 0x0402027C, 0x00000000 },
    { 0x0000007F, 0x08080808, 0x08080808, 0x00000000 },
    { 0x00000042, 0x42424242, 0x4242423C, 0x00000000 },
    { 0x00000042, 0x42424242, 0x24241818, 0x00000000 },
    { 0x00000041, 0x41414149, 0x49495563, 0x00000000 },
    { 0x00000041, 0x41221408, 0x14224141, 0x00000000 },
    { 0x00000041, 0x41221408, 0x08080808, 0x00000000 },
    { 0x0000007E, 0x04080810, 0x1020207E, 0x00000000 },
    { 0x00001E10, 0x10101010, 0x10101010, 0x101E0000 },
    { 0x00004040, 0x20201010, 0x08080404, 0x02020000 },
    { 0x00007808, 0x08080808, 0x08080808, 0x08780000 },
    { 0x00001028, 0x44000000, 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00000000, 0x00FF0000 },
    { 0x00201008, 0x04000000, 0x00000000, 0x00000000 },
    { 0x00000000, 0x003C0202, 0x3E42423E, 0x00000000 },
    { 0x00004040, 0x407C4242, 0x4242427C, 0x00000000 },
    { 0x00000000, 0x003C4240, 0x4040423C, 0x00000000 },
    { 0x00000202, 0x023E4242, 0x4242423E, 0x00000000 },
    { 0x00000000, 0x003C4242, 0x7E40403E, 0x00000000 },
    { 0x00000E10, 0x107E1010, 0x10101010, 0x00000000 },
    { 0x00000000, 0x003E4242, 0x4242423E, 0x02023C00 },
    { 0x00004040, 0x407C4242, 0x42424242, 0x00000000 },
    { 0x00000808, 0x00380808, 0x0808083E, 0x00000000 },
    { 0x00000404, 0x001C0404, 0x04040404, 0x04043800 },
    { 0x00004040, 0x40444850, 0x70484442, 0x00000000 },
    { 0x00003808, 0x08080808, 0x0808083E, 0x00000000 },
    { 0x00000000, 0x00774949, 0x49494949, 0x00000000 },
    { 0x00000000, 0x007C4242, 0x42424242, 0x00000000 },
    { 0x00000000, 0x003C4242, 0x4242423C, 0x00000000 },
    { 0x00000000, 0x007C4242, 0x4242427C, 0x40404000 },
    { 0x00000000, 0x003E4242, 0x4242423E, 0x02020200 },
    { 0x00000000, 0x002E3020, 0x20202020, 0x00000000 },
    { 0x00000000, 0x003E4020, 0x1804027C, 0x00000000 },
    { 0x00000010, 0x107E1010, 0x1010100E, 0x00000000 },
    { 0x00000000, 0x00424242, 0x4242423E, 0x00000000 },
    { 0x00000000, 0x00424242, 0x24241818, 0x00000000 },
    { 0x00000000, 0x00414141, 0x49495563, 0x00000000 },
    { 0x00000000, 0x00412214, 0x08142241, 0x00000000 },
    { 0x00000000, 0x00424242, 0x4242423E, 0x02023C00 },
    { 0x00000000, 0x007E0408, 0x1020407E, 0x00000000 },
    { 0x000E1010, 0x101010E0, 0x10101010, 0x100E0000 },
    { 0x00080808, 0x08080808, 0x08080808, 0x08080000 },
    { 0x00700808, 0x08080807, 0x08080808, 0x08700000 },
    { 0x00003149, 0x46000000, 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },


};

struct per_word_data {
    uint msg;
    vec4 fg_colour;
    vec4 bg_colour;
};

// inputs 
layout (location = 0) in vec2 inTexCoord;
layout (location = 1) flat in per_word_data inMsg; 

// outputs
layout (location = 0) out vec4 outFragColor;

void main(){

    // Every two triangles encode four characters
    // we do this for extra efficiency. 
        
    // A character bitmap is encoded in 16 bytes, 
    // each byte represents one line of the character bitmap.
    //
    // Every 4 lines are encoded in one uint; with 
    // 4 uints encoding the full character. 
    
    const uint MSG_LEN = 4;

    // map the current texture coordinate to (unscaled) integer pixel coordinates
    // within an grid of dimensions: [8 * MSG_LEN, 16]
    ivec2 char_coord = ivec2(floor(inTexCoord.xy * vec2(8 * MSG_LEN,16)));
    
    // Our message is encoded as one uint -- little endian
    //
    // Pick the char that we want to draw based on the 
    // current fragment's x coordinate.
    //
    uint char_code = inMsg.msg >> 8 * (char_coord.x/8) & 0xff;
    
    if (char_code == 0x00){
        // We use the null char to pad any words that are 
        // not complete...
        discard;
    } 
    
    char_code -= 0x20; // lowest char is space, 0x20 hex
    
    char_coord.x = char_coord.x % 8;

    // Pick the correct character bitmap, and then
    // the uint holding covering the four lines that 
    // our y pixel coordinate is in.
    uint four_lines = font_data[char_code][char_coord.y/4];
    
    // Now we must pick the correct line
    // Note 3- ... this is because big/little endian
    uint current_line  = (four_lines >> (8*(3-(char_coord.y)%4))) & 0xff;
    uint current_pixel = (current_line >> (7-char_coord.x)) & 0x01;
    
    // if the pixel is 0, choose background colour, if it is 1 choose 
    // foreground colour
    vec4 colour = mix(inMsg.bg_colour, inMsg.fg_colour, current_pixel);
    
    outFragColor = colour;

    // outFragColor = vec4(inTexCoord.xy,0,1);
}
