//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/ExprCXX.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/ExprConcepts.h"
using namespace clang;
using namespace clang::ast_matchers;

/**
 * Misuse_Concept_In_Requires_Expression_323
 */ 
class MutatorFrontendAction_323 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(323)

private:
    class MutatorASTConsumer_323 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_323(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl *> concepts;
    };
};

//source file
#include "../include/Misuse_Concept_In_Requires_Expression_323.h"

// ========================================================================================================
#define MUT323_OUTPUT 1

void MutatorFrontendAction_323::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Record the candidate existing concept names for insertion
      concepts.push_back(CD);
    } else if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>(
                   "RequiresExpr")) {
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getBeginLoc()))
        return;
      if (concepts.empty())
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             RE->getSourceRange());
      size_t index = getrandom::getRandomIndex(concepts.size() - 1);
      auto target = concepts[index];
      //Perform mutation on the source code text by applying string replacement
      string ins = target->getNameAsString() + " ";
      if (content.find("requires") != string::npos)
        content.insert(content.find("requires") + 9, ins);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RE->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_323::MutatorASTConsumer_323::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = conceptDecl().bind("Concepts");
    auto requires_matcher = requiresExpr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(requires_matcher, &callback);
    matchFinder.matchAST(Context);
}