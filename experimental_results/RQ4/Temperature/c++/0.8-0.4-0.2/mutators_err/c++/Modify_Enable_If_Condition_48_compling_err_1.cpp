//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Enable_If_Condition_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Enable_If_Condition_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getDescribedFunctionTemplate()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find("std::enable_if");
        if (pos != std::string::npos) {
          size_t condStart = declaration.find("(", pos);
          size_t condEnd = declaration.find(",", condStart);
          if (condStart != std::string::npos && condEnd != std::string::npos) {
            std::string mutatedCondition = "T::non_existent_member";
            declaration.replace(condStart + 1, condEnd - condStart - 1, mutatedCondition);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasDescendant(templateArgument().bind("funcDecl"))).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}