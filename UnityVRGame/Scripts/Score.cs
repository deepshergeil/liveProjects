using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Score : MonoBehaviour {

    private int score = 0;
    public int scoreLimit = 850;
    public GameObject GameEndScreen;

    public int getScore() { return score; }

    public void award(int amount) { score += amount;
        if(scoreLimit==score)
        {
            GameEndScreen.SetActive(true);
        }
    }
}
