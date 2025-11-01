//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Remove_Typename_In_Dependent_Context_171
 */ 
class MutatorFrontendAction_171 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(171)

private:
    class MutatorASTConsumer_171 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_171(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_typename_in_dependent_context_171.h"

// ========================================================================================================
#define MUT171_OUTPUT 1

void MutatorFrontendAction_171::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Expr = Result.Nodes.getNodeAs<clang::DependentNameTypeLoc>("DependentType")) {
        //Filter nodes in header files
        if (!Expr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Expr->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(Expr->getSourceRange()), 
                                               Result.Context->getSourceManager(), 
                                               Result.Context->getLangOpts());

        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText = sourceText.str();
        size_t pos = mutatedText.find("typename ");
        if (pos != std::string::npos) {
            mutatedText.erase(pos, 9); // Remove 'typename '
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Expr->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_171::MutatorASTConsumer_171::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = typeLoc(dependentNameTypeLoc()).bind("DependentType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}