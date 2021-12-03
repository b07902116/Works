SP_programming2 B07902116 陳富春
1.Execution
執行方法和助教提供的相同
./bidding_system [host_num] [player_num]
./host [host_id] [random_key] [depth]
./player [player_id]

2.Description
在bidding_system和root_host之間是透過FIFO傳訊息, 利用fgets可以確定每次讀進來的剛好是一個root_host的回應, 而不會一次讀到多個root_host的回應
在root_host, child_host, leaf_host之間是透過pipe傳訊息, 建好四條pipes分別是parent write to child, parent read from child, child write to parent, child read from parent, 這樣用read就不會讓傳的訊息混在一起
在bidding_system中先依照host_num的數量fork出root_host, 並且都分配一次比賽下去, 確保沒有root_host閒置, 然後由回應的random value判斷是哪一個root_host並且立刻分配新的比賽下去, 等所有比賽都結束後, 依照host_num的數量獲得最後的回應並傳結束訊息給root_host然後wait, 確保沒有zombie process