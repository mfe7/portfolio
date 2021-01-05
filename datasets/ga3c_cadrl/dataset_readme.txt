dataset_readme.txt
Last updated: 6/7/18 by mfe

-------------------------------------------------------------

There are 4 datasets included in this Dropbox folder.
There are 2 styles (regular, RNN) and each style has 2 parts (test, train).

The datasets are in pickle format (https://docs.python.org/3/library/pickle.html).
Using python, it is easy to pickle.load the filename, which will de-serialize the data into a Python datatype.

For example, in python 2.7:
import pickle
data = pickle.load(open(filename_str))
states = data[0]
actions = data[1]
values = data[2]
states.shape ---> (100000, 26)
states[0,:] ---> one observation vector

The data is structured as:
data = [states, actions, values] where
- states is an np.array of stacked observations (num observations (100,000) x observation length (26))
- actions is the associated speed and delta heading angle for each observation (num observations (100,000) x num actions (2))
- values is the associated V(state) according to a reasonably-well trained version of CADRL (num observations (100,000) x 1)

In the paper, we used this dataset as a way of initializing the NN to a decent policy.
You should be able to do supervised learning with states as inputs, and actions and/or values as outputs, regardless of the exact type of RL you're going to use.
The supervised learning took a few minutes to converge.
As mentioned in the paper, the solution after SL is still not good and will lead to many collisions.
It's just a way to teach the most fundamental skill of moving toward the goal, whereas the skill of collision avoidance is learned with RL.


For the regular dataset, the observations as structured as:
[dist to goal, heading to goal, pref speed, radius, 
  other px, other py, other vx, other vy, other radius, combined radius, distance between, is_on,
  other px, other py, other vx, other vy, other radius, combined radius, distance between, is_on,
  other px, other py, other vx, other vy, other radius, combined radius, distance between, is_on]
which should all be self-explanatory (is_on is a binary flag of whether that agent exists (a simple way of handling variable # agents))

For the RNN dataset, the observations are structured as:
[num other agents, dist to goal, heading to goal, pref speed, radius,
	other px, other py, other vx, other vy, other radius, combined radius, distance between,
	other px, other py, other vx, other vy, other radius, combined radius, distance between,
	other px, other py, other vx, other vy, other radius, combined radius, distance between]
where the entries for other agents can be ignored beyond num other agents (I think they're set to zeros).

For both observation styles, all of these values are in the agent's local frame (x axis is from agent's center to goal).

The training sets have 100k entries, and the test sets have 20k entries.
The data was recorded sequentially, so neighboring observations will be highly correlated (might want to randomize selection).

-------------------------------------------------------------------------------------------------

Please cite the following work if you see fit:
@article{everett2018motion,
  title={Motion Planning Among Dynamic, Decision-Making Agents with Deep Reinforcement Learning},
  author={Everett, Michael and Chen, Yu Fan and How, Jonathan P},
  journal={arXiv preprint arXiv:1805.01956},
  year={2018}
}