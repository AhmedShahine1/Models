# import re
# import random

# class Individual:
#     def __init__(self, genes):
#         self.genes = genes
#         self.fitness = None

# class GeneticAlgorithm:
#     def __init__(self, model_file, crossover_rate, mutation_rate, generations):
#         self.model_file = model_file
#         self.crossover_rate = crossover_rate
#         self.mutation_rate = mutation_rate
#         self.generations = generations
#         self.population = []
#         self.objective, self.constraints, self.lower, self.upper, self.pop_size = self.parse_lp_file()

#     def parse_lp_file(self):
#         with open(self.model_file, encoding='ISO-8859-1') as file:
#             lines = file.readlines()

#         objective = ""
#         constraints = []
#         bounds = {}
#         variable_set = set()

#         in_objective = False
#         in_constraints = False
#         in_bounds = False
#         in_binaries = False

#         for line in lines:
#             line = line.strip()
            
#             if line.startswith("Maximize") or line.startswith("Minimize"):
#                 in_objective = True
#                 in_constraints = False
#                 in_bounds = False
#                 in_binaries = False
#                 continue
            
#             if line.startswith("Subject To"):
#                 in_objective = False
#                 in_constraints = True
#                 in_bounds = False
#                 in_binaries = False
#                 continue
            
#             if line.startswith("Bounds"):
#                 in_objective = False
#                 in_constraints = False
#                 in_bounds = True
#                 in_binaries = False
#                 continue
            
#             if line.startswith("Binaries"):
#                 in_objective = False
#                 in_constraints = False
#                 in_bounds = False
#                 in_binaries = True
#                 continue

#             if line.startswith("End"):
#                 in_objective = False
#                 in_constraints = False
#                 in_bounds = False
#                 in_binaries = False
#                 continue
            
#             if in_objective:
#                 objective += line + " "
            
#             if in_constraints:
#                 constraints.append(line)
            
#             if in_bounds:
#                 variables = re.findall(r'x\d+', line)
#                 bounds[variables[0]] = tuple(map(float, re.findall(r'[-+]?\d*\.\d+|\d+', line)))

#             if in_binaries:
#                 variables = re.findall(r'x\d+', line)
#                 for var in variables:
#                     bounds[var] = (0, 1)  # Default bounds for binary variables

#         # Extract variable names from objective and constraints
#         variables = re.findall(r'x\d+', objective)
#         variable_set.update(variables)
#         for constraint in constraints:
#             variables = re.findall(r'x\d+', constraint)
#             variable_set.update(variables)
        
#         # Sort variables based on their indices
#         sorted_variables = sorted(variable_set, key=lambda x: int(x[1:]))
        
#         # Initialize upper and lower bounds
#         upper = []
#         lower = []
#         for var in sorted_variables:
#             if var in bounds:
#                 lower.append(bounds[var][0])
#                 upper.append(bounds[var][1])
#             else:
#                 lower.append(float('-inf'))  # Default bounds for unbounded variables
#                 upper.append(float('inf'))

#         num_variables = len(sorted_variables)
        
#         # Clean and format objective function
#         objective = objective.replace("obj1:", "").strip()

#         return objective, constraints, lower, upper, num_variables

#     def initialize_population(self):
#         for _ in range(self.pop_size):
#             genes = {}
#             for i in range(self.pop_size):  # Corrected to use pop_size which is number of variables
#                 genes[f'x{i+1}'] = random.uniform(self.lower[i], self.upper[i])
#             individual = Individual(genes)
#             self.population.append(individual)


#     def decode_genes(self, genes):
#         decoded_genes = {}
#         for i, (var, gene_value) in enumerate(genes.items()):
#             lb, ub = self.lower[i], self.upper[i]
#             if lb == float('-inf'):
#                 lb = -1e6  # Set a large negative value for unbounded lower bound
#             if ub == float('inf'):
#                 ub = 1e6  # Set a large positive value for unbounded upper bound
#             real_value = lb + gene_value * (ub - lb)
#             decoded_genes[var] = real_value
#         return decoded_genes

#     def objective_function(self, genes):
#         # Extract coefficients and variable names from the objective function
#         terms = re.findall(r'([+-]?\s*\d*\.?\d+)?\s*([a-zA-Z]\w*)', self.objective)
#         coeffs = {}
#         for term in terms:
#             coeff = term[0].replace(' ', '')
#             coeff = float(coeff) if coeff else 1.0
#             var = term[1]
#             coeffs[var] = coeff
        
