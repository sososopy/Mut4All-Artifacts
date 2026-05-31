//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Block_Expression_In_Generic_Lambda_495
 */ 
class MutatorFrontendAction_495 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(495)

private:
    class MutatorASTConsumer_495 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_495(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_495.h"

// ========================================================================================================
#define MUT495_OUTPUT 1

void MutatorFrontendAction_495::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambdaDecl = MT->getLambdaClass();
      if (!lambdaDecl) return;
      
      // Check if lambda has generic parameter (auto)
      bool hasAutoParam = false;
      for (auto param : lambdaDecl->parameters()) {
        if (param->getType()->isAutoType()) {
          hasAutoParam = true;
          break;
        }
      }
      if (!hasAutoParam) return;
      
      // Check capture list
      auto captureList = MT->getCaptureList();
      if (captureList.empty()) return;
      
      // Choose a captured variable
      std::string captureVar;
      for (auto capture : captureList) {
        if (capture.capturesVariable()) {
          auto varDecl = capture.getCapturedVar();
          if (varDecl) {
            captureVar = varDecl->getNameAsString();
            break;
          }
        }
      }
      if (captureVar.empty()) return;
      
      // Get lambda body
      auto body = MT->getBody();
      if (!body) return;
      
      // Get source range of body
      SourceRange bodyRange = body->getSourceRange();
      SourceLocation insertLoc = bodyRange.getBegin().getLocWithOffset(1);
      
      //Perform mutation on the source code text by applying string replacement
      std::string blockExpr = "^{ (void)" + captureVar + "; };\n";
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertText(insertLoc, blockExpr);
    }
}
  
void MutatorFrontendAction_495::MutatorASTConsumer_495::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasAnyCapture()).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}