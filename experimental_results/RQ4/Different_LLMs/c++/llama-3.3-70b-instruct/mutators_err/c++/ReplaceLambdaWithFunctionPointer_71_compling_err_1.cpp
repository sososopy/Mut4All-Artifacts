```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceLambdaWithFunctionPointer_71
 */ 
class MutatorFrontendAction_71 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(71)

private:
    class MutatorASTConsumer_71 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_71(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceLambdaWithFunctionPointer_71.h"

// ========================================================================================================
#define MUT71_OUTPUT 1

void MutatorFrontendAction_71::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace lambda with function pointer
      std::string funcPtrDecl = "void (*func_ptr)() = nullptr;";
      std::string mutatedCode = declaration;
      size_t pos = mutatedCode.find("auto");
      if (pos != std::string::npos) {
        mutatedCode.replace(pos, 4, funcPtrDecl);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_71::MutatorASTConsumer_71::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}