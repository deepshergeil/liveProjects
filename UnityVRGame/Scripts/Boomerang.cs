using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Boomerang : MonoBehaviour {

    private BoomerangThrower myThrower = null;
    private Vector3 myStartPosition;
    private Vector3 myEndPosition;

    private float mySpeedUnitsPerSecond;
    private float myRotationSpeedAnglePerSecond;
    private float myHangTimeSeconds;

    private float startTime;
    private bool isReturning = false;
    private bool isHanging = false;

    public Transform axe;

    private Score toAwardPoints;

    private AudioSource audio;
    public AudioClip throwClip;
    public AudioClip structureHitClip;
    public AudioClip enemyHitClip;

    private Client client = null;

    void Start()
    {
        GameObject clientGO = GameObject.Find("Client");
        if (clientGO) client = clientGO.GetComponent<Client>();
    }

    public void Throw(BoomerangThrower inThrower, Vector3 inEndPosition, float inSpeedUnitsPerSecond, float inRotationSpeedAnglePerSecond, float inHangTimeSeconds) {
        myThrower = inThrower;
        myStartPosition = transform.position;
        myEndPosition = inEndPosition;
        mySpeedUnitsPerSecond = inSpeedUnitsPerSecond;
        myRotationSpeedAnglePerSecond = inRotationSpeedAnglePerSecond;
        myHangTimeSeconds = inHangTimeSeconds;
        startTime = Time.time;
        toAwardPoints = inThrower.gameObject.transform.root.GetComponent<Score>();
        audio = GetComponent<AudioSource>();
        PlayAudio(throwClip);
    }

	void FixedUpdate () {
        axe.Rotate(Vector3.forward, myRotationSpeedAnglePerSecond * Time.fixedDeltaTime);
        if (isReturning) {
            if (TravelLerp(myEndPosition, myThrower.transform.position)) {
                audio.Stop();
                Destroy(gameObject);
            }
        }
        else if (isHanging) {
            if (Time.time - startTime >= myHangTimeSeconds) {
                ReturnToThrower();
            }
        }
        else {
            if (TravelLerp(myStartPosition, myEndPosition)) {
                isHanging = true;
                startTime = Time.time;
            }
        }
	}

    private bool TravelLerp(Vector3 fromPosition, Vector3 toPosition) {
        float distCovered = (Time.time - startTime) * mySpeedUnitsPerSecond;
        float fracJourney = distCovered / (Vector3.Distance(fromPosition, toPosition));
        transform.position = Vector3.Lerp(fromPosition, toPosition, fracJourney);
        return fracJourney >= 1.0F;
    }

    private void ReturnToThrower() {
        myEndPosition = transform.position;
        startTime = Time.time;
        isHanging = false;
        isReturning = true;
    }

    void OnTriggerEnter(Collider col) {
        bool notMyThrower = false;
        if (col.gameObject != myThrower.gameObject && !myThrower.gameObject.transform.IsChildOf(col.gameObject.transform))
        {
            if (col.gameObject.GetComponent<Boomerang>()) return;
            Debug.Log(col.gameObject.name);
            ReturnToThrower();
            notMyThrower = true;
        }

        

        PointWorth hitScore = col.GetComponent<PointWorth>();
		EnemyHealth enemyHealth = col.GetComponent<EnemyHealth> ();
		if (hitScore && enemyHealth) {
            if (client == null || !client.isRunning())
            {
                enemyHealth.Kill();
            } else if(client.IsServer())client.Send("KILL,"+enemyHealth.gameObject.name);
            toAwardPoints.award(hitScore.GetPoints());
            PlayAudio(enemyHitClip);
        } else { 
            if (notMyThrower) {
                PlayAudio(structureHitClip); 
            }
        }
    }

    private void PlayAudio(AudioClip clipToPlay) {
        if (audio.clip != clipToPlay) {
            audio.Stop();
            audio.clip = clipToPlay;
            audio.Play();
        } else {
            if (!audio.isPlaying) {
                audio.Play();
            }
        }
    }
}
