# Note TP2

## Exercice 1

**Question 1 - Dans ce schéma, qui est le client et qui est le serveur ?**

Dans ce schéma, le serveur est `tirage` et le client est `pi`. Car `pi` interroge `tirage` pour obtenir un nombre. 


## Exercice 2 - Chat 

**Pourquoi les sockets stream (par exemple, via TCP) sont mieux adaptées à ce type d’application ?** 

En mode socket stream, je suis connecté,il y une fiabilité de la connexion, si le message n'est pas reçu, le message sera redemandé. Cela permet également de savoir combien de clients sont connectés pour leur envoyer les messages. On peut également gérer les déconnexions.