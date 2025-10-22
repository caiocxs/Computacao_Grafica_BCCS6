using UnityEngine;

public class SpawnEnemyScript : MonoBehaviour
{
    public float timeRoll = 1.5f;
    public GameObject _obj;
    public float currentTime = 0.0f;
    public float probability = 0.25f;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        currentTime += Time.deltaTime;

        if (currentTime >= timeRoll)
        {
            currentTime = 0.0f;

            if (Random.Range(0.0f, 1.0f) <= probability)
            {
                var _enemy = Instantiate(_obj);
                _enemy.transform.position = transform.position;
            }
        }
    }
}
