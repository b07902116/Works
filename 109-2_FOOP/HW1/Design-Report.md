# Design Report


## Software Design

### 1. Main  
Main is the entry point of the program.  
It will new GameManager class and necessary PatternCard classes
and then call the GameStart method in GameManager.

### 2. GameManager
GameManager controls the game flow.   
 - #### In the initialization procedure  
   It calls IOhandler to read input deck 
   and player names and call Deck and Player classes to distribute the cards.  
 - #### After the game starts  
   It calls IOhandler and Player to translate the input to cards being played.  
   It calls IOhandler when there is output
   message.  
   It calls Player to release or sort their handcards and check
   winning condition.


### 3. Deck
Deck stores the input deck and then distribute the cards to players.  
It interacts with Player class when distribute the cards.

### 4. Player
Player keeps a player's data, including the name and hand cards.  
It interacts with GameManager when a player is trying to play cards.

### 5. IOhandler
IOhandler is responsible for most IO operations.  
It interacts with GameManager and Player classes.

### 6. Card
Card stores information about a single card.  
It interacts with Deck and Player to keep card information.

### 7. PatternCard
PatternCard is an abstract class that can be inherited by other classes
like Single, Pair, Fullhouse, Straight or other card pattern.  
It interacts with GameManager to check whether a play is valid.

### 8. Single / Pair / Fullhouse / Straight
These classes are valid card patterns a player can play.  
These classes inherit PatternCard and interacts with GameManager to check
whether a play is valid.


## Bonus Design: Open-Closed Principle (OCP)
In the Main class, main method will new all valid card patterns and then
pass them to GameManager. If there is a new card pattern to join, then
we only need to write the new card pattern class and modify the main
method. There is no need to modify other classes.