//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_NTTP_With_Concept_Constraint_129
 */ 
class MutatorFrontendAction_129 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(129)

private:
    class MutatorASTConsumer_129 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_129(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Default_NTTP_With_Concept_Constraint_129.h"

// ========================================================================================================
#define MUT129_OUTPUT 1

void MutatorFrontendAction_129::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("Template")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      if (TP->hasDefaultArgument())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TP->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("Invokable") != string::npos) {
        content += "/*mut129*/=[]{return 1;}";
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(TP->getSourceRange()), content);
      }
    }
}
  
void MutatorFrontendAction_129::MutatorASTConsumer_129::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = nonTypeTemplateParmDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}