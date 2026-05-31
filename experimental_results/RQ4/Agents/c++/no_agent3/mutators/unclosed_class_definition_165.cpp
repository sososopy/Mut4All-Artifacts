//header file
#pragma once
#include "Mutator_base.h"

/**
 * Unclosed_Class_Definition_165
 */ 
class MutatorFrontendAction_165 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(165)

private:
    class MutatorASTConsumer_165 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_165(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/unclosed_class_definition_165.h"

// ========================================================================================================
#define MUT165_OUTPUT 1

void MutatorFrontendAction_165::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDef")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (!CD->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto classRange = CD->getSourceRange();
      auto classText = stringutils::rangetoStr(*(Result.SourceManager), classRange);
      //Perform mutation on the source code text by applying string replacement
      if (classText.rfind('}') != string::npos) {
        classText.erase(classText.rfind('}'));
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(classRange), classText);
    }
}
  
void MutatorFrontendAction_165::MutatorASTConsumer_165::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("ClassDef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}