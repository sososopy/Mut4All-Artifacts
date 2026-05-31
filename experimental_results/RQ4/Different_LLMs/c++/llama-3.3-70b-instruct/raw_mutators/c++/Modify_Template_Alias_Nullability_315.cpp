```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Alias_Nullability_315
 */ 
class MutatorFrontendAction_315 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(315)

private:
    class MutatorASTConsumer_315 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_315(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Alias_Nullability_315.h"

// ========================================================================================================
#define MUT315_OUTPUT 1

void MutatorFrontendAction_315::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateAlias>({"TemplateAlias"})) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (declaration.find("_Nullable") != string::npos) {
        declaration.replace(declaration.find("_Nullable"), 9, "_Nonnull");
      } else if (declaration.find("_Nonnull") != string::npos) {
        declaration.replace(declaration.find("_Nonnull"), 9, "");
      } else {
        declaration.insert(declaration.find(">"), " _Nullable");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_315::MutatorASTConsumer_315::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateAliasDecl().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}