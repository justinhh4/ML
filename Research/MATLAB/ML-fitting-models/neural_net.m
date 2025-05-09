load MLpractice.mat; 

cvp = cvpartition(size(T, 1), 'KFold', 10); 
%neuralNet_mdl = fitrnet(T, binarray, 'CrossVal','on', 'Activations', 'sigmoid', 'LayerSizes', [5, 5, 5, 1, 1]);
%MSE1 = kfoldLoss(neuralNet_mdl); 


% lambda = (0:0.5:5)*1e-3;
% cvloss = zeros(length(lambda),1);
% for i = 1:length(lambda)
%     cvMdl = fitrnet(T, binarray,"Lambda",lambda(i),'CrossVal', 'on',"Standardize",true, 'LayerSizes', [7]);
%     cvloss(i) = kfoldLoss(cvMdl);
% end
% 
% plot(lambda,cvloss)
% xlabel("Regularization Strength")
% ylabel("Cross-Validation Loss")

T = normalize(T)
k = [1, 2]; 
likelihood = zeros(length(k), 1); 
for i=1:length(k)
    GMM_model = fitgmdist(T, k(i));
    likelihood(i) = GMM_model.NegativeLogLikelihood; 
end 

plot(k, likelihood); 
