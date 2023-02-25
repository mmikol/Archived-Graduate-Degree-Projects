import numpy as np
from pandas import DataFrame, read_csv
from sys import argv

class MultiLayerPerceptron:
    def __init__(self, learning_rate, number_of_epochs, batch_size, number_of_layers, value_limit, rows, columns):
        self.learning_rate = learning_rate
        self.number_of_epochs = number_of_epochs
        self.batch_size = batch_size
        self.number_of_layers = number_of_layers
        self.number_limit = value_limit
        rows, columns =  list(rows.values()),  list(columns.values())
        self.weights = [np.random.randn(rows[i], columns[i]) * np.sqrt(1/columns[i]) for i in range(number_of_layers)]
        self.biases = [np.random.randn(rows[i], 1)*np.sqrt(1/columns[i]) for i in range(number_of_layers)]
        
    def learn(self, images, labels, n):
        for _ in range(self.number_of_epochs):
            v = np.arange(n)
            np.random.shuffle(v)
            images, labels = images[:,v], labels[:,v]
            for images_, labels_ in self.generate_batches(images, labels, n):
                cache = self.forward_propagation(images_)
                gradients = self.backward_propagation(images_, labels_, cache, images_.shape[1]) 
                self.update_weights(gradients)

    def test(self, images):
        return np.argmax(self.forward_propagation(images)['a2'], axis=0)
   
    def generate_batches(self, images, labels, n):
        for i in range(1, n//self.batch_size):
            yield images[:,(i-1)*self.batch_size : i*self.batch_size], labels[:,(i-1)*self.batch_size : i*self.batch_size]
        if n % self.batch_size != 0:
            return images[:, self.batch_size * (n//self.batch_size) : n], labels[:, self.batch_size * n // self.batch_size : n]

    def forward_propagation(self, images):  
        cache = {}
        for i in range(self.number_of_layers):
            cache[f'z{i}'] = np.dot(self.weights[i], images)+self.biases[i] if i == 0 else np.dot(self.weights[i], cache[f'a{i-1}'])+self.biases[i]
            cache[f'a{i}'] = self.softmax_activation(cache[f'z{i}']) if i == self.number_of_layers-1 else self.sigmoid_activation(cache[f'z{i}'])        
        return cache
    
    def backward_propagation(self, images, labels, cache, n):
        gradients = {}
        for i in range(self.number_of_layers-1, -1, -1):
            gradients[f'da{i}'] = None if i == self.number_of_layers-1 else np.dot(self.weights[i+1].T, gradients[f'dz{i+1}'])
            gradients[f'dz{i}'] = cache['a2']-labels if i == self.number_of_layers-1 else self.sigmoid_derivative(gradients[f'da{i}'], cache[f'z{i}']) 
            gradients[f'dw{i}'] = (1/n)*np.dot(gradients[f'dz{i}'], images.T) if i == 0 else (1/n)*np.dot(gradients[f'dz{i}'], cache[f'a{i-1}'].T)
            gradients[f'db{i}'] = (1/n)*np.sum(gradients[f'dz{i}'], keepdims=True, axis=1) 
        return gradients
    
    def update_weights(self, gradients):
        for i in range(self.number_of_layers):
            self.weights[i] = self.weights[i]-(self.learning_rate*gradients[f'dw{i}'])
            self.biases[i] = self.biases[i]-(self.learning_rate*gradients[f'db{i}'])

    def sigmoid_activation(self, z):
        return 1/(1+np.exp(-np.clip(z, -self.number_limit, self.number_limit)))
    
    def sigmoid_derivative(self, da, z):
        s = self.sigmoid_activation(z)
        return (1-s)*da*s
    
    def softmax_activation(self, z):
        s = np.exp(z-z.max())
        return (1/np.sum(s, axis=0))*s

if __name__ == '__main__':
    training_images, training_labels, testing_images = read_csv(argv[1], header=None).T.values, read_csv(argv[2], header=None).T.values, read_csv(argv[3], header=None).T.values
    NN = MultiLayerPerceptron(learning_rate=0.03, number_of_epochs=100, batch_size=50, number_of_layers=3, value_limit=500, rows={'pixel_combos': 512, 'pixel_range': 256, 'integer_labels': 10}, columns={'pixels': 784, 'pixel_combos': 512, 'pixel_range': 256})    
    train_set = np.zeros((training_labels.size, training_labels.max()+1))
    train_set[np.arange(training_labels.size), training_labels] = 1
    NN.learn(training_images, train_set.T, training_images.shape[1])
    DataFrame(NN.test(testing_images)).to_csv('test_predictions.csv', index=None, header=None)
