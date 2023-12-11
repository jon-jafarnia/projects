# Projects
This repository holds some of the significant coding projects I have worked on.

## CS89_Final_project
This project is completed in python and it predicts the Federal Funds Rate. We scrape text data from FOMC Statements, FOMC minutes, and Federal Reserve Speeches. We also gather semantic embeddings from 10-K and 10-Q filings from the 30 companies included in the Dow Jones Industrial Average, and we use FRED to collect macro-economic indicators of the market. We use this data to see if we can predict and classify the Federal Funds Rate. We found that TF-IDF vectorization of text from the Federal Reserve, macroeconomic indicators, microeconomic sentiment, and Word2Vec semantic features created the best logistic regression classification model. This model has an F1 score of 0.650 and an accuracy of 0.6682.

## Lab4_EC
This project is completed in python and plots a shortest path between two vertices. For this project, I used a map of Dartmouth's campus and marked significant locations as vertices. When two vertices are selected, we use BFS to find the shortest path between these points and then change the color of this shortest path.

## project-team-15-main
This project was completed in C at the culmination of the course COSC 050 (Software Design and Implementation). This project uses reinforcement learning in order to find the optimal decision strategy when playing a game of Blackjack. Furthermore, this project uses clients and servers to allow for others to connect to the game as well.

## tse-jon-jafarnia-main
This project was completed in C and it implements a tiny search engine. The project is split into 3 sections: crawler, indexer, and querier.

1. The **crawler** crawls a website and retrieves webpages starting with a specified URL.
   It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to some threshold number of hops from the seed URL, and avoiding visiting any given URL more than once.
   It saves the pages, URL, and depth for each, in files.
   When the crawler process is complete, the indexing of the collected documents can begin.

2. The **indexer** extracts all the keywords for each stored webpage and creates a lookup table that maps each word found to all the documents (webpages) where the word was found.
   It saves this table in a file.

3. The **query engine** responds to requests (queries) from users.
   The *query processor module* loads the index file and searches for pages that include the search keywords.
   Because there may be many hits, we use a *ranking module* to rank the results (e.g., high to low number of instances of a keyword on a page).

Each subsystem is a standalone program executed from the command line, but they inter-connect through files in the file system. 
