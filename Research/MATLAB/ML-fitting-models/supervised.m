load MLpractice.mat; 
tree_mdl1 = fitrtree(T, binarray); 
tree_mdl2 = fitrtree(T, binarray, 'CrossVal','on'); 
%tree_mdl3 = fitrtree(T, binarray,'OptimizeHyperparameters','auto'); 
tree_mdl3 = fitrtree(T, binarray, 'CrossVal', 'on','MinLeafSize', 20);
disp(kfoldLoss(tree_mdl3));
%bagging_mdl = fitrensemble(T, binarray, 'Method','Bag'); 

% hold on
% for i=1:5
%     bagging_mdl = TreeBagger(20, T, binarray,'Method', 'regression', 'OOBPrediction','on', 'MinLeafSize', i*10); 
%     %bagging_mdl = fitrensemble(T, binarray, 'Method','Bag', 'NumLearningCycles', i); 
%     plot(oobError(bagging_mdl));
% end
% xlabel('NumberTrees')
% ylabel('MSE') 
% legend("MinLeafSize", {'10' '20' '30' '40' '50'},'Location','NorthEast')
% hold off

t = templateTree('MaxNumSplits',1);
boosted_mdl = fitrensemble(T, binarray, 'Learners', t, 'CrossVal','on', 'NumLearningCycles', 200);

%cv = cvpartition(size(T,1), 'KFold', 10);
%opt_mdl = fitrensemble(T, binarray, 'OptimizeHyperparameters', {'NumLearningCycles'}, 'Learners', t);  

kflc = kfoldLoss(boosted_mdl,'Mode','cumulative');
figure;
plot(kflc);
ylabel('10-fold cross-validated MSE');
xlabel('Learning cycle');

lin_mdl = fitlm(T, binarray);

% svm_mdl1 = fitrsvm(T, binarray, 'Standardize',true, 'KFold', 5, 'KernelFunction','linear'); 
% svm_mdl2 = fitrsvm(T, binarray, 'Standardize',true, 'KFold', 5, 'KernelFunction','gaussian');
% svm_mdl3 = fitrsvm(T, binarray, 'Standardize',true, 'KFold', 5, 'KernelFunction','rbf'); 
% svm_mdl4 = fitrsvm(T, binarray, 'Standardize',true, 'KFold', 5, 'KernelFunction','polynomial', 'PolynomialOrder', 2); 
% MSE1 = kfoldLoss(svm_mdl1); 
% MSE2 = kfoldLoss(svm_mdl2);
% MSE3 = kfoldLoss(svm_mdl3); 
% MSE4 = kfoldLoss(svm_mdl4); 


