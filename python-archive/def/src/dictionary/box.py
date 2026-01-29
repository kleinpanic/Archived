import textwrap
import shutil

class Box:
    def __init__(self, word: str, defs_and_pos: list[tuple[str, str]], no_ansi: bool):
        self.word = word
        self.defs_and_pos = defs_and_pos

        def_widths, pos_widths = zip(*[[len(i), len(j)] 
            for i, j in self.defs_and_pos])
        self.inner_width = max(max(def_widths), max(pos_widths), len(self.word)) + 5 # in columns

        # box would be bigger than terminal
        self.cols = shutil.get_terminal_size().columns
        if self.inner_width > self.cols - 2:
            self.inner_width = self.cols - 2

        self.no_ansi = no_ansi


    def _draw_header(self) -> None:
        leftpad = (self.inner_width - len(self.word)) // 2
        rightpad = self.inner_width - len(self.word) - leftpad
        print('┌', '─' * self.inner_width, '┐', sep='')
        print('│', leftpad * ' ', self._bold(self.word), rightpad * ' ', '│', sep='')
        print('├', '─' * self.inner_width, '┤', sep='')


    def _draw_base(self) -> None:
        print('└', '─' * self.inner_width, '┘', sep='')


    def _draw_defs_and_pos(self) -> None:
        total_items = len(self.defs_and_pos)
        for i, (definition, pos) in enumerate(self.defs_and_pos, start=1):
            rightpad = (self.inner_width - len(pos) - 3 - len(str(i)))
            print('│ ', i, '. ', self._bold(pos), rightpad * ' ', '│', sep='')
            definition = textwrap.wrap(definition, width=self.inner_width - 6)

            for line in definition:
                leftpad = len(str(i)) + 3
                rightpad = self.inner_width - len(line) - leftpad
                print('│', leftpad * ' ', line, rightpad * ' ', '│', sep='')

            if i < total_items:
                print('│', self.inner_width * ' ', '│', sep='')


    def _bold(self, string):
        if self.no_ansi:
            return string
        return '\u001b[1m' + string + '\u001b[0m'

    
    def draw(self):
        self._draw_header()
        self._draw_defs_and_pos()
        self._draw_base()
