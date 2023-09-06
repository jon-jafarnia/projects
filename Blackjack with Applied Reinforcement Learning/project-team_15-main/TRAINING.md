# CS50 Final Project
## Training Explanation

- In order to obtain a substantial q value, our group chose to run our q test for 100,000 hands of black jack. 
- We specifically chose 100,000 as the number for our tests for a multitude of reasons.
- From a probability standpoint, we wanted each specific scenario to have been run 100 times to give the program the most accurate data to work with. 
- 18 potential player hands, 10 potential dealer hands, 2 oppurtunities depending on ace, and 2 decisions can be made. 
-   Thus 18 * 10 * 2 * 2 = 720. 
- Since we wanted each program to have a minimum of 100 cases each, this number went up to 72,000 test cases. Although we could have stopped here, it is important to note that probabilities do not always work out the way we intend for them to. So there is potential for some scenarios to be run 50 times, while others may get 150 times. 
- In order to combat this, we added 38,000 more test cases which effectively evened out the tests for modules that may not have got a particularly even spread. 
- Ultimately our system to calculate q using 100,000 scenarios worked extremely well in giving us a precise and effective q solution to use for our tests.
