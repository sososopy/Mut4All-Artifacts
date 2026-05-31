```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Conditional_Directive_Modifier_409
 */ 
class MutatorFrontendAction_409 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(409)

private:
    class MutatorASTConsumer_409 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_409(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Conditional_Directive_Modifier_409.h"

// ========================================================================================================
#define MUT409_OUTPUT 1

void MutatorFrontendAction_409::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::IfDirective>(("IfDirective"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto directive = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) {
        // Invert the condition
        directive.replace(directive.find("#if"), 3, "#if not");
      } else if (choice == 1) {
        // Swap #if and #elif
        if (directive.find("#elif") != std::string::npos) {
          directive.replace(directive.find("#elif"), 5, "#if");
        } else {
          directive.replace(directive.find("#if"), 3, "#elif");
        }
      } else {
        // Remove or add an #else clause
        if (directive.find("#else") != std::string::npos) {
          directive.erase(directive.find("#else"), 5);
        } else {
          directive.append("\n#else\n#endif");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), directive);
    }
}

void MutatorFrontendAction_409::MutatorASTConsumer_409::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = ifDirective().bind("IfDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}