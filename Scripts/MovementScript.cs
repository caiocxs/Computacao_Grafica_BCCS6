using UnityEngine;

public class MovementScript : MonoBehaviour
{
    public float speed;
    public Transform cameraPosition;
    public Vector3 distance = Vector3.zero;
    public Rigidbody rb;
    public float force;
    public Vector3 respawnPosition = Vector3.zero;
    public bool canJump = false;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        
        if (Input.GetKey(KeyCode.Space) && canJump)
        {
            rb.AddForce(new Vector3(0.0f, force, 0.0f));
            canJump = false;
        }

        if (Input.GetKey(KeyCode.W))
            transform.position += new Vector3(0.0f, 0.0f, speed);
        if (Input.GetKey(KeyCode.S))
            transform.position += new Vector3(0.0f, 0.0f, -speed);
        if (Input.GetKey(KeyCode.A))
            transform.position += new Vector3(-speed, 0.0f, 0.0f);
        if (Input.GetKey(KeyCode.D))
            transform.position += new Vector3(speed, 0.0f, 0.0f);

        cameraPosition.position = transform.position + distance;
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Safe")
        {
            canJump = true;
        }
        else if (collision.gameObject.tag == "Enemy")
        {
            var _gameObject = Instantiate(gameObject, respawnPosition, Quaternion.identity);
            Destroy(gameObject);

        }
    }
}
