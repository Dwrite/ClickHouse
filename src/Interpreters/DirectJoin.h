#pragma once


#include <Core/Block.h>

#include <Interpreters/IJoin.h>
#include <Interpreters/TableJoin.h>

#include <QueryPipeline/SizeLimits.h>

#include <Interpreters/IKeyValueEntity.h>
#include <Storages/IStorage_fwd.h>

namespace DB
{

class NotJoinedBlocks;

class DirectKeyValueJoin : public IJoin
{
public:
    DirectKeyValueJoin(
        std::shared_ptr<TableJoin> table_join_,
        const Block & right_sample_block_,
        std::shared_ptr<const IKeyValueEntity> storage_);

    DirectKeyValueJoin(
        std::shared_ptr<TableJoin> table_join_,
        const Block & right_sample_block_,
        std::shared_ptr<const IKeyValueEntity> storage_,
        const Block & right_sample_block_with_storage_column_names_);

    std::string getName() const override { return "DirectKeyValueJoin"; }
    virtual const TableJoin & getTableJoin() const override { return *table_join; }

    virtual bool addBlockToJoin(const Block &, bool) override;
    virtual void checkTypesOfKeys(const Block &) const override;

    /// Join the block with data from left hand of JOIN to the right hand data (that was previously built by calls to addBlockToJoin).
    /// Could be called from different threads in parallel.
    virtual void joinBlock(Block & block, std::shared_ptr<ExtraBlock> &) override;

    virtual size_t getTotalRowCount() const override { return 0; }

    virtual size_t getTotalByteCount() const override { return 0; }

    virtual bool alwaysReturnsEmptySet() const override { return false; }

    virtual bool isFilled() const override { return true; }

    virtual IBlocksStreamPtr
    getNonJoinedBlocks(const Block &, const Block &, UInt64) const override
    {
        return nullptr;
    }

private:
    std::shared_ptr<TableJoin> table_join;
    std::shared_ptr<const IKeyValueEntity> storage;
    Block right_sample_block;
    Block right_sample_block_with_storage_column_names;
    Block sample_block_with_columns_to_add;
    LoggerPtr log;

};

}
