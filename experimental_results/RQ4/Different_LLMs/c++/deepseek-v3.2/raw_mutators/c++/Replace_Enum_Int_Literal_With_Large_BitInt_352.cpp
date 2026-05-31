//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Enum_Int_Literal_With_Large_BitInt_352
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT352_OUTPUT 1

void MutatorFrontendAction_352::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::EnumConstantDecl>("EnumConstant")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto *initExpr = MT->getInitExpr();
      if (!initExpr) return;
      if (auto *intLiteral = dyn_cast<clang::IntegerLiteral>(initExpr)) {
        SourceRange literalRange = intLiteral->getSourceRange();
        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, literalRange);
        //Perform mutation on the source code text by applying string replacement
        std::string newLiteral = "99999999999999999999999999999999999999999999999999wb"; // 50 digits
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(literalRange, newLiteral);
      }
    }
}
  
void MutatorFrontendAction_352::MutatorASTConsumer_352::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumConstantDecl(hasInitExpr(integerLiteral())).bind("EnumConstant");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}