//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Comma_In_Requires_Clause_307
 */ 
class MutatorFrontendAction_307 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(307)

private:
    class MutatorASTConsumer_307 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_307(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_comma_in_requires_clause_307.h"

// ========================================================================================================
#define MUT307_OUTPUT 1

void MutatorFrontendAction_307::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.find("requires") == string::npos)
        return;
      auto req_content = content.substr(content.find("requires"));
      if (req_content.find(',') == string::npos)
        return;
      auto comma_pos = req_content.find(',');
      auto req_pos = content.find("requires");
      auto target_pos = comma_pos + req_pos;
      content.erase(target_pos, 1);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_307::MutatorASTConsumer_307::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}