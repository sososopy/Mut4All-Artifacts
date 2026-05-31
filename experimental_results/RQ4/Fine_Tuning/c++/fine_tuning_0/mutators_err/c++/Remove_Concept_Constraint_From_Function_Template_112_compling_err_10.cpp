//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Concept_Constraint_From_Function_Template_112
 */ 
class MutatorFrontendAction_112 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(112)

private:
    class MutatorASTConsumer_112 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_112(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Concept_Constraint_From_Function_Template_112.h"

// ========================================================================================================
#define MUT112_OUTPUT 1

void MutatorFrontendAction_112::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (!FT->getRequiresClause())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FT->getSourceRange());
      llvm::outs() << content << '\n';
      auto pos = content.find("requires");
      if (pos == string::npos)
        return;
      content = content.substr(0, pos);
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_112::MutatorASTConsumer_112::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("FuncTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}