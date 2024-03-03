from src.ui.ui_code import ConsoleUI
from dotenv import load_dotenv

if __name__ == "__main__":
    load_dotenv()
    ui = ConsoleUI()
    ui.run()
