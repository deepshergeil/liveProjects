using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class EnemyMovements : MonoBehaviour {
	Transform player;
	NavMeshAgent nav;
	Animator anim;
	EnemyHealth enemyHealth;

    private Client client = null;

    int toUpdate = 0;
    int triggerUpdate = 10;

    void Start()
    {
        GameObject clientGO = GameObject.Find("Client");
        if (clientGO) client = clientGO.GetComponent<Client>();
    }

    void Awake(){
		player = GameObject.FindGameObjectWithTag ("Player").transform;
		nav = GetComponent<NavMeshAgent> ();
		anim = GetComponent<Animator> ();
		enemyHealth = GetComponent<EnemyHealth> ();
	}

	// Update is called once per frame
	void Update () {
		if (!enemyHealth.IsDead()) {
			anim.SetBool ("IsMoving", true);
            if (client == null || client.IsServer() || !client.isRunning())
            {
                nav.SetDestination(player.position);
                toUpdate++;
                if (toUpdate == triggerUpdate)
                {
                    Vector3 pos = gameObject.transform.position;
                    if (client != null) client.Send("ENEMYPOS," + gameObject.name + "," + +pos.x + "," + pos.y + "," + pos.z);
                    toUpdate = 0;
                }
            }
            else {
                transform.LookAt(player.transform);
            }
		}
	}
}
