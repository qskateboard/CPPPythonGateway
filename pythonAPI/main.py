import api
import time


def present_hook():
    while 1:
        api.log('ok')
        players = api.get_players()
        for raw_player in players.split("&"):
            args = raw_player.split(";")
            if len(args) < 2:
                continue

            player = {
                "name": args[1],
                "pos": [float(i) for i in args[0].split(",")],
                }
            print(player)
        
        time.sleep(3)


if __name__ == "__main__":
    api.log("[+] enabled python script")
