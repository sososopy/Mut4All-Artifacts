//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Namespace_Wrappers_437
 */ 
class MutatorFrontendAction_437 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(437)

private:
    class MutatorASTConsumer_437 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_437(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Namespace_Wrappers_437.h"

// ========================================================================================================
#define MUT437_OUTPUT 1

void MutatorFrontendAction_437::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      if (!ED->isCompleteDefinition())
        return;
      if (ED->getAccess() == AccessSpecifier::AS_public)
        return;
      auto parent = ED->getParent();
      if (!parent->isStruct() && !parent->isClass())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ED->getSourceRange());
      auto parent_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    parent->getSourceRange());
      llvm::outs() << "enum: " << content << '\n';
      llvm::outs() << "parent: " << parent_content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()),
                          parent_content);
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(parent->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_437::MutatorASTConsumer_437::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}