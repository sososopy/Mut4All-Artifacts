//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Exception_Specification_289
 */ 
class MutatorFrontendAction_289 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(289)

private:
    class MutatorASTConsumer_289 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_289(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_289.h"

// ========================================================================================================
#define MUT289_OUTPUT 1

void MutatorFrontendAction_289::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (FD->isTemplateInstantiation()) {
        auto pos = declaration.find("{");
        if (pos != std::string::npos) {
          declaration.insert(pos, " throw(UninstantiatedType) ");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_289::MutatorASTConsumer_289::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}