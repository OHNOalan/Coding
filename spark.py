# real_spark_example.py
from pyspark.sql import SparkSession
from pyspark.sql.functions import broadcast

spark = SparkSession.builder \
    .appName("Concept Demo") \
    .master("local[2]") \
    .getOrCreate()

sc = spark.sparkContext

# === 1. Shuffle Example ===
rdd = sc.parallelize([("A", 1), ("B", 2), ("A", 3), ("C", 4)])
result = rdd.reduceByKey(lambda a, b: a + b).collect()
print("ReduceByKey result:", result)  # [('A', 4), ('C', 4), ('B', 2)]

# === 2. Broadcast Example ===
small_dict = {"US": "USA", "CN": "China"}
broadcast_var = sc.broadcast(small_dict)

large_rdd = sc.parallelize(["US", "CN", "FR"])
mapped = large_rdd.map(lambda code: (code, broadcast_var.value.get(code, "Unknown")))
print("Broadcast result:", mapped.collect())

# === 3. Data Skew —— hard to demo in small data, but you can observe in UI ===


input("👉 现在打开浏览器访问 http://localhost:4040 ，按回车退出...")

spark.stop()