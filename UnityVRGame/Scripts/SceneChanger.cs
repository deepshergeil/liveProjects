using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class SceneChanger : MonoBehaviour {

    public string pathOfScene;

    private Button changeSceneButton;

	// Use this for initialization
	void Start () {
        changeSceneButton = GetComponent<Button>();
       changeSceneButton.onClick.AddListener(ChangeScene);
    }
    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.GetComponent<Boomerang>())
        {
            SceneManager.LoadScene(pathOfScene);
            SceneManager.SetActiveScene(SceneManager.GetSceneByName(pathOfScene));
            
        }
    }

    // Changes to the specified scene
    void ChangeScene () {
        SceneManager.LoadScene(pathOfScene);
        SceneManager.SetActiveScene(SceneManager.GetSceneByName(pathOfScene));
	} 
}
