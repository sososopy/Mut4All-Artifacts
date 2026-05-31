//header file
#pragma once
#include "Mutator_base.h"

/**
 * remove_default_argument_from_constructor_523
 */ 
class MutatorFrontendAction_523 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(523)

private:
    class MutatorASTConsumer_523 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_523(Rewriter &R) : TheRewriter(R) {}
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
#include "Mutator_base.h"

// ========================================================================================================
#define MUT523_OUTPUT 1

void MutatorFrontendAction_523::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CtorDecl = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ConstructorWithDefaultArg")) {
      //Filter nodes in header files
      if (!CtorDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CtorDecl->getLocation()))
        return;

      // Check if the constructor has any default arguments
      for (auto *Param : CtorDecl->parameters()) {
        if (Param->hasDefaultArg()) {
          //Get the source code text of target node
          auto paramRange = Param->getSourceRange();
          auto paramText = Lexer::getSourceText(CharSourceRange::getTokenRange(paramRange), *Result.SourceManager, Result.Context->getLangOpts());

          // Remove the default argument from the parameter
          size_t equalPos = paramText.find('=');
          if (equalPos != std::string::npos) {
            paramText = paramText.substr(0, equalPos);
          }

          //Perform mutation on the source code text by applying string replacement
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramRange), paramText);
        }
      }
    }
}
  
void MutatorFrontendAction_523::MutatorASTConsumer_523::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyParameter(hasDefaultArg())).bind("ConstructorWithDefaultArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}