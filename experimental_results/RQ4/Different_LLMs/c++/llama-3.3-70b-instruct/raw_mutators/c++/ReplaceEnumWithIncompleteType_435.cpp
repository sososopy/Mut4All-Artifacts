//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceEnumWithIncompleteType_435
 */ 
class MutatorFrontendAction_435 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(435)

private:
    class MutatorASTConsumer_435 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_435(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_ReplaceEnumWithIncompleteType_435.h"

// ========================================================================================================
#define MUT435_OUTPUT 1

void MutatorFrontendAction_435::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      //Get the source code text of target node
      std::string enumText = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedEnumText = "enum " + ED->getNameAsString() + " : struct IncompleteType {\n";
      for (auto &enumerator : ED->enumerators()) {
        mutatedEnumText += enumerator->getNameAsString() + ",\n";
      }
      mutatedEnumText += "};";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), mutatedEnumText);
    }
}
  
void MutatorFrontendAction_435::MutatorASTConsumer_435::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}