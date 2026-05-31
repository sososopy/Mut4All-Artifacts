#include "conf.hpp"
#include "mutator.hpp"

AST_strore_t::AST_strore_t() = default;
AST_strore_t::AST_strore_t(const std::string& name, const std::string& cont)
: class_name(name), content(cont)
{}

std::ostream& operator<<(std::ostream& os, const AST_strore_t& ast)
{
    os << "Class: " << ast.class_name << "\n"
       << "Content: " << ast.content << "\n";
    return os;
}

bool AST_strore_t::operator==(const AST_strore_t& other) const
{
    return class_name == other.class_name && content == other.content;
}

std::size_t std::hash<AST_strore_t>::operator()(const AST_strore_t& ast) const
{
    return std::hash<std::string>()(ast.class_name) ^ std::hash<std::string>()(ast.content);
}

mutator_t::entity_store_t::entity_store_t() = default;
mutator_t::entity_store_t::entity_store_t(const std::string& name) : class_name(name) {}

void mutator_t::entity_store_t::try_add_entity(const std::shared_ptr<AST_strore_t>& entity)
{
    if (!entity)
    {
        return;
    }
    if (entity_map.find(entity) != entity_map.end())
    {
        // Entity already exists, do not add
        return;
    }
    size_t len = entity->content.length();
    double init_weight;
    if (len < 100)
    {
        init_weight = 1.0;
    }
    else
    {
        init_weight = 0.7 / (len - 100 + 1) + conf().get<double>("MIN_CHOOSE");
    }
    size_t idx = entities.size();
    entities.push_back(entity);
    entity_map[entity] = idx;
    weights.push_back(init_weight); // Initialize weight to 1.0
    total_weight += init_weight;
}

template <typename rnd, typename distribution_t>
// The random device must be initialized outside this function
std::shared_ptr<AST_strore_t> mutator_t::entity_store_t::get_random_entity(rnd& rd)
{
    if (entities.empty())
    {
        return nullptr;
    }
    distribution_t dist(0.0, total_weight);
    double         random_value = dist(rd);
    size_t         idx          = weights.lower_bound_traditional(random_value);
    if (idx >= entities.size())
    {
        return nullptr; // Should not happen
    }
    auto entity = entities[idx];
    if (!entity)
    {
        return nullptr; // Should not happen
    }
    return entity;
}

size_t mutator_t::entity_store_t::try_get_entity(const std::shared_ptr<AST_strore_t>& entity) const
{
    auto it = entity_map.find(entity);
    if (it != entity_map.end())
    {
        return it->second;
    }
    return static_cast<size_t>(-1); // Not found
}

size_t mutator_t::entity_store_t::get_or_insert_entity(const std::shared_ptr<AST_strore_t>& entity)
{
    auto res = try_get_entity(entity);
    if (res == static_cast<size_t>(-1))
    {
        try_add_entity(entity);
        res = try_get_entity(entity);
    }
    return res;
}

void mutator_t::entity_store_t::try_modify_weight(const size_t idx, const double adj_w)
{
    weights.modify(idx, adj_w);
    total_weight += adj_w;
}

mutator_t::mutator_t() : rng(std::mt19937(rd_dev()))
{
    MAX_ANALYZE_DEPTH = conf().get<size_t>("MAX_ANALYZE_DEPTH");
    MIN_CHOOSE        = conf().get<double>("MIN_CHOOSE");
    CHOOSE_ADJ_RATE   = conf().get<double>("CHOOSE_ADJ_RATE");
}

void mutator_t::do_job_add(AST_strore_t& ast, clang::SourceRange r, clang::ASTContext& ctx,
                           clang::Rewriter& rewr)
{
    if (ast.class_name.empty() || ast.content.empty())
    {
        return;
    }
    auto res = std::make_shared<AST_strore_t>(ast);
    entity_stores[ast.class_name].try_add_entity(res);
}

void mutator_t::do_job_adjust(AST_strore_t& ast, clang::SourceRange r, clang::ASTContext& ctx,
                              clang::Rewriter& rewr, bool up)
{
    if (ast.class_name.empty() || ast.content.empty())
    {
        return;
    }
    auto  ast_shared   = std::make_shared<AST_strore_t>(ast);
    auto& entity_store = entity_stores[ast.class_name];
    auto  entity_idx   = entity_store.get_or_insert_entity(ast_shared);

    auto current_w = entity_store.weights.query_at(entity_idx);
    if (current_w <= MIN_CHOOSE)
    {
        return;
    }
    double new_w;
    if (up)
    {
        new_w = current_w * 1.05;
    }
    else
    {
        new_w = current_w * 0.95;
    }
    entity_store.try_modify_weight(entity_idx, new_w - current_w);
}

bool mutator_t::do_job_modify(AST_strore_t& ast, clang::SourceRange r, clang::ASTContext& ctx,
                              clang::Rewriter& rewr)
{
    if (mutate_done) {
        return false;
    }
    if (ast.class_name.empty() || ast.content.empty())
    {
        return true;
    }
    auto ast_shared = std::make_shared<AST_strore_t>(ast);

    auto& entity_store = entity_stores[ast.class_name];
    if (entity_store.entities.size() == 0)
    {
        std::cout << "No entities to modify for class: " << ast.class_name << std::endl;
        return true;
    }

    // Should we modify this entity?
    auto dist = std::uniform_real_distribution<double>(0.0, 1.0);
    if (dist(rng) > 0.08)
    {
        return true;
    }

    // Yes, we should modify it.

    auto new_ast = entity_store.get_random_entity(rng);
    if (!new_ast)
    {
        return true;
    }

    rewr.ReplaceText(r, new_ast->content);
    auto new_ast_idx = entity_store.try_get_entity(new_ast);
    if (new_ast_idx != static_cast<size_t>(-1))
    {
        auto current_w = entity_store.weights.query_at(new_ast_idx);
        auto new_w     = current_w * CHOOSE_ADJ_RATE;
        if (new_w > MIN_CHOOSE)
        {
            entity_store.try_modify_weight(new_ast_idx, new_w - current_w);
        }
    }

    return false;
}

bool mutator_t::do_job(AST_strore_t& ast, clang::SourceRange r, clang::ASTContext& ctx,
                       clang::Rewriter& rewr, bool up)
{
    if (r.isInvalid())
    {
        return true;
    }
    if (std::any_of(skip_cls.begin(), skip_cls.end(),
                    [&ast](const std::string& cls) { return ast.class_name == cls; }))
    {
        return true;
    }
    analysis_depth++;
    if (analysis_depth > MAX_ANALYZE_DEPTH)
    {
        return false;
    }

    bool res = true;

    switch (job_type)
    {
    case job_ty::ADD:
        do_job_add(ast, r, ctx, rewr);
        break;
    case job_ty::MODIFY:
        res = do_job_modify(ast, r, ctx, rewr);
        break;
    case job_ty::ADJUST:
        do_job_adjust(ast, r, ctx, rewr, up);
        break;
    default:
        std::cerr << "Unknown job type: " << static_cast<int>(job_type) << std::endl;
        res = false;
        break;
    }

    return res;
}
