import os
import subprocess
import colorsys
from skimage.color import lab2rgb, rgb2lab, hsv2rgb, rgb2hsv


class Colors:# {{{
    def __init__(self, name):# {{{
        self.name = name
        self.type = None
        self.colors = {}
        self.hex = {}
    # }}}

    def __fromhsvl(self, hues, hueshifts, satshifts, vlshifts,# {{{
                   saturation=49, vl=60, white=None, black=None,
                   seven=None, eight=None):
        for i, (h, hs, ss, vls) in enumerate(
                zip(hues, hueshifts, satshifts, vlshifts)
        ):
            self.colors[i+1] = [h - hs/2, saturation - ss, vl - vls]
            self.colors[i+9] = [h + hs/2, saturation - ss, vl - vls]

        self.colors[0] = black if black else [0, 0, 0]
        self.colors[15] = white if white else [0, 0, vl]
        if seven is None or eight is None:
            self.defseveneight()
        if seven: self.colors[7] = seven
        if eight: self.colors[8] = eight
    # }}}

    def defseveneight(self, sevenlight=.5, eightlight=.10):# {{{
        self.colors[7] = [
            (1-sevenlight)*b + sevenlight*w
            for b, w in zip(self.colors[0], self.colors[15])
        ]
        self.colors[8] = [
            (1-eightlight)*b + eightlight*w
            for b, w in zip(self.colors[0], self.colors[15])
        ]
        return self
    # }}}

    def fromsibling(self, sibling):# {{{
        self.type = sibling.type
        self.colors.update(sibling.colors)
        return self
    # }}}

    def fromhsvshifts(self, hues, hueshifts, satshifts, valshifts,# {{{
                      saturation=49, value=60, white=None, black=None,
                      seven=None, eight=None):
        self.type = 'hsv'
        self.__fromhsvl(hues, hueshifts, satshifts, valshifts, saturation,
                        value, white, black, seven, eight)
        return self
    # }}}

    def fromhslshifts(self, hues, hueshifts, satshifts, lightshifts,# {{{
                      saturation=49, lightness=60, white=None, black=None,
                      seven=None, eight=None):
        self.type = 'hsl'
        self.__fromhsvl(hues, hueshifts, satshifts, lightshifts, saturation,
                        lightness, white, black, seven, eight)
        return self
    # }}}

    def fromhsv(self, colors):# {{{
        self.type = 'hsv'
        self.colors.update(colors)
        return self
    # }}}

    def fromhsl(self, colors):# {{{
        self.type = 'hsl'
        self.colors.update(colors)
        return self
    # }}}

    def fromrgb(self, colors):# {{{
        if isinstance(colors[0], str):
            return self.fromhex(colors)
        self.type = 'rgb'

        normalized = 1
        for rgb in colors.values():
            for c in rgb:
                if c > 1.0:
                    normalized = 0
                    break
            if not normalized:
                break

        if normalized:
            old = colors
            colors = {i: [c*255 for c in rgb] for i, rgb in old.items()}

        self.colors.update(colors)
        return self
    # }}}

    def fromhex(self, colors):# {{{
        self.type = 'hex'
        self.colors.update(colors)
        self.__intorgb()
        return self
    # }}}

    def __intorgbsingle(self, color, profile=None):# {{{
        if profile is None:
            profile = self.type

        if profile == 'rgb':
            if (len([c for c in color if c < 1.0]) == 3 and
                len([c for c in color if c > .001]) > 0):
                color = [c*255 for c in color]
        elif profile == 'hex':
            color = [int(h, 16) for h in [color[0:2], color[2:4], color[4:6]]]
        elif profile == 'hsl':
            rgb = colorsys.hls_to_rgb(color[0]/360, color[2]/100, color[1]/100)
            color = [c*255 for c in rgb]
        elif profile == 'hsv':
            rgb = hsv2rgb((color[0]/360, color[1]/100, color[2]/100))
            color = [c*255 for c in rgb]

        return [round(c) for c in color]
    # }}}

    def __intorgb(self):# {{{
        if self.type == 'rgb':
            return
        for k, val in self.colors.items():
            self.colors[k] = self.__intorgbsingle(val)
        self.type = 'rgb'
    # }}}

    def __defhex(self):# {{{
        self.__intorgb()
        for k, rgb in self.colors.items():
            self.hex[k] = ''.join([format(round(c), '02x') for c in rgb])
    # }}}

    def shiftelemto(self, i, color, weight=.5, profile='hex'):# {{{
        self.__intorgb()
        color = self.__intorgbsingle(color, profile)
        self.colors[i] = [weight*c + (1-weight)*o
                          for (c, o) in zip(color, self.colors[i])]
        return self
    # }}}

    def shiftallto(self, color, weight=.5, profile='hex',# {{{
                   skipcolors=(0, 7, 8)):
        self.__intorgb()
        color = self.__intorgbsingle(color, profile)
        for k, val in self.colors.items():
            if k in skipcolors:
                continue
            self.colors[k] = [weight*c + (1-weight)*o
                              for (c, o) in zip(color, val)]
        return self
    # }}}

    def setelem(self, i, color, profile='hex'):# {{{
        if profile == self.type:
            self.colors[i] = color
        else:
            self.__intorgb()
            self.colors[i] = self.__intorgbsingle(color, profile)
        return self
    # }}}

    def equalize(self, skipcolors=(0, 7, 8, 15)):# {{{
        self.__intorgb()
        lightness = rgb2lab([c/255 for c in self.colors[15]])[0]
        for k, val in self.colors.items():
            if k in skipcolors:
                continue
            L, a, b = rgb2lab([c/255 for c in val])
            rgb = lab2rgb((lightness, a, b))
            self.colors[k] = [c*255 for c in rgb]
        self.__defhex()
        return self
    # }}}

    def genalacrittyconfig(self):# {{{
        self.__defhex()

        filename = os.getenv('HOME')+'/.config/alacritty/'+self.name+'.yml'

        with open(filename, 'w') as f:
            print(
                'schemes:',
                '  {0}: &{0}'.format(self.name),
                '    primary:',
                '      background: \'#{}\''.format(self.hex[0]),
                '      foreground: \'#{}\''.format(self.hex[15]),
                '    normal:',
                '      black:      \'#{}\''.format(self.hex[0]),
                '      red:        \'#{}\''.format(self.hex[1]),
                '      green:      \'#{}\''.format(self.hex[2]),
                '      yellow:     \'#{}\''.format(self.hex[3]),
                '      blue:       \'#{}\''.format(self.hex[4]),
                '      magenta:    \'#{}\''.format(self.hex[5]),
                '      cyan:       \'#{}\''.format(self.hex[6]),
                '      white:      \'#{}\''.format(self.hex[7]),
                '    bright:',
                '      black:      \'#{}\''.format(self.hex[8]),
                '      red:        \'#{}\''.format(self.hex[9]),
                '      green:      \'#{}\''.format(self.hex[10]),
                '      yellow:     \'#{}\''.format(self.hex[11]),
                '      blue:       \'#{}\''.format(self.hex[12]),
                '      magenta:    \'#{}\''.format(self.hex[13]),
                '      cyan:       \'#{}\''.format(self.hex[14]),
                '      white:      \'#{}\''.format(self.hex[15]),
                'colors: *{}'.format(self.name),
                sep='\n', file=f
            )

        return self
    # }}}

    def genkonsoleconfig(self):# {{{
        self.__intorgb()

        datadir = os.getenv('XDG_DATA_HOME')
        if datadir is None:
            datadir = os.getenv('HOME')+'/.local/share'
        filename = datadir+'/konsole/'+self.name+'.colorscheme'

        fields = {}
        for i in range(8):
            fields[i] = [s.format(i) for s in ['Color{}', 'Color{}Faint']]
            fields[i+8] = ['Color{}Intense'.format(i)]
        fields[0] += ['Background'+s for s in ['', 'Faint', 'Intense']]
        fields[15] += ['Foreground'+s for s in ['', 'Faint', 'Intense']]

        with open(filename, 'w') as f:
            for i, l in fields.items():
                rgbstr = ','.join([str(round(c)) for c in self.colors[i]])
                for t in l:
                    print('[{}]\nColor={}\n'.format(t, rgbstr), file=f)
            print('[General]\nDescription={}\nOpacity=1'.format(self.name),
                  file=f)

        return self
    # }}}

    def gengnometerminalconfig(self, uuid=None):# {{{
        self.__intorgb()

        gtkrgb = {i: 'rgb({},{},{})'.format(*[round(c) for c in val])
                  for i, val in self.colors.items()}

        if uuid is None:
            uuid = subprocess.check_output('uuidgen', text=1).removesuffix('\n')
        fields = {'background-color': "'{}'".format(gtkrgb[0]),
                  'foreground-color': "'{}'".format(gtkrgb[15])}
        fields['palette'] = str([gtkrgb[i] for i in range(16)])
        fields['use-theme-colors'] = 'false'
        fields['visible-name'] = "'{}'".format(self.name)

        prefix = '/org/gnome/terminal/legacy/profiles:/:'+uuid

        for k, val in fields.items():
            os.system('dconf write {}/{} "{}"'.format(prefix, k, val))

        return self
    # }}}

    def genconfigs(self):# {{{
        self.genalacrittyconfig()
        self.genkonsoleconfig()
        #self.gengnometerminalconfig()
        return self
    # }}}
