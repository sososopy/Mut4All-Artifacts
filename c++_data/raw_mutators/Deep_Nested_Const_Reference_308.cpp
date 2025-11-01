//header file
#pragma once
#include "Mutator_base.h"

/**
 * Deep_Nested_Const_Reference_308
 */ 
class MutatorFrontendAction_308 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(308)

private:
    class MutatorASTConsumer_308 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_308(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/deep_nested_const_reference_308.h"

// ========================================================================================================
#define MUT308_OUTPUT 1

void MutatorFrontendAction_308::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      auto type = VD->getType().getAsString();
      std::string nestedType = type;
      for (int i = 0; i < 20; ++i) {
          nestedType = "const " + nestedType + " &";
      }
      
      SourceLocation startLoc = VD->getBeginLoc();
      SourceLocation endLoc = VD->getTypeSourceInfo()->getTypeLoc().getEndLoc();
      Rewrite.ReplaceText(SourceRange(startLoc, endLoc), nestedType);
    }
}
  
void MutatorFrontendAction_308::MutatorASTConsumer_308::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(isInteger())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}