using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemySpawner : MonoBehaviour {

    public GameObject toSpawnPrefab;

    public void SpawnEnemies(Transform parent)
    {
		Debug.Log ("Enemy spawned for: " + parent);
		GameObject spawned = Instantiate(toSpawnPrefab, transform.position, transform.rotation, parent);
        spawned.name = "ENEMY_" + gameObject.name;
    }

    void OnDrawGizmos()
    {
		Gizmos.color = Color.magenta;
		Gizmos.DrawWireSphere(gameObject.transform.position, 0.5F);
    }

}
