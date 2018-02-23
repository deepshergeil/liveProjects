using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class HealthUI : MonoBehaviour {
    
    public List<Image> heartImages = new List<Image>();

    private DynamicHUD toAlert;
    private int prevHealth = -1;

    public Health toDisplay;

    void Start() {
        toAlert = GetComponent<DynamicHUD>();
    }

    void Update() {
        int health = toDisplay.getHealth();
        if (prevHealth != health) {
            UpdateHealth(health);
            prevHealth = health;
            if (toAlert) toAlert.ShowHUD();
        }
    }

    private void UpdateHealth(int health) {
        foreach (Image heartImage in heartImages) heartImage.gameObject.SetActive(false);
        for (int i = 0; i < health && i < toAlert.gameObject.transform.childCount; ++i)
        {
            heartImages[i].gameObject.SetActive(true);
        }
    }
}