#         # Calculate the objective value
#         value = sum(coeffs.get(var, 0) * genes.get(var, 0) for var in genes)
#         return value

#     def constraint_satisfied(self, genes, constraint):
#         # Parse the constraint
#         if '=' in constraint:
#             lhs, rhs = constraint.split('=')
#             operator = '='
#         elif '<=' in constraint:
#             lhs, rhs = constraint.split('<=')
#             operator = '<='
#         elif '>=' in constraint:
#             lhs, rhs = constraint.split('>=')
#             operator = '>='
        
#         # Extract coefficients and variable names from the lhs of the constraint
#         terms = re.findall(r'([+-]?\s*\d*\.?\d+)?\s*([a-zA-Z]\w*)', lhs)
#         lhs_value = sum(float(term[0].replace(' ', '')) * genes.get(term[1], 0) if term[0] else genes.get(term[1], 0) for term in terms)
        
#         rhs_value = float(rhs.strip())
        
#         if operator == '=' and lhs_value == rhs_value:
#             return True
#         elif operator == '<=' and lhs_value <= rhs_value:
#             return True
#         elif operator == '>=' and lhs_value >= rhs_value:
#             return True
#         return False

#     def evaluate_individual(self, individual):
#         # Decode genes to real values
#         real_genes = self.decode_genes(individual.genes)

#         # Check constraints
#         for constraint in self.constraints:
#             if not self.constraint_satisfied(real_genes, constraint):
#                 # Penalize fitness for violating constraints
#                 individual.fitness = float('-inf')
#                 return individual.fitness
        
#         # Evaluate fitness based on the objective function
#         individual.fitness = self.objective_function(real_genes)
#         return individual.fitness

#     def select_parents(self):
#         # Tournament selection without using built-in functions
#         tournament_size = 3
#         tournament = random.sample(self.population, tournament_size)
#         parent = max(tournament, key=lambda ind: ind.fitness)
#         return parent

#     def crossover(self, parent1, parent2):
#         if random.random() < self.crossover_rate:
#             child1_genes = {}
#             child2_genes = {}
#             for var in parent1.genes.keys():
#                 if random.random() < 0.5:
#                     child1_genes[var] = parent1.genes[var]
#                     child2_genes[var] = parent2.genes[var]
#                 else:
#                     child1_genes[var] = parent2.genes[var]
#                     child2_genes[var] = parent1.genes[var]
#             child1 = Individual(child1_genes)
#             child2 = Individual(child2_genes)
#         else:
#             child1 = Individual(parent1.genes.copy())
#             child2 = Individual(parent2.genes.copy())
#         return child1, child2

#     def mutate(self, individual):
#         for var in individual.genes.keys():
#             if random.random() < self.mutation_rate:
#                 i = int(var[1:]) - 1  # Extract index from variable name
#                 individual.genes[var] = random.uniform(self.lower[i], self.upper[i])
                
#     def replace_population(self, new_population):
#         self.population = new_population

#     def run(self):
#         self.initialize_population()
#         for generation in range(self.generations):
#             for individual in self.population:
#                 if individual.fitness is None:
#                     self.evaluate_individual(individual)  # Ensure fitness evaluation

#             new_population = []
#             for _ in range(self.pop_size // 2):
#                 parent1 = self.select_parents()
#                 parent2 = self.select_parents()
#                 child1, child2 = self.crossover(parent1, parent2)
#                 self.mutate(child1)
#                 self.mutate(child2)
#                 new_population.extend([child1, child2])

#             for individual in new_population:
#                 if individual.fitness is None:
#                     self.evaluate_individual(individual)  # Ensure fitness evaluation

#             self.replace_population(new_population)
#             best_individual = max(self.population, key=lambda ind: ind.fitness)
#             print(f'Generation {generation}: Best Fitness = {best_individual.fitness}')

# if __name__ == "__main__":
#     model_file = 'model.lp'
#     ga = GeneticAlgorithm(model_file=model_file, crossover_rate=0.8, mutation_rate=0.1, generations=50)
#     ga.run()

import re
import random
import xml.etree.ElementTree as ET

