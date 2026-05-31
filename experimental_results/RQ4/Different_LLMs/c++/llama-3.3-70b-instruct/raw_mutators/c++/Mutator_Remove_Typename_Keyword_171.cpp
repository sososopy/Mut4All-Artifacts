```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Typename_Keyword_171
 */ 
class MutatorFrontendAction_Remove_Typename_Keyword_171 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(171)

private:
    class MutatorASTConsumer_Remove_Typename_Keyword_171 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Remove_Typename_Keyword_171(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Remove_Typename_Keyword_171.h"

// ========================================================================================================
#define MUT171_OUTPUT 1

void MutatorFrontendAction_Remove_Typename_Keyword_171::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParmDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t typenamePos = declaration.find("typename");
      if (typenamePos != string::npos) {
        declaration.erase(typenamePos, 8);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_Remove_Typename_Keyword_171::MutatorASTConsumer_Remove_Typename_Keyword_171::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl().bind("TemplateTypeParmDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}