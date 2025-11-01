//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Constraints_With_Self_Referential_Constructs_481
 */ 
class MutatorFrontendAction_481 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(481)

private:
    class MutatorASTConsumer_481 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_481(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_constraints_with_self_referential_constructs_481.h"

// ========================================================================================================
#define MUT481_OUTPUT 1

void MutatorFrontendAction_481::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithRequires")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t requiresPos = declaration.find("requires");
      if (requiresPos != std::string::npos) {
          size_t endPos = declaration.find(">", requiresPos);
          if (endPos != std::string::npos) {
              std::string originalConstraint = declaration.substr(requiresPos, endPos - requiresPos + 1);
              std::string mutatedConstraint = "requires std::is_constructible_v<F, std::decay_t<F>>";
              declaration.replace(requiresPos, originalConstraint.length(), mutatedConstraint);
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_481::MutatorASTConsumer_481::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasRequiresClause()).bind("FunctionWithRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}