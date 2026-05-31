//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/ExprCXX.h"

/**
 * Replace_DirectInit_Arg_With_AggregateInit_99
 */ 
class MutatorFrontendAction_99 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(99)

private:
    class MutatorASTConsumer_99 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_99(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_DirectInit_Arg_With_AggregateInit_99.h"
#include "clang/AST/ExprCXX.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/ASTContext.h"
#include "stringutils.h"
#include "getrandom.h"

// ========================================================================================================
#define MUT99_OUTPUT 1

void MutatorFrontendAction_99::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *newExpr = Result.Nodes.getNodeAs<clang::CXXNewExpr>("newExpr")) {
        // Filter nodes in header files
        if (!Result.Context->getSourceManager().isWrittenInMainFile(newExpr->getBeginLoc()))
            return;

        // Check if it's a direct new expression (no placement args)
        if (newExpr->getNumPlacementArgs() > 0)
            return;

        QualType newType = newExpr->getType();
        if (!newType->isRecordType())
            return;

        if (!Result.Context->isAggregateType(newType))
            return;

        auto *constructExpr = newExpr->getConstructExpr();
        if (!constructExpr)
            return;

        auto argsText = stringutils::rangetoStr(*Result.Context->getSourceManager(), constructExpr->getSourceRange());
        if (argsText.size() < 2)
            return;

        std::string argsContent = argsText.substr(1, argsText.size() - 2);
        llvm::StringRef ref(argsContent);
        std::vector<llvm::StringRef> splitArgs;
        ref.split(splitArgs, ',');
        if (splitArgs.empty())
            return;

        std::vector<std::string> args;
        for (auto &arg : splitArgs) {
            args.push_back(arg.str().trim());
        }

        if (args.empty())
            return;

        int index = getrandom::getRandomIndex(args.size());
        args[index] = "{" + args[index] + "}";

        std::string newContent;
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0)
                newContent += ", ";
            newContent += args[i];
        }

        std::string newArgsText = "(" + newContent + ")";
        Rewrite.ReplaceText(constructExpr->getSourceRange(), newArgsText);
    }
}

void MutatorFrontendAction_99::MutatorASTConsumer_99::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    StatementMatcher matcher = cxxNewExpr().bind("newExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}