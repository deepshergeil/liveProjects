using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyChatter : MonoBehaviour {
	public float minSeconds = 0f;
	public float maxSeconds = 2f;
	public AudioClip chatterClip;
	public AudioClip vanquishedClip;

	private AudioSource audio;
	private EnemyHealth health;
	private bool audioHasBeenPlayed;
	private bool breathedLast;

	// Use this for initialization
	void Start () {
		audio = GetComponent<AudioSource>();
		health = GetComponent<EnemyHealth>();
		audio.clip = chatterClip;
		audio.playOnAwake = true;
		audioHasBeenPlayed = true;
		breathedLast = false;
	}
	
	// Update is called once per frame
	void Update () {
		if (!breathedLast) {
			if (health.IsDead()) { 
				LastBreath(); 
			} else { 
				Chatter(); 
			}
		}
	}

	void LastBreath() {
		audio.clip = vanquishedClip;
		audio.Play();
		breathedLast = true;
	}

	void Chatter() {
		if (audio.isPlaying) { audioHasBeenPlayed = true; }
		
		if (!audio.isPlaying && audioHasBeenPlayed) {
			float randomWait = Random.Range(minSeconds, maxSeconds);
			audio.PlayDelayed(randomWait);
			audioHasBeenPlayed = false;
		} //else { waiting for the delay, then audio will be played }
	}
}
