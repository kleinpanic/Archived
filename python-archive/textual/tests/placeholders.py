# app.py

from textual.app import App
from textual.widgets import Header, Footer, Placeholder

class QuickStart(App):
    def compose(self):
        yield Header()
        yield Placeholder()
        yield Footer()

if __name__ == "__main__":
    QuickStart().run()
