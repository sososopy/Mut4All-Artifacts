//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_Initialize_Struct_Members_334
 */ 
class MutatorFrontendAction_334 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(334)

private:
    class MutatorASTConsumer_334 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_334(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Default_Initialize_Struct_Members_334.h"

// ========================================================================================================
#define MUT334_OUTPUT 1

void MutatorFrontendAction_334::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FieldDecl>("StructMembers")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isStaticDataMember())
        return;
      if (MT->hasInClassInitializer())
        return;
      auto type = MT->getType();
      auto typestr = stringutils::rangetoStr(*(Result.SourceManager),
                                             type.getTypePtr()->getLocallyUnqualifiedSingleStepDesugaredType()->getBeginLoc(),
                                             type.getTypePtr()->getLocallyUnqualifiedSingleStepDesugaredType()->getEndLoc());
      llvm::outs() << "Field Type: " << typestr << '\n';
      if (type->isIntegralType(*(Result.Context))) {
        Rewrite.ReplaceText(MT->getEndLoc(), 0, "/*mut334*/=0");
      } else if (type->isFloatingType()) {
        Rewrite.ReplaceText(MT->getEndLoc(), 0, "/*mut334*/=0.0");
      } else if (type->isPointerType()) {
        Rewrite.ReplaceText(MT->getEndLoc(), 0, "/*mut334*/=nullptr");
      } else {
        Rewrite.ReplaceText(MT->getEndLoc(), 0, "/*mut334*/={}");
      }
    }
}
  
void MutatorFrontendAction_334::MutatorASTConsumer_334::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(hasAncestor(cxxRecordDecl(isStruct()))).bind("StructMembers");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}