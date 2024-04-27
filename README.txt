Following are the differences in operating time for several URLs/files running normally and running in parallel:
    NOTE: The times shown are in minutes and seconds and are the "real" time acquired from the "time" command.

URL: https://users.tricity.wsu.edu/~bobl/cpts360/
    Normal: 0m7.421s
    Parallel: 0m1.659s
    Difference: Parallel is 4.47x faster

URL: https://tricities.wsu.edu/
    Normal: 0m32.929s
    Parallel: 0m2.507s
    Difference: Parallel is 13.1x faster

URL: https://en.wikipedia.org/wiki/C_(programming_language)
    Normal: 3m13.409s
    Parallel: 0m8.904s
    Difference: Parallel is 21.7x faster

URL: https://www.youtube.com/
    Normal: 0m9.913s
    Parallel: 0m1.682s
    Difference: Parallel is 5.89x faster

URL: https://www.gnu.org/home.en.html
    Normal: 0m23.898s
    Parallel: 0m3.315s
    Difference: Parallel is 7.21x faster

The differences between the normal and parallel operating modes are massive.
Parallel is clearly faster, even to the point of several orders of magnitude.
