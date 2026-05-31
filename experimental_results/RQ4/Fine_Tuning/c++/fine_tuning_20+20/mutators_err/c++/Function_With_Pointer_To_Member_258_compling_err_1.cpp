//header file
#pragma once
#include "Mutator_base.h"

/**
 * Function_With_Pointer_To_Member_258
 */ 
class MutatorFrontendAction_258 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(258)

private:
    class MutatorASTConsumer_258 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_258(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Function_With_Pointer_To_Member_258.h"

// ========================================================================================================
#define MUT258_OUTPUT 1

void MutatorFrontendAction_258::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberCallExpr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      content = "/*mut258*/" + content;
      auto pos = content.find(".*");
      if (pos != string::npos) {
        content.replace(pos + 2, 2, "undef");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_258::MutatorASTConsumer_258::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMemberCallExpr().bind("MemberCallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}