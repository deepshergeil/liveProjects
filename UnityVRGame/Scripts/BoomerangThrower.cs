using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoomerangThrower : MonoBehaviour {

    public float maxDistance = 50.0F;

    [Header("Preview Boomerang")]
    public PreviewBoomerang previewPrefab;
    public float previewRotationAnglePerSecond = 360.0F;
    public float previewDurationSeconds = 3.0F;

    [Header("Real Boomerang")]
    public Boomerang boomerangPrefab;
    public float rotationAnglePerSecond = 720.0f;
    public float speedUnitsPerSecond = 25.0F;
    public float hangTimeSeconds = 1.0F;

    private PreviewBoomerang previewBoomerang = null;
    private Boomerang boomerang = null;

    private Client client = null;

    void Start() {
        GameObject clientGO = GameObject.Find("Client");
        if (clientGO) client = clientGO.GetComponent<Client>();
    }
	
	void FixedUpdate () {
        if (Input.GetMouseButtonDown(0)) {
            if (!previewBoomerang && !boomerang) {
                previewBoomerang = Instantiate(previewPrefab, transform.position, transform.rotation, transform);
                previewBoomerang.SetRotationSpeedAnglePerSecond(previewRotationAnglePerSecond);
            }
        }
        else if (Input.GetMouseButtonUp(0)) {
            if(!client)Throw();
            else if (previewBoomerang && !boomerang)
            {
                Vector3 targetPosition = previewBoomerang.transform.position;
                client.Send("THROW," + targetPosition.x + "," + targetPosition.y + "," + targetPosition.z);
                Throw();
            }
        }
        if (previewBoomerang && !boomerang) {
            float previewTimeElapsed = (Time.fixedTime - previewBoomerang.GetStartTime());
            float fracJourney = previewTimeElapsed / previewDurationSeconds;
            previewBoomerang.transform.position = Vector3.Lerp(transform.position, transform.position + (transform.forward * maxDistance), fracJourney);
            if (fracJourney >= 1.0F) {
                if (!client) Throw();
                else if (previewBoomerang && !boomerang)
                {
                    Vector3 targetPosition = previewBoomerang.transform.position;
                    client.Send("THROW," + targetPosition.x + "," + targetPosition.y + "," + targetPosition.z);
                    Throw();
                }
            }
        }
	}

    public void NetworkThrow(Vector3 targetPosition) {
        Boomerang netboom = Instantiate(boomerangPrefab, transform.position, transform.rotation);
        netboom.Throw(this, targetPosition, speedUnitsPerSecond, rotationAnglePerSecond, hangTimeSeconds);
        netboom.transform.parent = null;
    }

    private void Throw() {
        if (previewBoomerang && !boomerang) {
            Vector3 targetPosition = previewBoomerang.transform.position;
            Destroy(previewBoomerang.gameObject);
            boomerang = Instantiate(boomerangPrefab, transform.position, transform.rotation);
            boomerang.Throw(this, targetPosition, speedUnitsPerSecond, rotationAnglePerSecond, hangTimeSeconds);
            boomerang.transform.parent = null;
        }
    }
}
