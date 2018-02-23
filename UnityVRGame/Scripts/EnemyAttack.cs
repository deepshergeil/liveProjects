using UnityEngine;
using System.Collections;


public class EnemyAttack : MonoBehaviour
{
	public float timeBetweenAttacks = 0.5f;     // The time in seconds between each attack.
	public int attackDamage = 10;               // The amount of health taken away per attack.


	Animator anim;                              // Reference to the animator component.
	GameObject player;                          // Reference to the player GameObject.
	EnemyHealth enemyHealth;                    // Reference to this enemy's health.
	bool playerInRange;                         // Whether player is within the trigger collider and can be attacked.
	float timer;                                // Timer for counting up to the next attack.
	Health playerHealth;
	DamageAmount damage;

    private Client client = null;

    void Start()
    {
        GameObject clientGO = GameObject.Find("Client");
        if (clientGO) client = clientGO.GetComponent<Client>();
    }

    void Awake ()
	{
		// Setting up the references.
		player = GameObject.FindGameObjectWithTag ("Player");
		playerHealth = player.GetComponent<Health> ();
		damage = GetComponent<DamageAmount>();
		enemyHealth = GetComponent<EnemyHealth>();
		anim = GetComponent <Animator> ();
	}


	void OnTriggerEnter (Collider other)
	{
		// If the entering collider is the player...
		if(other.gameObject == player)
		{
			// ... the player is in range.
			playerInRange = true;
            //Handheld.Vibrate();
		}
	}


	void OnTriggerExit (Collider other)
	{
		// If the exiting collider is the player...
		if(other.gameObject == player)
		{
			// ... the player is no longer in range.
			playerInRange = false;
		}
	}


	void Update ()
	{
		// Add the time since Update was last called to the timer.
		timer += Time.deltaTime;

		// If the timer exceeds the time between attacks, the player is in range and this enemy is alive...
		if(timer >= timeBetweenAttacks && playerInRange && !enemyHealth.IsDead())
		{
			// ... attack.
			Attack ();
		}
	}


	void Attack ()
	{
		// Reset the timer.
		timer = 0f;
		transform.LookAt (player.transform);
		anim.SetTrigger ("Attack");
        if (client == null || client.IsServer() || !client.isRunning())
        {
            Debug.Log("SERVERONLY");
            playerHealth.Damage(damage.GetDamage());
            if(client) client.Send("HEALTH,"+playerHealth.getHealth());
        }
	}
}