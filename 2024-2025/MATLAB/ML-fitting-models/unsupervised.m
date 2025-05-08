load MLpractice.mat; 

lin_mdl = fitlm(T, binarray); 
%k-means clustering
maxK = 100;
mse_values = zeros(maxK, 1);
for k = 1:maxK
    [idx, C, sumd] = kmeans(T, k, 'Replicates', 5);
    mse_values(k) = sum(sumd) / size(T, 1);
end
figure;
plot(1:maxK, mse_values, '-o');

%opts = statset('Display','final');

%PCA
coeff = pca(T); 

