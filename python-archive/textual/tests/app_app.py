# app.py

from textual.app import App
from textual.widgets import Header, Footer, Button, Static

class CounterApp(App):
    CSS = """
    Screen {
        align: center middle;
    }
    Button {
        margin: 1;
        width: 20;
    }
    Static#counter {
        margin: 1;
        padding: 1;
        border: round yellow;
    }
    """

    def compose(self):
        yield Header()
        yield Static("0", id="counter")
        yield Button("Increment", id="increment")
        yield Button("Decrement", id="decrement")
        yield Footer()

    def on_button_pressed(self, event: Button.Pressed):
        counter = self.query_one("#counter", Static)
        value = int(counter.renderable)
        if event.button.id == "increment":
            value += 1
        elif event.button.id == "decrement":
            value -= 1
        counter.update(str(value))

if __name__ == "__main__":
    CounterApp().run()

