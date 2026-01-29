# stocks_info.py

from textual.widget import Widget
from textual.reactive import Reactive
import yfinance as yf


class StocksInfo(Widget):
    """Widget to display stock information."""

    data: Reactive[str] = Reactive("Loading stock info...")

    def on_mount(self) -> None:
        self.set_interval(60, self.refresh_data)
        self.refresh_data()

    def render(self) -> str:
        return self.data

    def refresh_data(self) -> None:
        companies = ['AAPL', 'GOOGL', 'MSFT']  # Replace with desired stock symbols
        info = ""
        for symbol in companies:
            stock = yf.Ticker(symbol)
            price = stock.info.get('regularMarketPrice')
            change = stock.info.get('regularMarketChangePercent')
            if price is not None and change is not None:
                info += f"{symbol}: ${price:.2f} ({change:+.2f}%)\n"
            else:
                info += f"{symbol}: Data not available\n"
        self.data = info.strip()
