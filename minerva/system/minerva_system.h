#pragma once
#include "common/singleton.h"
#include "dag/logical_dag.h"
#include "dag/physical_dag.h"
#include "procedures/dag_procedure.h"
#include "narray/narray.h"
#include "device/device_factory.h"
#include "common/inspector.h"
#include <unordered_set>

namespace minerva {

class ExpandEngine;
class PhysicalEngine;
class DataStore;
class ThreadPool;
class ImplDecider;

class MinervaSystem :
  public EverlastingSingleton<MinervaSystem> {
  friend class NArray;
  friend class EverlastingSingleton<MinervaSystem>;
  friend class Inspector<MinervaSystem>;
 public:
  ~MinervaSystem();
  void Initialize(int* argc, char*** argv);
  void Finalize();
  LogicalDag& logical_dag() { return logical_dag_; }
  PhysicalDag& physical_dag() { return physical_dag_; }
  DeviceFactory& device_factory() { return *device_factory_; }
  PhysicalEngine& physical_engine() { return *physical_engine_; }

  void SetImplDecider(ImplDecider* );
  float* GetValue(NArray& narr);
  void Eval(const std::vector<NArray>& narrs);
  void EvalAsync(const std::vector<NArray>& narrs);
  void WaitForEvalFinish();

  void set_device_id(uint64_t id);
  uint64_t device_id() const;
  uint64_t CreateCPUDevice();
  uint64_t CreateGPUDevice(int gid);
  uint64_t CreateGPUDevice(int gid, int num_stream);
  Device* GetDevice(uint64_t id);
  uint64_t GenerateDataID();

 private:
  DISALLOW_COPY_AND_ASSIGN(MinervaSystem);
  MinervaSystem();
  void LoadBuiltinDagMonitors();
  void IncrExternRC(LogicalDag::DNode*, int amount = 1);
  void GeneratePhysicalDag(const std::vector<uint64_t>& lids);
  void ExecutePhysicalDag(const std::vector<uint64_t>& pids);

 private:
  LogicalDag logical_dag_;
  PhysicalDag physical_dag_;

  ExpandEngine* expand_engine_;
  PhysicalEngine* physical_engine_;

  DeviceFactory* device_factory_;
  uint64_t device_id_;

  std::unordered_set<uint64_t> extern_rc_changed_ldnodes_;
};

} // end of namespace minerva
