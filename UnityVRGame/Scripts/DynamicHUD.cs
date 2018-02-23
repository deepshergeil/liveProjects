using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DynamicHUD : MonoBehaviour {

    enum CanvasTransparencyState { Hidden, FadeIn, Opaque, FadeOut};

    private CanvasTransparencyState myState = CanvasTransparencyState.Hidden;

    private CanvasRenderer[] renderers;

    public const float fadeInTime = 1.0F;
    public const float opaqueTime = 1.0F;
    public const float fadeOutTime = 1.0F;

    private float timeInCurrentState = 0.0F;

	void Start () {
        renderers = GetComponentsInChildren<CanvasRenderer>();
    }

    public void ShowHUD() {
        switch (myState) {
            case CanvasTransparencyState.Hidden:
                ChangeState(CanvasTransparencyState.FadeIn);
                break;
            case CanvasTransparencyState.Opaque:
                ChangeState(CanvasTransparencyState.Opaque);
                break;
            case CanvasTransparencyState.FadeOut:
                ChangeState(CanvasTransparencyState.Opaque);
                break;
        }
    }

    private void ChangeState(CanvasTransparencyState newState) {
        myState = newState;
        timeInCurrentState = 0.0F;
    }

    void Update() {
        timeInCurrentState += Time.deltaTime;
        float alpha = 0.0F;
        switch (myState)
        {
            case CanvasTransparencyState.Hidden:
                alpha = 0.0F;
                break;
            case CanvasTransparencyState.FadeIn:
                alpha = timeInCurrentState / fadeInTime;
                if (timeInCurrentState >= fadeInTime) ChangeState(CanvasTransparencyState.Opaque);
                break;
            case CanvasTransparencyState.Opaque:
                alpha = 1.0F;
                if (timeInCurrentState >= opaqueTime) ChangeState(CanvasTransparencyState.FadeOut);
                break;
            case CanvasTransparencyState.FadeOut:
                alpha = 1.0F - timeInCurrentState / fadeOutTime;
                if (timeInCurrentState >= fadeOutTime) ChangeState(CanvasTransparencyState.Hidden);
                break;
            default:
                Debug.LogError("This is an impossible state for DynamicHUD to be in!");
                break;
        }
        foreach (CanvasRenderer cr in renderers) cr.SetAlpha(alpha);
    }

}
