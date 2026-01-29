# app.py

from textual.app import App
from textual.widgets import Header, Footer, Placeholder
from textual.layouts.grid import GridLayout

class QuickStart(App):
    def on_mount(self):
        grid = GridLayout(name="main_grid")
        self.view.dock(grid)
        grid.add_column("left", fraction=1)
        grid.add_column("right", fraction=1)
        grid.add_row("top", fraction=1)
        grid.add_row("bottom", fraction=1)
        grid.add_areas(
            area1="left,top",
            area2="right,top",
            area3="left,bottom",
            area4="right,bottom",
        )
        grid.place(
            area1=Placeholder(name="Area 1"),
            area2=Placeholder(name="Area 2"),
            area3=Placeholder(name="Area 3"),
            area4=Placeholder(name="Area 4"),
        )

if __name__ == "__main__":
    QuickStart().run()


