//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Global_Module_Fragment_Before_Module_Declaration_72
 */ 
class MutatorFrontendAction_72 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(72)

private:
    class MutatorASTConsumer_72 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_72(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Global_Module_Fragment_Before_Module_Declaration_72.h"

// ========================================================================================================
#define MUT72_OUTPUT 1

void MutatorFrontendAction_72::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::ModuleDecl>("ModuleDecl")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      //Get the source code text of target node
      auto moduleDeclRange = MD->getSourceRange();
      auto moduleDeclText = stringutils::rangetoStr(*(Result.SourceManager), moduleDeclRange);
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "module;\n/*mut72*/" + moduleDeclText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(moduleDeclRange), mutatedText);
    }
}
  
void MutatorFrontendAction_72::MutatorASTConsumer_72::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = moduleDecl().bind("ModuleDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}