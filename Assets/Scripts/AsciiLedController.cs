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

    // Executed each frame
    void Update()
    {
        //---------------------------------------------------------------------
        // Send data
        //---------------------------------------------------------------------

        // If you press one of these keys send it to the serial device. A
        // sample serial device that accepts this input is given in the README.
        if (Input.GetKeyDown(KeyCode.A))
        {
            Debug.Log("Sending A");
            serialController.SendSerialMessage("A");
        }

        if (Input.GetKeyDown(KeyCode.Z))
        {
            Debug.Log("Sending Z");
            serialController.SendSerialMessage("Z");
        }


        //---------------------------------------------------------------------
        // Receive data
        //---------------------------------------------------------------------

        string message = serialController.ReadSerialMessage();

        if (message == null)
            return;

        // Check if the message is plain data or a connect/disconnect event.
        if (ReferenceEquals(message, SerialController.SERIAL_DEVICE_CONNECTED))
            Debug.Log("Connection established");
        else if (ReferenceEquals(message, SerialController.SERIAL_DEVICE_DISCONNECTED))
            Debug.Log("Connection attempt failed or disconnection detected");
        else
            Debug.Log("Message arrived: " + message);
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
