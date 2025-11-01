//header file
#pragma once
#include "Mutator_base.h"

/**
 * Overflowing_Enum_Initializer_352
 */ 
class MutatorFrontendAction_352 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(352)

private:
    class MutatorASTConsumer_352 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_352(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Overflowing_Enum_Initializer_352.h"

// ========================================================================================================
#define MUT352_OUTPUT 1

void MutatorFrontendAction_352::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ET = Result.Nodes.getNodeAs<clang::EnumConstantDecl>("EnumConstant")) {
      //Filter nodes in header files
      if (!ET || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ET->getLocation()))
        return;

      //Get the source code text of target node
      auto initializer = ET->getInitVal();
      if (initializer.isNull())
          return;

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedInitializer = "12345678901234567890wb";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ET->getInitExpr()->getSourceRange(), mutatedInitializer);
    }
}
  
void MutatorFrontendAction_352::MutatorASTConsumer_352::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumConstantDecl(hasInitializer(integerLiteral())).bind("EnumConstant");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}