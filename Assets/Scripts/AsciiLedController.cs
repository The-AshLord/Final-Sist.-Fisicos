using UnityEngine;
using System.Collections;

public class AsciiLedController : MonoBehaviour
{
    public SerialController serialController;
    [SerializeField] bool IsAsciiLedOn = false;

    // Initialization
    void Start()
    {
        serialController = GameObject.Find("SerialController").GetComponent<SerialController>();

        Debug.Log("Press BottonA to execute some actions");
    }


    public void AsciiLedSwitch()
    {   
        
        if(IsAsciiLedOn == false)
        {
            IsAsciiLedOn = true;
            Debug.Log("Encendiendo el Led 2");
            serialController.SendSerialMessage("A");
        }
        else if (IsAsciiLedOn == true)
        {
            IsAsciiLedOn = false;
            Debug.Log("Apagando el Led 2");
            serialController.SendSerialMessage("Z");
        }
    
    }
}
