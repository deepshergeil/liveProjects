using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StoryManager : MonoBehaviour {
	int nextWave;
	GameObject player;
	PlayerMovement playerMovement;
	int numEnemies;
    int count = 0;
    private Client client = null;
    public GameObject GameEndScreen;

    void Start()
    {
        GameObject clientGO = GameObject.Find("Client");
        if (clientGO) client = clientGO.GetComponent<Client>();
    }

    void Awake(){
		player = GameObject.FindGameObjectWithTag ("Player");
		playerMovement = player.GetComponent<PlayerMovement> ();
		nextWave = 0;
	}

	public void NextWave(int wave){
		if(transform.childCount != 0){
			numEnemies = 0;
			Transform wavePoint = transform.GetChild (wave);
            if (wavePoint.gameObject.activeInHierarchy)
            {
                playerMovement.GotoPoint(wavePoint.position);
                foreach (Transform spawnPoint in wavePoint)
                {
                    if (spawnPoint.gameObject.activeInHierarchy)
                    {
                        spawnPoint.gameObject.GetComponent<EnemySpawner>().SpawnEnemies(transform);
                        numEnemies++;
                    }
                }
            }
            nextWave = (nextWave + 1) % transform.childCount;
        }
	}
	
	// Update is called once per frame
	void Update () {
        if (client == null || client.IsServer() || !client.isRunning())
        {
            if (numEnemies <= 0)
            {
                Debug.Log("Sending next wave!");
                if(client) client.Send("NEXTWAVE," + nextWave);
                NextWave(nextWave);
                count++;
              
                
            }
        }
	}

	public void DestroyEnemy(GameObject enemy){
		Destroy (enemy, 2f);
        numEnemies--;
	}
}
