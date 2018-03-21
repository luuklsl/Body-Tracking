using System.Collections;
using UnityEngine;
using System.IO.Ports;
using System;
using System.Collections.Generic;
using System.Globalization;

public class Movement : MonoBehaviour {

    // Use this for initialization
    Dictionary<char, BodyPart> bodyparts;
    public Arduino arduino;

    void Start () {
        Debug.Log("START");
        arduino = new Arduino("COM3", 9600, 1);
        arduino.read_until_close((KeyValuePair<char, Vector3> c) => mover(c), this);

        bodyparts = new Dictionary<char, BodyPart>() //this is being used for linking parts with the body
        {
            { 'a', new BodyPart("neck")},
            { 'b', new BodyPart("spine_02")},
            { 'c', new BodyPart("spine_01")}
        };

    }

    // Update is called once per frame

    void Update () {
        
        //hip.transform.Rotate(new Vector3(0,10,0));
    }

    public void mover(KeyValuePair<char, Vector3> data)
    {
        BodyPart bodyPart = bodyparts[data.Key];
        bodyPart.move(data.Value);
    }

    void OnApplicationQuit()
    {
        arduino.close();
    }
}

public class Arduino
{
    public SerialPort stream;
    private MonoBehaviour myMonoBehaviour;

    public Arduino(String port,int bautrate,int readTimeout)
    {
        Debug.Log("arduino start");
        stream = new SerialPort(port, bautrate);
        stream.ReadTimeout = readTimeout;
        stream.Open();
    }

    public void write(string message)
    {
        stream.WriteLine(message);
        stream.BaseStream.Flush();
    }
    private IEnumerator coroutine;

    public void read_until_close(Action<KeyValuePair<char, Vector3>> callback, MonoBehaviour myMonoBehaviour_)//(string s) => mover(s)
    {
        myMonoBehaviour = myMonoBehaviour_;
        coroutine = AsynchronousReadFromArduino(callback,() => Debug.LogError("Error!"));
        myMonoBehaviour.StartCoroutine(coroutine);
    }

    public void close()
    {
       myMonoBehaviour.StopCoroutine(coroutine);
       stream.Dispose();
    }


    public IEnumerator AsynchronousReadFromArduino(Action<KeyValuePair<char, Vector3>> callback, Action fail = null)
    {
        string value = "";
        do
        {
            int new_value;

            try
            {
                new_value = stream.ReadChar();     
            }
            catch (TimeoutException)
            {
                new_value = 0;
            }

            if (new_value != 0) {

                char new_value_char = (char)new_value;

                if (IsEnglishLetter(new_value_char))//if end of the command
                {
                    Vector3 axis;
                    try
                    {
                        string[] axis_string = value.Split(',');
                        axis = new Vector3(float.Parse(axis_string[0]), float.Parse(axis_string[1]), float.Parse(axis_string[2]));
                        KeyValuePair<char, Vector3> command = new KeyValuePair<char, Vector3>(new_value_char, axis);
                        callback(command);
                        Debug.Log("command" + command.ToString());
                    }
                    catch(FormatException e) //catch when string cannot be converted to float
                    {
                        Debug.Log("value" + value);
                        Debug.Log(e);
                    }

                    value = ""; //reset
                    yield return null;
                }
                else
                {
                    value += new_value_char; //if command is nog completed add to the rest
                }
            }
            else
            {
                yield return new WaitForSeconds(0.05f);
            }
        } while (true);
    }

    bool IsEnglishLetter(char c)
    {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }
}

public class BodyPart
{
    private char serial_id;
    private string gameobject_id;
    private GameObject bodypart;
    private Quaternion startState;
    public BodyPart(string gameObject_id)
    {
        bodypart = GameObject.Find(gameObject_id);
        startState = bodypart.transform.rotation;
    }

    public void move(Vector3 rotation)
    {
        bodypart.transform.rotation = startState;
        bodypart.transform.Rotate(rotation);
    }
}

