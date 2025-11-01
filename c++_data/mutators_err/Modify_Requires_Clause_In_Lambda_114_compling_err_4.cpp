//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Refactoring.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Modify_Requires_Clause_In_Lambda_114
 */ 
class MutatorFrontendAction_114 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(114)

private:
    class MutatorASTConsumer_114 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_114(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Modify_Requires_Clause_In_Lambda_114.h"

// ========================================================================================================
#define MUT114_OUTPUT 1

void MutatorFrontendAction_114::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaWithRequires")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto RequiresClause = LambdaExpr->getTrailingRequiresClause();
        if (RequiresClause) {
            // Get the source code text of the requires clause
            auto RequiresText = Lexer::getSourceText(CharSourceRange::getTokenRange(RequiresClause->getSourceRange()), 
                                                     *Result.SourceManager, 
                                                     Result.Context->getLangOpts());

            // Find a suitable in-scope variable or introduce a dummy variable
            std::string additionalRequirement;
            if (auto *Func = dyn_cast<FunctionDecl>(LambdaExpr->getLambdaClass()->getParent())) {
                for (auto *Param : Func->parameters()) {
                    additionalRequirement = Param->getNameAsString();
                    break;
                }
            }

            if (additionalRequirement.empty()) {
                additionalRequirement = "dummyVar";
                Rewrite.InsertTextBefore(LambdaExpr->getBeginLoc(), "constexpr int dummyVar = 0;\n");
            }

            // Perform mutation on the source code text by applying string replacement
            std::string MutatedRequires = RequiresText.str() + "; " + additionalRequirement + ";";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(RequiresClause->getSourceRange()), MutatedRequires);
        }
    }
}
  
void MutatorFrontendAction_114::MutatorASTConsumer_114::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(hasTrailingRequiresClause()).bind("LambdaWithRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}