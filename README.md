# Depth First Search with Branch and Bound
## Algorithm Practice
預估該點LB = calScheduledSumC + estimateLB

input: job permutation，目前走到的 level，jobs 總數，走過的點數
1. 透過 swap 產生 job permutation
2. 再針對該點做預估，預估結果小於目前最佳解(best)才繼續長樹


