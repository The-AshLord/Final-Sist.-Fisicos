using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class CubeAMod : MonoBehaviour
{
    public SerialController serialController;
    public GameObject cube;
    public Material material;

    // Initialization
    void Start()
    {
        serialController = GameObject.Find("SerialController").GetComponent<SerialController>();

        Debug.Log("Use los botones del arduino");
    }

    // Executed each frame
    void Update()
    {
 
        //---------------------------------------------------------------------
        // Receive data
        //---------------------------------------------------------------------

        string message = serialController.ReadSerialMessage();

        if (message == null)
            return;
        if (message == "Color")
        {
            RandomcolorMaterial();
            Debug.Log("Se Cambió el color");
        }

        if (message == "Size")
        {
            RamdomSizeCubeA();
            Debug.Log("Se Cambió el Size");
        }

        // Check if the message is plain data or a connect/disconnect event.
        if (ReferenceEquals(message, SerialController.SERIAL_DEVICE_CONNECTED))
            Debug.Log("Connection established");
        else if (ReferenceEquals(message, SerialController.SERIAL_DEVICE_DISCONNECTED))
            Debug.Log("Connection attempt failed or disconnection detected");
        else
            Debug.Log("Message arrived: " + message);
    }

    public void RandomcolorMaterial()
    {
        material.color = Random.ColorHSV();
    }

    public void RandomColorCube()
    {
        cube.GetComponent<Renderer>().material.color = Random.ColorHSV();
        // cube.GetComponent<Renderer>().materials[e).color - Randon.ColorHSv();
    }   
    
    public void RamdomSizeCubeA()
    {
        transform.localScale = new Vector3(Random.Range(0.2f, 5.0f),1,Random.Range(0.2f,5.0f));
    }

}
