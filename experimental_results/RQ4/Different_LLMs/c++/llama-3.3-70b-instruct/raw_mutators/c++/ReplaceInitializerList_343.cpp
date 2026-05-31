```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceInitializerList_343
 */ 
class MutatorFrontendAction_343 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(343)

private:
    class MutatorASTConsumer_343 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_343(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_ReplaceInitializerList_343.h"

// ========================================================================================================
#define MUT343_OUTPUT 1

void MutatorFrontendAction_343::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VarDecl = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VarDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VarDecl->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VarDecl->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find('{');
      if (pos != std::string::npos) {
        size_t endPos = declaration.find('}', pos);
        if (endPos != std::string::npos) {
          declaration.replace(pos + 1, endPos - pos - 1, "");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarDecl->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_343::MutatorASTConsumer_343::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer()).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}