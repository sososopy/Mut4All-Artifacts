```cpp
//header file
#pragma once
#include "Mutator_base.h"
#include <type_traits>

/**
 * modify_template_exception_specification_591
 */ 
class MutatorFrontendAction_591 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(591)

private:
    class MutatorASTConsumer_591 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_591(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_exception_specification_591.h"

// ========================================================================================================
#define MUT591_OUTPUT 1

void MutatorFrontendAction_591::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FT->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find("noexcept");
      if (pos != std::string::npos) {
          declaration.replace(pos, 8, "noexcept(std::is_same_v<T, int>)");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_591::MutatorASTConsumer_591::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(functionDecl(isNoThrow()))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```