window.BENCHMARK_DATA = {
  "lastUpdate": 1676981883510,
  "repoUrl": "https://github.com/feliwir/Thyme",
  "entries": {
    "Thyme Benchmarks": [
      {
        "commit": {
          "author": {
            "email": "vedder@mbits.info",
            "name": "Stephan Vedder",
            "username": "feliwir"
          },
          "committer": {
            "email": "vedder@mbits.info",
            "name": "Stephan Vedder",
            "username": "feliwir"
          },
          "distinct": true,
          "id": "5cdd92f86f8b8c06c4880d7da4958483de5c10bd",
          "message": "Add CI step to run performance regression test",
          "timestamp": "2023-02-20T16:08:57+01:00",
          "tree_id": "e9d7e8ead7672b66663c42ca205f21d4decc35e3",
          "url": "https://github.com/feliwir/Thyme/commit/5cdd92f86f8b8c06c4880d7da4958483de5c10bd"
        },
        "date": 1676981882835,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "RefPack_Compress",
            "value": 27560963.079995416,
            "unit": "ns/iter",
            "extra": "iterations: 25\ncpu: 27533924.000000015 ns\nthreads: 1"
          },
          {
            "name": "RefPack_Uncompress",
            "value": 86534.5663803691,
            "unit": "ns/iter",
            "extra": "iterations: 8150\ncpu: 85953.44785276073 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Fast_To_Int_Floor",
            "value": 699.5908979174123,
            "unit": "ns/iter",
            "extra": "iterations: 997927\ncpu: 701.144672906922 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Fast_To_Int_Ceil",
            "value": 697.266037097969,
            "unit": "ns/iter",
            "extra": "iterations: 999432\ncpu: 700.2896645299147 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Fast_To_Int_Truncate",
            "value": 699.2736467364016,
            "unit": "ns/iter",
            "extra": "iterations: 995243\ncpu: 701.6490445045629 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Floor",
            "value": 695.9817323417176,
            "unit": "ns/iter",
            "extra": "iterations: 1002865\ncpu: 697.8027949922088 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Ceil",
            "value": 697.3849982685324,
            "unit": "ns/iter",
            "extra": "iterations: 1003137\ncpu: 699.333889587883 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Cos",
            "value": 697.7839354114076,
            "unit": "ns/iter",
            "extra": "iterations: 940293\ncpu: 699.9398059955338 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Sin",
            "value": 696.5576063493864,
            "unit": "ns/iter",
            "extra": "iterations: 1001617\ncpu: 698.7398376826491 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Fast_To_Int_Floor",
            "value": 890.9732833752672,
            "unit": "ns/iter",
            "extra": "iterations: 785841\ncpu: 893.5041566930271 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Fast_To_Int_Ceil",
            "value": 892.27590559699,
            "unit": "ns/iter",
            "extra": "iterations: 785265\ncpu: 894.672371747487 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Fast_To_Int_Truncate",
            "value": 889.7245160942689,
            "unit": "ns/iter",
            "extra": "iterations: 785378\ncpu: 891.6118098555754 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Floor",
            "value": 699.0049140785717,
            "unit": "ns/iter",
            "extra": "iterations: 998968\ncpu: 700.6819037235744 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Ceil",
            "value": 699.1849509051107,
            "unit": "ns/iter",
            "extra": "iterations: 998757\ncpu: 701.137013309612 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Cos",
            "value": 723.6447987619422,
            "unit": "ns/iter",
            "extra": "iterations: 969062\ncpu: 721.062739015182 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Sin",
            "value": 717.870184506491,
            "unit": "ns/iter",
            "extra": "iterations: 973990\ncpu: 719.5121099775911 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}