class Individual:
    def __init__(self, genes):
        self.genes = genes
        self.fitness = None

class GeneticAlgorithm:
    def __init__(self, model_file, solution_file, crossover_rate, mutation_rate, generations):
        self.model_file = model_file
        self.solution_file = solution_file
        self.crossover_rate = crossover_rate
        self.mutation_rate = mutation_rate
        self.generations = generations
        self.population = []
        self.objective, self.constraints, self.lower, self.upper, self.pop_size = self.parse_lp_file()
        self.solution_values = self.parse_solution_file()

    def parse_lp_file(self):
        with open(self.model_file, encoding='ISO-8859-1') as file:
            lines = file.readlines()

        objective = ""
        constraints = []
        bounds = {}
        variable_set = set()

        in_objective = False
        in_constraints = False
        in_bounds = False
        in_binaries = False

        for line in lines:
            line = line.strip()
            
            if line.startswith("Maximize") or line.startswith("Minimize"):
                in_objective = True
                in_constraints = False
                in_bounds = False
                in_binaries = False
                continue
            
            if line.startswith("Subject To"):
                in_objective = False
                in_constraints = True
                in_bounds = False
                in_binaries = False
                continue
            
            if line.startswith("Bounds"):
                in_objective = False
                in_constraints = False
                in_bounds = True
                in_binaries = False
                continue
            
            if line.startswith("Binaries"):
                in_objective = False
                in_constraints = False
                in_bounds = False
                in_binaries = True
                continue

            if line.startswith("End"):
                in_objective = False
                in_constraints = False
                in_bounds = False
                in_binaries = False
                continue
            
            if in_objective:
                objective += line + " "
            
            if in_constraints:
                constraints.append(line)
            
            if in_bounds:
                variables = re.findall(r'x\d+', line)
                bounds[variables[0]] = tuple(map(float, re.findall(r'[-+]?\d*\.\d+|\d+', line)))

            if in_binaries:
                variables = re.findall(r'x\d+', line)
                for var in variables:
                    bounds[var] = (0, 1)  # Default bounds for binary variables

        # Extract variable names from objective and constraints
        variables = re.findall(r'x\d+', objective)
        variable_set.update(variables)
        for constraint in constraints:
            variables = re.findall(r'x\d+', constraint)
            variable_set.update(variables)
        
        # Sort variables based on their indices
        sorted_variables = sorted(variable_set, key=lambda x: int(x[1:]))
        
        # Initialize upper and lower bounds
        upper = []
        lower = []
        for var in sorted_variables:
            if var in bounds:
                lower.append(bounds[var][0])
                upper.append(bounds[var][1])
            else:
                lower.append(float('-inf'))  # Default bounds for unbounded variables
                upper.append(float('inf'))

        num_variables = len(sorted_variables)
        
        # Clean and format objective function
        objective = objective.replace("obj1:", "").strip()

        return objective, constraints, lower, upper, num_variables

    def parse_solution_file(self):
        tree = ET.parse(self.solution_file)
        root = tree.getroot()

        variable_values = {}
        for variable in root.findall('.//variable'):
            var_name = f"x{int(variable.get('index')) + 1}"
            value = float(variable.get('value'))
            variable_values[var_name] = value

        return variable_values

    def initialize_population(self):
        for _ in range(self.pop_size):
            genes = {}
            for i in range(self.pop_size):
                var_name = f'x{i+1}'
                if self.lower[i] == float('-inf') or self.upper[i] == float('inf'):
                    # Use the value from the solution file for unbounded variables
                    value = self.solution_values.get(var_name, 0)
                    genes[var_name] = value
                    # Update bounds to the value from the solution file
                    self.lower[i] = value
                    self.upper[i] = value
                else:
                    genes[var_name] = random.uniform(self.lower[i], self.upper[i])
            individual = Individual(genes)
            self.population.append(individual)

    def decode_genes(self, genes):
        decoded_genes = {}
        for i, (var, gene_value) in enumerate(genes.items()):
            lb, ub = self.lower[i], self.upper[i]
            real_value = lb + gene_value * (ub - lb)
            decoded_genes[var] = real_value
        return decoded_genes

    def objective_function(self, genes):
        # Extract coefficients and variable names from the objective function
        terms = re.findall(r'([+-]?\s*\d*\.?\d+)?\s*([a-zA-Z]\w*)', self.objective)
        coeffs = {}
        for term in terms:
            coeff = term[0].replace(' ', '')
            coeff = float(coeff) if coeff else 1.0
            var = term[1]
            coeffs[var] = coeff
        
        # Calculate the objective value
        value = sum(coeffs.get(var, 0) * genes.get(var, 0) for var in genes)
        return value

    def constraint_satisfied(self, genes, constraint):
        # Parse the constraint
        if '=' in constraint:
            lhs, rhs = constraint.split('=')
            operator = '='
        elif '<=' in constraint:
            lhs, rhs = constraint.split('<=')
            operator = '<='
        elif '>=' in constraint:
            lhs, rhs = constraint.split('>=')
            operator = '>='
        
        # Extract coefficients and variable names from the lhs of the constraint
        terms = re.findall(r'([+-]?\s*\d*\.?\d+)?\s*([a-zA-Z]\w*)', lhs)
        lhs_value = sum(float(term[0].replace(' ', '')) * genes.get(term[1], 0) if term[0] else genes.get(term[1], 0) for term in terms)
        
        rhs_value = float(rhs.strip())
        
        if operator == '=' and lhs_value == rhs_value:
            return True
        elif operator == '<=' and lhs_value <= rhs_value:
            return True
        elif operator == '>=' and lhs_value >= rhs_value:
            return True
        return False

    def evaluate_individual(self, individual):
        # Decode genes to real values
        real_genes = self.decode_genes(individual.genes)

        # Check constraints
        for constraint in self.constraints:
            if not self.constraint_satisfied(real_genes, constraint):
                # Penalize fitness for violating constraints
                individual.fitness = float('-inf')
        
        # Evaluate fitness based on the objective function
        individual.fitness = self.objective_function(real_genes)
        return individual.fitness

    def select_parents(self):
        # Tournament selection without using built-in functions
        tournament_size = 3
        tournament = random.sample(self.population, tournament_size)
        parent = max(tournament, key=lambda ind: ind.fitness)
        return parent

    def crossover(self, parent1, parent2):
        if random.random() < self.crossover_rate:
            child1_genes = {}
            child2_genes = {}
            for var in parent1.genes.keys():
                if random.random() < 0.5:
                    child1_genes[var] = parent1.genes[var]
                    child2_genes[var] = parent2.genes[var]
                else:
                    child1_genes[var] = parent2.genes[var]
                    child2_genes[var] = parent1.genes[var]
            child1 = Individual(child1_genes)
            child2 = Individual(child2_genes)
        else:
            child1 = Individual(parent1.genes.copy())
            child2 = Individual(parent2.genes.copy())
        return child1, child2

    def mutate(self, individual):
        for var in individual.genes.keys():
            if random.random() < self.mutation_rate:
                i = int(var[1:]) - 1  # Extract index from variable name
                if self.lower[i] == self.upper[i]:
                    # Use the value from the solution file if bounds are equal (implying previously unbounded)
                    individual.genes[var] = self.solution_values.get(var, individual.genes[var])
                else:
                    individual.genes[var] = random.uniform(self.lower[i], self.upper[i])
                
    def replace_population(self, new_population):
        self.population = new_population

    def run(self):
        self.initialize_population()
        for generation in range(self.generations):
            for individual in self.population:
                self.evaluate_individual(individual)

            new_population = []
            for _ in range(self.pop_size // 2):
                parent1 = self.select_parents()
                parent2 = self.select_parents()
                child1, child2 = self.crossover(parent1, parent2)
                self.mutate(child1)
                self.mutate(child2)
                new_population.extend([child1, child2])

            for individual in new_population:
                self.evaluate_individual(individual)

            self.replace_population(new_population)
            
            # Track best individual
            best_individual = max(self.population, key=lambda ind: ind.fitness)
            print(f"Generation {generation + 1}: Best Fitness = {best_individual.fitness}")
        
        return max(self.population, key=lambda ind: ind.fitness)

# Example usage
model_file = 'model.lp'
solution_file = 'solution.lp'
crossover_rate = 0.8
mutation_rate = 0.1
generations = 50

ga = GeneticAlgorithm(model_file, solution_file, crossover_rate, mutation_rate, generations)
best_individual = ga.run()
print(f"Best Individual: {best_individual.genes} with Fitness: {best_individual.fitness}")
