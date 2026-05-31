//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceArrayInitializerWithAssignment_299
 */ 
class MutatorFrontendAction_299 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(299)

private:
    class MutatorASTConsumer_299 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_299(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/ReplaceArrayInitializerWithAssignment_299.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT299_OUTPUT 1

void MutatorFrontendAction_299::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VA = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VA->getLocation()))
        return;
      //Get the source code text of target node
      auto init = VA->getInit();
      if (!init)
        return;
      if (auto *IA = dyn_cast<clang::InitListExpr>(init)) {
        std::vector<std::string> assignments;
        for (unsigned i = 0; i < IA->getNumInits(); ++i) {
          auto *IE = IA->getInit(i);
          if (auto *CE = dyn_cast<clang::ConstantExpr>(IE)) {
            if (CE->isIntegerConstantExpr()) {
                assignments.push_back(VA->getNameAsString() + "[" + std::to_string(i) + "] = " + std::to_string(CE->getIntegerConstantExpr()->getValue().getSExtValue()) + ";");
            }
          }
        }
        std::string assignmentStr;
        for (const auto &assignment : assignments) {
          assignmentStr += assignment + "\n";
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VA->getSourceRange()), VA->getNameAsString() + ";");
        Rewrite.InsertTextAfterToken(VA->getEndLoc(), assignmentStr);
      }
    }
}
  
void MutatorFrontendAction_299::MutatorASTConsumer_299::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}