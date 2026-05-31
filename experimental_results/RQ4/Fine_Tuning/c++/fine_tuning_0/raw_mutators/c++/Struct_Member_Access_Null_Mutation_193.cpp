//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Member_Access_Null_Mutation_193
 */ 
class MutatorFrontendAction_193 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(193)

private:
    class MutatorASTConsumer_193 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_193(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Struct_Member_Access_Null_Mutation_193.h"

// ========================================================================================================
#define MUT193_OUTPUT 1

void MutatorFrontendAction_193::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberAccess")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto member_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << member_name << '\n';
      string null_name = "/*mut193*/";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          null_name);
    }
}
  
void MutatorFrontendAction_193::MutatorASTConsumer_193::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = memberExpr().bind("MemberAccess");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}