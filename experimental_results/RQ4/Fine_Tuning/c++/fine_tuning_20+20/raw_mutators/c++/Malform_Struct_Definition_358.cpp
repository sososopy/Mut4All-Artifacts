//header file
#pragma once
#include "Mutator_base.h"

/**
 * Malform_Struct_Definition_358
 */ 
class MutatorFrontendAction_358 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(358)

private:
    class MutatorASTConsumer_358 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_358(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/malform_struct_definition_358.h"

// ========================================================================================================
#define MUT358_OUTPUT 1

void MutatorFrontendAction_358::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      if (!ST->isCompleteDefinition())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ST->getSourceRange());
      if (content.rfind('}') != string::npos)
        content.erase(content.rfind('}'), 1);
      content += " struct mut358;";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_358::MutatorASTConsumer_358::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}