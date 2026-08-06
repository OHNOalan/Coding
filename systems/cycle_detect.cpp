#include <iostream>
#include <map>
#include <mpi.h>
#include <unistd.h> // For sleep()
#include <vector>

// --- 1. The Distributed Graph Engine (The "Database") ---
class DistributedGraph {
private:
    int my_rank;
    int world_size;
    // Local RAM: Maps Vertex ID -> List of Outgoing Neighbors
    std::map<int, std::vector<int>> local_adjacency_list;

public:
    DistributedGraph(int rank, int size) : my_rank(rank), world_size(size) {}

    // SHARDING FUNCTION: Determines who owns a vertex
    // In this simulation: Rank 0 owns Vertex 0, Rank 1 owns Vertex 1, etc.
    int get_owner(int vertex_id) const { return vertex_id % world_size; }

    // INGESTION: Store edge only if I own the source vertex
    void add_edge_if_owner(int source, int dest) {
        if (get_owner(source) == my_rank) {
            local_adjacency_list[source].push_back(dest);
        }
    }

    // RETRIEVAL: Strict check to enforce distributed constraints
    std::vector<int> get_local_neighbors(int vertex_id) {
        // Enforce physical memory isolation
        if (get_owner(vertex_id) != my_rank) {
            std::cerr << "[CRITICAL ERROR] Rank " << my_rank << " tried to read remote Vertex " << vertex_id
                      << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        return local_adjacency_list[vertex_id];
    }
};

// --- Constants ---
const int TAG_PROBE = 1;

int main(int argc, char **argv) {
    // 1. Initialize MPI
    MPI_Init(&argc, &argv);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size < 3) {
        if (world_rank == 0) std::cerr << "Error: Run with -np 3" << std::endl;
        MPI_Finalize();
        return 1;
    }

    // 2. Instantiate the Storage Engine
    DistributedGraph graph(world_rank, world_size);

    // 3. Define the Graph Topology (Data Ingestion)
    // We define the edges globally, but 'add_edge_if_owner' ensures
    // each node only stores its own part.
    // Topology: 0->1, 1->2, 2->0 (Ring Cycle)
    std::vector<std::pair<int, int>> raw_edges = {{0, 1}, {1, 2}, {2, 0}};

    for (auto edge: raw_edges) {
        graph.add_edge_if_owner(edge.first, edge.second);
    }

    // Wait for everyone to finish loading data
    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0) printf("--- Graph Loaded & Distributed ---\n");


    // 4. The Algorithm (Chandy-Misra-Haas)

    // Step A: Initiation (Node 0 suspects a deadlock)
    // In this demo, we assume Rank N maps to Node N.
    int my_node_id = world_rank;

    if (world_rank == 0) {
        printf("[Node 0] Initiating Deadlock Detection...\n");
        int initiator = 0;

        // Retrieve my neighbors from the Graph Engine
        std::vector<int> neighbors = graph.get_local_neighbors(my_node_id);

        for (int neighbor: neighbors) {
            // Note: In a real generic graph, 'neighbor' might be on a different rank.
            // We calculate the destination rank using get_owner().
            int dest_rank = graph.get_owner(neighbor);

            printf("[Node 0] Sending PROBE(Init:0) to Node %d (Rank %d)\n", neighbor, dest_rank);
            MPI_Send(&initiator, 1, MPI_INT, dest_rank, TAG_PROBE, MPI_COMM_WORLD);
        }
    }

    // Step B: Event Loop
    while (true) {
        int incoming_initiator;
        MPI_Status status;

        // Blocking Receive
        MPI_Recv(&incoming_initiator, 1, MPI_INT, MPI_ANY_SOURCE, TAG_PROBE, MPI_COMM_WORLD, &status);

        // Logic: Cycle Detected?
        if (incoming_initiator == my_node_id) {
            printf("\n!!! [Node %d] CYCLE DETECTED! Probe returned to me. !!!\n\n", my_node_id);
            break;
        }

        // Logic: Forward the Probe
        // 1. Look up MY dependencies (neighbors) from the Graph Engine
        std::vector<int> neighbors = graph.get_local_neighbors(my_node_id);

        sleep(1); // Simulate network latency

        for (int neighbor_id: neighbors) {
            // 2. Calculate where that neighbor lives
            int dest_rank = graph.get_owner(neighbor_id);

            printf("[Node %d] Forwarding PROBE(Init: %d) -> Node %d (Rank %d)\n", my_node_id, incoming_initiator,
                   neighbor_id, dest_rank);

            MPI_Send(&incoming_initiator, 1, MPI_INT, dest_rank, TAG_PROBE, MPI_COMM_WORLD);
        }

        // Termination for simulation (to exit cleanly)
        if (incoming_initiator == 0 && world_rank != 0) break;
    }

    MPI_Finalize();
    return 0;
}
