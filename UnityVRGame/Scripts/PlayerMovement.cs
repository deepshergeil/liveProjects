using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class PlayerMovement : MonoBehaviour
{
    private NavMeshAgent agent;
    private GvrAudioSource audio1;

    private Client client = null;

    int toUpdate = 0;
    int triggerUpdate = 10;

    public void NetworkMove(Vector3 position) {
        agent.destination = position;
    }

    void Start()
    {
        GameObject clientGO = GameObject.Find("Client");
        if (clientGO) client = clientGO.GetComponent<Client>();

        agent = GetComponent<NavMeshAgent>();
        if ((audio1 == null) && (GetComponent<GvrAudioSource>() != null))
        {
            audio1 = GetComponent<GvrAudioSource>();
            audio1.loop = true;
            audio1.playOnAwake = false;
        }
    }

    void Update() {
        if (client == null || client.IsServer() || !client.isRunning())
        {
            if (ShouldStopMoving())
            {
                if (audio1 != null)
                {
                    if (audio1.isPlaying) { audio1.Stop(); }
                }
                else
                {
                    if (audio1 != null)
                        if (!audio1.isPlaying) { audio1.Play(); }
                }
            }
            toUpdate++;
            if (toUpdate == triggerUpdate)
            {
                toUpdate = 0;
                if (client) client.Send("POS," + transform.position.x + "," + transform.position.y + "," + transform.position.z);
            }
        }
    }


	public void GotoPoint(Vector3 position)
    {
        if (client == null || client.IsServer() || !client.isRunning())
        {
            if (ShouldStopMoving())
            {
                // Set the agent to go to the currently selected destination.
                agent.destination = position;
            }
        }
    }

    private bool ShouldStopMoving() {
        return agent && agent.remainingDistance < 0.5f;
    }
}
