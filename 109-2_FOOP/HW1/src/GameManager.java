
public class GameManager {
    private Player[] players;
    private IOhandler IO;
    private int round_count, consecutive_pass, now_player_index;
    private int[] now_play, top_play;


    public GameManager(){
        Deck deck = new Deck();
        players = new Player[4];
        now_play = new int[2];
        top_play = new int[2];
        IO = new IOhandler();
        IO.ReadDeck(deck);
        IO.ReadPlayerData(players);
        deck.Distribute(players);
        for (int i = 0; i < 4; i++)
            players[i].SortCard();
        round_count = 0;
        consecutive_pass = 0;
    }

    public void GameStart(PatternCard... allpattern){
        int[] input;
        boolean valid;
        while (true){
            // new Round
            if (round_count == 0 || consecutive_pass == 3){
                IO.PrintNewRound();
                consecutive_pass = 0;
                // find the first player
                if (round_count == 0){
                    for (int i = 0; i < 4; i++){
                        if (players[i].GetHandcard(0).Get().ordinal() == 0){
                            now_player_index = i;
                            break;
                        }
                    }
                }
                // first player decide the card pattern
                IO.PrintNextPlayer(players[now_player_index]);
                IO.PrintHandcard(players[now_player_index]);
                valid = false;
                while (!valid){
                    input = IO.ReadInput();
                    for (PatternCard p : allpattern){
                        now_play = p.ValidCheck(players[now_player_index], input);
                        if (!PatternCard.IsInvalid(now_play[0]))
                            break;
                    }
                    if (PatternCard.IsInvalid(now_play[0])){
                        IO.PrintInvalidPlay();
                        IO.PrintHandcard(players[now_player_index]);
                    }
                    else if (PatternCard.IsPass(now_play[0])){
                        IO.PrintInvalidPass();
                        IO.PrintHandcard(players[now_player_index]);
                    }
                    else{
                        top_play = now_play;
                        for (PatternCard p : allpattern){
                            if (now_play[0] == p.GetType()){
                                p.PrintPlayMsg(players[now_player_index], input);
                                break;
                            }
                        }
                        players[now_player_index].ValidReleaseCard(input);
                        players[now_player_index].SortCard();
                        if (players[now_player_index].IsWin()){
                            IO.PrintWinner(players[now_player_index]);
                            return;
                        }
                        now_player_index = (now_player_index + 1) % 4;
                        round_count++;
                        valid = true;
                    }
                }
            }
            // other player simply check whether input is that pattern
            valid = false;
            IO.PrintNextPlayer(players[now_player_index]);
            IO.PrintHandcard(players[now_player_index]);
            while (!valid){
                input = IO.ReadInput();
                for (PatternCard p : allpattern){
                    now_play = p.ValidCheck(players[now_player_index], input);
                    if (!PatternCard.IsInvalid(now_play[0]))
                        break;
                }
                if (PatternCard.IsInvalid(now_play[0])){
                    IO.PrintInvalidPlay();
                    IO.PrintHandcard(players[now_player_index]);
                }
                else if (PatternCard.IsPass(now_play[0])){
                    IO.PrintValidPass(players[now_player_index]);
                    valid = true;
                    now_player_index = (now_player_index + 1) % 4;
                    consecutive_pass++;
                }
                else{
                    if (PatternCard.CanBeat(now_play, top_play)){
                        for (PatternCard p : allpattern){
                            if (now_play[0] == p.GetType()){
                                p.PrintPlayMsg(players[now_player_index], input);
                                break;
                            }
                        }
                        players[now_player_index].ValidReleaseCard(input);
                        players[now_player_index].SortCard();
                        if (players[now_player_index].IsWin()){
                            IO.PrintWinner(players[now_player_index]);
                            return;
                        }
                        valid = true;
                        now_player_index = (now_player_index + 1) % 4;
                        consecutive_pass = 0;
                        top_play = now_play;
                    }
                    else{
                        IO.PrintInvalidPlay();
                        IO.PrintHandcard(players[now_player_index]);
                    }
                }

            }
        }

    }
}
