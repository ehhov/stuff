# equalize Dracula!
# create a handler class...

import colorsys
import os
from skimage.color import lab2rgb, rgb2lab, hsv2rgb, rgb2hsv


def gencolors(hues, hueshifts, saturationshifts, vlshifts,# {{{
              saturation=49, vl=60, white=None, black=None,
              seven=None, eight=None, lightness=None, value=None):
    colors = {}

    if lightness is not None:
        vl = lightness
    elif value is not None:
        vl = value

    for i, (h, hs, ss, ls) in enumerate(zip(hues, hueshifts, saturationshifts, vlshifts)):
        colors[i+1] = [h - hs/2, saturation - ss, vl - ls]
        colors[i+9] = [h + hs/2, saturation - ss, vl - ls]

    colors[0] = black if black else [0, 0, 0]
    colors[7] = seven if seven else [0, 0, vl/2]
    colors[8] = eight if eight else [0, 0, vl/8]
    colors[15] = white if white else [0, 0, vl]

    return colors
# }}}


def equalize(colors, profile='hsl'):# {{{
    if profile == 'hsl':
        torgb = lambda hsl: colorsys.hls_to_rgb(hsl[0]/360, hsl[2]/100, hsl[1]/100)
        def fromrgb(rgb):
            h, l, s = colorsys.rgb_to_hls(*rgb)
            return h*360, s*100, l*100
    else:
        torgb = lambda hsv: hsv2rgb((hsv[0]/360, hsv[1]/100, hsv[2]/100))
        def fromrgb(rgb):
            h, s, v = rgb2hsv(rgb)
            return h*360, s*100, v*100

    newcolors = {}

    lightness = rgb2lab(torgb(colors[15]))[0]

    newcolors = {key: torgb(val) for key, val in colors.items()}
    newcolors = {key: rgb2lab(val) for key, val in newcolors.items()}
    newcolors = {key: [lightness, a, b] for key, (L, a, b) in newcolors.items()}
    newcolors = {key: lab2rgb(val) for key, val in newcolors.items()}
    newcolors = {key: fromrgb(val) for key, val in newcolors.items()}

    for i in [0, 7, 8, 15]:
        newcolors[i] = colors[i]

    return newcolors
# }}}


def genalacrittyconfig(colors, filename=None, name='bespoke', profile='hsl'):# {{{
    if profile == 'hsl':
        def tohex(hsl):
            rgb = colorsys.hls_to_rgb(hsl[0]/360, hsl[2]/100, hsl[1]/100)
            hexstr = ''.join([format(round(c*255), '02x') for c in rgb])
            return '\'#{}\'  # HSL: {:3.0f}, {:2.0f}, {:2.0f}'.format(hexstr, *hsl)
    else:
        def tohex(hsv):
            rgb = hsv2rgb((hsv[0]/360, hsv[1]/100, hsv[2]/100))
            hexstr = ''.join([format(round(c*255), '02x') for c in rgb])
            return '\'#{}\'  # HSV: {:3.0f}, {:2.0f}, {:2.0f}'.format(hexstr, *hsv)

    if filename is None:
        filename = os.getenv('HOME')+'/.config/alacritty/'+name+'.yml'

    def gen8(which, offset):
        lines = [which+':']
        for i, c in enumerate(['black', 'red', 'green', 'yellow',
                               'blue', 'magenta', 'cyan', 'white']):
            lines.append('  {:11s} {}'.format(c+':', tohex(colors[i+offset])))
        return lines

    lines = [
            'schemes:',
            '  {name}: &{name}'.format(name=name),
            '    primary:',
            '      background: {}'.format(tohex(colors[0])),
            '      foreground: {}'.format(tohex(colors[15])),
    ]
    lines += ['    '+line for line in gen8('normal', 0)]
    lines += ['    '+line for line in gen8('bright', 8)]
    lines.append('colors: *'+name)

    with open(filename, 'w') as f:
        print(*lines, sep='\n', file=f)
# }}}


# Zero and Fifteen are duplicates of background and foreground
# Seven and Eight are reserved for selection and cursorline in Vim
#genalacrittyconfig(
#    gencolors(
#        [  0, 130,  30, 260, 330, 190],
#        [ 10,  25,  15,  40,  20,  10],
#        [  9,  25,   5,  15,  12,  14],
#        [  5,   9,   5,   0,   0,  13],
#        black=[240, 20,  6],
#        white=[  0,  0, 51],
#        seven=[240,  5, 29],
#        eight=[290, 10, 10],
#        saturation=45,
#        lightness=60
#    ),
#    name='gentledark'
#)

#genalacrittyconfig(
#    colors={
#        0: [  0, 0,  6],   8: [  0, 0, 11],
#        1: [  0, 0, 45],   9: [  0, 0, 47],
#        2: [120, 0, 55],  10: [120, 0, 57],
#        3: [ 60, 0, 50],  11: [ 60, 0, 52],
#        4: [240, 0, 40],  12: [240, 0, 45],
#        5: [300, 0, 58],  13: [300, 0, 59],
#        6: [180, 0, 60],  14: [180, 0, 62],
#        7: [  0, 0, 27],  15: [  0, 0, 55],
#    },
#    name='grayscale'
#)

genalacrittyconfig(
    equalize(gencolors(
        [360, 130,  30, 260, 330, 190],
        [ 10,  25,  15,  40,  20,  10],
        [-10,  10,  -5,  12,   0,   0],
        [  9,   0,   0,   0,   0,   0],
        black=[240, 20,  9],
        white=[  0,  0, 51],
        seven=[240, 11, 35],
        eight=[290, 15, 13],
        saturation=45,
        value=55
    ), profile='hsv'),
    name='gentledark',
    profile='hsv'
)
