//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Initializer_List_With_Rvalue_10
 */ 
class MutatorFrontendAction_10 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)

private:
    class MutatorASTConsumer_10 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_initializer_list_with_rvalue_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("initializerListVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (VD->getType()->isSpecificBuiltinType(BuiltinType::Int)) {
        //Get the source code text of target node
        auto initExpr = VD->getInit();
        if (initExpr && isa<InitListExpr>(initExpr)) {
          auto initListExpr = cast<InitListExpr>(initExpr);
          std::string newInit = "std::initializer_list<int>({";
          for (unsigned i = 0; i < initListExpr->getNumInits(); ++i) {
            if (i != 0) newInit += ", ";
            newInit += Lexer::getSourceText(CharSourceRange::getTokenRange(initListExpr->getInit(i)->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
          }
          newInit += "}).begin()";
          //Perform mutation on the source code text by applying string replacement
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), "auto it = " + newInit + ";");
        }
      }
    }
}
  
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(isSpecificBuiltinType(BuiltinType::Int))).bind("initializerListVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}