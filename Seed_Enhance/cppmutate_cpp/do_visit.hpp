#pragma once

#include "mutator.hpp"
#include <clang/Basic/SourceLocation.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <string>

#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#define DEFAULT_INCLUDES                                                                                \
    "-I/usr/lib/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1",                         \
        "-I/usr/lib/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/x86_64-pc-linux-gnu", \
        "-I/usr/lib/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/backward",            \
        "-I/usr/lib/gcc/x86_64-pc-linux-gnu/15.1.1/include", "-I/usr/local/include",                    \
        "-I/usr/lib/gcc/x86_64-pc-linux-gnu/15.1.1/include-fixed", "-I/usr/include",                    \
        "-I/usr/include/c++/15.1.1"

#define DEFAULT_FLAGS                                                                              \
    "--std=c++17", "-nostdinc", "-nostdinc++", "-Wno-inline-namespace-reopened-noninline", "-w"

struct file_ctx_t
{
    std::string orig_file;
    std::vector<const char*> args;
    int                      argc;
    llvm::cl::OptionCategory TC = llvm::cl::OptionCategory("CppMutate Tool Options");
    llvm::Expected<clang::tooling::CommonOptionsParser> parser;
    std::shared_ptr<clang::tooling::ClangTool>          tool;
    std::string                                         cur_code;

    file_ctx_t() = delete;
    file_ctx_t(const std::string& f);
};

template <typename T>
clang::SourceRange get_source_range_impl(T nd, clang::ASTContext& context);

template <>
clang::SourceRange get_source_range_impl(clang::SourceRange r, clang::ASTContext& _context);

template <>
clang::SourceRange get_source_range_impl(clang::SourceLocation r, clang::ASTContext& _context);

struct MutateVisitor : public clang::RecursiveASTVisitor<MutateVisitor>
{
    clang::ASTContext&         context;
    clang::Rewriter&           rewr;
    std::shared_ptr<mutator_t> cur;
    explicit MutateVisitor(clang::ASTContext& context, clang::Rewriter& rewr,
                           std::shared_ptr<mutator_t> cur_env);

    template <typename T>
    std::string get_content(T t);

    template <typename T>
    bool do_job(T t, std::string cls_name);

    // -------------------------
    // bool VisitAttr(Attr* nd);
    bool VisitStmt(clang::Stmt* nd);
    bool VisitBinaryOperator(clang::BinaryOperator* nd);
    bool VisitUnaryOperator(clang::UnaryOperator* nd);
    // bool VisitType(Type* nd);

    bool VisitTypeLoc(clang::TypeLoc nd);
    bool VisitDecl(clang::Decl* nd);
};

struct MutateASTConsumer : public clang::ASTConsumer
{
    MutateVisitor visitor;
    explicit MutateASTConsumer(clang::ASTContext& context, clang::Rewriter& rewr,
                               std::shared_ptr<mutator_t> cur_env);

    virtual void HandleTranslationUnit(clang::ASTContext& context) override;
};

class IgnoreDiagnosticsConsumer : public clang::DiagnosticConsumer
{
public:
    void HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                          const clang::Diagnostic&        Info) override
    {}
};

extern std::shared_ptr<mutator_t> cur_mutator_ctx;
struct MutateASTAction : public clang::ASTFrontendAction
{
    clang::Rewriter              rewr;
    std::shared_ptr<file_ctx_t>& file_ctx;
    std::shared_ptr<mutator_t>   cur_mutator;

    MutateASTAction(std::shared_ptr<file_ctx_t>& file_ctx, std::shared_ptr<mutator_t> cur_mutatot);

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& CI,
                                                          clang::StringRef         file) override;
    void                                ExecuteAction() override;
    void                                EndSourceFileAction() override;
};