# Note TP2

## Exercice 1

**Question 1 - Dans ce schéma, qui est le client et qui est le serveur ?**

Dans ce schéma, le serveur est `tirage` et le client est `pi`. Car `pi` interroge `tirage` pour obtenir un nombre. 


## Exercice 2 - Chat 

**Pourquoi les sockets stream (par exemple, via TCP) sont mieux adaptées à ce type d’application ?** 

En mode socket stream, je suis connecté,il y une fiabilité de la connexion, si le message n'est pas reçu, le message sera redemandé. Cela permet également de savoir combien de clients sont connectés pour leur envoyer les messages. On peut également gérer les déconnexions.

**Testez l’application. Quel port utilise cette application pour communiquer (utilisez la commande ss et sa page de man pour le découvrir) ? Quelle option de ss avez-vous utilisé ?**

Avec la commende `ss -pant` pour afficher tout les ports en écoute. 
Le serveur écoute toutes les adresses sur le port 5015.

| State | Recv-Q | Send-Q | Local Address:Port | Peer Address:Port | Port
| :--- | :--- | :--- | :--- | :--- | :--- |
| LISTEN | 0 | 128 | 0.0.0.0:5015 | 0.0.0.0:* | users:(("chat_serveur",pid=6110,fd=5),("chat_serveur",pid=5531,fd=5)) |  
ESTAB  | 0  |0  |  127.0.0.1:5015 | 127.0.0.1:54050 |  users:(("chat_serveur",pid=6110,fd=6),("chat_serveur",pid=5531,fd=6))|
|  ESTAB   | 0 |0|127.0.0.1:54050 |127.0.0.1:5015 | users:(("chat_client",pid=6109,fd=3),("chat_client",pid=6108,fd=3))                      


**De combien de processus l’application serveur est-elle composée pour gérer 1, 2, n clients (utilisez la commande ps et sa page de man pour le découvrir) ?**
