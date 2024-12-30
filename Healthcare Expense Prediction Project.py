import pandas as pd # type: ignore
from sklearn.preprocessing import StandardScaler # type: ignore
from sklearn.model_selection import train_test_split # type: ignore
from sklearn.metrics import classification_report # type: ignore
from sklearn.linear_model import LogisticRegression # type: ignore
from sklearn.metrics import roc_curve # type: ignore
from sklearn.tree import DecisionTreeClassifier # type: ignore
from sklearn.ensemble import RandomForestClassifier # type: ignore
import matplotlib.pyplot as plt # type: ignore

#read the data
info = pd.read_csv("USAMASTERDATA.csv")

# convert SPMMEDXPNS to binary
threshold = info['SPMMEDXPNS'].median()
info['SPMMEDXPNS_binary'] = (info['SPMMEDXPNS'] > threshold).astype(int)

X = info[['SEX', 'AGE', 'RACE', 'BPL', 'YRSUSA1', 'HCOVANY', 'DIFFREM', 'DIFFPHYS', 'DIFFMOB', 'DIFFCARE', 'DIFFSENS']]
y = info['SPMMEDXPNS_binary']

# scale the features for uniformity
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# split data into train and test sets (50% training, 50% testing)
X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=0.5, random_state=16)

########### Logistic Regression Method
# initialize and train Logistic Regression method
logreg = LogisticRegression(random_state=16, max_iter=1000)
logreg.fit(X_train, y_train)

y_pred_logreg = logreg.predict(X_test)
print("Logistic Regression Classification Report:")
print(classification_report(y_test, y_pred_logreg))

# ROC curve for Logistic Regression
fpr_logreg, tpr_logreg, _ = roc_curve(y_test, logreg.predict_proba(X_test)[:, 1])
plt.plot(fpr_logreg, tpr_logreg, label='Logistic Regression', linestyle='--')

######### Random Forest and Decision Tree Methods
# initialize models for Random Forest and Decision Trees 
n_estimators = 100
models = [DecisionTreeClassifier(max_depth=3),RandomForestClassifier(n_estimators=n_estimators)]

model_titles = ['DecisionTree','RandomForest']

# initialize empty lists for storing results
surv_preds, surv_probs, scores, fprs, tprs = ([] for i in range(5))

# train and evaluate each model
for i, model in enumerate(models):
    clf = model.fit(X_train,y_train)
    surv_preds.append(model.predict(X_test))
    surv_probs.append(model.predict_proba(X_test))
    scores.append(model.score(X_test, y_test))
    fpr, tpr, _ = roc_curve(y_test, surv_probs[i][:,1])  
    fprs.append(fpr)
    tprs.append(tpr)

# plot ROC curves
for i, _ in enumerate(models):
    plt.plot(fprs[i], tprs[i], label=model_titles[i])
# print classification report for both models
for i, preds in enumerate(surv_preds):
    print(f"\nClassification Report for {model_titles[i]}:")
    print(classification_report(y_test, preds))

plt.legend(model_titles + ['Logistic Regression'])
plt.show()
