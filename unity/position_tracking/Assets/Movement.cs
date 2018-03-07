using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//https://www.alanzucconi.com/2015/10/07/how-to-integrate-arduino-with-unity/
public class Movement : MonoBehaviour {

    // Use this for initialization
    public GameObject hip, upperarm_l,lowerarm_l, lowerarm_r;

    void Start () {
        hip = GameObject.Find("spine_01");
        upperarm_l = GameObject.Find("upperarm_l");
        lowerarm_l = GameObject.Find("lowerarm_l");
        lowerarm_r = GameObject.Find("lowerarm_r");
    }
	
	// Update is called once per frame
	void Update () {
        // upperarm_l.transform.Rotate(new Vector3(0, 10, 0));
        lowerarm_l.   .AddForce(Vector3.up * 0.1f, ForceMode.Acceleration);
        lowerarm_r.transform.Rotate(new Vector3(0, 0, 10));
        //hip.transform.Rotate(new Vector3(0,10,0));
    }
}
