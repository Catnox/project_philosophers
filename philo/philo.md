✅ Ces décalages sont NORMAUX
Pourquoi ces décalages existent :
usleep(1000) dans precise_sleep() :

Pause de 1ms à chaque vérification
Peut créer des décalages de 1-2ms
Concurrence des threads :

Les philosophes s'exécutent en parallèle
L'ordonnanceur du système peut créer de petits délais
Précision de get_current_time() :

Basé sur gettimeofday() avec précision microseconde
Mais les appels système ont une latence
Mutex et synchronisation :

Attente sur les mutex peut créer des micro-délais
Les pthread_mutex_lock/unlock ne sont pas instantanés
Ces décalages sont acceptables car :
✅ Conformes au sujet 42 : Le sujet demande des timestamps précis, pas une synchronisation parfaite au microseconde
✅ Fonctionnellement corrects : Les philosophes mangent, dorment, pensent dans l'ordre correct
✅ Pas de dérive importante : Les décalages ne s'accumulent pas
✅ Comportement réaliste : Dans un vrai système, ces micro-décalages sont inévitables

Pour minimiser (si souhaité) :
Tu pourrais réduire usleep(1000) à usleep(500) ou usleep(100), mais cela :

Consommerait plus de CPU
N'éliminerait pas complètement les décalages
N'est pas nécessaire pour le projet 42