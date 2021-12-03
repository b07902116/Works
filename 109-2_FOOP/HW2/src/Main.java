import java.util.HashMap;
import java.util.Map;

/**
 * The RPG game will be started from the Main class.
 */
public class Main {
    public static void main(String[] args) {
        RPG RPGflow = new RPG();
        Map<String, Action> actionmap = new HashMap<>();
        actionmap.put("Basic Attack", new BasicAttack());
        actionmap.put("Waterball", new Waterball());
        actionmap.put("Fireball", new Fireball());
        actionmap.put("SelfHealing", new SelfHealing());
        actionmap.put("Petrochemical", new Petrochemical());
        actionmap.put("Poison", new Poison());
        actionmap.put("Summon", new Summon());
        actionmap.put("SelfExplosion", new SelfExplosion());
        actionmap.put("Cheerup", new Cheerup());
        actionmap.put("Curse", new Curse());
        actionmap.put("OnePunch", new MyOnePunch());
        // can new other action here
        RPGflow.GameStart(actionmap);
    }
}
