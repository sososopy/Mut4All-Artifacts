//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * alter_template_parameter_constraints_525
 */ 
class MutatorFrontendAction_525 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(525)

private:
    class MutatorASTConsumer_525 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_525(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/alter_template_parameter_constraints_525.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT525_OUTPUT 1

void MutatorFrontendAction_525::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithRequires")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->getTrailingRequiresClause()) {
        //Get the source code text of target node
        auto requiresClause = stringutils::rangetoStr(*(Result.SourceManager),
                                                      MT->getTrailingRequiresClause()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string newCondition = " && dim + rank == 4u";
        requiresClause.insert(requiresClause.rfind(')'), newCondition);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getTrailingRequiresClause()->getSourceRange()), requiresClause);
      }
    }
}
  
void MutatorFrontendAction_525::MutatorASTConsumer_525::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = functionDecl(hasRequiresClause()).bind("FunctionWithRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}