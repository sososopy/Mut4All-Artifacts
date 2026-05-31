//header file
#pragma once
#include "Mutator_base.h"

/**
 * Relax_Template_Template_Constrained_NTTP_60
 */ 
class MutatorFrontendAction_60 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(60)

private:
    class MutatorASTConsumer_60 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_60(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Relax_Template_Template_Constrained_NTTP_60.h"

// ========================================================================================================
#define MUT60_OUTPUT 1

void MutatorFrontendAction_60::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TM = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("TemplateTemplates")) {
      //Filter nodes in header files
      if (!TM || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TM->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TM->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << content << '\n';
      if (content.find("concept") != string::npos) {
        content.erase(content.find("concept"), 7);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(TM->getSourceRange(), content);
    }
}
  
void MutatorFrontendAction_60::MutatorASTConsumer_60::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTemplateParmDecl().bind("TemplateTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}