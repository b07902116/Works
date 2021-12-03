import java.util.Arrays;
import java.util.Scanner;

public class IOhandler {
    private Scanner scanner;

    public IOhandler(){
        scanner = new Scanner(System.in);
    }

    public void ReadDeck(Deck deck){
        for (int i = 0; i < 52; i++){
            String in = scanner.next().replaceAll("[\\[\\]]", "");
            deck.Push(Card.SuitAndRank.valueOf(in));
        }
        scanner.nextLine(); // remove the first '\n'
    }
    public void ReadPlayerData(Player[] players){
        for (int i = 0; i < 4; i++){
            players[i] = new Player(scanner.nextLine());
        }
    }
    public int[] ReadInput(){
        String arr[] = scanner.nextLine().split("\\ ");
        int[] input = new int[arr.length];
        for (int i = 0; i < arr.length; i++){
            input[i] = Integer.parseInt(arr[i]);
        }
        Arrays.sort(input);
        return input;
    }

    public void PrintNewRound(){
        System.out.println("New round begins.");
    }
    public void PrintNextPlayer(Player player){
        System.out.printf("Next turn: %s\n", player.GetName());
    }
    public void PrintHandcard(Player player){
        StringBuilder numbers = new StringBuilder();
        StringBuilder cards = new StringBuilder();
        for (int i = 0; i < player.GetHandcardNum(); i++){
            String card = player.GetHandcard(i).GetOutputFormat();
            numbers.append(String.format("%"+(-card.length())+"s", i)).append(" ");
            cards.append(card).append(" ");
        }
        System.out.println(numbers.toString().stripTrailing());
        System.out.println(cards.toString().stripTrailing());

    }
    public void PrintInvalidPlay(){
        System.out.println("Invalid play, please try again.");
    }
    public void PrintValidPass(Player player){
        System.out.printf("Player %s passes.\n", player.GetName());
    }
    public void PrintInvalidPass(){
        System.out.println("You can't pass in the new round.");
    }
    public void PrintWinner(Player player){
        System.out.printf("Game over, the winner is %s.\n", player.GetName());
    }
}
