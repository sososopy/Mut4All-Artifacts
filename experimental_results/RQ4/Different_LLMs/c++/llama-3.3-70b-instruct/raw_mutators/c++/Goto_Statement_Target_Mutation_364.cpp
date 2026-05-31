//header file
#pragma once
#include "Mutator_base.h"

/**
 * Goto_Statement_Target_Mutation_364
 */ 
class MutatorFrontendAction_364 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(364)

private:
    class MutatorASTConsumer_364 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_364(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> labels;
    };
};

//source file
#include "../include/Goto_Statement_Target_Mutation_364.h"

// ========================================================================================================
#define MUT364_OUTPUT 1

void MutatorFrontendAction_364::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Goto = Result.Nodes.getNodeAs<clang::GotoStmt>("GotoStmt")) {
      //Filter nodes in header files
      if (!Goto || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Goto->getLocation()))
        return;
      //Get the source code text of target node
      auto label = Goto->getLabel()->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      if (!labels.empty()) {
        auto newLabel = labels[getrandom::getRandomIndex(labels.size() - 1)];
        std::string mutatedCode = "goto " + newLabel + ";";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Goto->getSourceRange()), mutatedCode);
      }
    } else if (auto *Label = Result.Nodes.getNodeAs<clang::LabelDecl>("LabelDecl")) {
      //Filter nodes in header files
      if (!Label || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Label->getLocation()))
        return;
      labels.push_back(Label->getNameAsString());
    }
}
  
void MutatorFrontendAction_364::MutatorASTConsumer_364::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StmtMatcher matcher1 = gotoStmt().bind("GotoStmt");
    DeclMatcher matcher2 = labelDecl().bind("LabelDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}