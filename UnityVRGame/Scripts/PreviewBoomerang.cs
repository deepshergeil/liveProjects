using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PreviewBoomerang : MonoBehaviour {

    private float myRotationSpeedAnglePerSecond;

    private float startTime;

    void Awake() {
        startTime = Time.fixedTime;
    }

    public float GetStartTime() {
        return startTime;
    }

    public void SetRotationSpeedAnglePerSecond(float inRotationSpeedAnglePerSecond) {
        myRotationSpeedAnglePerSecond = inRotationSpeedAnglePerSecond;
    }
    
    void FixedUpdate () {
        transform.Rotate(Vector3.up, myRotationSpeedAnglePerSecond * Time.fixedDeltaTime);
    }
}
