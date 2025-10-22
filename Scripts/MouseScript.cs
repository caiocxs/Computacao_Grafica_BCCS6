using UnityEngine;

public class MouseScript : MonoBehaviour
{
    public float sensitivity = 2.0f;
    public float verticalClamp = 45.0f;

    private float rotationX = 0.0f;
    private float rotationY = 0.0f;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        Cursor.lockState = CursorLockMode.Locked;
        Cursor.visible = false;
    }

    // Update is called once per frame
    void Update()
    {
        var mouseX = Input.GetAxis("Mouse X") * sensitivity;
        var mouseY = Input.GetAxis("Mouse Y") * sensitivity;

        rotationX -= mouseY;
        rotationX = Mathf.Clamp(rotationX, -verticalClamp, verticalClamp);

        rotationY += mouseX;
        transform.localRotation = Quaternion.Euler(rotationX, rotationY, 0.0f);
    }
}
