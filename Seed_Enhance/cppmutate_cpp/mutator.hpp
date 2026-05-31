#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <cstddef>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <string>
#include <unordered_map>

#include "fenwick.hpp"

struct AST_strore_t
{
    std::string class_name;
    std::string content;

    AST_strore_t();
    AST_strore_t(const std::string& name, const std::string& cont);

    friend std::ostream& operator<<(std::ostream& os, const AST_strore_t& ast);

    bool operator==(const AST_strore_t& other) const;
};
template <>
struct std::hash<AST_strore_t>
{
    std::size_t operator()(const AST_strore_t& ast) const;
};

struct mutator_t
{
    size_t MAX_ANALYZE_DEPTH;
    double MIN_CHOOSE;
    double CHOOSE_ADJ_RATE;
    struct entity_store_t
    {
        std::string                                               class_name;
        std::deque<std::shared_ptr<AST_strore_t>>                 entities;
        std::unordered_map<std::shared_ptr<AST_strore_t>, size_t> entity_map;
        fenwick_t<double>                                         weights;
        double                                                    total_weight = 0.0;

        entity_store_t();
        entity_store_t(const std::string& name);

        void try_add_entity(const std::shared_ptr<AST_strore_t>& entity);

        template <typename rnd, typename distribution_t = std::uniform_real_distribution<double>>
        // The random device must be initialized outside this function
        std::shared_ptr<AST_strore_t> get_random_entity(rnd& rd);

        size_t try_get_entity(const std::shared_ptr<AST_strore_t>& entity) const;

        size_t get_or_insert_entity(const std::shared_ptr<AST_strore_t>& entity);

        void try_modify_weight(const size_t idx, const double adj_w);
    };

    enum class job_ty
    {
        ADD,
        MODIFY,
        ADJUST,
    };

    job_ty job_type;

    std::deque<std::string>                         skip_cls;
    size_t                                          analysis_depth = 0;
    bool                                            mutate_done;
    std::unordered_map<std::string, entity_store_t> entity_stores;
    std::random_device                              rd_dev;
    std::mt19937                                    rng;

    mutator_t();

    void do_job_add(AST_strore_t& ast, clang::SourceRange r, clang::ASTContext& ctx,
                    clang::Rewriter& rewr);

    bool do_job_modify(AST_strore_t& ast, clang::SourceRange r, clang::ASTContext& ctx,
                       clang::Rewriter& rewr);

    void do_job_adjust(AST_strore_t& ast, clang::SourceRange r, clang::ASTContext& ctx,
                       clang::Rewriter& rewr, bool up = false);

    bool do_job(AST_strore_t& ast, clang::SourceRange r, clang::ASTContext& ctx,
                clang::Rewriter& rewr, bool up = false);
};
