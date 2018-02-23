using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MenuManager : MonoBehaviour {
    public CanvasGroup []menus;
    public int currentMenu = 0;

    private void Awake()
    {
        SetCanvasGroupEnabled(menus[currentMenu], true);
    }

    public void ChangeScene(string scenePath)
    {
        SceneManager.LoadScene(scenePath);
        SceneManager.SetActiveScene(SceneManager.GetSceneByName(scenePath));
    }

    public void ToggleMenu(int newMenu)
    {         
        SetCanvasGroupEnabled(menus[currentMenu], false);
        SetCanvasGroupEnabled(menus[newMenu],true);
        currentMenu = newMenu;
    }

    private void SetCanvasGroupEnabled(CanvasGroup canvasGroup, bool enabled)
    {
        canvasGroup.alpha = enabled ? 1 : 0;
        canvasGroup.interactable = enabled;
        canvasGroup.blocksRaycasts = enabled;
    }
}
