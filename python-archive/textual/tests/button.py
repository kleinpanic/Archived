# app.py

from textual.app import App
from textual.widgets import Header, Footer, Button, Static

class QuickStart(App):
    CSS = """
    Button {
        margin: 1;
    }
    """

    def compose(self):
        yield Header()
        yield Button("Click Me", id="click_button")
        yield Static(id="output")
        yield Footer()

    def on_button_pressed(self, event: Button.Pressed):
        output = self.query_one("#output", Static)
        output.update("Button was clicked!")

if __name__ == "__main__":
    QuickStart().run()
