window.BENCHMARK_DATA = {
  "lastUpdate": 1676982655274,
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
      },
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
          "id": "a73ab77e3b777b85623a4b4b3512ec20a178cab0",
          "message": "Remove release step from CI",
          "timestamp": "2023-02-21T13:22:18+01:00",
          "tree_id": "cd83a94e16ee61b836b4bfa4fd612a67baeb60ef",
          "url": "https://github.com/feliwir/Thyme/commit/a73ab77e3b777b85623a4b4b3512ec20a178cab0"
        },
        "date": 1676982654128,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "RefPack_Compress",
            "value": 40353395.235292606,
            "unit": "ns/iter",
            "extra": "iterations: 17\ncpu: 40291458.8235294 ns\nthreads: 1"
          },
          {
            "name": "RefPack_Uncompress",
            "value": 104888.65169989826,
            "unit": "ns/iter",
            "extra": "iterations: 6971\ncpu: 104872.25649117773 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Fast_To_Int_Floor",
            "value": 798.9316458849929,
            "unit": "ns/iter",
            "extra": "iterations: 847088\ncpu: 799.8676642804927 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Fast_To_Int_Ceil",
            "value": 788.4210405677741,
            "unit": "ns/iter",
            "extra": "iterations: 881352\ncpu: 790.1834908177643 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Fast_To_Int_Truncate",
            "value": 767.5621867136131,
            "unit": "ns/iter",
            "extra": "iterations: 863110\ncpu: 769.1696307542932 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Floor",
            "value": 756.0714518744373,
            "unit": "ns/iter",
            "extra": "iterations: 946007\ncpu: 756.0045538773147 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Ceil",
            "value": 759.5084297235611,
            "unit": "ns/iter",
            "extra": "iterations: 922804\ncpu: 758.186028669855 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Cos",
            "value": 760.632695900928,
            "unit": "ns/iter",
            "extra": "iterations: 922244\ncpu: 759.3501285988747 ns\nthreads: 1"
          },
          {
            "name": "GameMath_CM_Sin",
            "value": 777.2043784300034,
            "unit": "ns/iter",
            "extra": "iterations: 939356\ncpu: 775.6856825326129 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Fast_To_Int_Floor",
            "value": 995.772644780636,
            "unit": "ns/iter",
            "extra": "iterations: 738426\ncpu: 994.2346287917868 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Fast_To_Int_Ceil",
            "value": 956.6080096390017,
            "unit": "ns/iter",
            "extra": "iterations: 705451\ncpu: 959.8909066695059 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Fast_To_Int_Truncate",
            "value": 988.9243312490141,
            "unit": "ns/iter",
            "extra": "iterations: 733988\ncpu: 990.8224657636481 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Floor",
            "value": 765.782378856982,
            "unit": "ns/iter",
            "extra": "iterations: 939104\ncpu: 758.3750042590833 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Ceil",
            "value": 791.231742945706,
            "unit": "ns/iter",
            "extra": "iterations: 919368\ncpu: 785.9727551954643 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Cos",
            "value": 785.2677505586026,
            "unit": "ns/iter",
            "extra": "iterations: 921996\ncpu: 787.2064520894453 ns\nthreads: 1"
          },
          {
            "name": "GameMath_GM_Sin",
            "value": 792.3089354668242,
            "unit": "ns/iter",
            "extra": "iterations: 906581\ncpu: 787.9920271883918 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}