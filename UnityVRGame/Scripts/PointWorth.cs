using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PointWorth : MonoBehaviour {

    //Allows quick changes to the points in the editor
    //but do not want the points changing after being set and game starts
    public int points = 10;

    private int POINTS;

    void Start()
    {
        POINTS = points;
    }

    public int GetPoints()
    {
        return POINTS;
    }
}
