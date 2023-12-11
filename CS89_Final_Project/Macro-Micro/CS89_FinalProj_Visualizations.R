## Libraries
library(tidyverse)
library(lubridate)

## Observational Chart for Microeconomic Sentimnet
micro_macro <- read_csv("/Users/jacksonbattey/Desktop/Dartmouth/CS89/FinalProject/Macro-Micro/micro_and_macro.csv")

micro_macro %>%
  mutate(avg_sent = total_sentiment / total_sentiment_count) %>%
  filter(year(date) >= 2000) %>%
  ggplot(aes(x=date)) +
  geom_line(aes(y = avg_sent, color="Company Sentiment")) +
  geom_line(aes(y = (Eff_Fed_Funds-5)/10, color="Fed Funds Rate")) +
  scale_x_date(date_breaks = "1 year", date_labels = "%Y") +
  scale_y_continuous(name = "Company Sentiment", sec.axis = sec_axis(~.*10 + 5, name = "Fed Funds Rate")) +
  scale_color_manual(values = c("#003366", "#6497b1")) +
  theme_minimal() +
  theme(legend.title = element_blank(),
        legend.position = c(0.85,0.9),
        panel.grid.minor = element_blank(),
        axis.text.x = element_text(angle = 90, vjust = 0.5, hjust = 0)) + 
  labs(x = NULL, y = "Sentiment Score", title = "Company Sentiment v. Fed Funds Rate Since 2000")


## Results Bar Charts
results <- read_csv("/Users/jacksonbattey/Desktop/Dartmouth/CS89/FinalProject/result_df.csv")

## Function to get features used in a model
get_model_type <- function(row) {
  selected_features <- names(row)[which(row == 1)]
  model_type <- paste(selected_features, collapse = ", ")
  return(model_type)
}

## Add model features text column to df
models <- apply(results, 1, get_model_type)
results$model_type <- models

## Bar Chart Visualization of Regression Results
results[c("model_type", "mse", "mae")] %>%
  pivot_longer(, cols = c("mse", "mae"),
               names_to = "metric", values_to = "value") %>%
  mutate(model_type = factor(model_type, levels = c("macro", "micro", "tfidf", "semantic", "macro, micro", "tfidf, macro", "macro, semantic", "tfidf, micro", "micro, semantic", "tfidf, semantic", "tfidf, macro, micro", "macro, micro, semantic", "tfidf, micro, semantic", "tfidf, macro, semantic", "tfidf, macro, micro, semantic"))) %>%
  ggplot(aes(x=model_type, y=value, fill=metric)) +
  geom_bar(stat = "identity", position="dodge", width=0.7) +
  scale_fill_manual(values = c("#c0d6e4", "#6497b1")) +
  scale_y_continuous(limits = c(0, 0.15)) +
  theme_minimal() +
  theme(legend.position = c(0.9,0.9),
        legend.title = element_blank(),
        axis.text.x = element_text(angle = 90, vjust = 0.5, hjust = 1),
        axis.title.x = element_blank(),
        axis.title.y = element_blank(),
        panel.grid.major.x = element_blank(),
        panel.grid.minor.x = element_blank())

## Bar Chart Vizualization of Classification Results
results[c("model_type", "f1", "accuracy")] %>%
  pivot_longer(, cols = c("f1", "accuracy"),
               names_to = "metric", values_to = "value") %>%
  mutate(model_type = factor(model_type, levels = c("macro", "micro", "tfidf", "semantic", "macro, micro", "tfidf, macro", "macro, semantic", "tfidf, micro", "micro, semantic", "tfidf, semantic", "tfidf, macro, micro", "macro, micro, semantic", "tfidf, micro, semantic", "tfidf, macro, semantic", "tfidf, macro, micro, semantic"))) %>%
  ggplot(aes(x=model_type, y=value, fill=metric)) +
  geom_bar(stat = "identity", position="dodge", width=0.7) +
  scale_fill_manual(values = c("#c0d6e4", "#6497b1")) +
  scale_y_continuous(limits = c(0, 1)) +
  theme_minimal() +
  theme(legend.position = c(0.9,0.9),
        legend.title = element_blank(),
        axis.text.x = element_text(angle = 90, vjust = 0.5, hjust = 1),
        axis.title.x = element_blank(),
        axis.title.y = element_blank(),
        panel.grid.major.x = element_blank(),
        panel.grid.minor.x = element_blank())
  



