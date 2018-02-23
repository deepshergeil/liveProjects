using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TutorialTargetCollisionHandler : MonoBehaviour {

	private TutorialManager tutorialManager;

	// Use this for initialization
	void Start () {
		tutorialManager = GetComponentInParent<TutorialManager>();
	}

	void OnTriggerEnter(Collider other)
	{
		if (other.gameObject.GetComponent<Boomerang>()) {
			tutorialManager.NextStep();
		}
	}

	/*
	 * Modified from the GVR Teleport script
	 */
	public void TeleportRandomly() {
		Vector3 direction = Random.onUnitSphere * 3f;
		direction.y = Mathf.Clamp(direction.y, 0.5f, 1f);
		float distance = 2 * Random.value + 1.5f;
		transform.localPosition = direction * distance;

		float currentX = transform.localPosition.x;
		float currentY = transform.localPosition.y;
		float currentZ = transform.localPosition.z;
		const float minY = 3f;
		if (currentX >= -2f && currentX <= 2 && currentY < minY) {
			Vector3 position = new Vector3(currentX, minY, currentZ);
			transform.localPosition = position;
		}
  }
}
