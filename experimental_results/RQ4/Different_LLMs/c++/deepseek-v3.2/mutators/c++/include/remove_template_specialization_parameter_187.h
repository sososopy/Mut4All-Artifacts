//header file
#ifndef MUTATOR_187_H
#define MUTATOR_187_H

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Transformer/RangeSelector.h"
#include "clang/Tooling/Transformer/Stencil.h"
#include "clang/Tooling/Transformer/Transformer.h"
#include "llvm/Support/raw_ostream.h"
#include <random>
#include <string>
#include <vector>

using namespace clang;

class MutatorFrontendAction_187 {
public:
    class MutatorASTConsumer_187 : public ASTConsumer {
    public:
        explicit MutatorASTConsumer_187(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;

    private:
        Rewriter &TheRewriter;
    };

    class Callback : public MatchFinder::MatchCallback {
    public:
        explicit Callback(Rewriter &R) : Rewrite(R) {}
        void run(const MatchFinder::MatchResult &Result) override;

    private:
        Rewriter &Rewrite;
    };

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_187>(TheRewriter);
    }

private:
    Rewriter TheRewriter;
};

#endif

