//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_Corruption_164
 */ 
class MutatorFrontendAction_164 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(164)

private:
    class MutatorASTConsumer_164 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_164(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Template_Instantiation_Corruption_164.h"

// ========================================================================================================
#define MUT164_OUTPUT 1

void MutatorFrontendAction_164::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TI = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TI->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Introduce an invalid token within the template parameters
      size_t pos = declaration.find('<');
      if (pos != std::string::npos) {
        declaration.insert(pos + 1, "@"); // @ represents an invalid token
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TI->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_164::MutatorASTConsumer_164::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}