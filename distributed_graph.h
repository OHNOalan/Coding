#include <algorithm>
#include <iostream>
#include <map>
#include <mpi.h>
#include <vector>

// A Mimic of a Distributed Graph Storage Engine
class DistributedGraph {
private:
    int my_rank;
    int world_size;

    // The "Local Storage" (RAM)
    // Map: Global Vertex ID -> List of Outgoing Neighbors
    std::map<int, std::vector<int>> local_adjacency_list;

public:
    DistributedGraph(int rank, int size) : my_rank(rank), world_size(size) {}

    // --- 1. The Partitioning Logic (The Sharding Function) ---
    // Decides who owns a vertex. Using simple Modulo for this demo.
    // In production, this would be a Consistent Hash or Range Partition.
    int get_owner(int global_vertex_id) const { return global_vertex_id % world_size; }

    // --- 2. Customization (Loading the Graph) ---
    // This mimics loading a massive dataset.
    // We pass a "potential" edge, but the node only stores it if it OWNS the source.
    void add_edge_if_owner(int source, int dest) {
        if (get_owner(source) == my_rank) {
            local_adjacency_list[source].push_back(dest);
            // printf("[Rank %d] I own Vertex %d. Storing edge %d -> %d\n", my_rank, source, source, dest);
        } else {
            // In a real system, we might forward this to the owner.
            // Here, we simply ignore it (assuming the caller broadcasts the graph def).
        }
    }

    // --- 3. The Constraint Enforcer ---
    // This function mimics the physical memory isolation.
    // If you try to access a node you don't own, it throws an error.
    std::vector<int> get_local_neighbors(int vertex_id) {
        // Security Check: Do I own this data?
        if (get_owner(vertex_id) != my_rank) {
            std::cerr << "CRITICAL ERROR: Rank " << my_rank << " tried to access remote Vertex " << vertex_id
                      << " (Owned by Rank " << get_owner(vertex_id) << ")" << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1); // Crash the program simulating SegFault
        }

        // Return data from local RAM
        if (local_adjacency_list.find(vertex_id) != local_adjacency_list.end()) {
            return local_adjacency_list[vertex_id];
        }
        return {}; // No outgoing edges
    }

    // Check if a vertex is stored locally
    bool is_local(int vertex_id) const { return get_owner(vertex_id) == my_rank; }
};
