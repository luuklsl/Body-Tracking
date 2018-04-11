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
        string axis_string = ""; //string wherein the command is recreated
        do
        {
            int new_value; //values come in as INTs that represent characters in ascii. for every loop we try to retreive a new one

            try
            {
                new_value = stream.ReadChar();  //read int from the arduino   
            }
            catch (TimeoutException)
            {
                new_value = 0; //if the arduino does not send an int
            }

            if (new_value != 0) { //when a new character is send
                char new_axis_char = (char)new_value; //convert newly retreived int to a char

                if (IsEnglishLetter(new_axis_char))//if end of the command send to axis to the body part new_axis_char
                {
                    try
                    {
                        string[] axis_string_split = axis_string.Split(','); //split up the axis string to seperate angles XYZ

                        if(axis_string_split.Length == 3) //if the axis have the right amount of axis
                        {
                            Vector3 axis = new Vector3(float.Parse(axis_string_split[0]), float.Parse(axis_string_split[1]), float.Parse(axis_string_split[2])); //Parse string angles to a axis vector
                            KeyValuePair<char, Vector3> command = new KeyValuePair<char, Vector3>(new_axis_char, axis); //create dictinary entry for body part represented by the character in new_axis_char
                            callback(command);
                            Debug.Log("command" + command.ToString());
                        }

                    }
                    catch(FormatException e) //catch when string cannot be converted to float
                    {
                        Debug.Log("value" + axis_string);
                        Debug.Log(e);
                    }

                    axis_string = ""; //reset axis for retreiving a new one
                    yield return null;
                }
                else
                {
                    axis_string += new_axis_char; //if axis string is not completed add to the rest
                }
            }
            else //nothing is detected wait a little bit 1/60fps for next try
            {
                yield return new WaitForSeconds(0.05f);
            }
        } while (true);//run all the time
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

