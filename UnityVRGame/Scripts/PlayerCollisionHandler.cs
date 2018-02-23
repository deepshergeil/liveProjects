using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerCollisionHandler : MonoBehaviour
{

    private Health playerHealth;

    void Start()
    {
        playerHealth = GetComponent<Health>();
    }

    void OnTriggerEnter(Collider col)
    {
        Debug.Log(col.gameObject.name);
        DamageAmount damage = col.gameObject.GetComponent<DamageAmount>();
        if (damage)
        {
            /*
			EnemyHealth enemyHealth = col.gameObject.GetComponent<EnemyHealth> ();
			enemyHealth.Kill ();
			*/
            //playerHealth.damage(damage.GetDamage());
        }
    }

    void OnDrawGizmos()
    {
        Gizmos.color = Color.green;
        Gizmos.DrawSphere(gameObject.transform.position + GetComponent<SphereCollider>().center, GetComponent<SphereCollider>().radius);
    }

}
