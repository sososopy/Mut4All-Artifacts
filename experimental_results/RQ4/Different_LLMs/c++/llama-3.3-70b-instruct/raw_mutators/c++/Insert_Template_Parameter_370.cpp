```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Template_Parameter_370
 */ 
class MutatorFrontendAction_370 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(370)

private:
    class MutatorASTConsumer_370 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_370(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Template_Parameter_370.h"

// ========================================================================================================
#define MUT370_OUTPUT 1

void MutatorFrontendAction_370::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TAD = Result.Nodes.getNodeAs<clang::TemplateAliasDecl>("TemplateAliasDecl")) {
      //Filter nodes in header files
      if (!TAD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TAD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TAD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (!TAD->getTemplateParameters()) {
        declaration.insert(declaration.find("using"), " template <typename K>");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TAD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_370::MutatorASTConsumer_370::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateAliasDecl().bind("TemplateAliasDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}