```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Transparent_Union_Attribute_151
 */ 
class MutatorFrontendAction_151 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(151)

private:
    class MutatorASTConsumer_151 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_151(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Transparent_Union_Attribute_151.h"

// ========================================================================================================
#define MUT151_OUTPUT 1

void MutatorFrontendAction_151::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TU = Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedef")) {
      //Filter nodes in header files
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TU->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find("__attribute__((transparent_union))");
      if (pos != string::npos) {
        declaration.erase(pos, 24);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TU->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_151::MutatorASTConsumer_151::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typedefDecl(hasAttribute(attr::TransparentUnion)).bind("Typedef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}