#include <clang/Frontend/FrontendAction.h>
#include <fstream>
#include <sstream>

#include "do_visit.hpp"

file_ctx_t::file_ctx_t(const std::string& f)
: orig_file(f), args({
                    "cppmutate", // Tool name, argv[0]
                    f.c_str(),
                    "--",
                    DEFAULT_FLAGS,
                    DEFAULT_INCLUDES,
                }),
  argc(args.size()), parser(clang::tooling::CommonOptionsParser::create(argc, args.data(), TC)),
  tool(std::make_shared<clang::tooling::ClangTool>(parser->getCompilations(),
                                                   parser->getSourcePathList()))
{
    std::ifstream file(f);
    if (!file)
    {
        std::cerr << "Failed to open file: " << f << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    cur_code = buffer.str();
    file.close();
}

template <typename T>
clang::SourceRange get_source_range_impl(T nd, clang::ASTContext& context)
{
    auto&                 SM  = context.getSourceManager();
    clang::SourceLocation loc = SM.getFileLoc(nd->getBeginLoc());
    if (!SM.isInMainFile(loc))
    {
        return clang::SourceRange();
    }
    auto r   = nd->getSourceRange();
    auto beg = r.getBegin();
    auto end = r.getEnd();
    if (beg > end)
    {
        // return clang::SourceRange(end, beg);
        return clang::SourceRange();
    }
    return r;
}

template <>
clang::SourceRange get_source_range_impl(clang::SourceRange r, clang::ASTContext& _context)
{
    return r;
}

template <>
clang::SourceRange get_source_range_impl(clang::SourceLocation r, clang::ASTContext& _context)
{
    return r;
}

MutateVisitor::MutateVisitor(clang::ASTContext& context, clang::Rewriter& rewr,
                             std::shared_ptr<mutator_t> cur_env)
: context(context), rewr(rewr), cur(cur_env)
{}

#define SKIP_INCLUDE(st)                                                                           \
    {                                                                                              \
        auto&                 SM  = context.getSourceManager();                                    \
        clang::SourceLocation loc = SM.getFileLoc(st->getBeginLoc());                              \
        if (!SM.isInMainFile(loc))                                                                 \
        {                                                                                          \
            return true;                                                                           \
        }                                                                                          \
    }

template <typename T>
std::string MutateVisitor::get_content(T t)
{
    clang::SourceRange r = get_source_range_impl(t, context);
    if (r.isInvalid())
    {
        return "";
    }
    return rewr.getRewrittenText(r);
}

template <typename T>
bool MutateVisitor::do_job(T t, std::string cls_name)
{
    if (cur->mutate_done)
    {
        return false;
    }
    auto as = AST_strore_t(cls_name, get_content(t));
    return cur->do_job(as, get_source_range_impl(t, context), context, rewr);
}

// -------------------------
// bool VisitAttr(Attr* nd)
// {
//     cout << "Attr: " << nd->getAttrName() << " Cont: " << get_content2(nd) << endl;
//     return true;
// }

bool MutateVisitor::VisitStmt(clang::Stmt* nd)
{
    SKIP_INCLUDE(nd);
    return do_job(nd, nd->getStmtClassName());
}

bool MutateVisitor::VisitBinaryOperator(clang::BinaryOperator* nd)
{
    SKIP_INCLUDE(nd);
    return do_job(nd, nd->getStmtClassName());
}

bool MutateVisitor::VisitUnaryOperator(clang::UnaryOperator* nd)
{
    SKIP_INCLUDE(nd);
    return do_job(nd, nd->getStmtClassName());
}

// bool VisitType(Type* nd)
// {
//     cout << "Type: " << nd->getTypeClassName() << endl;
//     return true;
// }

bool MutateVisitor::VisitTypeLoc(clang::TypeLoc nd)
{
    auto p = &nd;
    SKIP_INCLUDE(p);
    return do_job(p, p->getType()->getTypeClassName());
}

bool MutateVisitor::VisitDecl(clang::Decl* nd)
{
    SKIP_INCLUDE(nd);
    return do_job(nd, nd->getDeclKindName());
}

// --------------------------

MutateASTConsumer::MutateASTConsumer(clang::ASTContext& context, clang::Rewriter& rewr,
                                     std::shared_ptr<mutator_t> cur_env)
: visitor(context, rewr, cur_env)
{}

void MutateASTConsumer::HandleTranslationUnit(clang::ASTContext& context)
{
    visitor.TraverseDecl(context.getTranslationUnitDecl());
}

MutateASTAction::MutateASTAction(std::shared_ptr<file_ctx_t>& file_ctx,
                                 std::shared_ptr<mutator_t>   cur_mutatot)
: file_ctx(file_ctx), cur_mutator(cur_mutatot)
{}

std::unique_ptr<clang::ASTConsumer> MutateASTAction::CreateASTConsumer(clang::CompilerInstance& CI,
                                                                       clang::StringRef file)
{
    rewr.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutateASTConsumer>(CI.getASTContext(), rewr, cur_mutator);
}

void MutateASTAction::ExecuteAction()
{
    clang::CompilerInstance& CI = getCompilerInstance();
    CI.getDiagnostics().setClient(new IgnoreDiagnosticsConsumer, true);

    clang::ASTFrontendAction::ExecuteAction();
}

void MutateASTAction::EndSourceFileAction()
{
    std::string              ns;
    llvm::raw_string_ostream os(ns);
    rewr.getEditBuffer(rewr.getSourceMgr().getMainFileID()).write(os);
    os.flush();
    file_ctx->cur_code = ns;
}