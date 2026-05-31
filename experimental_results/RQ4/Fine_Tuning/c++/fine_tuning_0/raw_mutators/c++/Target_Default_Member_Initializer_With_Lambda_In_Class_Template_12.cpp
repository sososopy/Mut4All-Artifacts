//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Default_Member_Initializer_With_Lambda_In_Class_Template_12
 */ 
class MutatorFrontendAction_12 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(12)

private:
    class MutatorASTConsumer_12 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_12(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Target_Default_Member_Initializer_With_Lambda_In_Class_Template_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassTemplates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (!MT->isTemplateDecl())
        return;
      auto fields = MT->fields();
      for (auto field : fields) {
        if (field->hasInClassInitializer()) {
          auto inclassinit = field->getInClassInitializer();
          if (inclassinit->getStmtClass() == Stmt::StmtClass::LambdaExprClass) {
            auto initrange = inclassinit->getSourceRange();
            auto initstr = stringutils::rangetoStr(*(Result.SourceManager), initrange);
            llvm::outs() << initstr << '\n';
            Rewrite.ReplaceText(inclassinit->getSourceRange(), "0");
          }
        }
      }
    }
}
  
void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("ClassTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}