# }}}

grayscale = Colors('grayscale').fromhsl({
        0: [  0, 0,  6],   8: [  0, 0, 11],
        1: [  0, 0, 45],   9: [  0, 0, 47],
        2: [120, 0, 55],  10: [120, 0, 57],
        3: [ 60, 0, 50],  11: [ 60, 0, 52],
        4: [240, 0, 40],  12: [240, 0, 45],
        5: [300, 0, 58],  13: [300, 0, 59],
        6: [180, 0, 60],  14: [180, 0, 62],
        7: [  0, 0, 27],  15: [  0, 0, 55],
}).genconfigs()

gentledark = Colors('gentledark').fromhsvshifts(
        [360, 130,  30, 260, 330, 190],
        [ 10,  25,  15,  40,  20,  10],
        [-10,  10,  -5,  12,   0,   0],
        [  9,   0,   0,   0,   0,   0],
        black=[240, 20, 12],
        white=[  0,  0, 51],
        seven=[240, 21, 35],
        eight=[290, 29, 15],
        saturation=45,
        value=55
).equalize().genconfigs()

dracula = Colors('dracula').fromhex({
        0: '282A36',    8: '4D4D4D',
        1: 'FF5555',    9: 'FF6E67',
        2: '50FA7B',   10: '5AF78E',
        3: 'F1FA8C',   11: 'F4F99D',
        4: 'BD93F9',   12: 'CAA9FA',
        5: 'FF79C6',   13: 'FF92D0',
        6: '8BE9FD',   14: '9AEDFE',
        7: 'BFBFBF',   15: '888888',
}).shiftelemto(0, '000000', .5, 'hex').defseveneight(
        eightlight=.09
).shiftelemto(8, '440000', .09, 'hex') \
 .shiftelemto(15, '000000', .1, 'hex').equalize().genconfigs()

green = Colors('green').fromsibling(gentledark).shiftallto(
        color='44ff00', weight=.3, profile='hex'
).setelem(15, '44ff00', 'hex').shiftelemto(15, '000000', .3, 'hex') \
 .equalize().genconfigs()

# GREEN 44 ff 00

gentledark.gengnometerminalconfig(
        uuid='b1dcc9dd-5262-4d8d-a863-c897e6d979b9'
)
