# app.py

from textual.app import App
from textual.widgets import Header, Footer, Placeholder
from textual.layouts.grid import GridLayout

class QuickStart(App):
    def on_mount(self):
        grid = GridLayout()
        self.view.dock(grid)
        grid.add_column("col", fraction=1)
        grid.add_row("row", fraction=1)
        grid.add_areas(area="col,row")
        grid.place(area=Placeholder())

if __name__ == "__main__":
    QuickStart().run()
