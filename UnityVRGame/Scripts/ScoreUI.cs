using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent (typeof(Text))]
public class ScoreUI : MonoBehaviour {

    public Score toDisplay;

    private DynamicHUD toAlert;
    private Text textField;

    private int prevScore = -1;

    void Start() {
        textField = GetComponent<Text>();
        toAlert = GetComponent<DynamicHUD>();
    }
	
	// Update is called once per frame
	void Update () {
        if (toDisplay) {
            int score = toDisplay.getScore();
            if (prevScore != score) {
                UpdateDisplay(score);
                prevScore = score;
                if (toAlert) toAlert.ShowHUD();
            }
        }
	}

    private void UpdateDisplay(int score) {
        string zeros = "";
        if (score < 10) zeros = "000";
        else if (score < 100) zeros = "00";
        else if (score < 1000) zeros = "0";
        textField.text = "Score: " + zeros + score;
    }

}
