using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class NetworkSetup : MonoBehaviour {

    public Server server = null;
    public Client client = null;

    public Text ipText = null;
    public Text connectedText = null;
    public int multiplayerMainMenu = 2;
    public int multiplayerHostMenu = 4;

    MenuManager menuManager = null;
    
    void Start()
    {
        menuManager = GetComponent<MenuManager>();
        if (server && ipText) ipText.text = "Your IP: " + server.GetLocalIPAddress();
    }

    public void CancelNetworking()
    {
        server.ShutDown();
        client.ShutDown();
        menuManager.ToggleMenu(multiplayerMainMenu);
    }

    public void SelectHost()
    {
        menuManager.ToggleMenu(multiplayerHostMenu);
        Debug.Log("Server run!");
        server.Run();
    }

    void Update()
    {
        if (server && connectedText) connectedText.text = "Connected: " + server.NumConnected();
    }

    public void BeginGame()
    {
        if (server) server.Broadcast("Start Level _Scenes/The Viking Village");
    }
}
