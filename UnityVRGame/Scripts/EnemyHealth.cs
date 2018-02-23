using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class EnemyHealth : MonoBehaviour {
	public float sinkSpeed = 2.5f;

	Animator anim;
	CapsuleCollider capsuleCollider;
	bool isDead;
	bool isSinking;
	StoryManager storyManager;

	void Awake(){
		anim = GetComponent<Animator> ();
		capsuleCollider = GetComponent<CapsuleCollider> ();
		storyManager = transform.parent.GetComponent<StoryManager> ();
	}
		
	// Update is called once per frame
	void Update () {
		if (isSinking) {
			transform.Translate (-Vector3.up * sinkSpeed * Time.deltaTime);
		}
	}

	public void Kill(){
		if (!isDead) {
			Debug.Log ("Enemy being killed.");
			isDead = true;
			capsuleCollider.isTrigger = true;
			anim.SetTrigger("Die");
		}
	}

	public void StartSinking(){
		Debug.Log("Started Sinking.");
		GetComponent<NavMeshAgent> ().enabled = false;
		GetComponent<Rigidbody> ().isKinematic = true;
		isSinking = true;
		storyManager.DestroyEnemy(gameObject);
	}

	public bool IsDead(){
		return isDead;
	}
}
