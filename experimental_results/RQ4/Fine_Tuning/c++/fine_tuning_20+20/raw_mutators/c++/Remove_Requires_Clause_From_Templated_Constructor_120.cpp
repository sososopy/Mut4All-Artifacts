//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Requires_Clause_From_Templated_Constructor_120
 */ 
class MutatorFrontendAction_120 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(120)

private:
    class MutatorASTConsumer_120 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_120(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_requires_clause_from_templated_constructor_120.h"

// ========================================================================================================
#define MUT120_OUTPUT 1

void MutatorFrontendAction_120::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructors")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isTemplated())
        return;
      if (!DL->isFunctionTemplateSpecialization())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("requires") == string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      content.erase(content.find("requires"),
                    content.find('>', content.find("requires")) -
                        content.find("requires") + 1);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_120::MutatorASTConsumer_120::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructors");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}