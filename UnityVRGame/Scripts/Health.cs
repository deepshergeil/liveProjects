using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;

public class Health : MonoBehaviour {

    public int health = 15;

    public int getHealth() { return health; }
    public GameObject DeathScreen;
   // public void SetHealth(int inHealth) { health = inHealth; Damage(0); }

    public void Damage(int amount) {
        print(health);
        health -= amount;
        if(health==0)
        {
            Debug.Log("DIED");
            if (client == null || client.IsServer())
            {
                if (client) client.Send("DEATH");
            }
            DeathScreen.SetActive(true);
        
            StartCoroutine(routine());
    
        }
    }
    IEnumerator routine()
    {
        yield return new WaitForSeconds(2f);
        changeScene();
        
    }
    void changeScene()
    {
        string pathOfScene = "_Scenes/Main Menu";
        SceneManager.LoadScene(pathOfScene);
        SceneManager.SetActiveScene(SceneManager.GetSceneByName(pathOfScene));
    }

    public void NetworkedDeath() {
        DeathScreen.SetActive(true);

        StartCoroutine(routine());
    }

    private Client client = null;

    void Start()
    {
        GameObject clientGO = GameObject.Find("Client");
        if (clientGO) client = clientGO.GetComponent<Client>();
    }


}
