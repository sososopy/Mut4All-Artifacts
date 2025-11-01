//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Variadic_Template_Condition_471
 */ 
class MutatorFrontendAction_471 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(471)

private:
    class MutatorASTConsumer_471 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_471(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Variadic_Template_Condition_471.h"

// ========================================================================================================
#define MUT471_OUTPUT 1

void MutatorFrontendAction_471::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("boolVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      //Get the source code text of target node
      auto boolDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                              VD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = boolDecl.find("std::integral<T>");
      if (pos != std::string::npos) {
          boolDecl.replace(pos, std::string("std::integral<T>").length(), "(std::integral<T> && std::is_arithmetic<T>::value)");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), boolDecl);
    }
}
  
void MutatorFrontendAction_471::MutatorASTConsumer_471::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(isBoolean())).bind("boolVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}