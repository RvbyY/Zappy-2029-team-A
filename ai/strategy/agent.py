class Agent:

    def __init__(self, client):
        self.client = client
    
    def run(self):
        print("Agent started...")

        inventory = self.client.command("Inventory")
        print("Inventory:", inventory)

        look = self.client.command("Look")
        print("Look:", look)