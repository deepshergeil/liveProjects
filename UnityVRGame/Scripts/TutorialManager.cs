using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class TutorialManager : MonoBehaviour {

	public GameObject firstLevelButton;

	private LinkedList<string> instructions;
	private LinkedListNode<string> progression;
	private Text instText;
	private TutorialTargetCollisionHandler target;
	private bool tutorialComplete;

	// Use this for initialization
	void Start () {
		instructions = InitTutorialInstructions();
		progression = instructions.First;

		instText = GetComponentInChildren<Text>();
		target = GetComponentInChildren<TutorialTargetCollisionHandler>();

		tutorialComplete = false;
		ToDoFirst();
	}

	private LinkedList<string> InitTutorialInstructions() {
		LinkedList<string> insts = new LinkedList<string>();
		string[] toDos = {
            "To aim: Hold down the button and a holographic axe will appear.\n\nTo shoot: Release the button and your axe will propel forward.", 
			"Good.\nNow find and hit 3 targets.\n0/3",
			"Great!\nNow find and hit 2 targets.\n1/3",
			"Awesome!!\nNow find and hit 1 more target.\n2/3",
			"Continue to practice your aim or hit the button on the floor to face some real action in the Viking village!"
		};
		for (int i = 0; i < toDos.Length; i ++) {
			insts.AddLast(toDos[i]);
		}
		return insts;
	}

	private void ToDoFirst() {
		UpdateInstructions(progression.Value);
		target.gameObject.SetActive(false);
		firstLevelButton.GetComponent<Button>().interactable = false;
	}

	void Update() {
		//Completed ToDoFirst
		if (instructions.First == progression && Input.GetMouseButtonUp(0)) {
			target.gameObject.SetActive(true);
			NextStep();
		}
	}

	public void NextStep() {
		if (tutorialComplete) {
			UpdateTarget();
		} else {
			progression = progression.Next;
			if (instructions.Last == progression) {
				ToDoLast();
			} else {
				UpdateInstructions(progression.Value);
				UpdateTarget();
			}
		}
	}

	private void ToDoLast() {
		UpdateInstructions(progression.Value);
		UpdateTarget();
		firstLevelButton.GetComponent<Button>().interactable = true;
		tutorialComplete = true;
	}

	private void UpdateInstructions(string toDo) {
		instText.text = toDo;
	}

	private void UpdateTarget() {
		target.TeleportRandomly();
	}

	public void MainMenuClicked() {
		Debug.Log("DEBUG: main menu clicked");
        SceneManager.LoadScene("_Scenes/Main Menu");
	}

	public void FirstLeveLClicked() {
		SceneManager.LoadScene("_Scenes/The Viking Village");
	}
}
