
public class Deck {
    private int size;
    private Card[] cards;

    public Deck(){
        cards = new Card[52];
        this.size = 0;
    }
    public void Push(Card.SuitAndRank inSR){
        cards[size] = new Card(inSR);
        size++;
    }
    public Card Pull(){
        size--;
        return cards[size];
    }
    public void Distribute(Player[] players){
        for (int i = 0; i < 52; i++){
            players[i % 4].DealCard(this.Pull());
        }
    }

}
