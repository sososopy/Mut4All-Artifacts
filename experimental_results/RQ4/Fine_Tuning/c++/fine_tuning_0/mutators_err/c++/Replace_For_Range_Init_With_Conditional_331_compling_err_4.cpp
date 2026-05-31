//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_For_Range_Init_With_Conditional_331
 */ 
class MutatorFrontendAction_331 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(331)

private:
    class MutatorASTConsumer_331 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_331(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_For_Range_Init_With_Conditional_331.h"

// ========================================================================================================
#define MUT331_OUTPUT 1

void MutatorFrontendAction_331::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::ForStmt>("ForWithRangeDecl")) {
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getBeginLoc()))
        return;

      auto DL = FS->getConditionVariable();
      if (!DL)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find("auto") == string::npos)
        return;
      auto VD = DL;
      if (!VD)
        return;
      auto type = VD->getType();
      if (!type->isDependentType())
        return;
      auto init = VD->getInit();
      if (!init)
        return;
      if (!init->getType()->isDependentType())
        return;
      auto init_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  init->getSourceRange());
      if (init_content.find('?') == string::npos)
        return;
      llvm::outs() << "Mut331: " << content << '\n';
      auto new_content = content;
      new_content.replace(new_content.find(init_content), init_content.size(),
                          "false");
      llvm::outs() << "Mut331: " << new_content << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), new_content);
    }
}
  
void MutatorFrontendAction_331::MutatorASTConsumer_331::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = forStmt(hasLoopInit(declStmt(hasSingleDecl(
                                 varDecl(hasInitializer(expr(hasType(
                                     hasUnqualifiedDesugaredType(
                                         clang::Type::Dependent)))))))))
                       .bind("ForWithRangeDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}