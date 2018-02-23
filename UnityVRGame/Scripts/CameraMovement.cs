using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour {

	public float turnSpeed = 4.0f;

	private Vector3 mouseOrigin;

	void Update () {
		if (Input.GetMouseButtonDown (0)) {
			mouseOrigin = Input.mousePosition;

			Vector3 pos = Camera.main.ScreenToViewportPoint (Input.mousePosition - mouseOrigin);

			transform.RotateAround (transform.position, Vector3.up, pos.x * turnSpeed);
			transform.RotateAround (transform.position, transform.right, -pos.y * turnSpeed);
		}
	}
}
