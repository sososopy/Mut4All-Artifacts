```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Cast_With_Dynamic_Cast_59
 */ 
class MutatorFrontendAction_59 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(59)

private:
    class MutatorASTConsumer_59 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_59(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Static_Cast_With_Dynamic_Cast_59.h"

// ========================================================================================================
#define MUT59_OUTPUT 1

void MutatorFrontendAction_59::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("StaticCast")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = declaration;
      size_t pos = declaration.find("static_cast");
      if (pos != std::string::npos) {
        mutatedDeclaration.replace(pos, 11, "dynamic_cast");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_59::MutatorASTConsumer_59::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cStyleCastExpr().bind("StaticCast");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}