//header file
#pragma once
#include "Mutator_base.h"

/**
 * nested_array_initialization_620
 */ 
class MutatorFrontendAction_620 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(620)

private:
    class MutatorASTConsumer_620 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_620(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/nested_array_initialization_620.h"

// ========================================================================================================
#define MUT620_OUTPUT 1

void MutatorFrontendAction_620::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 VD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string lambdaInit = "[&] { return ";
      std::string arrayAccess = "t[0][1].j + t[1][1].j";
      lambdaInit += arrayAccess + "; }()";
      auto pos = declaration.find(";");
      if (pos != std::string::npos) {
        declaration.insert(pos, " = " + lambdaInit);
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_620::MutatorASTConsumer_620::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType(hasElementType(recordType())))).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}